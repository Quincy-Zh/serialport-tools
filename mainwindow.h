#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionAboutQt_triggered();

    void on_actionAbout_triggered();

private:
    void loadConfigures();
    void restoreConfigures();

private:
    Ui::MainWindow *ui;

    bool noPromptBeforeExit;
};

#endif // MAINWINDOW_H
