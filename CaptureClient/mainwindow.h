#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CaptureVideoAndAudio.h"
#include "sniffer.h"
#include "CapRTSPLive.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_stateButton_clicked();

    void on_Open_clicked();

    void on_uploadButton_clicked();

    void on_liveState_clicked();

private:
    Ui::MainWindow *ui;
    CaptureVideoAndAudio *recorder;
    liveBuffer *liveSource;
    CapRTSPLive* liveServer;
    Sniffer networkSniffer;
    bool isLive;
    bool status;
    std::string snifferFileName;
    std::string capFileName;
    std::string outDir ;
    std::string snifferFile;
    std::string capFile;
    std::string userName;
    std::string password;
    std::string ftpDir;
};

#endif // MAINWINDOW_H
