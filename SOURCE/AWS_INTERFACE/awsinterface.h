#ifndef AWSINTERFACE_H
#define AWSINTERFACE_H

#include <QObject>
#include <QMap>
#include "tcpipserver.h"
#include "commandprotocol.h"

/**
 * @brief This is the AWS protocol command interface implementation.
 * \nosubgrouping
 *
 * # PROTOCOL DESCRIPTION
 * See the \ref PG_TD_AWS_PROTO section for details.
 *
 */
class awsInterface : public QObject
{
    Q_OBJECT
public:
    explicit awsInterface(QString address, int port, QObject *parent = nullptr);

    void restartServer(QString ip, uint port);
signals:

    void receivedFrameSgn(QByteArray data);
    void transmitFrameSgn(QByteArray data);
public:

    /**
    * @addtogroup AWS-COMMANDS Aws Command frames  interface description
    *
    * This section describes the AWS Commands that the Gantry processes.
    * \ingroup AWS-INTERFACE-MODULE
    * @{
    * @name AWS Commands List
    * @{
    */
    virtual void EXEC_Close(void) {pTcpIpServer->txData(protocol.formatNaAnswer("EXEC_Close"));};
    virtual void EXEC_OpenStudy(void) {pTcpIpServer->txData(protocol.formatNaAnswer("EXEC_OpenStudy"));};
    virtual void GET_RotationAngles(void) {pTcpIpServer->txData(protocol.formatNaAnswer("GET_RotationAngles"));};
    virtual void GET_Accessories(void) {pTcpIpServer->txData(protocol.formatNaAnswer("GET_Accessories"));};

    virtual void EXEC_AbortProjections(void) {pTcpIpServer->txData(protocol.formatNaAnswer("EXEC_AbortProjections"));};
    virtual void EXEC_ArmPosition(void) {pTcpIpServer->txData(protocol.formatNaAnswer("EXEC_ArmPosition"));};
    virtual void EXEC_PowerOff(void) {pTcpIpServer->txData(protocol.formatNaAnswer("EXEC_PowerOff"));};
    virtual void EXEC_ResetError(void) {pTcpIpServer->txData(protocol.formatNaAnswer("EXEC_ResetError"));};
    virtual void EXEC_StartXraySequence(void) {pTcpIpServer->txData(protocol.formatNaAnswer("EXEC_StartXraySequence"));};
    virtual void EXEC_TrxPosition(void) {pTcpIpServer->txData(protocol.formatNaAnswer("EXEC_TrxPosition"));};

    virtual void GET_CompressorData(void) {pTcpIpServer->txData(protocol.formatNaAnswer("GET_CompressorData"));};
    virtual void GET_ExposureCompletionData(void) {pTcpIpServer->txData(protocol.formatNaAnswer("GET_ExposureCompletionData"));};
    virtual void GET_ReadyForExposure(void) {pTcpIpServer->txData(protocol.formatNaAnswer("GET_ReadyForExposure"));};

    virtual void GET_TomoConfig(void) {pTcpIpServer->txData(protocol.formatNaAnswer("GET_TomoConfig"));};
    virtual void GET_TomoConfigFile(void) {pTcpIpServer->txData(protocol.formatNaAnswer("GET_TomoConfigFile"));};
    virtual void GET_TubeTemperature(void) {pTcpIpServer->txData(protocol.formatNaAnswer("GET_TubeTemperature"));};
    virtual void GET_XrayPushStatus(void) {pTcpIpServer->txData(protocol.formatNaAnswer("GET_XrayPushStatus"));};
    virtual void SET_CompressionRelease(void) {pTcpIpServer->txData(protocol.formatNaAnswer("SET_CompressionRelease"));};
    virtual void SET_Language(void) {pTcpIpServer->txData(protocol.formatNaAnswer("SET_Language"));};
    virtual void SET_ProjectionList(void) {pTcpIpServer->txData(protocol.formatNaAnswer("SET_ProjectionList"));};
    virtual void SET_PulseData(void) {pTcpIpServer->txData(protocol.formatNaAnswer("SET_PulseData"));};
    virtual void SET_XrayPushEnable(void) {pTcpIpServer->txData(protocol.formatNaAnswer("SET_XrayPushEnable"));};
    virtual void SET_ExposureMode(void) {pTcpIpServer->txData(protocol.formatNaAnswer("SET_ExposureMode"));};


