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
    QTimer::singleShot(0,this, SLOT(startupProcesses()));

    ui->window_title_label->setStyleSheet(NO_BACKGROUND_IMAGE + BACKGROUND_COLOR(TRANSPARENT) + BOTTOM_BORDER(1,CTITLE) + FONT_I(700,30) + COLOR(CTITLE));
    ui->installed_package_frame->setStyleSheet(NO_BACKGROUND_IMAGE + BACKGROUND_COLOR(TRANSPARENT) + NO_BORDER + FONT_I(100,20) + COLOR(CTEXT));

    ui->device_startup_frame->setGeometry(40,210,521,771);
    ui->device_startup_frame->hide();
    ui->process_startup_frame->setGeometry(40,210,521,771);
    ui->process_startup_frame->hide();
    ui->motors_startup_frame->setGeometry(40,210,521,771);
    ui->motors_startup_frame->hide();
}

void startup::exitWindow(void)
{

}

#define HIGHLIGHT_FRAME_BORDER  NO_BACKGROUND_IMAGE +  BACKGROUND_COLOR(CTITLE) + BORDER(1,CTITLE) + FONT_N(100,19) +  COLOR(CREVTEXT)
#define CORRECT_FRAME_BORDER    NO_BACKGROUND_IMAGE + BACKGROUND_COLOR(TRANSPARENT) + BORDER(1,COK) + FONT_N(100,19) + COLOR(CTEXT)
#define ERROR_FRAME_BORDER      NO_BACKGROUND_IMAGE + BACKGROUND_COLOR(TRANSPARENT) + BORDER(1,CERROR) + FONT_N(100,19) + COLOR(CTEXT)

