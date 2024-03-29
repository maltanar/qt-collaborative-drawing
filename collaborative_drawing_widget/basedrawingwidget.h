#ifndef BASEDRAWINGWIDGET_H
#define BASEDRAWINGWIDGET_H
#include "appglobals.h"
#include <QGraphicsView>
#include <QResizeEvent>
#include <QPicture>
#include <QPainter>
#include "drawingdata.h"



class BaseDrawingWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BaseDrawingWidget(QWidget *parent = 0);
    ~BaseDrawingWidget();

    void setDrawingData(DrawingData * newData);
    DrawingData * getDrawingData();    
    QPen getDrawingPen();
    QBrush getDrawingBrush();
    DrawingMode getDrawingMode();

protected:
    void mouseMoveEvent (QMouseEvent * event);
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);
    void resizeEvent (QResizeEvent * event);

protected:
    QPointF mouseDownPoint;
    QPointF mouseUpPoint;
    QPointF mousePrevPoint;
    DrawingData * drawingData;
    QPen drawingPen;
    QBrush drawingBrush;
    DrawingMode drawingMode;
    QGraphicsItem * currentItem;
    QPixmap * stage;

    QPicture picture;
    QPainter picturePainter;

    virtual void drawingStart(QPointF startPoint);
    virtual void drawingUpdate(QPointF updatePoint);
    virtual void drawingEnd(QPointF endPoint);
    virtual void handleDrawingState(DrawingState state, QPointF lastPoint);
    virtual void commitDrawing(QPicture drawingData);

signals:

public slots:
    void setDrawingBrush(QBrush brush);
    void setDrawingPen(QPen pen);
    void setDrawingMode(DrawingMode mode);
    void setDrawingColor(QColor color);
    void increasePenWidth();
    void decreasePenWidth();
    void clear();

};

#endif // BASEDRAWINGWIDGET_H
