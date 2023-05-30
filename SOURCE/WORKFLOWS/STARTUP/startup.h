#ifndef STARTUP_H
#define STARTUP_H

/**
 * \defgroup windowModule Debug Window Module
 */
#include <QWidget>
#include "applog.h"
#include "windows.h"



QT_BEGIN_NAMESPACE
namespace Ui { class startupWindow; }
QT_END_NAMESPACE


class startup : public windowsClass
{
    Q_OBJECT



public slots:


public:
    startup( int W, int H,QWidget *parent = nullptr);
    ~startup();

    //static void debugMessageHandler(QtMsgType type, QString msg); //!< Handles the debug messages in case of -win Application
    inline static startup* instance = nullptr; //!< Assigned to the unique instance of the class



// WINDOWS MODULE VIRTUAL FUNCTIONS
protected:
    virtual void initWindow(void); //!< Override function for the opening activities
    virtual void exitWindow(void); //!< Override function for the exiting activities


signals:


// GUI MODULE SLOTS
public slots:


private slots:
    void startupFase(void);

private:

   Ui::startupWindow *ui;

   uchar startup_fase;
   enum{
       _STARTUP_INIT,
       _STARTUP_CAN_PROCESS,
       _STARTUP_CAN_PROCESS_CONNECTION,
       _STARTUP_CAN_PROCESS_REVISION,
       _STARTUP_CAN_PROCESS_ERROR,
       _STARTUP_SERVICE_POWER,
       _STARTUP_SERVICE_CONNECTION,
       _STARTUP_SERVICE_POWER_REVISION,
       _STARTUP_SERVICE_POWER_ERROR,
       _STARTUP_COLLIMATOR,
       _STARTUP_COLLIMATOR_CONNECTION,
       _STARTUP_COLLIMATOR_REVISION,
       _STARTUP_COLLIMATOR_ERROR,
   }startup_fase_enum;



};


#endif // STARTUP_H
