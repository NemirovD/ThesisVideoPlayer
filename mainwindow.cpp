#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //DO NOT DELETE THE NEXT LINE
    ui->setupUi(this); // if deleted there will be no UI and
    //everything will not be good


    vidController = new VidController(this);
    connect(vidController,
            SIGNAL(processedImage(QImage)),
            this,
            SLOT(updatePlayerUI(QImage)));

    //there doesn't seem to be a way to change visibility
    //in the UI editor so it has to be done here
    ui->sideBar->setVisible(false);
}
MainWindow::~MainWindow()
//when mainwindow tries to be deconstructed it will tell
//vidController to stop what it's doing and then quit.
{
    vidController->exit();
    vidController->wait();
    delete ui;
    qApp->quit();
}

void MainWindow::openVid()
//the following code opens a file and then tells the videocontroller
//to prepare it for viewing. You should be able to understand what's
//going on by the function names.
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

            //if OpenCV reports the wrong framerate then:
            //game over man, it's over, its over
            int totalSecs = (int)vidController->getNumberOfFrames()/
                    (int)vidController->getFrameRate();

            formatTime.setTotalTime(totalSecs);
            ui->timeLabel->setText(tr(formatTime.getFormatTime().c_str()));
        }
    }
}

void MainWindow::onPlay()
{
    //plays or pauses the video
    //if there is no video open
    //open the file dialog so that
    //you can.
    //Just realised that there's no
    //error checking to see if the chosen
    //file is actually a video
    //I wonder what would happen.
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
        //This portion of the code shows takes the QImage provided
        //and applies it to the VidLabel, I don't remember what the
        //setAlignment portion of the code is for but I'm sure if
        //you removed it you'd understand
        ui->vidLabel->setAlignment(Qt::AlignCenter);
        ui->vidLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->vidLabel->size(),
                                                                Qt::KeepAspectRatio,
                                                                Qt::FastTransformation));

        //This next block only updates the trackBar if the
        //video is currently playing
        //this is so that you can move the trackBar easily
        //without the slider jumping back to the old position
        //A Better version of this code is to
        //have a boolean that sees if the trackBar isPressed
        //that might already exist I dunno
        if(!vidController->isStopped()){
            ui->trackBarSlider->setValue(vidController->getCurrentFrame());
            this->updateLabelTime();
        }
    }
}

void MainWindow::trackBarSliderPressed()
//pretty obvious what happens here
{
    vidController->stopVid();
}

void MainWindow::trackBarSliderReleased()
//should also be obvious what happens here
//OH if it's not totally clear the trackBarSlider->value()
//gets a number that's between 0 and the total number of frames
//because we set the max val for the trackBar to be the last frame
//of the video in the openVid function
{
    vidController->setCurrentFrame(ui->trackBarSlider->value());
    this->updateLabelTime();

    vidController->play();
}

void MainWindow::updateLabelTime()
//should be obvious what's happening here by the
//function name
{
    ui->timeLabel->setText(tr(formatTime.getFormatTime((int)vidController->getCurrentFrame()
                                                   /(int)vidController->getFrameRate()).c_str()));
}
