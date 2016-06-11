#include <stdio.h>

#include <qfiledialog.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonGo, SIGNAL(clicked()), this, SLOT(HandleGoFile()));
    QObject::connect(ui->fileTool, SIGNAL(clicked()), this, SLOT(HandleFileTool()));

    // FIXME: for debugging purposes, hand pointer to label to pixmap
    pi.DebugMe(ui->labelImage, ui->scrollImage, ui->spinFrame);

    QObject::connect(ui->scrollImage, SIGNAL(valueChanged(int)), this, SLOT(HandleSlider(int)));
    QObject::connect(ui->scrollImage, SIGNAL(valueChanged(int)), ui->spinFrame, SLOT(setValue(int)));
    QObject::connect(ui->spinFrame, SIGNAL(valueChanged(int)), ui->scrollImage, SLOT(setValue(int)));

    ws = new WebServer(this, &pi);
    QObject::connect(ui->scrollImage, SIGNAL(valueChanged(int)), ws, SLOT(updateFrame(int)));
}

void MainWindow::HandleSlider(int v) {
    pi.UpdateImage(v);
}

void MainWindow::GoFile(const char *name, int nFrom, int nTo) {
    ui->fileEdit->setText(name);
    HandleGoFile(nFrom, nTo);
}

void MainWindow::HandleGoFile(int nFrom, int nTo) {
    printf("Open this file: %s!\n", ui->fileEdit->text().toStdString().c_str());
    pi.Open(ui->fileEdit->text().toStdString().c_str(),nFrom,nTo);
}

void MainWindow::HandleFileTool() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PCAP file"), ".", tr("PCAP files (*.pcap)"));
    ui->fileEdit->setText(fileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}
