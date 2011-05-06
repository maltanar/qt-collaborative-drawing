#include "collaborationsession.h"

CollaborationSession::CollaborationSession(QObject *parent) :
    QObject(parent)
{
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

QList<QString> CollaborationSession::getSessionParticipants()
{
    return m_sessionParticipants;
}

void CollaborationSession::setSessionParticipants(QList<QString> participantList)
{
    m_sessionParticipants = participantList;
}

bool CollaborationSession::addSessionParticipant(QString userName, QString sessionPassword)
{
    if(sessionPassword == m_sessionPassword) {
        // password is correct, check if participant already exists in this session
        if(m_sessionParticipants.contains(userName))
            //participant already exists in session
            return false;
        else {
            // everything in order, we can add this participant to the session
            m_sessionParticipants.append(userName);
            return true;
        }
    } else
        // incorrect password
        return false;
}

void CollaborationSession::removeSessionParticipant(QString userName)
{
    if(m_sessionParticipants.contains(userName))
        m_sessionParticipants.removeOne(userName);
}

QPicture CollaborationSession::getSessionDrawingState()
{
    return m_sessionDrawingData;
}

void CollaborationSession::addDrawingStep(QPicture newDrawingStep)
{
    m_picturePainter.begin(&m_sessionDrawingData);
    m_picturePainter.drawPicture(0,0,newDrawingStep);
    m_picturePainter.end();
}
