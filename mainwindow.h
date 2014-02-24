#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include "utils.h"
#include "vidcontroller.h"
#include "editobjectdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void openVid();
    void onPlay();
    void updatePlayerUI(QImage img);

    void trackBarSliderPressed();
    void trackBarSliderReleased();

    void setMouseCallbackAddObject(bool);
    void setMouseCallbackMoveRect(bool);

    void showEditObjectDialog();

    void onItemDoubleClick(QModelIndex);
    void showObjects(bool);

    void onVidEnding();
private:
    void updateLabelTime();

    EditObjectDialog * editObjectDialog;
    std::string currentFileName;
    ul::FormatTime formatTime;
    VidController * vidController;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
