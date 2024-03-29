#include "collaborativedrawingwidget.h"

CollaborativeDrawingWidget::CollaborativeDrawingWidget(QWidget *parent) :
    BaseDrawingWidget(parent)
{
    // disable scrollbars for collaboration area
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // collaboration widget is disabled until we join a session
    setEnabled(false);
}

void CollaborativeDrawingWidget::commitDrawing(QPicture drawingPictureData)
{
    qWarning() << "commitDrawing size" << drawingPictureData.size();

    BaseDrawingWidget::commitDrawing(drawingPictureData);

    emit drawingCommited(m_currentSession, drawingPictureData);

}

void CollaborativeDrawingWidget::drawingArrived(QString sessionName, QByteArray picData, bool isInitialState)
{
    // this slot will be invoked when the user joins a new session and the server
    // sends the current drawing state for this session
    QPicture pic;
    qWarning() << "Session state data of size : " << picData.size() << "from session" << sessionName;
    if(isInitialState) {
        // set the session name
        m_currentSession = sessionName;
        // clear up any old data
        getDrawingData()->clear();
        // the widget should be enabled now
        setEnabled(true);
    }
    pic.setData(picData.constData(), picData.size());

    // commit the new data
    BaseDrawingWidget::commitDrawing(pic);
}

void CollaborativeDrawingWidget::initialSessionState(QString sessionName, QImage img)
{
    //Set the session name
    m_currentSession = sessionName;
    //Clear up any old data
    getDrawingData()->clear();
    //The widget should be enabled now
    setEnabled(true);

    getDrawingData()->setImage(img);
}
