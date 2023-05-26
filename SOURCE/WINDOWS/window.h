#ifndef WINDOW_H
#define WINDOW_H

/**
 * \defgroup windowModule Debug Window Module
 */
#include <QWidget>
#include "applog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class debugWindow; }
QT_END_NAMESPACE


class debugWindow : public QWidget
{
    Q_OBJECT

public:
    debugWindow( QWidget *parent = nullptr);
    ~debugWindow();

    static void debugMessageHandler(QtMsgType type, QString msg); //!< Handles the debug messages in case of -win Application
    inline static debugWindow* instance = nullptr; //!< Assigned to the unique instance of the class


    void updateParameters(void);
    void updateData(void);
    void updateStatus(void);

// WINDOWS MODULE VIRTUAL FUNCTIONS
protected:
    virtual void initWindow(void); //!< Override function for the opening activities
    virtual void exitWindow(void); //!< Override function for the exiting activities





signals:


// GUI MODULE SLOTS
public slots:

    void onDebugClearButton(void);
    void onLogClearButton(void);
    void onSoftPoffButton(void);
    void onAbortButton(void);

    void assignData(void);


    void rxFromCan(ushort canId, QByteArray data);
    void txToCan(ushort canId, QByteArray data);

    void timerEvent(QTimerEvent* ev);


private slots:
    void on_logEnableCheck_stateChanged(int arg1);

private:

    Ui::debugWindow *ui;

   QByteArray pollingFrame;
   int pollingTimer;
   int polling;

    uint generalDebugIndex;

};


#endif // WINDOW_H