typedef enum{
    _STARTUP_MOTORS_DEVICE_INIT,
    _STARTUP_MOTORS_DEVICE_TRX_INIT,
    _STARTUP_MOTORS_DEVICE_TRX_INITIALIZE,
    _STARTUP_MOTORS_DEVICE_SLIDE_INIT,
    _STARTUP_MOTORS_DEVICE_SLIDE_INITIALIZE,
    _STARTUP_MOTORS_DEVICE_CARM_INIT,
    _STARTUP_MOTORS_DEVICE_CARM_INITIALIZE,
    _STARTUP_MOTORS_DEVICE_BODY_INIT,
    _STARTUP_MOTORS_DEVICE_BODY_INITIALIZE,
    _STARTUP_MOTORS_DEVICE_MVERT_INIT,
    _STARTUP_MOTORS_DEVICE_MVERT_INITIALIZE,

    _STARTUP_MOTORS_DEVICE_TERMINATED,
    _STARTUP_MOTORS_DEVICE_ERROR
}startup_motors_enum;
void startup::startupMotors(void)
{
    static unsigned int tmo = 0;
    static bool startup_motors_result;
    static startup_motors_enum startup_motors_fase =  _STARTUP_MOTORS_DEVICE_INIT;


    switch(startup_motors_fase){

    case _STARTUP_MOTORS_DEVICE_INIT:        
        // GUI initialization
        startup_motors_result = true;
        ui->motors_startup_frame->show();
        ui->main_motorse_start_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->main_motorse_start_frame->show();

        ui->trx_motor_frame->hide();
        ui->slide_motor_frame->hide();
        ui->body_motor_frame->hide();
        ui->mvert_motor_frame->hide();
        ui->carm_motor_frame->hide();

        ui->trx_setup_label->hide();
        ui->slide_setup_label->hide();
        ui->mvert_setup_label->hide();
        ui->body_setup_label->hide();
        ui->carm_setup_label->hide();

        startup_motors_fase = _STARTUP_MOTORS_DEVICE_TRX_INIT;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        return;

    case _STARTUP_MOTORS_DEVICE_TRX_INIT:
        ui->trx_motor_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->trx_motor_frame->show();
        startup_motors_fase = _STARTUP_MOTORS_DEVICE_TRX_INITIALIZE;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        return;

    case _STARTUP_MOTORS_DEVICE_TRX_INITIALIZE:
        if(!MOTORS->isTrxInitialized()){
            tmo--;
            if(tmo==0){
                startup_motors_result = false;
                startup_motors_fase = _STARTUP_MOTORS_DEVICE_SLIDE_INIT;
                ui->trx_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
                sysErrors::addlog(SYSERR_MOTORS_TRX_INITIALIZATION);
            } else MOTORS->SEND_TRX_INITIALIZE();
            QTimer::singleShot(500,this, SLOT(startupMotors()));
            return;
        }

        if(MOTORS->isTrxInitError()){
            startup_motors_result = false;
            ui->trx_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
            sysErrors::addlog(SYSERR_MOTORS_TRX_INITIALIZATION);
        }else{
            ui->trx_motor_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        }

        startup_motors_fase = _STARTUP_MOTORS_DEVICE_SLIDE_INIT;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        break;

    case _STARTUP_MOTORS_DEVICE_SLIDE_INIT:
        ui->slide_motor_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->slide_motor_frame->show();
        startup_motors_fase = _STARTUP_MOTORS_DEVICE_SLIDE_INITIALIZE;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        return;

    case _STARTUP_MOTORS_DEVICE_SLIDE_INITIALIZE:
        if(!MOTORS->isSlideInitialized()){
            tmo--;
            if(tmo==0){
                startup_motors_result = false;
                startup_motors_fase = _STARTUP_MOTORS_DEVICE_BODY_INIT;
                ui->slide_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
                sysErrors::addlog(SYSERR_MOTORS_SLIDE_INITIALIZATION);
            } else MOTORS->SEND_SLIDE_INITIALIZE();
            QTimer::singleShot(500,this, SLOT(startupMotors()));
            return;
        }

        if(MOTORS->isSlideInitError()){
            startup_motors_result = false;
            ui->slide_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
            sysErrors::addlog(SYSERR_MOTORS_SLIDE_INITIALIZATION);
        }else{
            ui->slide_motor_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        }

        startup_motors_fase = _STARTUP_MOTORS_DEVICE_BODY_INIT;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        break;

    case _STARTUP_MOTORS_DEVICE_BODY_INIT:
        ui->body_motor_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->body_motor_frame->show();
        startup_motors_fase = _STARTUP_MOTORS_DEVICE_BODY_INITIALIZE;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        return;

    case _STARTUP_MOTORS_DEVICE_BODY_INITIALIZE:
        if(!MOTORS->isBodyInitialized()){
            tmo--;
            if(tmo==0){
                startup_motors_result = false;
                startup_motors_fase = _STARTUP_MOTORS_DEVICE_MVERT_INIT;
                ui->body_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
                sysErrors::addlog(SYSERR_MOTORS_BODY_INITIALIZATION);
            } else MOTORS->SEND_BODY_INITIALIZE();
            QTimer::singleShot(500,this, SLOT(startupMotors()));
            return;
        }

        if(MOTORS->isBodyInitError()){
            startup_motors_result = false;
            ui->body_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
            sysErrors::addlog(SYSERR_MOTORS_BODY_INITIALIZATION);
        }else{
            ui->body_motor_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        }

        startup_motors_fase = _STARTUP_MOTORS_DEVICE_MVERT_INIT;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        break;

    case _STARTUP_MOTORS_DEVICE_MVERT_INIT:
        ui->mvert_motor_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->mvert_motor_frame->show();
        startup_motors_fase = _STARTUP_MOTORS_DEVICE_MVERT_INITIALIZE;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        return;

    case _STARTUP_MOTORS_DEVICE_MVERT_INITIALIZE:
        if(!MOTORS->isMvertInitialized()){
            tmo--;
            if(tmo==0){
                startup_motors_result = false;
                startup_motors_fase = _STARTUP_MOTORS_DEVICE_CARM_INIT;
                ui->mvert_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
                sysErrors::addlog(SYSERR_MOTORS_MVERT_INITIALIZATION);
            } else MOTORS->SEND_MVERT_INITIALIZE();
            QTimer::singleShot(500,this, SLOT(startupMotors()));
            return;
        }

        if(MOTORS->isMvertInitError()){
            startup_motors_result = false;
            ui->mvert_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
            sysErrors::addlog(SYSERR_MOTORS_MVERT_INITIALIZATION);
        }else{
            ui->mvert_motor_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        }

        startup_motors_fase = _STARTUP_MOTORS_DEVICE_CARM_INIT;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        break;


    case _STARTUP_MOTORS_DEVICE_CARM_INIT:
        ui->carm_motor_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->carm_motor_frame->show();
        startup_motors_fase = _STARTUP_MOTORS_DEVICE_CARM_INITIALIZE;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        return;

    case _STARTUP_MOTORS_DEVICE_CARM_INITIALIZE:
        if(!MOTORS->isCarmInitialized()){
            tmo--;
            if(tmo==0){
                startup_motors_result = false;
                startup_motors_fase = _STARTUP_MOTORS_DEVICE_TERMINATED;
                ui->mvert_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
                sysErrors::addlog(SYSERR_MOTORS_CARM_INITIALIZATION);
            } else MOTORS->SEND_CARM_INITIALIZE();
            QTimer::singleShot(500,this, SLOT(startupMotors()));
            return;
        }

        if(MOTORS->isCarmInitError()){
            startup_motors_result = false;
            ui->carm_motor_frame->setStyleSheet(ERROR_FRAME_BORDER );
            sysErrors::addlog(SYSERR_MOTORS_CARM_INITIALIZATION);
        }else{
            ui->carm_motor_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        }

        startup_motors_fase = _STARTUP_MOTORS_DEVICE_TERMINATED;
        QTimer::singleShot(0,this, SLOT(startupMotors()));
        break;

    case _STARTUP_MOTORS_DEVICE_TERMINATED:
        if(!startup_motors_result){
            startup_motors_fase = _STARTUP_MOTORS_DEVICE_ERROR;
            QTimer::singleShot(0,this, SLOT(startupMotors()));
            return;
        }

        qDebug() << "STARTUP MOTORS SUCCESS";
        startup_motors_fase = _STARTUP_MOTORS_DEVICE_INIT;
        return;

    case _STARTUP_MOTORS_DEVICE_ERROR:
        startup_motors_fase = _STARTUP_MOTORS_DEVICE_INIT;
        return;
    }
}

