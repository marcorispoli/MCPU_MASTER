#include "application.h"
#include "ui_window.h"



debugWindow::debugWindow(QWidget *parent)
    : ui(new Ui::debugWindow)
{
    debugWindow::instance = this;
    ui->setupUi(this);
    generalDebugIndex = 0;

    // Set the View to handle the rotation


    connect(ui->logClearButton, SIGNAL(pressed()), this, SLOT(onLogClearButton()), Qt::UniqueConnection);
    connect(ui->debugClearButton, SIGNAL(pressed()), this, SLOT(onDebugClearButton()), Qt::UniqueConnection);
    connect(ui->logEnableCheck, SIGNAL(stateChanged(int)), this, SLOT(on_logEnableCheck_stateChanged(int)));

    connect(ui->softPoffButton, SIGNAL(pressed()), this, SLOT(onSoftPoffButton()), Qt::UniqueConnection);
    connect(ui->abortButton, SIGNAL(pressed()), this, SLOT(onAbortButton()), Qt::UniqueConnection);

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


void debugWindow::onLogClearButton(void){
    ui->canText->appendPlainText("");
    ui->canText->clear();
}
void debugWindow::onDebugClearButton(void){
    ui->debugText->appendPlainText("");
    ui->debugText->clear();
}



void debugWindow::on_logEnableCheck_stateChanged(int arg1)
{
    static bool connected = false;

    if((arg1 == Qt::Checked) && (!connected)) {
        connected = true;
        connect(PROTOCOL, SIGNAL(dataReceivedFromCan(ushort,QByteArray)), this, SLOT(rxFromCan(ushort,QByteArray)), Qt::QueuedConnection);
        connect(PROTOCOL, SIGNAL(txToCan(ushort,QByteArray )), this, SLOT(txToCan(ushort,QByteArray)), Qt::QueuedConnection);
    }else if((arg1 == Qt::Unchecked) && (connected)) {
        connected = false;
        disconnect(PROTOCOL, SIGNAL(dataReceivedFromCan(ushort,QByteArray )), this, SLOT(rxFromCan(ushort,QByteArray)));
        disconnect(PROTOCOL, SIGNAL(txToCan(ushort,QByteArray)), this, SLOT(txToCan(ushort,QByteArray)));
    }
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
void debugWindow::rxFromCan(ushort canId, QByteArray data){

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
void debugWindow::txToCan(ushort canId, QByteArray data){

    QString stringa = QString("%1> TO CANID:0x%3 - ").arg(((double) clock())/CLOCKS_PER_SEC).arg(canId,1,16);
    for(int i=0; i< data.size();i++){
        stringa.append(QString(" 0x%1").arg((uchar) data[i],1,16));
    }
    ui->canText->appendPlainText(stringa);

}


void debugWindow::debugMessageHandler(QtMsgType type, QString msg){
    if(!debugWindow::instance) return;
    if(debugWindow::instance->ui->debugEnable->isChecked())   debugWindow::instance->ui->debugText->appendPlainText(msg);

}

void debugWindow::updateParameters(void){
    ui->param0_0->setText(QString("%1").arg((uchar) PROTOCOL->getParam(0).d[0]));
    ui->param1_0->setText(QString("%1").arg((uchar) PROTOCOL->getParam(1).d[0]));
    ui->param2_0->setText(QString("%1").arg((uchar) PROTOCOL->getParam(2).d[0]));
    ui->param3_0->setText(QString("%1").arg((uchar) PROTOCOL->getParam(3).d[0]));
    ui->param4_0->setText(QString("%1").arg((uchar) PROTOCOL->getParam(4).d[0]));
    ui->param4_1->setText(QString("%1").arg((uchar) PROTOCOL->getParam(4).d[1]));
    ui->param5_0->setText(QString("%1").arg((uchar) PROTOCOL->getParam(5).d[0]));
    ui->param5_1->setText(QString("%1").arg((uchar) PROTOCOL->getParam(5).d[1]));
    ui->param6_0->setText(QString("%1").arg((uchar) PROTOCOL->getParam(6).d[0]));
    ui->param6_1->setText(QString("%1").arg((uchar) PROTOCOL->getParam(6).d[1]));
    ui->param7_0->setText(QString("%1").arg((uchar) PROTOCOL->getParam(7).d[0]));
    ui->param7_1->setText(QString("%1").arg((uchar) PROTOCOL->getParam(7).d[1]));
    ui->param8_0->setText(QString("%1").arg((uchar) PROTOCOL->getParam(8).d[0]));
    ui->param8_1->setText(QString("%1").arg((uchar) PROTOCOL->getParam(8).d[1]));

};

void debugWindow::updateData(void){
    ui->data0_0->setChecked((PROTOCOL->getData(0).d[0] & 0x1));
    ui->data0_1->setChecked((PROTOCOL->getData(0).d[0] & 0x2));
    ui->data0_2->setChecked((PROTOCOL->getData(0).d[0] & 0x4));
    ui->data0_3->setChecked((PROTOCOL->getData(0).d[0] & 0x8));
    ui->data0_4->setChecked((PROTOCOL->getData(0).d[0] & 0x10));
    ui->data0_5->setChecked((PROTOCOL->getData(0).d[0] & 0x20));

    ui->data1_0->setChecked((PROTOCOL->getData(0).d[1] & 0x1));
    ui->data1_1->setChecked((PROTOCOL->getData(0).d[1] & 0x2));
    ui->data1_2->setChecked((PROTOCOL->getData(0).d[1] & 0x4));
    ui->data1_3->setChecked((PROTOCOL->getData(0).d[1] & 0x8));
    ui->data1_4->setChecked((PROTOCOL->getData(0).d[1] & 0x10));
    ui->data1_5->setChecked((PROTOCOL->getData(0).d[1] & 0x20));

}
void debugWindow::assignData(void){

    PROTOCOL->setData(0, 0, 0x1, ui->data0_0->isChecked());
    PROTOCOL->setData(0, 0, 0x2, ui->data0_1->isChecked());
    PROTOCOL->setData(0, 0, 0x4, ui->data0_2->isChecked());
    PROTOCOL->setData(0, 0, 0x8, ui->data0_3->isChecked());
    PROTOCOL->setData(0, 0, 0x10, ui->data0_4->isChecked());
    PROTOCOL->setData(0, 0, 0x10, ui->data0_5->isChecked());

    PROTOCOL->setData(0, 1, 0x1, ui->data1_0->isChecked());
    PROTOCOL->setData(0, 1, 0x2, ui->data1_1->isChecked());
    PROTOCOL->setData(0, 1, 0x4, ui->data1_2->isChecked());
    PROTOCOL->setData(0, 1, 0x8, ui->data1_3->isChecked());
    PROTOCOL->setData(0, 1, 0x10, ui->data1_4->isChecked());
    PROTOCOL->setData(0, 1, 0x10, ui->data1_5->isChecked());

}

void debugWindow::updateStatus(void){

    ui->status0_0->setChecked((PROTOCOL->getStatus(0).d[0] & 0x1));
    ui->status0_1->setChecked((PROTOCOL->getStatus(0).d[0] & 0x2));
    ui->status0_2->setChecked((PROTOCOL->getStatus(0).d[0] & 0x4));
    ui->status0_3->setChecked((PROTOCOL->getStatus(0).d[0] & 0x8));
    ui->status0_4->setChecked(PROTOCOL->getStatus(0).d[0] & 0x10);

    ui->status0_5->setChecked((PROTOCOL->getStatus(0).d[0] & 0x20));
    ui->status0_6->setChecked((PROTOCOL->getStatus(0).d[0] & 0x40));
    ui->status0_7->setChecked((PROTOCOL->getStatus(0).d[0] & 0x80));

    ui->status1_0->setChecked((PROTOCOL->getStatus(0).d[1] & 0x1));
    ui->status1_1->setChecked((PROTOCOL->getStatus(0).d[1] & 0x2));
    ui->status1_2->setChecked((PROTOCOL->getStatus(0).d[1] & 0x4));
    ui->status1_3->setChecked((PROTOCOL->getStatus(0).d[1] & 0x8));
    ui->status1_4->setChecked((PROTOCOL->getStatus(0).d[1] & 0x10));
    ui->status1_5->setChecked((PROTOCOL->getStatus(0).d[1] & 0x20));
    ui->status1_6->setChecked((PROTOCOL->getStatus(0).d[1] & 0x40));
    ui->status1_7->setChecked((PROTOCOL->getStatus(0).d[1] & 0x80));

    ui->status2_0->setChecked((PROTOCOL->getStatus(0).d[2] & 0x1));
    ui->status2_1->setChecked((PROTOCOL->getStatus(0).d[2] & 0x2));
    ui->status2_2->setChecked((PROTOCOL->getStatus(0).d[2] & 0x4));
    ui->status2_3->setChecked((PROTOCOL->getStatus(0).d[2] & 0x8));
    ui->status2_4->setChecked((PROTOCOL->getStatus(0).d[2] & 0x10));
    ui->status2_5->setChecked((PROTOCOL->getStatus(0).d[2] & 0x20));
    ui->status2_6->setChecked((PROTOCOL->getStatus(0).d[2] & 0x40));

    ui->status3_0->setChecked((PROTOCOL->getStatus(0).d[3] & 0x1));
    ui->status3_1->setChecked((PROTOCOL->getStatus(0).d[3] & 0x2));
    ui->status3_2->setChecked((PROTOCOL->getStatus(0).d[3] & 0x4));
    ui->status3_3->setChecked((PROTOCOL->getStatus(0).d[3] & 0x8));

    ui->batt1V->setText(QString("%1 (V)").arg((float) PROTOCOL->getStatus(1).d[0] * 0.1) );
    ui->batt2V->setText(QString("%1 (V)").arg((float) PROTOCOL->getStatus(1).d[0] * 0.1) );
}

void debugWindow::onSoftPoffButton(void){
    PROTOCOL->requestSoftPowerOff();
}
void debugWindow::onAbortButton(void){
    PROTOCOL->requestAbort();
}
