#include "application.h"
#include <QStringConverter>


/**
 * This function create the server structure.
 *
 * @param ipaddress: server IP address
 * @param port: server port
 */
Server::Server(QString ipaddress, int port):QTcpServer()
{
    socketList.clear();
    localip = QHostAddress(ipaddress);
    localport = port;
    idseq=0;

}

/**
 * Distructor of the Server structure.
 *
 * When the server is destroyed, the client queue needs to be destroyed as well.
 *
 */
Server::~Server()
{
    if(socketList.size()){
        for(int i = 0; i<socketList.size(); i++ ){
            if(socketList[i]->socket != nullptr) {
                socketList[i]->socket->close();
                socketList[i]->socket->deleteLater();
            }
        }
    }
}

/**
 * @brief This function activates the server listening.
 *
 * @return
 */
bool Server::Start(void)
{
    qDebug() << "VSCAN DRIVER INTERFACE ADDRESS: IP=" << localip.toString() << ", PORT=" << localport ;
    if (!this->listen(localip,localport)) return false;
    return true;

}

/**
 * This callback is called from the Socket library \n
 * whenever an incoming connection is requested by a remote CLient.
 *
 * When the connection is accepted, the CLient is added to
 * the server queue of the connected clients.
 *
 *
 * @param socketDescriptor
 */
