#include "application.h"
#include "ui_window.h"



/**
 * @brief This is the class constructor
 *
 * The constructor initialize the connections with the GUI widget and the related callbacks.
 *
 * @param parent: QOBJect pointer parent
 */
debugWindow::debugWindow(QWidget *parent)
    : ui(new Ui::debugWindow)
{
    debugWindow::instance = this;
    ui->setupUi(this);

    // Set the View to handle the rotation


    connect(ui->logClearButton, SIGNAL(pressed()), this, SLOT(onLogClearButton()), Qt::UniqueConnection);
    connect(ui->debugClearButton, SIGNAL(pressed()), this, SLOT(onDebugClearButton()), Qt::UniqueConnection);
    connect(ui->logEnableCheck, SIGNAL(stateChanged(int)), this, SLOT(on_logEnableCheck_stateChanged(int)));
    connect(ui->logEnableEthCheck, SIGNAL(stateChanged(int)), this, SLOT(on_logEnableEthCheck_stateChanged(int)));



    pollingTimer  = startTimer(500);

}

debugWindow::~debugWindow()
{
    delete ui;
}

void debugWindow::initWindow(void)
{


}

void debugWindow::exitWindow(void)
{

    if(pollingTimer)    killTimer(pollingTimer);

}


void debugWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == pollingTimer)
    {

        return;
    }


}

/**
 * @brief This function clears the content of the Can frame data logging
 */
void debugWindow::onLogClearButton(void){
    ui->canText->appendPlainText("");
    ui->canText->clear();
}

/**
 * @brief This function clears the content of the debug string data logging
 */
void debugWindow::onDebugClearButton(void){
    ui->debugText->appendPlainText("");
    ui->debugText->clear();
}


/**
 * @brief This function receives the data coming from the CAN network.
 *
 * The Data packet received from the CAN network is displayed with the message number in the proper panel.
 *
 *
 * @param canId: this is the canId of the can message
 * @param data: this is the data content of the frame
 */
void debugWindow::receivedCanFrame(ushort canId, QByteArray data){

    QString stringa = QString("%1> FROM CANID:0x%3 - ").arg(((double) clock())/CLOCKS_PER_SEC).arg(canId,1,16);
    for(int i=0; i< 8;i++){
        stringa.append(QString(" 0x%1").arg((uchar) data[i],1,16));
    }
    ui->canText->appendPlainText(stringa);

}

/**
 * @brief This function is activated whenever the Client data are forwarded to the CAN network.
 *
 * The data content is Logged in the Can Data traffic panel
 *
 * @param canId: this is the canId of the can message
 * @param data: this is the data content of the frame
 */
void debugWindow::sendToCan(ushort canId, QByteArray data){

    QString stringa = QString("%1> TO CANID:0x%3 - ").arg(((double) clock())/CLOCKS_PER_SEC).arg(canId,1,16);
    for(int i=0; i< data.size();i++){
        stringa.append(QString(" 0x%1").arg((uchar) data[i],1,16));
    }
    ui->canText->appendPlainText(stringa);

}


/**
 * @brief This function enables/disables the Can Data loggin
 *
 * The Check box vconnects or disconnects the debugWindow::receivedCanFrame() \n
 * from the CAN signal.
 *
 * @param arg1
 */
void debugWindow::on_logEnableCheck_stateChanged(int arg1)
{

    static bool connected = false;
    if(arg1){
        if(!connected) connect(CAN,SIGNAL(receivedCanFrame(ushort , QByteArray )), WINDOW, SLOT(receivedCanFrame(ushort , QByteArray)),Qt::QueuedConnection);
        connected = true;
    }else{
        disconnect(CAN,SIGNAL(receivedCanFrame(ushort , QByteArray )), WINDOW, SLOT(receivedCanFrame(ushort , QByteArray)));
        connected = false;

    }

}

/**
 * @brief This function enables/disables the Can Data loggin
 *
 * The Check box vconnects or disconnects the debugWindow::receivedCanFrame() \n
 * from the CAN signal.
 *
 * @param arg1
 */
void debugWindow::on_logEnableEthCheck_stateChanged(int arg1)
{

    static bool connected = false;
    if(arg1){
        if(!connected) connect(INTERFACE,SIGNAL(sendToCan(ushort , QByteArray )), WINDOW, SLOT(sendToCan(ushort , QByteArray)),Qt::QueuedConnection);
        connected = true;
    }else{
        disconnect(INTERFACE,SIGNAL(sendToCan(ushort , QByteArray )), WINDOW, SLOT(sendToCan(ushort , QByteArray)));
        connected = false;

    }

}

void debugWindow::debugMessageHandler(QtMsgType type, QString msg){
    if(!debugWindow::instance) return;
    if(debugWindow::instance->ui->debugEnable->isChecked())   debugWindow::instance->ui->debugText->appendPlainText(msg);
}

