#ifndef SETTINGSLABEL_H
#define SETTINGSLABEL_H

#include <QMenu>
#include <QPoint>
#include <QLabel>
#include <QAction>
#include <QMouseEvent>

class SettingsLabel : public QLabel
{
    Q_OBJECT
public:
    explicit SettingsLabel(QWidget *parent = 0);
    ~SettingsLabel();
protected:
    void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // SETTINGSLABEL_H