void Server::incomingConnection(qintptr socketDescriptor)
{

    SocketItem* item = new SocketItem();

    item->socket = new QTcpSocket(this); // Create a new socket
    if(!item->socket->setSocketDescriptor(socketDescriptor))
    {
        delete item->socket;
        delete item;
        return;
    }

    item->socket->setSocketOption(QAbstractSocket::LowDelayOption,1);
    socketList.append(item);


    // Interface signal connection
    connect(item->socket,SIGNAL(readyRead()), item, SLOT(socketRxData()),Qt::UniqueConnection);
    connect(item->socket,SIGNAL(disconnected()),item, SLOT(disconnected()),Qt::UniqueConnection);
    connect(item,SIGNAL(sendToClient(QByteArray)),item, SLOT(socketTxData(QByteArray)),Qt::QueuedConnection);

    connect(item,SIGNAL(itemDisconnected(ushort )),this, SLOT(disconnected(ushort )),Qt::UniqueConnection);
    connect(item->socket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),item,SLOT(socketError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
    connect(item,SIGNAL(sendToCan(ushort, QByteArray )),this, SLOT(sendToCanSlot(ushort, QByteArray )),Qt::UniqueConnection);

    item->id = this->idseq++;
    item->filter_mask = 0xFFFF;
    item->filter_address = 0; // No device registered yet

    return;
 }

/**
 * Socket disconnected callback
 */
void SocketItem::disconnected(void){
    emit itemDisconnected(this->id);
}

/**
 * Socket error callback
 *
 * @param error: the error code
 */
void SocketItem::socketError(QAbstractSocket::SocketError error)
{
    emit itemDisconnected(this->id);
    return;
}

/**
 * This function is called whenever a Client disconnects.
 *
 * The function deletes the client socket structure \n
 * removing the client from the active Client connection queue.
 *
 * @param id: the client identifier
 */
void Server::disconnected(ushort id)
{

    for(int i =0; i < socketList.size(); i++ ){
        if(socketList[i]->id == id){

            disconnect(socketList[i]);
            socketList[i]->socket->deleteLater();
            delete socketList[i];
            socketList.remove(i);
            return;
        }
    }

}

/**
 * This Slot is connected to the internal client socket reception
 * procedure.
 *
 * When a can frame shall be sent from the client, this SLot makes
 * a bridge from the client socket and the canDriver.
 *
 * @param canId: identifier of the target CAN device;
 * @param dataFrame: the CAN data content.
 */
void Server::sendToCanSlot(ushort canId, QByteArray dataFrame)
{

    emit sendToCan(canId, dataFrame);
}

ushort SocketItem::getItem(int* index, QByteArray* data, bool* data_ok){
    *data_ok = false;
    bool is_hex = false;
    QString val;

    for(; *index< data->size(); (*index)++) if(data->at(*index) != ' ') break; // Removes the spaces

    for(; *index< data->size(); (*index)++){
        if(data->at(*index) == ' '){
            if(!val.size()) return 0;
            *data_ok = true;
            if(is_hex) return val.toUShort(data_ok, 16);
            else return val.toUShort();
        }
        if((data->at(*index) == 'x') |(data->at(*index) == 'X')) is_hex = true;
        val.append(data->at(*index));
    }

    // Non è terminato con uno spazio: errore
    return 0;
}

/**
 * This function decodes a single frame received from the Client.
 *
 * In case of successfully Registering frame identification,\n
 * the CLient is registered to a canId target device;
 *
 * In case of successfully Can Data frame identification,\n
 * the data are sent to the target device;
 *
 *
 * @param data: the pointer to the protocol frame to be decoded.
 */
void SocketItem::handleSocketFrame(QByteArray* data){

    QByteArray frame;
    bool is_register;
    bool is_valid;
    int i;
    bool data_ok;


    is_valid = false;
    for(i=0; i< data->size(); i++){
        if(data->at(i)== ' ') continue;
        if(data->at(i)== 'F') {
            is_register = true;
            is_valid = true;
            i++;
            break;
        }

        if(data->at(i)== 'D') {
            is_register = false;
            is_valid = true;
            i++;
            break;
        }
    }
    if(!is_valid) return;

    if(is_register){// Can Registering Frame: set the reception mask and address


        filter_mask = getItem(&i, data, &data_ok);
        if(!data_ok){
            filter_mask = 0xFFFF;
            qDebug() << "CLIENT REGISTRATION WRONG MASK FORMAT";
            return;
        }

        filter_address = getItem(&i, data, &data_ok) & filter_mask;
        if(!data_ok){
            filter_mask = 0xFFFF;
            filter_address = 0;
            qDebug() << "CLIENT REGISTRATION WRONG ADDRESS FORMAT";
            return;
        }

        frame.append("<");
        frame.append(*data);
        frame.append(">");
        emit sendToClient(frame);

        qDebug() << QString("CLIENT REGISTERED TO: MASK=0x%1, ADDR:0x%2").arg(filter_mask,1,16).arg(filter_address,1,16);
        return;
    }else{

        frame.clear();
        ushort canid = getItem(&i, data, &data_ok);
        if(!data_ok) {
            return;
        }
        ushort val;
        for(; (i< data->size()) && (frame.size() <= 8) ; i++){
            val = getItem(&i, data, &data_ok);
            if(data_ok) frame.append((unsigned char) val);
            else break;
        }

        // If a valid set of data has been identified they will be sent to the driver        
        if(frame.size()) emit sendToCan(canid,frame);
    }

}

/**
 * This callback is called whenever a data stream is received
 * from a connected Client.
 *
 * Because a stream can contain multilple formatted frames,
 * this function unpack each frame sndind the content to the \n
 * SocketItem::handleSocketFrame() to decode a single data content.
 *
 */
void SocketItem::socketRxData()
{
    QByteArray frame;    

    if(socket->bytesAvailable()==0) return;
    QByteArray data = socket->readAll();



    // Identifies all the possible frames in the received stream
    for(int i=0; i<data.size(); i++){
        if(data.at(i) == '<') {
            frame.clear();
        }else if(data.at(i) == '>'){
            if(frame.size() > 4) {
                frame.append(' ');
                handleSocketFrame(&frame);
                frame.clear();
            }
        }else{
            frame.append(data.at(i));
        }
    }


}

/**
 * This function writes the socket data frame to ethernet
 *
 * @param data: the frame data content
 */
void SocketItem::socketTxData(QByteArray data)
{
    this->socket->write(data);
    this->socket->waitForBytesWritten(100);
    return;

}


/**
 * @brief This function receives the data coming from the CAN network.
 *
 * The Data packet received from the CAN network is forwarded to \n
 * the connected Slaves that has been registered with the canId identifier \n
 * or with the broadcast address (0).
 *
 * The Data is put in the socket packet as for the protocol:\n
 * <D> (uchar) b0 .. (uchar) b7
 *
 *
 * @param canId: this is the canId of the can message
 * @param data: this is the data content of the frame
 */
void Server::receivedCanFrame(ushort canId, QByteArray data){
    QByteArray frame;
    frame.append("<D ");
    frame.append(QString("%1 ").arg((ushort) canId).toLatin1());

    for(int i=0; i< 8;i++){
        if(i >= data.size()) frame.append("0 ");
        else frame.append(QString("%1 ").arg((uchar) data[i]).toLatin1());
    }
    frame += " > \n\r";

    for(int i =0; i< socketList.size(); i++){
        if( (socketList[i]->filter_mask & canId) == (socketList[i]->filter_address)){
            socketList[i]->socket->write(frame);
            socketList[i]->socket->waitForBytesWritten(100);
        }
    }

}


