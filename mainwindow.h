#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "utils.h"
#include "vidcontroller.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openVid();
    void onPlay();
    void updatePlayerUI(QImage img);

    void trackBarSliderMoved(int position);
    void trackBarSliderPressed();
    void trackBarSliderReleased();
private:
    void updateLabelTime();
    Ui::MainWindow *ui;

    std::string curFileName;
    ul::FormatTime formatTime;
    VidController* vidController;
};

#endif // MAINWINDOW_H
