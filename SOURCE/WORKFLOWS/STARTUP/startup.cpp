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
    startup_fase = _STARTUP_STARTING_PROCESS;
    startupActivateFase();

}

void startup::exitWindow(void)
{

}


void startup::startupActivateFase(void)
{

    switch(startup_fase){
    case _STARTUP_STARTING_PROCESS:

        ui->package_revision->setText(QString("ID%1").arg(PKGCONFIG->getParam<QString>(PKG_ID, 0)));
        ui->power_sevice_revision->setText("__.__.__");
        ui->compressor_revision->setText("__.__.__");
        ui->collimator_revision->setText("__.__.__");
        ui->filter_revision->setText("__.__.__");
        ui->motors_revision->setText("__.__.__");
        ui->generator_revision->setText("__.__.__");
        ui->potter_revision->setText("__.__.__");
        ui->main_process_start_frame->show();

        break;
    }
}
