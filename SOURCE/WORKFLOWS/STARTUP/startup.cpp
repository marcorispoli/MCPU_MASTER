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
    startup_process_fase = _STARTUP_PROCESS_INIT;
    QTimer::singleShot(0,this, SLOT(startupProcesses()));

    ui->window_title_label->setStyleSheet(NO_BACKGROUND_IMAGE + BACKGROUND_COLOR(TRANSPARENT) + BOTTOM_BORDER(1,CTITLE) + FONT_I(700,30) + COLOR(CTITLE));
    ui->installed_package_frame->setStyleSheet(NO_BACKGROUND_IMAGE + BACKGROUND_COLOR(TRANSPARENT) + NO_BORDER + FONT_I(100,20) + COLOR(CTEXT));

    ui->device_startup_frame->setGeometry(40,210,521,771);
    ui->device_startup_frame->hide();
    ui->process_startup_frame->setGeometry(40,210,521,771);
    ui->process_startup_frame->hide();
}

void startup::exitWindow(void)
{

}

#define HIGHLIGHT_FRAME_BORDER  NO_BACKGROUND_IMAGE +  BACKGROUND_COLOR(CTITLE) + BORDER(1,CTITLE) + FONT_N(100,19) +  COLOR(CREVTEXT)
#define CORRECT_FRAME_BORDER    NO_BACKGROUND_IMAGE + BACKGROUND_COLOR(TRANSPARENT) + BORDER(1,COK) + FONT_N(100,19) + COLOR(CTEXT)
#define ERROR_FRAME_BORDER      NO_BACKGROUND_IMAGE + BACKGROUND_COLOR(TRANSPARENT) + BORDER(1,CERROR) + FONT_N(100,19) + COLOR(CTEXT)
void startup::startupProcesses(void)
{
    static unsigned int tmo = 0;
    static bool process_start_result;

    switch(startup_process_fase){
    case _STARTUP_PROCESS_INIT:

        process_start_result = true;
        ui->process_startup_frame->show();
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

        startup_process_fase = _STARTUP_CAN_PROCESS;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_CAN_PROCESS:
        ui->can_process_frame->show();
        ui->can_process_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        CANDRIVER->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_CAN_DRIVER,0), PKGCONFIG->getParam<uint>(PKG_MCPU_CAN_DRIVER,1));

        if(!CANDRIVER->startDriver()){
            startup_process_fase = _STARTUP_CAN_PROCESS_ERROR;
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }
        CANDRIVER->Start();

        startup_process_fase = _STARTUP_CAN_PROCESS_CONNECTION;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_CAN_PROCESS_CONNECTION:
        if(!CANDRIVER->isConnected()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_CAN_PROCESS_ERROR;
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        CANDRIVER->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_CAN_PROCESS_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_CAN_PROCESS_REVISION:
        if(!CANDRIVER->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_CAN_PROCESS_ERROR;
            CANDRIVER->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->can_process_revision->setText(QString("%1.%2 [%4.%5]").arg(CANDRIVER->getMajRevision()).arg(CANDRIVER->getMinRevision()).arg(CANDRIVER->getMajPkgRevision()).arg(CANDRIVER->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!CANDRIVER->isValidRevision()){
           startup_process_fase = _STARTUP_CAN_PROCESS_ERROR;
           QTimer::singleShot(20,this, SLOT(startupProcesses()));
           return;
        }

        ui->can_process_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_process_fase = _STARTUP_SERVICE_POWER;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_CAN_PROCESS_ERROR:
        ui->can_process_frame->setStyleSheet(ERROR_FRAME_BORDER );
        process_start_result = false;
        QTimer::singleShot(1000,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_SERVICE_POWER:
        ui->power_service_frame->show();
        ui->power_service_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        POWERSERVICE->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_POWER_SERVICE,0), PKGCONFIG->getParam<uint>(PKG_MCPU_POWER_SERVICE,1));

        if(!POWERSERVICE->startDriver()){
            startup_process_fase = _STARTUP_SERVICE_POWER_ERROR;
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }
        POWERSERVICE->Start();

        startup_process_fase = _STARTUP_SERVICE_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_SERVICE_CONNECTION:
        if(!POWERSERVICE->isConnected()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_SERVICE_POWER_ERROR;
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        POWERSERVICE->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_SERVICE_POWER_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_SERVICE_POWER_REVISION:
        if(!POWERSERVICE->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_SERVICE_POWER_ERROR;
            POWERSERVICE->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->power_sevice_revision->setText(QString("%1.%2 [%4.%5]").arg(POWERSERVICE->getMajRevision()).arg(POWERSERVICE->getMinRevision()).arg(POWERSERVICE->getMajPkgRevision()).arg(POWERSERVICE->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!POWERSERVICE->isValidRevision()){
           startup_process_fase = _STARTUP_SERVICE_POWER_ERROR;
           QTimer::singleShot(20,this, SLOT(startupProcesses()));
           return;
        }

        ui->power_service_frame->setStyleSheet(CORRECT_FRAME_BORDER );        
        startup_process_fase = _STARTUP_COMPRESSOR;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_SERVICE_POWER_ERROR:
        ui->power_service_frame->setStyleSheet(ERROR_FRAME_BORDER );
        process_start_result = false;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        startup_process_fase = _STARTUP_COMPRESSOR;
        break;


    case _STARTUP_COMPRESSOR:
        ui->compressor_frame->show();
        ui->compressor_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        COMPRESSOR->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_COMPRESSOR,0), PKGCONFIG->getParam<uint>(PKG_MCPU_COMPRESSOR,1));

        if(!COMPRESSOR->startDriver()){
            startup_process_fase = _STARTUP_COMPRESSOR_ERROR;
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }
        COMPRESSOR->Start();

        startup_process_fase = _STARTUP_COMPRESSOR_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COMPRESSOR_CONNECTION:
        if(!COMPRESSOR->isConnected()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_COMPRESSOR_ERROR;
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        COMPRESSOR->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_COMPRESSOR_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COMPRESSOR_REVISION:
        if(!COMPRESSOR->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_COMPRESSOR_ERROR;
            COMPRESSOR->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->compressor_revision->setText(QString("%1.%2 [%4.%5]").arg(COMPRESSOR->getMajRevision()).arg(COMPRESSOR->getMinRevision()).arg(COMPRESSOR->getMajPkgRevision()).arg(COMPRESSOR->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!COMPRESSOR->isValidRevision()){
           startup_process_fase = _STARTUP_COMPRESSOR_ERROR;
           QTimer::singleShot(20,this, SLOT(startupProcesses()));
           return;
        }

        ui->compressor_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_process_fase = _STARTUP_COLLIMATOR;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COMPRESSOR_ERROR:
        ui->compressor_frame->setStyleSheet(ERROR_FRAME_BORDER );
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
         process_start_result = false;
        startup_process_fase = _STARTUP_COLLIMATOR;
        break;


    case _STARTUP_COLLIMATOR:
        ui->collimator_frame->show();
        ui->collimator_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        COLLIMATOR->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_COLLIMATOR,0), PKGCONFIG->getParam<uint>(PKG_MCPU_COLLIMATOR,1));

        if(!COLLIMATOR->startDriver()){
            startup_process_fase = _STARTUP_COLLIMATOR_ERROR;
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }
        COLLIMATOR->Start();

        startup_process_fase = _STARTUP_COLLIMATOR_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COLLIMATOR_CONNECTION:
        if(!COLLIMATOR->isConnected()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_COLLIMATOR_ERROR;
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        COLLIMATOR->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_COLLIMATOR_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COLLIMATOR_REVISION:
        if(!COLLIMATOR->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_COLLIMATOR_ERROR;
            COLLIMATOR->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->collimator_revision->setText(QString("%1.%2 [%4.%5]").arg(COLLIMATOR->getMajRevision()).arg(COLLIMATOR->getMinRevision()).arg(COLLIMATOR->getMajPkgRevision()).arg(COLLIMATOR->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!COLLIMATOR->isValidRevision()){
           startup_process_fase = _STARTUP_COLLIMATOR_ERROR;
           QTimer::singleShot(20,this, SLOT(startupProcesses()));
           return;
        }

        ui->collimator_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_process_fase = _STARTUP_FILTER;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COLLIMATOR_ERROR:
        ui->collimator_frame->setStyleSheet(ERROR_FRAME_BORDER );
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
         process_start_result = false;
        startup_process_fase = _STARTUP_FILTER;
        break;


    case _STARTUP_FILTER:
        ui->filter_frame->show();
        ui->filter_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        FILTER->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_FILTER,0), PKGCONFIG->getParam<uint>(PKG_MCPU_FILTER,1));

        if(!FILTER->startDriver()){
            startup_process_fase = _STARTUP_FILTER_ERROR;
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }
        FILTER->Start();

        startup_process_fase = _STARTUP_FILTER_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_FILTER_CONNECTION:
        if(!FILTER->isConnected()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_FILTER_ERROR;
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        FILTER->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_FILTER_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_FILTER_REVISION:
        if(!FILTER->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_FILTER_ERROR;
            FILTER->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->filter_revision->setText(QString("%1.%2 [%4.%5]").arg(FILTER->getMajRevision()).arg(FILTER->getMinRevision()).arg(FILTER->getMajPkgRevision()).arg(FILTER->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!FILTER->isValidRevision()){
           startup_process_fase = _STARTUP_FILTER_ERROR;
           QTimer::singleShot(20,this, SLOT(startupProcesses()));
           return;
        }

        ui->filter_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_process_fase = _STARTUP_POTTER;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_FILTER_ERROR:
        ui->filter_frame->setStyleSheet(ERROR_FRAME_BORDER );
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        startup_process_fase = _STARTUP_POTTER;
         process_start_result = false;
        break;




    case _STARTUP_POTTER:
        ui->potter_frame->show();
        ui->potter_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        POTTER->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_POTTER,0), PKGCONFIG->getParam<uint>(PKG_MCPU_POTTER,1));

        if(!POTTER->startDriver()){
            startup_process_fase = _STARTUP_POTTER_ERROR;
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }
        POTTER->Start();

        startup_process_fase = _STARTUP_POTTER_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_POTTER_CONNECTION:
        if(!POTTER->isConnected()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_POTTER_ERROR;
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        POTTER->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_POTTER_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_POTTER_REVISION:
        if(!POTTER->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_POTTER_ERROR;
            POTTER->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->potter_revision->setText(QString("%1.%2 [%4.%5]").arg(POTTER->getMajRevision()).arg(POTTER->getMinRevision()).arg(POTTER->getMajPkgRevision()).arg(POTTER->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!POTTER->isValidRevision()){
           startup_process_fase = _STARTUP_POTTER_ERROR;
           QTimer::singleShot(20,this, SLOT(startupProcesses()));
           return;
        }

        ui->potter_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_process_fase = _STARTUP_MOTORS;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_POTTER_ERROR:
        ui->potter_frame->setStyleSheet(ERROR_FRAME_BORDER );
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        startup_process_fase = _STARTUP_MOTORS;
         process_start_result = false;
        break;


    case _STARTUP_MOTORS:
        ui->motors_frame->show();
        ui->motors_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        MOTORS->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_MOTORS,0), PKGCONFIG->getParam<uint>(PKG_MCPU_MOTORS,1));

        if(!MOTORS->startDriver()){
            startup_process_fase = _STARTUP_MOTORS_ERROR;
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }
        MOTORS->Start();

        startup_process_fase = _STARTUP_MOTORS_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_MOTORS_CONNECTION:
        if(!MOTORS->isConnected()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_MOTORS_ERROR;
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        MOTORS->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_MOTORS_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_MOTORS_REVISION:
        if(!MOTORS->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_MOTORS_ERROR;
            MOTORS->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }

        ui->motors_revision->setText(QString("%1.%2 [%4.%5]").arg(MOTORS->getMajRevision()).arg(MOTORS->getMinRevision()).arg(MOTORS->getMajPkgRevision()).arg(MOTORS->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!MOTORS->isValidRevision()){
           startup_process_fase = _STARTUP_MOTORS_ERROR;
           QTimer::singleShot(20,this, SLOT(startupProcesses()));
           return;
        }

        ui->motors_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_process_fase = _STARTUP_GENERATOR;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_MOTORS_ERROR:
        ui->motors_frame->setStyleSheet(ERROR_FRAME_BORDER );
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        startup_process_fase = _STARTUP_GENERATOR;
         process_start_result = false;
        break;


    case _STARTUP_GENERATOR:
        ui->generator_frame->show();
        ui->generator_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the packaga revision to the driver
        GENERATOR->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_GENERATOR,0), PKGCONFIG->getParam<uint>(PKG_MCPU_GENERATOR,1));

        if(!GENERATOR->startDriver()){
            startup_process_fase = _STARTUP_GENERATOR_ERROR;
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }
        GENERATOR->Start();

        startup_process_fase = _STARTUP_GENERATOR_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_GENERATOR_CONNECTION:
        if(!GENERATOR->isConnected()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_GENERATOR_ERROR;
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        GENERATOR->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_GENERATOR_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_GENERATOR_REVISION:
        if(!GENERATOR->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_GENERATOR_ERROR;
            GENERATOR->SEND_GET_REVISION();
            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }

        ui->generator_revision->setText(QString("%1.%2 [%4.%5]").arg(GENERATOR->getMajRevision()).arg(GENERATOR->getMinRevision()).arg(GENERATOR->getMajPkgRevision()).arg(GENERATOR->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!GENERATOR->isValidRevision()){
           startup_process_fase = _STARTUP_GENERATOR_ERROR;
           QTimer::singleShot(20,this, SLOT(startupProcesses()));
           return;
        }

        ui->generator_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_process_fase = _STARTUP_PROCESS_TERMINATED;
        QTimer::singleShot(2000,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_GENERATOR_ERROR:
        ui->generator_frame->setStyleSheet(ERROR_FRAME_BORDER );
        startup_process_fase = _STARTUP_PROCESS_TERMINATED;
        process_start_result = false;
        QTimer::singleShot(2000,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_PROCESS_TERMINATED:
        if(process_start_result) {
           qDebug() << "STARTUP PROCESSES SUCCESS!";
           ui->process_startup_frame->hide();
           startup_devices_fase = _STARTUP_DEVICES_INIT;
           QTimer::singleShot(0,this, SLOT(startupDevices()));
           return;
        }

        qDebug() << "STARTUP PROCESSES FAILED!";
        startup_process_fase = _STARTUP_PROCESS_ERROR;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_PROCESS_ERROR:
        break;
    }
}

void startup::startupDevices(void)
{
    static unsigned int tmo = 0;
    static bool device_startup_result;

    switch(startup_devices_fase){

    case _STARTUP_DEVICES_INIT:
        device_startup_result = true;
        ui->device_startup_frame->show();
        ui->main_device_start_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->main_device_start_frame->show();

        ui->can_device_frame->hide();
        ui->powerservice_device_frame->hide();
        ui->compressor_device_frame->hide();
        ui->collimator_device_frame->hide();
        ui->filter_device_frame->hide();
        ui->potter_device_frame->hide();
        ui->generator_device_frame->hide();

        startup_devices_fase = _STARTUP_DEVICES_CAN_CONVERTER;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICES_CAN_CONVERTER:

        ui->can_device_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->can_device_frame->show();
        ui->can_device_converter_revision->hide();

        CANDRIVER->SEND_GET_STATUS();
        tmo = 10;
        startup_devices_fase = _STARTUP_DEVICE_CAN_STATUS;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_CAN_STATUS:
        if(!CANDRIVER->isStatusReceived()){
            tmo--;
            if(tmo==0) startup_devices_fase = _STARTUP_DEVICE_CAN_ERROR;
            else CANDRIVER->SEND_GET_STATUS();
            QTimer::singleShot(20,this, SLOT(startupDevices()));
            return;
        }

        if(!CANDRIVER->isDeviceOpen()){
            startup_devices_fase = _STARTUP_DEVICE_CAN_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }

        ui->can_device_converter_revision->setText(QString("API: %1.%2.%3").arg(CANDRIVER->getConverterApiMaj()).arg(CANDRIVER->getConverterApiMin()).arg(CANDRIVER->getConverterApiSub())  );
        ui->can_device_converter_revision->show();
        ui->can_device_frame->setStyleSheet(CORRECT_FRAME_BORDER );

        startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_CAN_ERROR:
        ui->can_device_frame->setStyleSheet(ERROR_FRAME_BORDER );
        device_startup_result = false;
        startup_devices_fase = _STARTUP_DEVICES_ERROR;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_POWERSERVICE:
        ui->powerservice_device_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->powerservice_device_frame->show();
        ui->powerservice_device_revision->hide();

        // Assignes the packaga revision to the driver
        //POWERSERVICE->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_POWER_SERVICE,0), PKGCONFIG->getParam<uint>(PKG_MCPU_POWER_SERVICE,1));
        startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE_BOARDINIT;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_POWERSERVICE_BOARDINIT:
        if(!POWERSERVICE->isBoardInitialized()){
            tmo--;
            if(tmo==0) startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE_ERROR;
            else POWERSERVICE->SEND_BOARD_INIT();
            QTimer::singleShot(500,this, SLOT(startupDevices()));
            return;
        }

        if(POWERSERVICE->getBootloaderError()){
            startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }

        if(POWERSERVICE->isBootloaderRunning()){
            startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }


       ui->powerservice_device_revision->setText(QString("%1.%2 [%4.%5]").arg(POWERSERVICE->getBoardAppMaj()) \
                                                 .arg(POWERSERVICE->getBoardAppMin()) \
                                                 .arg(0).arg(0)  );
       ui->powerservice_device_revision->show();

       // Test the expected release and the actual release
       /*
        if(!POWERSERVICE->isValidRevision()){
           startup_process_fase = _STARTUP_SERVICE_POWER_ERROR;
           QTimer::singleShot(20,this, SLOT(startupProcesses()));
           return;
        }*/
        ui->powerservice_device_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_COMPRESSOR;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_POWERSERVICE_ERROR:
        ui->powerservice_device_frame->setStyleSheet(ERROR_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_COMPRESSOR;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_COMPRESSOR:
        ui->compressor_device_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->compressor_device_frame->show();
        ui->compressor_device_revision->hide();
        startup_devices_fase = _STARTUP_DEVICES_TERMINATED;
        QTimer::singleShot(0,this, SLOT(startupDevices()));

    break;




    case _STARTUP_DEVICES_TERMINATED:

        if(device_startup_result) {
           qDebug() << "STARTUP DEVICE SUCCESS!";
           ui->device_startup_frame->hide();
           startup_devices_fase = _STARTUP_DEVICES_ERROR;
           QTimer::singleShot(0,this, SLOT(startupDevices()));
           return;
        }

        qDebug() << "STARTUP DEVICE FAILED!";
        startup_devices_fase = _STARTUP_DEVICES_ERROR;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICES_ERROR:
        break;
    }



}
