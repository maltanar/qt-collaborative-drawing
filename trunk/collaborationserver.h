#ifndef COLLABORATIONSERVER_H
#define COLLABORATIONSERVER_H

#include <QObject>
#include <QHash>
#include <QList>

#include "protocolhandler.h"

class CollaborationServer : public QObject
{
    Q_OBJECT
public:
    explicit CollaborationServer(QObject *parent = 0);

    void setProtocolHandler(ProtocolHandler * newProtocolHandler);
    ProtocolHandler * getProtocolHandler();

private:
    QList<QString> m_userList;
    ProtocolHandler * m_protocolHandler;

signals:
    void sendLoginResponse(QString destUserName, char result, QString infoMsg);
    void sendPictureResponse(QString destUserName, QString sessionName, QByteArray picData);
    void sendSessionJoinResponse(QString destUserName, QString sessionName, char result, QHash<QString, long> users);
    void sendSessionLeaveResponse(QString destUserName, QString sessionName, char result);
    void sendSessionListResponse(QString destUserName, QStringList sessionList);
    void sendSessionMemberUpdate(QString destUserName, QString sessionName, char updateType, QHash<QString, long> users);
    void sendWritePermissionStatus(QString destUserName, QChar status);

public slots:
    void receivedLoginRequest(QString userName);
    void receivedLogoutRequest(QString userName);
    void receivedPictureRequest(QString userName, QString sessionName);
    void receivedSessionJoinRequest(QString userName, QString sessionName, QString password);
    void receivedSessionLeaveRequest(QString userName, QString sessionName);
    void receivedSessionListRequest(QString userName);
    void receivedUpdateDrawing(QString userName, QString sessionName, QByteArray picData);
    void receivedWritePermissionRequest(QString userName);
};

#endif // COLLABORATIONSERVER_H
