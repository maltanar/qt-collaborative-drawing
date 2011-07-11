#include "collaborationsession.h"
#include <QPixmap>
#include <QDebug>

CollaborationSession::CollaborationSession(QObject *parent) :
    QObject(parent)
{
    m_sessionDrawingData = QImage(800, 600, QImage::Format_ARGB32_Premultiplied);
}

void CollaborationSession::setSessionName(QString newName)
{
    m_sessionName = newName;
}

QString CollaborationSession::getSessionName()
{
    return m_sessionName;
}

void CollaborationSession::setSessionPassword(QString newPassword)
{
    m_sessionPassword = newPassword;
}

QString CollaborationSession::getSessionPassword()
{
    return m_sessionPassword;
}

QMap<QString, qint32> CollaborationSession::getSessionParticipants()
{
    return m_sessionParticipants;
}

void CollaborationSession::setSessionParticipants(QMap<QString, qint32> participantList)
{
    m_sessionParticipants = participantList;

    //Initialize all session members as unacknowledged
    QMap<QString, qint32>::iterator itr;
    for (itr = participantList.begin(); itr != participantList.end(); itr++)
    {
        m_sessionAcknowledgements.insert(itr.key(),false);
    }
}

bool CollaborationSession::addSessionParticipant(QString userName, QString sessionPassword, qint32 userIpAddress)
{
    if(sessionPassword == m_sessionPassword) {
        // password is correct, check if participant already exists in this session
        if(m_sessionParticipants.contains(userName)) {
            //participant already exists in session
            qWarning() << "User" << userName << "could not be added as a participant as it already exists";
            return false;
        }
        else {
            // everything in order, we can add this participant to the session
            m_sessionParticipants[userName] = userIpAddress;

            return true;
        }
    } else
        qWarning() << "Session password is wrong!";
        // incorrect password
        return false;
}

void CollaborationSession::removeSessionParticipant(QString userName)
{
    if(m_sessionParticipants.contains(userName))
        m_sessionParticipants.remove(userName);
}

QImage CollaborationSession::getSessionDrawingState()
{
    return m_sessionDrawingData;
}

void CollaborationSession::addDrawingStep(QPicture newDrawingStep)
{
    m_picturePainter.begin(&m_sessionDrawingData);
    //m_picturePainter.drawPicture(0,0,m_sessionDrawingData);
    m_picturePainter.drawPicture(0,0,newDrawingStep);
    m_picturePainter.end();
}


void CollaborationSession::acknowledgePeer(QString userName)
{
    m_sessionAcknowledgements[userName] = true;
}

bool CollaborationSession::isAcknowledged(QString userName)
{
    return m_sessionAcknowledgements[userName];
}

bool CollaborationSession::isAllAcknowledged()
{
    QMap<QString, bool>::iterator itr;
    for (itr = m_sessionAcknowledgements.begin(); itr != m_sessionAcknowledgements.end(); itr++)
    {
        if (!itr.value()) return false;
    }
    return true;
}


void CollaborationSession::setSessionImage(QImage sessImg)
{
    this->m_sessionDrawingData = sessImg;
}
