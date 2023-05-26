#include "awsinterface.h"

awsInterface::awsInterface(QString address, int port, QObject *parent)
    : QObject{parent}
{

    connectionStatus = false;

    // Carica tutto il set di comandidi interfaccia
    awsCommands.insert("EXEC_AbortProjections" ,&awsInterface::EXEC_AbortProjections);
    awsCommands.insert("EXEC_ArmPosition" ,&awsInterface::EXEC_ArmPosition);
    awsCommands.insert("EXEC_Close" ,&awsInterface::EXEC_Close);
    awsCommands.insert("EXEC_OpenStudy" ,&awsInterface::EXEC_OpenStudy);
    awsCommands.insert("EXEC_PowerOff" ,&awsInterface::EXEC_PowerOff);
    awsCommands.insert("EXEC_ResetError" ,&awsInterface::EXEC_ResetError);
    awsCommands.insert("EXEC_StartXraySequence" ,&awsInterface::EXEC_StartXraySequence);
    awsCommands.insert("EXEC_TrxPosition" ,&awsInterface::EXEC_TrxPosition);
    awsCommands.insert("GET_Accessories" ,&awsInterface::GET_Accessories);
    awsCommands.insert("GET_CompressorData" ,&awsInterface::GET_CompressorData);
    awsCommands.insert("GET_ExposureCompletionData" ,&awsInterface::GET_ExposureCompletionData);
    awsCommands.insert("GET_ReadyForExposure" ,&awsInterface::GET_ReadyForExposure);
    awsCommands.insert("GET_RotationAngles" ,&awsInterface::GET_RotationAngles);
    awsCommands.insert("GET_TomoConfig" ,&awsInterface::GET_TomoConfig);
    awsCommands.insert("GET_TomoConfigFile" ,&awsInterface::GET_TomoConfigFile);
    awsCommands.insert("GET_TubeTemperature" ,&awsInterface::GET_TubeTemperature);
    awsCommands.insert("GET_XrayPushStatus" ,&awsInterface::GET_XrayPushStatus);
    awsCommands.insert("SET_CompressionRelease" ,&awsInterface::SET_CompressionRelease);
    awsCommands.insert("SET_Language" ,&awsInterface::SET_Language);
    awsCommands.insert("SET_ProjectionList" ,&awsInterface::SET_ProjectionList);
    awsCommands.insert("SET_PulseData" ,&awsInterface::SET_PulseData);
    awsCommands.insert("SET_XrayPushEnable" ,&awsInterface::SET_XrayPushEnable);
    awsCommands.insert("SET_ExposureMode" ,&awsInterface::SET_ExposureMode);



    // Instance of the socket connection with the aws device
    pTcpIpServer = new TcpIpServer( QHostAddress(address), port);
    encodingFormat = QStringConverter::Encoding::Utf16LE;
    connect(pTcpIpServer,SIGNAL(rxData(QByteArray)),this,SLOT(tcpIpServerRxData(QByteArray)),Qt::UniqueConnection);
    connect(pTcpIpServer,SIGNAL(serverConnection(bool)),this,SLOT(tcpIpServerConnection(bool)),Qt::UniqueConnection);
    pTcpIpServer->Start();


}
void awsInterface::restartServer(QString ip, uint port){
    pTcpIpServer->deleteLater();
    disconnect(pTcpIpServer);
    pTcpIpServer = new TcpIpServer( QHostAddress(ip), port);
    connect(pTcpIpServer,SIGNAL(rxData(QByteArray)),this,SLOT(tcpIpServerRxData(QByteArray)),Qt::UniqueConnection);
    connect(pTcpIpServer,SIGNAL(serverConnection(bool)),this,SLOT(tcpIpServerConnection(bool)),Qt::UniqueConnection);
    pTcpIpServer->Start();

}

void awsInterface::tcpIpServerConnection(bool status){
    connectionStatus = status;
    if(status){
        connect(this,SIGNAL(transmitFrameSgn(QByteArray)),pTcpIpServer,SLOT(txData(QByteArray)),Qt::UniqueConnection);
        emit transmitFrameSgn(commandProtocol::formatStatusCommand(this->encodingFormat, "GANTRY_Connected"));
    }else{
        disconnect(this,SIGNAL(transmitFrameSgn(QByteArray)),pTcpIpServer,SLOT(txData(QByteArray)));
    }
}

void awsInterface::tcpIpServerRxData(QByteArray data){

    emit receivedFrameSgn(data); // Echo data received

    protocol = commandProtocol(&data, encodingFormat);
    if(protocol.getDecodedType() != commandProtocol::_CMD_FRAME) return;

    funcPtr function = awsCommands.value(protocol.getCmd(),&awsInterface::AWS_NotRecognizedCommand);
    if(function == 0) AWS_NotRecognizedCommand();
    else (this->*function)();

}

