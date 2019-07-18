#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExp>

namespace Ui {
class MainWindow;
}

class QLabel;
class QSerialPort;
class QTextCodec;

class SerialportPlugin;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);

private slots:
    void serialportReadyRead();
    void clean();
    qint64 doSend(const QString &text);
    void pluginRequest(const QByteArray &dat);

    void on_actionAboutQt_triggered();

    void on_actionAbout_triggered();

    void on_actionTogglePort_triggered();

    void on_actionSetSerialParams_triggered();

    void on_checkBoxHexRecv_clicked(bool checked);

    void on_pushButtonSend_clicked();

    void on_checkBoxTimer_released();

    void on_actionActivatePlugin_triggered();

private:
    void modUI();
    void updatePortInfo();
    void loadConfigures();
    void restoreConfigures();
    bool openPort();
    void closePort();


    QByteArray prepeData(const QString &text);
    qint64 send(const QByteArray &dat);

    void appendPlainText(const QString &text, bool check_nl=false);
    void newDataArrived(const QByteArray &dat);

    void updateTrafficInfo();

    void loadPlugin(const QString &path);

signals:
    void serialportHasNewData(const QByteArray &);

private:
    Ui::MainWindow *ui;
    QTextCodec *codec;
    QSerialPort *serialport;
    QLabel *portInfo;
    QLabel *trafficInfo;
    QLabel *codecInfo;

    QString codecName;
    QString portName;
    int baudrate;
    int databits;
    int stopbits;
    int parity;
    int flowctrl;

    bool isPortOpen;
    QByteArray recvData;

    bool noPromptBeforeExit;

    quint64 bytesRecv;
    quint64 bytesSend;

    int timerId;

    QRegExp rexHexCharts;

    SerialportPlugin *plugin;
    int pluginTabPageIndex;

    static const QString pluginsPath;
};

#endif // MAINWINDOW_H
