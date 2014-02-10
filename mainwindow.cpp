#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vidController = new VidController(this);
    connect(vidController,
            SIGNAL(processedImage(QImage)),
            this,
            SLOT(updatePlayerUI(QImage)));
    ui->sideBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    vidController->exit();
    vidController->wait();
    delete ui;
    qApp->quit();
}

void MainWindow::openVid()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Video"), ".",
                                                    tr("Video Files (*.avi *.mpg *.mp4"));
    QFileInfo name = filename;

    if(!filename.isEmpty())
    {
        if(!vidController->loadVideo(filename.toStdString()))
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
        else
        {

            curFileName = ul::getFileNameWithoutExtension(filename.toStdString());
            ui->trackBarSlider->setEnabled(true);
            this->setWindowTitle(name.fileName());
            ui->trackBarSlider->setMaximum(vidController->getNumberOfFrames());
            int totalSecs = (int)vidController->getNumberOfFrames()/
                    (int)vidController->getFrameRate();
            formatTime.setTotalTime(totalSecs);
            ui->timeLabel->setText(tr(formatTime.getFormatTime().c_str()));
        }
    }
}

void MainWindow::onPlay()
{
    if(vidController->isOpened())
    {
        if(vidController->isStopped())
        {
            vidController->play();
        }
        else
        {
            vidController->stopVid();
        }
    }
    else
    {
        openVid();
    }
}

void MainWindow::updatePlayerUI(QImage img)
{
    if(!img.isNull())
    {
        ui->vidLabel->setAlignment(Qt::AlignCenter);
        ui->vidLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->vidLabel->size(),
                                                                Qt::KeepAspectRatio,
                                                                Qt::FastTransformation));

        ui->trackBarSlider->setValue(vidController->getCurrentFrame());
        this->updateLabelTime();
    }
}

void MainWindow::trackBarSliderMoved(int position)
{
    vidController->setCurrentFrame(position);
    this->updateLabelTime();
}

void MainWindow::trackBarSliderPressed()
{
    vidController->stopVid();
}

void MainWindow::trackBarSliderReleased()
{
    vidController->play();
}

void MainWindow::updateLabelTime()
{
    ui->timeLabel->setText(tr(formatTime.getFormatTime((int)vidController->getCurrentFrame()
                                                   /(int)vidController->getFrameRate()).c_str()));
}
