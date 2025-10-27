#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize(1920, 1080);
    this->setFixedSize(1920, 1080);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
    this->showFullScreen();

    sourceDir = "/home/rd/Documents/backup/";
    ui->progressBar->setValue(0);
    ui->labelStatus->setText("Waiting for USB Plugin...");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkUsb);
    timer->start(2000);

    // Check if the USB is already plugged in when the app starts
    QString usbPath = detectUsbPath();
    if (!usbPath.isEmpty()) {
        // USB detected, immediately start copying files
        ui->labelStatus->setText("USB already plugged in: " + usbPath);
        printf("Detected USB at startup: %s\n", usbPath.toStdString().c_str());
        lastUsbPath = usbPath;  // Set it as the last detected USB
        copyRecursively(sourceDir, usbPath); // Start copying files immediately
    }
}

QString MainWindow::detectUsbPath()
{
    QStringList mountRoots = {"/media", "/run/media"};
    QString user = qgetenv("USER");

    for (const QString &root : mountRoots) {
        QDir base(root + "/" + user);
        if (!base.exists())
            continue;

        QStringList disks = base.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &d : disks) {
            QString full = base.absoluteFilePath(d);
            QFileInfo fi(full);
            if (fi.isDir() && fi.isWritable()) {
                return full;
            }
        }
    }
    return "";
}

bool MainWindow::copyRecursively(const QString &srcPath, const QString &dstPath)
{
    printf("copy image\n");

    QDir srcDir(srcPath);
        QStringList files = srcDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

        if (files.isEmpty()) {
            return false; // nothing to move
        }

        ui->labelStatus->setText("Copying files...");
        ui->progressBar->setRange(0, 0);

        QProcess process;
        QString cmd = QString("bash -c \"mv '%1'/* '%2'/\"").arg(srcPath, dstPath);
        int ret = process.execute(cmd);

        ui->progressBar->setRange(0, 100);
        ui->progressBar->setValue(100);

        return (ret == 0);
}

void MainWindow::checkUsb()
{
    QString usbPath = detectUsbPath();

    if (!usbPath.isEmpty()) {
        // USB detected
        if (usbPath != lastUsbPath) {
            ui->labelStatus->setText("Detected USB: " + usbPath);
            printf("Detected USB: %s\n", usbPath.toStdString().c_str());
            lastUsbPath = usbPath;
        }

        // Continuously move files while USB is plugged in
        bool ok = copyRecursively(sourceDir, usbPath);
        if (ok) {
            ui->labelStatus->setText("✅ Files moved to: " + usbPath);
        } else {
            ui->labelStatus->setText("⚠️ No new files to move or copy failed");
        }
    }
    else if (usbPath.isEmpty() && !lastUsbPath.isEmpty()) {
        // USB removed
        ui->labelStatus->setText("USB Removed");
        ui->progressBar->setValue(0);
        lastUsbPath.clear();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