    ///@}
    ///@}

    //! Default answer in case the command should not be decoded
    void AWS_NotRecognizedCommand(void){pTcpIpServer->txData(protocol.formatNaAnswer("UNRECOGNIZED"));};

    /**
     * @defgroup AWS-INTERFACE-MODULE
     * This module defines the Statu and Command frames implemented
     */

     /**
     * @addtogroup AWS-STATUS Status frames interface description.
     *
     * This section describes the Status frames that the gantry sends\n
     * to the AWS in order to notify some internal status change.
     *
     * \ingroup AWS-INTERFACE-MODULE
     * @{
     * @name Gantry Status Commands List
     * @{
     */
    virtual void GANTRY_AbortProjection(void){ sendStatus("GANTRY_AbortProjection");}
    virtual void GANTRY_ERROR(QString errcode = ""){ sendStatus("GANTRY_ERROR " + errcode);}
    virtual void GANTRY_PulseCompleted(void) { sendStatus("GANTRY_PulseCompleted");}
    virtual void GANTRY_SelectProjection(QString item) { sendStatus("GANTRY_SelectProjection " + item);}
    virtual void GANTRY_SetAccessories(QString potterModel = "", QString comprPaddle ="", QString colliAccessory ="") { sendStatus("GANTRY_SetAccessories " + potterModel + " " + comprPaddle + " " + colliAccessory);}
    virtual void GANTRY_SetTubeTemperature(QString anodeTemp = "", QString housingTemp ="") { sendStatus("GANTRY_SetTubeTemperature " + anodeTemp + " " + housingTemp );}
    virtual void GANTRY_XrayPushEvent(void) { sendStatus("GANTRY_XrayPushEvent");}
    virtual void GANTRY_XraySequenceCompleted(QString result) { sendStatus("GANTRY_XraySequenceCompleted " + result);}
    virtual void GANTRY_CompressorData(QString thick, QString force) { sendStatus("GANTRY_CompressorData " + thick + " " +force);}


    ///@}
    ///@}




private slots:
    void tcpIpServerRxData(QByteArray data); //!< Received data slot
    void tcpIpServerConnection(bool status); //!< Connection event data slot

protected:
    typedef void (awsInterface::*funcPtr)(void);
    QMap<QString, funcPtr> awsCommands;

    //void _inline send(const QByteArray& data) {if(connectionStatus) pTcpIpServer->txData(data);}
    void _inline sendOk(QList<QString>* params = nullptr) {emit transmitFrameSgn(protocol.formatOk(params));}
    void _inline sendOk(int code, QList<QString>* params = nullptr) {emit transmitFrameSgn(protocol.formatOkCode(code, params));}
    void _inline sendNok(int code, QString error = "") {emit transmitFrameSgn(protocol.formatNokAnswer(code,error));}
    void _inline sendStatus(QString status, QList<QString>* params = nullptr){emit transmitFrameSgn(commandProtocol::formatStatusCommand(this->encodingFormat,status, params));}
    void _inline sendAsync(commandProtocol proto, int code, QList<QString>* params = nullptr){emit transmitFrameSgn(commandProtocol::formatDelayCompletedCommand(this->encodingFormat, proto.getId(), code, params));}



    commandProtocol protocol;
    bool connectionStatus;

private:
    TcpIpServer* pTcpIpServer; //!< Pointer to the streaming socket
    QStringConverter::Encoding encodingFormat; //!< Encoding format




};

#endif // AWSINTERFACE_H
