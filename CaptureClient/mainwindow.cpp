#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    status = false;
    isLive = false;
    recorder = new CaptureVideoAndAudio();
    liveServer = new CapRTSPLive();
    liveSource = new liveBuffer();
    ui->uABitrate->setText(QString::number(recorder->aBitRate,10));
    ui->uVBitrate->setText(QString::number(recorder->vBitRate,10));
    ui->uFramerate->setText(QString::number(recorder->frameRate,10));
    char *nicDescription;
    while(networkSniffer.getDescription(&nicDescription))
        ui->NICinfo->addItem(QString(nicDescription));

    userName = "user";
    password = "12345";
    ftpDir = "/test";
    snifferFileName = "sniffer.log";
    capFileName = "cap.mp4";
    outDir = ".\\out\\";
    snifferFile = outDir + snifferFileName;
    capFile = outDir + capFileName;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_stateButton_clicked()
{
    if(status){
        ui->stateButton->setText("begin");

        //stop capture
        if(isLive){
            on_liveState_clicked();
            isLive = false;
        }

        recorder->stop();

        networkSniffer.stopSniffing();
        status = !status;

    }else{

        bool ok = false;
        int frameRate = ui->uFramerate->text().toInt(&ok, 10);
        int vBitRate = ui->uVBitrate->text().toInt(&ok, 10);
        int aBitRate = ui->uABitrate->text().toInt(&ok, 10);
        if(ok){
            // start capture

            recorder->start(vBitRate, aBitRate, frameRate, capFile.c_str(), liveSource);
        }
        int nicIndex = ui->NICinfo->currentIndex();

        networkSniffer.startSniffing(nicIndex + 1, snifferFile.c_str());
        status = !status;
        ui->stateButton->setText("stop");
    }
}

void MainWindow::on_Open_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("out"));
}

void MainWindow::on_uploadButton_clicked()
{
    std::string serverIp = ui->serverAddr->text().toStdString();
    if(serverIp.length() < 4)
    {
         QMessageBox message(QMessageBox::Warning,"Information","Incorrect IP address!",QMessageBox::Ok,NULL);
         message.exec();
         return;
    }
    std::string cmd = "bin\\ftp.exe "+serverIp+" " + userName +" "+ password +" " + ftpDir\
            +" " + snifferFileName + " " + "out/" + snifferFileName;
    system(cmd.c_str());
    cmd = "bin\\ftp.exe "+serverIp+" " + userName +" "+ password +" " + ftpDir\
            +" " + capFileName + " " + "out/" + capFileName;
    system(cmd.c_str());
}

void MainWindow::on_liveState_clicked()
{
    if(!status)return;
    if(isLive){
        ui->liveState->setText("start live");
        isLive = !isLive;
        liveServer->close();
    }else{
        ui->liveState->setText("Stop live");
        const int factor = 5;
        int audioBufferSize = recorder->aBitRate / 8 * factor;
        int videoBufferSize = recorder->vBitRate / 8 * factor;
        liveSource->init(audioBufferSize, videoBufferSize);
        liveServer->open(liveSource, recorder->aBitRate / 1000, recorder->vBitRate / 1000, recorder->frameRate);
        isLive = !isLive;
    }
}
