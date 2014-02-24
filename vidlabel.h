#ifndef VIDLABEL_H
#define VIDLABEL_H

#include <iostream>
#include <QMouseEvent>
#include <QPoint>
#include <QLabel>

class VidLabel : public QLabel
{
    Q_OBJECT
public:
    explicit VidLabel(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
signals:
    void mouseMoved( const QPoint&, const QSize& );
    void mousePressed( const QPoint&, const QSize& );
    void mouseReleased( const QPoint&, const QSize& );
};
#endif // VIDLABEL_H
