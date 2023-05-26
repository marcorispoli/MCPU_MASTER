#include "application.h"
#include "ui_startup.h"



startup::startup(QWidget *parent)
    : ui(new Ui::startupWindow)
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


}

void startup::exitWindow(void)
{

}


