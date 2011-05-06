#ifndef COLLABORATIONCLIENT_H
#define COLLABORATIONCLIENT_H

#include <QObject>
#include <QHash>
#include <QStringList>

#include <protocolhandler.h>
#include <collaborationsession.h>

#define COLLABORATION_SERVER_NAME "$SERVER$"
#define SERVICE_BROADCAST_PORT 45455

class CollaborationClient : public QObject
{
    Q_OBJECT
public:
    explicit CollaborationClient(QObject *parent = 0);

    void setProtocolHandler(ProtocolHandler * newProtocolHandler);
    ProtocolHandler * getProtocolHandler();

    void dummyFunction(QString userName)
    {
        emit sendLoginRequest(userName);
    }


private:
    QStringList m_userList;
    QStringList m_sessionList;
    ProtocolHandler *m_protocolHandler;
    QHash<QString, CollaborationSession *> m_collaborationSessions;

    QUdpSocket serviceBroadcastReceiver;

signals:
    void sendLoginRequest(QString destUserName);
    void sendLogoutRequest(QString destUserName);
    void sendPeerHandshake(QString destUserName, QString sessionName);
    void sendPictureRequest(QString destUserName, QString sessionName);
    void sendSessionJoinRequest(QString destUserName, QString sessionName, QString password);
    void sendSessionLeaveRequest(QString destUserName, QString sessionName);
    void sendSessionListRequest(QString destUserName);
    void sendWritePermissionRequest(QString destUserName);

    void foundCollaborationServer(QHostAddress serverAddress);

public slots:
    void receivedLoginResponse(QString userName, QChar result, QString infoMsg);
    void receivedPeerHandshake(QString userName, QString sessionName);
    void receivedPictureResponse(QString userName, QString sessionName, QByteArray picData);
    void receivedSessionJoinResponse(QString userName, QString sessionName, char result, unsigned int userCount, QHash<QString, long> users);
    void receivedSessionLeaveResponse(QString userName, QString sessionName, char result);
    void receivedSessionListResponse(QString userName, QStringList sessionList);
    void receivedSessionMemberUpdate(QString userName, QString sessionName, char updateType, QHash<QString, long> users);
    void receivedUpdateDrawing(QString userName, QString sessionName, QByteArray picData);
    void receivedWritePermissionStatus(QString userName, QChar status);

private slots:
    void gotServiceBroadcast();

};

#endif // COLLABORATIONCLIENT_H
