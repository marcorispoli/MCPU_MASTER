#ifndef POWER_SERVICE_H
#define POWER_SERVICE_H

#include "QObject"
#include "masterInterface.h"

class powerService : public masterInterface
{
    Q_OBJECT

public:

    explicit powerService(void);
    ~powerService(){};

   void handleReceivedEvent(QList<QString>* event_content);


private slots:


private:
};

#endif // POWER_SERVICE_H
