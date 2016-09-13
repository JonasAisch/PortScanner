#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QProcess>
#include <QDebug>
#include <QHostInfo>
#include <unistd.h>
using namespace std;

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
    void execute();
    void setOwnIP();
    void activateButton();
    void reset();

private:
    Ui::MainWindow *ui;

    void checkPort( int portmin,int portmax, string ip);
    QString getOwnIP();
};

#endif // MAINWINDOW_H
