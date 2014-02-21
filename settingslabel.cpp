#include "settingslabel.h"

SettingsLabel::SettingsLabel(QWidget *parent) :
    QLabel(parent)
{
    this->setText(tr("Settings"));
}

SettingsLabel::~SettingsLabel()
{
}

void SettingsLabel::mouseReleaseEvent(QMouseEvent*)
{
    QMenu menu(this);
    menu.addActions(this->actions());
    QPoint pos = this->window()->pos() +
            this->pos();
    menu.exec(pos);
}
