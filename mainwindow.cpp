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
}

void MainWindow::GoFile(const char *name) {
    ui->fileEdit->setText(name);
    HandleGoFile();
}

void MainWindow::HandleGoFile() {
    printf("Open this file: %s!\n", ui->fileEdit->text().toStdString().c_str());
    pi.Open(ui->fileEdit->text().toStdString().c_str(), "tcp port 80");
}

void MainWindow::HandleFileTool() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PCAP file"), ".", tr("PCAP files (*.pcap)"));
    ui->fileEdit->setText(fileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}