typedef enum{
    _STARTUP_PROCESS_INIT,
    _STARTUP_CAN_PROCESS,
    _STARTUP_CAN_PROCESS_CONNECTION,
    _STARTUP_CAN_PROCESS_REVISION,
    _STARTUP_CAN_PROCESS_ERROR,
    _STARTUP_SERVICE_POWER,
    _STARTUP_SERVICE_CONNECTION,
    _STARTUP_SERVICE_POWER_REVISION,
    _STARTUP_SERVICE_POWER_ERROR,
    _STARTUP_COMPRESSOR,
    _STARTUP_COMPRESSOR_CONNECTION,
    _STARTUP_COMPRESSOR_REVISION,
    _STARTUP_COMPRESSOR_ERROR,
    _STARTUP_COLLIMATOR,
    _STARTUP_COLLIMATOR_CONNECTION,
    _STARTUP_COLLIMATOR_REVISION,
    _STARTUP_COLLIMATOR_ERROR,
    _STARTUP_FILTER,
    _STARTUP_FILTER_CONNECTION,
    _STARTUP_FILTER_REVISION,
    _STARTUP_FILTER_ERROR,
    _STARTUP_POTTER,
    _STARTUP_POTTER_CONNECTION,
    _STARTUP_POTTER_REVISION,
    _STARTUP_POTTER_ERROR,
    _STARTUP_MOTORS,
    _STARTUP_MOTORS_CONNECTION,
    _STARTUP_MOTORS_REVISION,
    _STARTUP_MOTORS_ERROR,
    _STARTUP_GENERATOR,
    _STARTUP_GENERATOR_CONNECTION,
    _STARTUP_GENERATOR_REVISION,
    _STARTUP_GENERATOR_ERROR,
    _STARTUP_PROCESS_TERMINATED,
    _STARTUP_PROCESS_ERROR
}startup_process_enum;
void startup::startupProcesses(void)
{
    static unsigned int tmo = 0;
    static bool process_start_result;
    static startup_process_enum startup_process_fase = _STARTUP_PROCESS_INIT;

    switch(startup_process_fase){

    case _STARTUP_PROCESS_INIT:
        // Initialize all the GUI items

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
        // Starts the CAN DRIVER process

        ui->can_process_frame->show();
        ui->can_process_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the package revision to the driver
        CANDRIVER->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_CAN_DRIVER,0), PKGCONFIG->getParam<uint>(PKG_MCPU_CAN_DRIVER,1));

        // Start the CAN DRIVER process
        if(!CANDRIVER->startDriver()){
            startup_process_fase = _STARTUP_CAN_PROCESS_ERROR;
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }

        // Starts the ethernet connection with the CAN DRIVER process
        CANDRIVER->Start();
        tmo = 100;
        startup_process_fase = _STARTUP_CAN_PROCESS_CONNECTION;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_CAN_PROCESS_CONNECTION:
        // Waits the connection with the CAN DRIVER
        if(!CANDRIVER->isConnected()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_CAN_PROCESS_ERROR;
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        // Request the CAN driver process revision
        CANDRIVER->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_CAN_PROCESS_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_CAN_PROCESS_REVISION:
        // Wait for the revision reception: try several times
        if(!CANDRIVER->isReceivedRevision()){
            tmo--;
            if(tmo==0) startup_process_fase = _STARTUP_CAN_PROCESS_ERROR;
            else CANDRIVER->SEND_GET_REVISION();
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

        // If the revision is OK proceed with the next startup process
        ui->can_process_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_process_fase = _STARTUP_SERVICE_POWER;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_CAN_PROCESS_ERROR:
        sysErrors::addlog(SYSERR_STARTUP_CAN);
        startup_process_fase = _STARTUP_PROCESS_TERMINATED;
        ui->can_process_frame->setStyleSheet(ERROR_FRAME_BORDER );
        process_start_result = false;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_SERVICE_POWER:
        ui->power_service_frame->show();
        ui->power_service_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER );

        // Assignes the package revision to the driver
        POWERSERVICE->setPkgRevision(PKGCONFIG->getParam<uint>(PKG_MCPU_POWER_SERVICE,0), PKGCONFIG->getParam<uint>(PKG_MCPU_POWER_SERVICE,1));

        // Activate the Powerservice process
        if(!POWERSERVICE->startDriver()){
            startup_process_fase = _STARTUP_SERVICE_POWER_ERROR;
            sysErrors::addlog(SYSERR_STARTUP_POWERSERVICE_PROCESS);
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }

        // Activate the communication with the process
        POWERSERVICE->Start();

        startup_process_fase = _STARTUP_SERVICE_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_SERVICE_CONNECTION:        
        if(!POWERSERVICE->isConnected()){
            tmo--;
            if(tmo==0){
                startup_process_fase = _STARTUP_SERVICE_POWER_ERROR;
                sysErrors::addlog(SYSERR_POWERSERVICE_CANDRIVER_CONNECTION);
            }
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        // Request the process revision
        POWERSERVICE->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_SERVICE_POWER_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_SERVICE_POWER_REVISION:
        if(!POWERSERVICE->isReceivedRevision()){
            tmo--;
            if(tmo) POWERSERVICE->SEND_GET_REVISION();
            else{
                // timeout event
                startup_process_fase = _STARTUP_SERVICE_POWER_ERROR;
                sysErrors::addlog(SYSERR_POWERSERVICE_PROCESS_REVISION_REQ);
            }
            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->power_sevice_revision->setText(QString("%1.%2 [%4.%5]").arg(POWERSERVICE->getMajRevision()).arg(POWERSERVICE->getMinRevision()).arg(POWERSERVICE->getMajPkgRevision()).arg(POWERSERVICE->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!POWERSERVICE->isValidRevision()){
           sysErrors::addlog(SYSERR_POWERSERVICE_PROCESS_WRONG_REVISION);
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

        // Activate the Process
        if(!COMPRESSOR->startDriver()){
            startup_process_fase = _STARTUP_COMPRESSOR_ERROR;
            sysErrors::addlog(SYSERR_STARTUP_COMPRESSOR_PROCESS);
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }

        // Activate the communication with the process
        COMPRESSOR->Start();

        startup_process_fase = _STARTUP_COMPRESSOR_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COMPRESSOR_CONNECTION:
        if(!COMPRESSOR->isConnected()){
            tmo--;
            if(tmo==0){
                startup_process_fase = _STARTUP_COMPRESSOR_ERROR;
                sysErrors::addlog(SYSERR_COMPRESSOR_CANDRIVER_CONNECTION);
            }
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        // Request the process revision
        COMPRESSOR->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_COMPRESSOR_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COMPRESSOR_REVISION:
        if(!COMPRESSOR->isReceivedRevision()){
            tmo--;
            if(tmo) COMPRESSOR->SEND_GET_REVISION();
            else{
                startup_process_fase = _STARTUP_COMPRESSOR_ERROR;
                sysErrors::addlog(SYSERR_COMPRESSOR_PROCESS_REVISION_REQUEST);
            }

            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->compressor_revision->setText(QString("%1.%2 [%4.%5]").arg(COMPRESSOR->getMajRevision()).arg(COMPRESSOR->getMinRevision()).arg(COMPRESSOR->getMajPkgRevision()).arg(COMPRESSOR->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!COMPRESSOR->isValidRevision()){
           sysErrors::addlog(SYSERR_COMPRESSOR_PROCESS_WRONG_REVISION);
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

        // Activate the Process
        if(!COLLIMATOR->startDriver()){
            startup_process_fase = _STARTUP_COLLIMATOR_ERROR;
            sysErrors::addlog(SYSERR_STARTUP_COLLIMATOR_PROCESS);
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }

        // Activate the communication with the process
        COLLIMATOR->Start();

        startup_process_fase = _STARTUP_COLLIMATOR_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COLLIMATOR_CONNECTION:
        if(!COLLIMATOR->isConnected()){
            tmo--;
            if(tmo==0){
                startup_process_fase = _STARTUP_COLLIMATOR_ERROR;
                sysErrors::addlog(SYSERR_COLLIMATOR_CANDRIVER_CONNECTION);
            }
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        // Request the process revision
        COLLIMATOR->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_COLLIMATOR_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_COLLIMATOR_REVISION:
        if(!COLLIMATOR->isReceivedRevision()){
            tmo--;
            if(tmo) COLLIMATOR->SEND_GET_REVISION();
            else{
                startup_process_fase = _STARTUP_COLLIMATOR_ERROR;
                sysErrors::addlog(SYSERR_COLLIMATOR_PROCESS_REVISION_REQUEST);
            }

            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->collimator_revision->setText(QString("%1.%2 [%4.%5]").arg(COLLIMATOR->getMajRevision()).arg(COLLIMATOR->getMinRevision()).arg(COLLIMATOR->getMajPkgRevision()).arg(COLLIMATOR->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!COLLIMATOR->isValidRevision()){
           sysErrors::addlog(SYSERR_COLLIMATOR_PROCESS_WRONG_REVISION);
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
            sysErrors::addlog(SYSERR_STARTUP_FILTER_PROCESS);
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }

        // Activate the communication with the process
        FILTER->Start();

        startup_process_fase = _STARTUP_FILTER_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_FILTER_CONNECTION:
        if(!FILTER->isConnected()){
            tmo--;
            if(tmo==0){
                startup_process_fase = _STARTUP_FILTER_ERROR;
                sysErrors::addlog(SYSERR_FILTER_CANDRIVER_CONNECTION);
            }
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        // Request the process revision
        FILTER->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_FILTER_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_FILTER_REVISION:
        if(!FILTER->isReceivedRevision()){
            tmo--;
            if(tmo) FILTER->SEND_GET_REVISION();
            else{
                startup_process_fase = _STARTUP_FILTER_ERROR;
                sysErrors::addlog(SYSERR_FILTER_PROCESS_REVISION_REQUEST);
            }

            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->filter_revision->setText(QString("%1.%2 [%4.%5]").arg(FILTER->getMajRevision()).arg(FILTER->getMinRevision()).arg(FILTER->getMajPkgRevision()).arg(FILTER->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!FILTER->isValidRevision()){
           sysErrors::addlog(SYSERR_FILTER_PROCESS_WRONG_REVISION);
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
            sysErrors::addlog(SYSERR_STARTUP_POTTER_PROCESS);
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }

        // Activate the communication with the process
        POTTER->Start();

        startup_process_fase = _STARTUP_POTTER_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_POTTER_CONNECTION:
        if(!POTTER->isConnected()){
            tmo--;
            if(tmo==0){
                startup_process_fase = _STARTUP_POTTER_ERROR;
                sysErrors::addlog(SYSERR_POTTER_CANDRIVER_CONNECTION);
            }
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        // Request the process revision
        POTTER->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_POTTER_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_POTTER_REVISION:
        if(!POTTER->isReceivedRevision()){
            tmo--;
            if(tmo) POTTER->SEND_GET_REVISION();
            else{
                startup_process_fase = _STARTUP_POTTER_ERROR;
                sysErrors::addlog(SYSERR_POTTER_PROCESS_REVISION_REQUEST);
            }

            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }


        ui->potter_revision->setText(QString("%1.%2 [%4.%5]").arg(POTTER->getMajRevision()).arg(POTTER->getMinRevision()).arg(POTTER->getMajPkgRevision()).arg(POTTER->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!POTTER->isValidRevision()){
           sysErrors::addlog(SYSERR_POTTER_PROCESS_WRONG_REVISION);
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
            sysErrors::addlog(SYSERR_STARTUP_MOTORS_PROCESS);
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }

        // Activate the communication with the process
        MOTORS->Start();

        startup_process_fase = _STARTUP_MOTORS_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_MOTORS_CONNECTION:
        if(!MOTORS->isConnected()){
            tmo--;
            if(tmo==0){
                startup_process_fase = _STARTUP_MOTORS_ERROR;
                sysErrors::addlog(SYSERR_MOTORS_CANDRIVER_CONNECTION);
            }
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        // Request the process revision
        MOTORS->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_MOTORS_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_MOTORS_REVISION:
        if(!MOTORS->isReceivedRevision()){
            tmo--;
            if(tmo) MOTORS->SEND_GET_REVISION();
            else{
                startup_process_fase = _STARTUP_MOTORS_ERROR;
                sysErrors::addlog(SYSERR_MOTORS_PROCESS_REVISION_REQUEST);
            }

            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }

        ui->motors_revision->setText(QString("%1.%2 [%4.%5]").arg(MOTORS->getMajRevision()).arg(MOTORS->getMinRevision()).arg(MOTORS->getMajPkgRevision()).arg(MOTORS->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!MOTORS->isValidRevision()){
           sysErrors::addlog(SYSERR_MOTORS_PROCESS_WRONG_REVISION);
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
            sysErrors::addlog(SYSERR_STARTUP_GENERATOR_PROCESS);
            QTimer::singleShot(0,this, SLOT(startupProcesses()));
            return;
        }

        // Activate the communication with the process
        GENERATOR->Start();

        startup_process_fase = _STARTUP_GENERATOR_CONNECTION;
        tmo = 50;
        QTimer::singleShot(100,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_GENERATOR_CONNECTION:
        if(!GENERATOR->isConnected()){
            tmo--;
            if(tmo==0){
                startup_process_fase = _STARTUP_GENERATOR_ERROR;
                sysErrors::addlog(SYSERR_GENERATOR_CANDRIVER_CONNECTION);
            }
            QTimer::singleShot(100,this, SLOT(startupProcesses()));
            return;
        }

        // Request the process revision
        GENERATOR->SEND_GET_REVISION();
        startup_process_fase = _STARTUP_GENERATOR_REVISION;
        tmo = 10;
        QTimer::singleShot(20,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_GENERATOR_REVISION:
        if(!GENERATOR->isReceivedRevision()){
            tmo--;
            if(tmo) GENERATOR->SEND_GET_REVISION();
            else{
                startup_process_fase = _STARTUP_GENERATOR_ERROR;
                sysErrors::addlog(SYSERR_GENERATOR_PROCESS_REVISION_REQUEST);
            }

            QTimer::singleShot(20,this, SLOT(startupProcesses()));
            return;
        }

        ui->generator_revision->setText(QString("%1.%2 [%4.%5]").arg(GENERATOR->getMajRevision()).arg(GENERATOR->getMinRevision()).arg(GENERATOR->getMajPkgRevision()).arg(GENERATOR->getMinPkgRevision())  );

        // Test the expected release and the actual release
        if(!GENERATOR->isValidRevision()){
           sysErrors::addlog(SYSERR_GENERATOR_PROCESS_WRONG_REVISION);
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
           // The Workflow proceeds in the next Workflow
           qDebug() << "STARTUP PROCESSES SUCCESS!";
           ui->process_startup_frame->hide();           
           startup_process_fase = _STARTUP_PROCESS_INIT;
           QTimer::singleShot(0,this, SLOT(startupDevices()));
           return;
        }

        // In case of Error the workflow remains in the same status
        qDebug() << "STARTUP PROCESSES FAILED!";
        startup_process_fase = _STARTUP_PROCESS_ERROR;
        QTimer::singleShot(0,this, SLOT(startupProcesses()));
        break;

    case _STARTUP_PROCESS_ERROR:
        startup_process_fase = _STARTUP_PROCESS_INIT;
        break;
    }
}

typedef enum{
    _STARTUP_DEVICES_INIT,
    _STARTUP_DEVICES_CAN_CONVERTER,
    _STARTUP_DEVICE_CAN_STATUS,
    _STARTUP_DEVICE_CAN_ERROR,
    _STARTUP_DEVICE_POWERSERVICE,
    _STARTUP_DEVICE_POWERSERVICE_BOARDINIT,
    _STARTUP_DEVICE_POWERSERVICE_ERROR,
    _STARTUP_DEVICE_COMPRESSOR,
    _STARTUP_DEVICE_COMPRESSOR_BOARDINIT,
    _STARTUP_DEVICE_COMPRESSOR_ERROR,
    _STARTUP_DEVICE_COLLIMATOR,
    _STARTUP_DEVICE_COLLIMATOR_BOARDINIT,
    _STARTUP_DEVICE_COLLIMATOR_ERROR,
    _STARTUP_DEVICE_FILTER,
    _STARTUP_DEVICE_FILTER_BOARDINIT,
    _STARTUP_DEVICE_FILTER_ERROR,
    _STARTUP_DEVICE_POTTER,
    _STARTUP_DEVICE_POTTER_BOARDINIT,
    _STARTUP_DEVICE_POTTER_ERROR,
    _STARTUP_DEVICES_TERMINATED,
    _STARTUP_DEVICES_ERROR
}startup_devices_enum;

/**
 * @brief startup::startupDevices
 *
 * For the Compressor, Collimator, Filter, Potter and Powerservice device,
 * this workflow executes the remote device initialization:
 * - enable the connection with the can bus;
 * - request for the dvice revisions;
 * - requests for the device register initialization;
 *
 */
void startup::startupDevices(void)
{
    static unsigned int tmo = 0;
    static bool device_startup_result;
    static startup_devices_enum startup_devices_fase = _STARTUP_DEVICES_INIT;

    switch(startup_devices_fase){

    case _STARTUP_DEVICES_INIT:
        // Initialize the GUI
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
        // Requests for the CAN DRIVER status
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
        sysErrors::addlog(SYSERR_STARTUP_CAN_STATUS);
        ui->can_device_frame->setStyleSheet(ERROR_FRAME_BORDER );
        device_startup_result = false;
        startup_devices_fase = _STARTUP_DEVICES_ERROR;        
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_POWERSERVICE:        
        // Requests for board initialization
        ui->powerservice_device_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->powerservice_device_frame->show();
        ui->powerservice_device_revision->hide();

        startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE_BOARDINIT;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_POWERSERVICE_BOARDINIT:
        if(!POWERSERVICE->isBoardInitialized()){
            tmo--;
            if(tmo==0){
                startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE_ERROR;
                sysErrors::addlog(SYSERR_POWERSERVICE_BOARD_INIT);
            }else POWERSERVICE->SEND_BOARD_INIT();
            QTimer::singleShot(500,this, SLOT(startupDevices()));
            return;
        }

        if(POWERSERVICE->getBootloaderError()){
            sysErrors::addlog(SYSERR_POWERSERVICE_BOOTLOADER_ERROR);
            startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }

        if(POWERSERVICE->isBootloaderRunning()){
            sysErrors::addlog(SYSERR_POWERSERVICE_BOOTLOADER_RUNNING);
            startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }


        // Assignes the packaga revision to the driver
        POWERSERVICE->setBoardPkgRevision(PKGCONFIG->getParam<uint>(PKG_FW_POWER_SERVICE,0), PKGCONFIG->getParam<uint>(PKG_FW_POWER_SERVICE,1));
        ui->powerservice_device_revision->setText(QString("%1.%2 [%4.%5]").arg(POWERSERVICE->getBoardAppMaj()) \
                                                 .arg(POWERSERVICE->getBoardAppMin()) \
                                                 .arg(POWERSERVICE->getBoardMajPkgRevision()).arg(POWERSERVICE->getBoardMinPkgRevision())  );
        ui->powerservice_device_revision->show();

        // Test the expected release and the actual release
        if(!POWERSERVICE->isValidPkgRevision()){
           sysErrors::addlog(SYSERR_POWERSERVICE_BOARD_WRONG_REVISION);
           startup_devices_fase = _STARTUP_DEVICE_POWERSERVICE_ERROR;
           QTimer::singleShot(20,this, SLOT(startupDevices()));
           return;
        }
        ui->powerservice_device_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_COMPRESSOR;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_POWERSERVICE_ERROR:
        ui->powerservice_device_frame->setStyleSheet(ERROR_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_COMPRESSOR;
        device_startup_result = false;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_COMPRESSOR:
        ui->compressor_device_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->compressor_device_frame->show();
        ui->compressor_device_revision->hide();

        startup_devices_fase = _STARTUP_DEVICE_COMPRESSOR_BOARDINIT;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_COMPRESSOR_BOARDINIT:
        if(!COMPRESSOR->isBoardInitialized()){
            tmo--;
            if(tmo==0){
                startup_devices_fase = _STARTUP_DEVICE_COMPRESSOR_ERROR;
                sysErrors::addlog(SYSERR_COMPRESSOR_BOARD_INIT);
            }else COMPRESSOR->SEND_BOARD_INIT();
            QTimer::singleShot(500,this, SLOT(startupDevices()));
            return;
        }

        if(COMPRESSOR->getBootloaderError()){
            sysErrors::addlog(SYSERR_COMPRESSOR_BOOTLOADER_ERROR);
            startup_devices_fase = _STARTUP_DEVICE_COMPRESSOR_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }

        if(COMPRESSOR->isBootloaderRunning()){
            sysErrors::addlog(SYSERR_COMPRESSOR_BOOTLOADER_RUNNING);
            startup_devices_fase = _STARTUP_DEVICE_COMPRESSOR_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }


        // Assignes the packaga revision to the driver
        COMPRESSOR->setBoardPkgRevision(PKGCONFIG->getParam<uint>(PKG_FW_COMPRESSOR,0), PKGCONFIG->getParam<uint>(PKG_FW_COMPRESSOR,1));
        ui->compressor_device_revision->setText(QString("%1.%2 [%4.%5]").arg(COMPRESSOR->getBoardAppMaj()) \
                                                 .arg(COMPRESSOR->getBoardAppMin()) \
                                                 .arg(COMPRESSOR->getBoardMajPkgRevision()).arg(COMPRESSOR->getBoardMinPkgRevision())  );
        ui->compressor_device_revision->show();


        // Test the expected release and the actual release
        if(!COMPRESSOR->isValidPkgRevision()){
           sysErrors::addlog(SYSERR_COMPRESSOR_BOARD_WRONG_REVISION);
           startup_devices_fase = _STARTUP_DEVICE_COMPRESSOR_ERROR;
           QTimer::singleShot(20,this, SLOT(startupDevices()));
           return;
        }

        ui->compressor_device_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_COLLIMATOR;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_COMPRESSOR_ERROR:
        ui->compressor_device_frame->setStyleSheet(ERROR_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_COLLIMATOR;
        device_startup_result = false;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;


    case _STARTUP_DEVICE_COLLIMATOR:
        ui->collimator_device_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->collimator_device_frame->show();
        ui->collimator_device_revision->hide();

        startup_devices_fase = _STARTUP_DEVICE_COLLIMATOR_BOARDINIT;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_COLLIMATOR_BOARDINIT:
        if(!COLLIMATOR->isBoardInitialized()){
            tmo--;
            if(tmo==0){
                startup_devices_fase = _STARTUP_DEVICE_COLLIMATOR_ERROR;
                sysErrors::addlog(SYSERR_COLLIMATOR_BOARD_INIT);
            }else COLLIMATOR->SEND_BOARD_INIT();
            QTimer::singleShot(500,this, SLOT(startupDevices()));
            return;
        }

        if(COLLIMATOR->getBootloaderError()){
            sysErrors::addlog(SYSERR_COLLIMATOR_BOOTLOADER_ERROR);
            startup_devices_fase = _STARTUP_DEVICE_COLLIMATOR_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }

        if(COLLIMATOR->isBootloaderRunning()){
            sysErrors::addlog(SYSERR_COLLIMATOR_BOOTLOADER_RUNNING);
            startup_devices_fase = _STARTUP_DEVICE_COLLIMATOR_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }


        // Assignes the packaga revision to the driver
        COLLIMATOR->setBoardPkgRevision(PKGCONFIG->getParam<uint>(PKG_FW_COLLIMATOR,0), PKGCONFIG->getParam<uint>(PKG_FW_COLLIMATOR,1));
        ui->collimator_device_revision->setText(QString("%1.%2 [%4.%5]").arg(COLLIMATOR->getBoardAppMaj()) \
                                                 .arg(COLLIMATOR->getBoardAppMin()) \
                                                 .arg(COLLIMATOR->getBoardMajPkgRevision()).arg(COLLIMATOR->getBoardMinPkgRevision())  );
        ui->collimator_device_revision->show();


        // Test the expected release and the actual release
        if(!COLLIMATOR->isValidPkgRevision()){
           sysErrors::addlog(SYSERR_COLLIMATOR_BOARD_WRONG_REVISION);
           startup_devices_fase = _STARTUP_DEVICE_COLLIMATOR_ERROR;
           QTimer::singleShot(20,this, SLOT(startupDevices()));
           return;
        }

        ui->collimator_device_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_FILTER;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_COLLIMATOR_ERROR:
        ui->collimator_device_frame->setStyleSheet(ERROR_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_FILTER;
        device_startup_result = false;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;


    case _STARTUP_DEVICE_FILTER:
        ui->filter_device_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->filter_device_frame->show();
        ui->filter_device_revision->hide();

        startup_devices_fase = _STARTUP_DEVICE_FILTER_BOARDINIT;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_FILTER_BOARDINIT:
        if(!FILTER->isBoardInitialized()){
            tmo--;
            if(tmo==0){
                startup_devices_fase = _STARTUP_DEVICE_FILTER_ERROR;
                sysErrors::addlog(SYSERR_FILTER_BOARD_INIT);
            }else FILTER->SEND_BOARD_INIT();
            QTimer::singleShot(500,this, SLOT(startupDevices()));
            return;
        }

        if(FILTER->getBootloaderError()){
            sysErrors::addlog(SYSERR_FILTER_BOOTLOADER_ERROR);
            startup_devices_fase = _STARTUP_DEVICE_FILTER_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }

        if(FILTER->isBootloaderRunning()){
            sysErrors::addlog(SYSERR_FILTER_BOOTLOADER_RUNNING);
            startup_devices_fase = _STARTUP_DEVICE_FILTER_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }


        // Assignes the packaga revision to the driver
        FILTER->setBoardPkgRevision(PKGCONFIG->getParam<uint>(PKG_FW_FILTER,0), PKGCONFIG->getParam<uint>(PKG_FW_FILTER,1));
        ui->filter_device_revision->setText(QString("%1.%2 [%4.%5]").arg(FILTER->getBoardAppMaj()) \
                                                 .arg(FILTER->getBoardAppMin()) \
                                                 .arg(FILTER->getBoardMajPkgRevision()).arg(FILTER->getBoardMinPkgRevision())  );
        ui->filter_device_revision->show();


        // Test the expected release and the actual release
        if(!FILTER->isValidPkgRevision()){
           sysErrors::addlog(SYSERR_FILTER_BOARD_WRONG_REVISION);
           startup_devices_fase = _STARTUP_DEVICE_FILTER_ERROR;
           QTimer::singleShot(20,this, SLOT(startupDevices()));
           return;
        }

        ui->filter_device_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_POTTER;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_FILTER_ERROR:
        ui->filter_device_frame->setStyleSheet(ERROR_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICE_POTTER;
        device_startup_result = false;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;


    case _STARTUP_DEVICE_POTTER:
        ui->potter_device_frame->setStyleSheet(HIGHLIGHT_FRAME_BORDER);
        ui->potter_device_frame->show();
        ui->potter_device_revision->hide();

        startup_devices_fase = _STARTUP_DEVICE_POTTER_BOARDINIT;
        tmo = 50;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_POTTER_BOARDINIT:
        if(!POTTER->isBoardInitialized()){
            tmo--;
            if(tmo==0){
                startup_devices_fase = _STARTUP_DEVICE_POTTER_ERROR;
                sysErrors::addlog(SYSERR_POTTER_BOARD_INIT);
            }else POTTER->SEND_BOARD_INIT();
            QTimer::singleShot(500,this, SLOT(startupDevices()));
            return;
        }

        if(POTTER->getBootloaderError()){
            sysErrors::addlog(SYSERR_POTTER_BOOTLOADER_ERROR);
            startup_devices_fase = _STARTUP_DEVICE_POTTER_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }

        if(POTTER->isBootloaderRunning()){
            sysErrors::addlog(SYSERR_POTTER_BOOTLOADER_RUNNING);
            startup_devices_fase = _STARTUP_DEVICE_POTTER_ERROR;
            QTimer::singleShot(0,this, SLOT(startupDevices()));
            return;
        }


        // Assignes the packaga revision to the driver
        POTTER->setBoardPkgRevision(PKGCONFIG->getParam<uint>(PKG_FW_POTTER,0), PKGCONFIG->getParam<uint>(PKG_FW_POTTER,1));
        ui->potter_device_revision->setText(QString("%1.%2 [%4.%5]").arg(POTTER->getBoardAppMaj()) \
                                                 .arg(POTTER->getBoardAppMin()) \
                                                 .arg(POTTER->getBoardMajPkgRevision()).arg(POTTER->getBoardMinPkgRevision())  );
        ui->potter_device_revision->show();


        // Test the expected release and the actual release
        if(!POTTER->isValidPkgRevision()){
           sysErrors::addlog(SYSERR_POTTER_BOARD_WRONG_REVISION);
           startup_devices_fase = _STARTUP_DEVICE_POTTER_ERROR;
           QTimer::singleShot(20,this, SLOT(startupDevices()));
           return;
        }

        ui->potter_device_frame->setStyleSheet(CORRECT_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICES_TERMINATED;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICE_POTTER_ERROR:
        ui->potter_device_frame->setStyleSheet(ERROR_FRAME_BORDER );
        startup_devices_fase = _STARTUP_DEVICES_TERMINATED;
        device_startup_result = false;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICES_TERMINATED:

        if(device_startup_result) {
           qDebug() << "STARTUP DEVICE SUCCESS!";
           ui->device_startup_frame->hide();
           startup_devices_fase = _STARTUP_DEVICES_INIT;
           QTimer::singleShot(0,this, SLOT(startupMotors()));
           return;
        }

        qDebug() << "STARTUP DEVICE FAILED!";
        startup_devices_fase = _STARTUP_DEVICES_ERROR;
        QTimer::singleShot(0,this, SLOT(startupDevices()));
        break;

    case _STARTUP_DEVICES_ERROR:
        startup_devices_fase = _STARTUP_DEVICES_INIT;

        break;
    }



}
