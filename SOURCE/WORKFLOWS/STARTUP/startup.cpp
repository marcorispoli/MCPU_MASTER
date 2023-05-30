#include "application.h"
#include "ui_startup.h"



startup::startup(int W, int H, QWidget *parent)
    : windowsClass(W, H, parent),  ui(new Ui::startupWindow)
{
    startup::instance = this;
    ui->setupUi(this);


}

startup::~startup()
{
    delete ui;
}

void startup::initWindow(void)
{

    // Sequence initialization
    startup_fase = _STARTUP_INIT;
    QTimer::singleShot(0,this, SLOT(startupFase()));

}

void startup::exitWindow(void)
{

}


void startup::startupFase(void)
{
    static unsigned int tmo = 0;

    switch(startup_fase){
    case _STARTUP_INIT:
        ui->main_process_start_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->package_revision->setText(QString("ID%1").arg(PKGCONFIG->getParam<QString>(PKG_ID, 0)));

        ui->compressor_revision->setText("__.__.__");
        ui->collimator_revision->setText("__.__.__");
        ui->filter_revision->setText("__.__.__");
        ui->motors_revision->setText("__.__.__");
        ui->generator_revision->setText("__.__.__");
        ui->potter_revision->setText("__.__.__");
        ui->can_process_revision->setText("__.__.__");


        ui->main_process_start_frame->show();
        ui->can_process_frame->hide();
        ui->power_service_frame->hide();
        ui->compressor_frame->hide();
        ui->collimator_frame->hide();
        ui->filter_frame->hide();
        ui->potter_frame->hide();
        ui->generator_frame->hide();
        ui->motors_frame->hide();

        startup_fase = _STARTUP_CAN_PROCESS;
        QTimer::singleShot(0,this, SLOT(startupFase()));
        break;

    case _STARTUP_CAN_PROCESS:
        ui->can_process_frame->show();
        ui->can_process_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        CANDRIVER->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_CAN_DRIVER,0), PKGCONFIG->getParam<uint>(PKG_MCPU_CAN_DRIVER,1));

        if(!masterInterface::startDriver(SYSCONFIG->getParam<QString>(SYS_CAN_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_CAN_PROCESS_PARAM,SYS_PROCESS_PARAM), this )){
            startup_fase = _STARTUP_CAN_PROCESS_ERROR;
            QTimer::singleShot(0,this, SLOT(startupFase()));
            return;
        }


        startup_fase = _STARTUP_CAN_PROCESS_CONNECTION;
        QTimer::singleShot(0,this, SLOT(startupFase()));
        break;

    case _STARTUP_CAN_PROCESS_CONNECTION:
        if(!CANDRIVER->isConnected()){
            tmo--;
            if(tmo==0) startup_fase = _STARTUP_CAN_PROCESS_ERROR;
            QTimer::singleShot(100,this, SLOT(startupFase()));
            return;
        }

        CANDRIVER->SEND_GET_REVISION();
        startup_fase = _STARTUP_CAN_PROCESS_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupFase()));
        break;

    case _STARTUP_CAN_PROCESS_REVISION:
        if(!CANDRIVER->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_fase = _STARTUP_CAN_PROCESS_ERROR;
            CANDRIVER->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupFase()));
            return;
        }


        ui->can_process_revision->setText(QString("%1.%2 [%4.%5]").arg(CANDRIVER->getMajRevision()).arg(CANDRIVER->getMinRevision()).arg(CANDRIVER->getMajPkgRevision()).arg(CANDRIVER->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!CANDRIVER->isValidRevision()){
           startup_fase = _STARTUP_CAN_PROCESS_ERROR;
           QTimer::singleShot(20,this, SLOT(startupFase()));
           return;
        }

        ui->can_process_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_fase = _STARTUP_SERVICE_POWER;
        QTimer::singleShot(0,this, SLOT(startupFase()));
        break;

    case _STARTUP_CAN_PROCESS_ERROR:
        ui->can_process_frame->setStyleSheet(ERROR_FRAME_BORDER );
        QTimer::singleShot(1000,this, SLOT(startupFase()));
        break;

    case _STARTUP_SERVICE_POWER:
        ui->power_service_frame->show();
        ui->power_service_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        POWERSERVICE->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_POWER_SERVICE,0), PKGCONFIG->getParam<uint>(PKG_MCPU_POWER_SERVICE,1));

        if(!masterInterface::startDriver(SYSCONFIG->getParam<QString>(SYS_POWERSERVICE_PROCESS_PARAM,SYS_PROCESS_NAME),SYSCONFIG->getParam<QString>(SYS_POWERSERVICE_PROCESS_PARAM,SYS_PROCESS_PARAM), this )){
            startup_fase = _STARTUP_SERVICE_POWER_ERROR;
            QTimer::singleShot(0,this, SLOT(startupFase()));
            return;
        }

        startup_fase = _STARTUP_SERVICE_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupFase()));
        break;

    case _STARTUP_SERVICE_CONNECTION:
        if(!POWERSERVICE->isConnected()){
            tmo--;
            if(tmo==0) startup_fase = _STARTUP_SERVICE_POWER_ERROR;
            QTimer::singleShot(100,this, SLOT(startupFase()));
            return;
        }

        POWERSERVICE->SEND_GET_REVISION();
        startup_fase = _STARTUP_SERVICE_POWER_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupFase()));
        break;

    case _STARTUP_SERVICE_POWER_REVISION:
        if(!POWERSERVICE->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_fase = _STARTUP_SERVICE_POWER_ERROR;
            POWERSERVICE->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupFase()));
            return;
        }


        ui->power_sevice_revision->setText(QString("%1.%2 [%4.%5]").arg(POWERSERVICE->getMajRevision()).arg(POWERSERVICE->getMinRevision()).arg(POWERSERVICE->getMajPkgRevision()).arg(POWERSERVICE->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!POWERSERVICE->isValidRevision()){
           startup_fase = _STARTUP_SERVICE_POWER_ERROR;
           QTimer::singleShot(20,this, SLOT(startupFase()));
           return;
        }

        ui->power_service_frame->setStyleSheet(CORRECT_FRAME_BORDER );        
        startup_fase = _STARTUP_COLLIMATOR;
        QTimer::singleShot(0,this, SLOT(startupFase()));
        break;

    case _STARTUP_SERVICE_POWER_ERROR:
        ui->power_service_frame->setStyleSheet(ERROR_FRAME_BORDER );
        QTimer::singleShot(0,this, SLOT(startupFase()));
        startup_fase = _STARTUP_COLLIMATOR;
        break;

    case _STARTUP_COLLIMATOR:
        ui->collimator_frame->show();
        ui->collimator_revision->setText("__.__.__");
        ui->collimator_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        startup_fase = _STARTUP_COLLIMATOR_REVISION;
        tmo = 50;
        //QTimer::singleShot(100,this, SLOT(startupFase()));
        break;
    }
}
