#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(execute()));
    connect(ui->checkBox, SIGNAL(clicked(bool)), this, SLOT(setOwnIP()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(activateButton()));
    connect(ui->lineEdit_2, SIGNAL(textChanged(QString)), this, SLOT(activateButton()));
    connect(ui->lineEdit_3, SIGNAL(textChanged(QString)), this, SLOT(activateButton()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(reset()));
    ui->lineEdit_3->setInputMask("000.000.000.000;_");
    ui->lineEdit->setValidator( new QIntValidator(0, 65535, this) );
    ui->lineEdit_2->setValidator( new QIntValidator(0, 65535, this) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkPort(int portmin, int portmax, string ip) {
    int dif = portmax - portmin;
    if (dif <= 0)
        return;

    unsigned int open = 0;
    unsigned int closed = 0;
    int checked = 0;

    ui->progressBar->setMaximum(dif);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    for (int i = portmin; i <= portmax; i++) {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_port = htons(i);

        int con = ::connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr));

        if (con == 0){
            ui->textEdit->setTextColor(Qt::green);
            ui->textEdit->append("Port " + QString::number(i) + " open.");
            open++;
            QApplication::beep();
        }

        if (con == -1) {
            ui->textEdit->setTextColor(Qt::red);
            ui->textEdit->append("Port " + QString::number(i) + " closed.");
            closed++;
        }

        ::close(con);
        ::close(s);
        checked++;
        ui->progressBar->setValue(checked);
        QApplication::processEvents();
    }

    QPalette *pal1 = new QPalette();
    pal1->setColor(QPalette::WindowText, Qt::green);
    ui->label_4->setPalette(*pal1);
    ui->label_4->setText(QString::number(open));

    QPalette *pal2 = new QPalette();
    pal2->setColor(QPalette::WindowText, Qt::red);
    ui->label_6->setPalette(*pal2);
    ui->label_6->setText(QString::number(closed));
}

void MainWindow::execute() {
    int portmin = ui->lineEdit->text().toInt();
    int portmax = ui->lineEdit_2->text().toInt();
    string ip = ui->lineEdit_3->text().toStdString();

    ui->pushButton->setText("Cancel");

    checkPort(portmin, portmax, ip);
}

void MainWindow::setOwnIP() {
    if (ui->checkBox->isChecked())
        ui->lineEdit_3->setText(getOwnIP());
    if (!(ui->checkBox->isChecked()))
        ui->lineEdit_3->setText("");
}

QString MainWindow::getOwnIP() {
    QString ip = "";
    QHostInfo info = QHostInfo::fromName( QHostInfo::localHostName() );
        QList<QHostAddress> l= info.addresses();
        for(int i=0; i<l.count(); i++) {
            ip += l[i].toString();
        }

        return ip;
    }

void MainWindow::activateButton() {
    if (ui->lineEdit->text() != "" && ui->lineEdit_2->text()!= "" && ui->lineEdit_3->text()!= "") {
        ui->pushButton->setEnabled(true);
    }
    else {
        ui->pushButton->setEnabled(false);
    }
}

void MainWindow::reset() {
    ui->textEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit->clear();
    ui->progressBar->setValue(0);
    ui->label_4->setText("");
    ui->label_6->setText("");
}
