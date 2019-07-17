#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    noPromptBeforeExit(false)
{
    ui->setupUi(this);

    loadConfigures();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!noPromptBeforeExit)
    {
        int ans = QMessageBox::question(this,
                                        tr(""),
                                        tr("确实要退出程序？"),
                                        QMessageBox::Yes| QMessageBox::No,
                                        QMessageBox::No);

        if(ans != QMessageBox::Yes)
        {
            event->ignore();
            return;
        }
    }

    restoreConfigures();

    event->accept();
}

void MainWindow::loadConfigures()
{
    QSettings s("cfg.ini", QSettings::IniFormat);

    noPromptBeforeExit = s.value("noPromptBeforeExit", false).toBool();
}

void MainWindow::restoreConfigures()
{
    QSettings s("cfg.ini", QSettings::IniFormat);

    s.setValue("noPromptBeforeExit", noPromptBeforeExit);
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       tr(""),
                       tr("Serialport tools"));
}
