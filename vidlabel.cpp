#include "vidlabel.h"

VidLabel::VidLabel(QWidget *parent) :
    QLabel(parent)
{
}

void VidLabel::mouseMoveEvent(QMouseEvent *ev)
{ 
    const QPoint p = ev->pos();
    if(ev->buttons() == Qt::LeftButton)
    {
        emit mouseMoved(p,this->size());
    }
}

void VidLabel::mousePressEvent(QMouseEvent* ev)
{
    const QPoint p = ev->pos();
    if(ev->buttons() == Qt::LeftButton)
    {
        emit mousePressed(p,this->size());
    }
}

void VidLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    const QPoint p = ev->pos();
    if(ev->buttons() == Qt::NoButton)
    {
        emit mouseReleased(p,this->size());
    }
}
