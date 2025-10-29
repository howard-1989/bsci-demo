#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDir>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void checkUsb();
    QString detectUsbPath();
    bool copyRecursively(const QString &srcPath, const QString &dstPath);

    QTimer *timer;
    QString lastUsbPath;
    QString sourceDir;

private slots:
    void on_btn_changepassword_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
