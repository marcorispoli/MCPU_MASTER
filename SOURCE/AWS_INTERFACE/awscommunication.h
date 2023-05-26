#ifndef AWSCOMMUNICATION_H
#define AWSCOMMUNICATION_H

#include <QObject>
#include "awsinterface.h"



class awsCommunication : public awsInterface
{
    Q_OBJECT

public:
    explicit awsCommunication(QString address, int port, QObject *parent = nullptr);

signals:



public:




private:


};

#endif // AWSCOMMUNICATION_H
