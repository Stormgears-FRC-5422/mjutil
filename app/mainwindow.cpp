#include <stdio.h>

#include <qfiledialog.h>
#include <qtimer.h>
#include <qpixmap.h>

#include <sys/types.h>
#include <unistd.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonGo, SIGNAL(clicked()), this, SLOT(HandleGoFile()));
    QObject::connect(ui->fileTool, SIGNAL(clicked()), this, SLOT(HandleFileTool()));
    QObject::connect(ui->buttonPlay, SIGNAL(clicked()), this, SLOT(HandlePlay()));

    // FIXME: for debugging purposes, hand pointer to label to pixmap
    //pi.DebugMe(ui->labelImage, ui->scrollImage, ui->spinFrame);

    QObject::connect(ui->scrollImage, SIGNAL(valueChanged(int)), this, SLOT(HandleSlider(int)));
    QObject::connect(ui->scrollImage, SIGNAL(valueChanged(int)), ui->spinFrame, SLOT(setValue(int)));
    QObject::connect(ui->spinFrame, SIGNAL(valueChanged(int)), ui->scrollImage, SLOT(setValue(int)));

    ws = new WebServer(this, &mji);
    QObject::connect(ui->scrollImage, SIGNAL(valueChanged(int)), ws, SLOT(updateFrame(int)));

    ui->buttonPlay->setIcon(QIcon(":/images/play.png"));
    ui->buttonPlay->setText("Play");
    isPlaying = false;
}

void MainWindow::HandleSlider(int v) {
    off_t len = MjiFile::PIXBUF_SIZE;
    if (mji.GetFrame(0, v, pixbuf, len)) {
        px.loadFromData((const uchar *)&pixbuf[0], len);
        ui->labelImage->setPixmap(px);
        ui->labelImage->update();
    }
}

void MainWindow::TimerUpdate() {
    if (isPlaying) {
        int val = ui->scrollImage->value() + 1;
        ui->scrollImage->setValue(val);
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        qint64 next = mji.GetMSec(0, val+1);
        qint64 dt = (next - t0_cap) - (now - t0_real);
        qWarning("scheduling delay of %i msec", dt);
        QTimer::singleShot(dt, this, SLOT(TimerUpdate()) );
    }
}

void MainWindow::HandlePlay() {
    if(ui->buttonPlay->text() == "Play") {
        ui->buttonPlay->setText("Pause");
        ui->buttonPlay->setIcon(QIcon(":/images/pause.png"));
        t0_real = QDateTime::currentMSecsSinceEpoch();
        qWarning("t0 real: %i", t0_real);
        t0_cap = mji.GetMSec(0, ui->scrollImage->value());
        qWarning("t0 cap %i", t0_cap);
        qint64 next = mji.GetMSec(0, ui->scrollImage->value()+1);
        qint64 dt = (next - t0_cap);
        qWarning("scheduling delay of %i msec", dt);
        QTimer::singleShot(dt, this, SLOT(TimerUpdate()));
        isPlaying = true;
    } else {
        ui->buttonPlay->setText("Play");
        ui->buttonPlay->setIcon(QIcon(":/images/play.png"));
        isPlaying = false;
    }
}

void MainWindow::GoFile(const char *name) {
    ui->fileEdit->setText(name);
    HandleGoFile();
}

void MainWindow::HandleGoFile() {
    printf("Open this file: %s!\n", ui->fileEdit->text().toStdString().c_str());
    mji.Open(ui->fileEdit->text().toStdString().c_str());
    HandleSlider(0); // load first image and display in tool
    int n_images = mji.NumFrames(0);
    ui->scrollImage->setMinimum(0);
    ui->scrollImage->setMaximum(n_images-1);
    ui->scrollImage->setValue(0);
    ui->spinFrame->setMinimum(0);
    ui->spinFrame->setMaximum(n_images-1);
    ui->spinFrame->setValue(0);
}

void MainWindow::HandleFileTool() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PCAP file"), ".", tr("PCAP files (*.pcap)"));
    ui->fileEdit->setText(fileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}
