#ifndef COLLABORATIONSESSION_H
#define COLLABORATIONSESSION_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QPicture>
#include <QPainter>

// represents the information and actions of a single collaborative drawing session

class CollaborationSession : public QObject
{
    Q_OBJECT
public:
    explicit CollaborationSession(QObject *parent = 0);

    void setSessionName(QString newName);
    QString getSessionName();

    void setSessionPassword(QString newPassword);
    QString getSessionPassword();

    QHash<QString, long> getSessionParticipants();
    void setSessionParticipants(QHash<QString, long> participantList);

    bool addSessionParticipant(QString userName, QString sessionPassword, long userIpAddress);
    void removeSessionParticipant(QString userName);

    QImage getSessionDrawingState();
    void addDrawingStep(QPicture newDrawingStep);

    void acknowledgePeer(QString userName);
    bool isAcknowledged(QString userName);
    bool isAllAcknowledged();

    void setSessionImage(QImage sessImg);

protected:
    QString m_sessionName;
    QString m_sessionPassword;
    QHash<QString, long> m_sessionParticipants;
    QHash<QString, bool> m_sessionAcknowledgements;
    QImage m_sessionDrawingData;
    QPainter m_picturePainter;

signals:

public slots:

};

#endif // COLLABORATIONSESSION_H
