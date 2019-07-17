#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogparams.h"

#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QSerialPort>
#include <QLabel>
#include <QScrollBar>
#include <QTextCursor>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    codecName("GBK"),
    portName("COM1"),
    baudrate(9600),
    databits(8),
    stopbits(1),
    parity('N'),
    flowctrl(0),
    isPortOpen(false),
    noPromptBeforeExit(false),
    bytesRecv(0),
    bytesSend(0),
    timerId(0),
    rexHexCharts("[^0-9A-Fa-f ]")
{
    ui->setupUi(this);
    modUI();

    loadConfigures();

    serialport = new QSerialPort();
    codec = QTextCodec::codecForName(codecName.toLocal8Bit());

    connect(ui->pushButtonClean, &QPushButton::clicked, this, &MainWindow::clean);
    connect(serialport, &QSerialPort::readyRead, this, &MainWindow::serialportReadyRead);

    codecInfo->setText(codecName);

    updatePortInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::modUI()
{
    portInfo = new QLabel(tr("Hello"));
    portInfo->setFrameShape(QFrame::Box);
    portInfo->setFrameShadow(QFrame::Sunken);

    trafficInfo = new QLabel(tr("R: 0 | T: 0"));
    trafficInfo->setFrameShape(QFrame::Box);
    trafficInfo->setFrameShadow(QFrame::Sunken);

    codecInfo = new QLabel(tr("-"));
    codecInfo->setFrameShape(QFrame::Box);
    codecInfo->setFrameShadow(QFrame::Sunken);

    statusBar()->addPermanentWidget(trafficInfo);
    statusBar()->addPermanentWidget(codecInfo);
    statusBar()->addPermanentWidget(portInfo);

    ui->checkBoxTimestamp->hide();
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

    codecName = s.value("codecName", "GBK").toString();

    noPromptBeforeExit = s.value("noPromptBeforeExit", false).toBool();
    portName = s.value("port", "COM1").toString();
    baudrate = s.value("baudrate", 9600).toInt();
    databits = s.value("databits", 8).toInt();
    stopbits = s.value("stopbits", 1).toInt();
    parity = s.value("parity", 'N').toChar().toLatin1();
    flowctrl = s.value("flowctrl", 0).toInt();
}

void MainWindow::restoreConfigures()
{
    QSettings s("cfg.ini", QSettings::IniFormat);

    s.setValue("codecName", codecName);
    s.setValue("noPromptBeforeExit", noPromptBeforeExit);
    s.setValue("port", portName);
    s.setValue("baudrate", baudrate);
    s.setValue("databits", databits);
    s.setValue("stopbits", stopbits);
    s.setValue("parity", QChar(parity));
    s.setValue("flowctrl", flowctrl);
}

void MainWindow::updatePortInfo()
{
    QString fc("OFF");
    QString st("x");

    if(flowctrl == 1)
        fc = tr("HW");
    else if(flowctrl == 2)
        fc = tr("SW");

    if(stopbits == 1)
        st = tr("1");
    else if(stopbits == 2)
        st = tr("2");
    else if(stopbits == 3)
        st = tr("1.5");

    portInfo->setText(tr("%1 %2,%3,%4,%5 | FlowCtrl: %6 | %7")
                      .arg(portName)
                      .arg(baudrate)
                      .arg(databits)
                      .arg(st)
                      .arg(QChar(parity))
                      .arg(fc)
                      .arg(isPortOpen? tr("打开"): tr("关闭")));
}

void MainWindow::updateTrafficInfo()
{
    trafficInfo->setText(tr("R: %1 | T: %2").arg(bytesRecv).arg(bytesSend));
}

bool MainWindow::openPort()
{
    if(!isPortOpen)
    {
        serialport->setPortName(portName);
        serialport->setBaudRate(baudrate);
        serialport->setDataBits(QSerialPort::DataBits(databits));
        serialport->setStopBits(QSerialPort::StopBits(stopbits));
        serialport->setParity(QSerialPort::Parity(parity));
        serialport->setFlowControl(QSerialPort::FlowControl(flowctrl));

        isPortOpen = serialport->open(QSerialPort::ReadWrite);
        if(!isPortOpen)
        {
            qDebug() << serialport->errorString();
        }
    }

    return isPortOpen;
}

void MainWindow::closePort()
{
    if(!isPortOpen)
    {
        return;
    }

    serialport->close();
    isPortOpen = false;
}

void MainWindow::serialportReadyRead()
{
    QByteArray r = serialport->readAll();
    newDataArrived(r);
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

void MainWindow::on_actionTogglePort_triggered()
{
    if(!isPortOpen)
    {
        openPort();

        if(!isPortOpen)
        {
            QMessageBox::critical(this,
                                  tr("错误"),
                                  tr("不能打开 %1, 可能串口已被占用.").arg(portName));

            return;
        }
        else
        {
            ui->actionTogglePort->setText(tr("关闭串口(&T)"));
            ui->actionTogglePort->setIcon(QIcon(":/icons/www.easyicon.net/network_port_32px_1083441_easyicon.net.png"));
            ui->actionTogglePort->setToolTip(tr("关闭串口"));
            ui->actionTogglePort->setStatusTip(tr("关闭串口"));

            ui->actionSetSerialParams->setEnabled(false);
        }
    }
    else
    {
        closePort();

        ui->actionTogglePort->setText(tr("打开串口(&T)"));
        ui->actionTogglePort->setIcon(QIcon(":/icons/www.easyicon.net/network_port_disconnect_32px_1083440_easyicon.net.png"));
        ui->actionTogglePort->setToolTip(tr("打开串口"));
        ui->actionTogglePort->setStatusTip(tr("打开串口"));

        ui->actionSetSerialParams->setEnabled(true);
    }

    updatePortInfo();
}

void MainWindow::on_actionSetSerialParams_triggered()
{
    DialogParams dlg(this);

    if(dlg.exec() != QDialog::Accepted)
    {
        return;
    }

    dlg.getParams(&portName, &baudrate, &databits, &stopbits, &parity, &flowctrl);

    updatePortInfo();
}

void MainWindow::newDataArrived(const QByteArray &dat)
{
    QString txt;

    if(ui->checkBoxHexRecv->isChecked())
    {
        txt = QString(dat.toHex(' ')) + QString(" ");
    }
    else
    {
        txt = codec->toUnicode(dat);
    }

    appendPlainText(txt);

    recvData.append(dat);
    bytesRecv += quint64(dat.size());

    updateTrafficInfo();
}

QByteArray MainWindow::prepeData(const QString &text)
{
    QByteArray retValue;

    if(ui->checkBoxHexSend->isChecked())
    {
        if(rexHexCharts.indexIn(text) < 0)
        {
            retValue = QByteArray::fromHex(text.toLocal8Bit());
        }
    }
    else
    {
        QString t(text);

        if(ui->checkBoxNewLine->isChecked())
        {
            t.append("\r\n");
        }

        if(codec->canEncode(t))
        {
            retValue = codec->fromUnicode(t);
        }
    }

    return retValue;
}

qint64 MainWindow::send(const QByteArray &dat)
{
    qint64 n = serialport->write(dat);

    if(n > 0)
    {
        bytesSend += quint64(n);
        updateTrafficInfo();
    }

    return n;
}

void MainWindow::appendPlainText(const QString &text, bool check_nl)
{
   QScrollBar *p_scroll_bar = ui->plainTextEdit->verticalScrollBar();
   bool bool_at_bottom = (p_scroll_bar->value() == p_scroll_bar->maximum());

   if (!check_nl)
   {
      QTextCursor text_cursor = QTextCursor(ui->plainTextEdit->document());
      text_cursor.movePosition(QTextCursor::End);
      text_cursor.insertText(text);
   }
   else
   {
      QTextCursor text_cursor = QTextCursor(ui->plainTextEdit->document());
      text_cursor.beginEditBlock();

      text_cursor.movePosition(QTextCursor::End);

      QStringList string_list = text.split('\n');

      for (int i = 0; i < string_list.size(); i++){
         text_cursor.insertText(string_list.at(i));
         if ((i + 1) < string_list.size()){
            text_cursor.insertBlock();
         }
      }

      text_cursor.endEditBlock();
   }

   if (bool_at_bottom)
   {
      p_scroll_bar->setValue(p_scroll_bar->maximum());
   }
}

void MainWindow::clean()
{
    recvData.clear();

    ui->plainTextEdit->clear();

    bytesRecv = 0;
    bytesSend = 0;

    updateTrafficInfo();
}

void MainWindow::on_checkBoxHexRecv_clicked(bool checked)
{
    QString txt;

    if(checked)
    {
        txt = QString(recvData.toHex(' ')) + QString(" ");
    }
    else
    {
        txt = codec->toUnicode(recvData);
    }

    ui->plainTextEdit->setPlainText(txt);
}

qint64 MainWindow::doSend(const QString &text)
{
    QByteArray dat = prepeData(text);

    if(!dat.isEmpty())
    {
        return send(dat);
    }

    return 0;
}

void MainWindow::on_pushButtonSend_clicked()
{
    if(!isPortOpen)
    {
        QMessageBox::warning(this,
                             tr("错误"),
                             tr("串口尚未打开"));
        return;
    }

    QString text = ui->lineEdit_2->text();

    if(text.isEmpty())
    {
        return;
    }

    if(doSend(text) < 0)
    {
        statusBar()->showMessage(tr("发送失败"));
    }
}

void MainWindow::on_checkBoxTimer_released()
{
    if(ui->checkBoxTimer->isChecked())
    {
        if(!isPortOpen)
        {
            ui->checkBoxTimer->setCheckState(Qt::Unchecked);

            QMessageBox::warning(this,
                                 tr("错误"),
                                 tr("串口尚未打开"));
            return;
        }

        int tick = ui->lineEditTime->text().toInt();
        if(tick == 0)
        {
            ui->checkBoxTimer->setCheckState(Qt::Unchecked);

            QMessageBox::warning(this,
                                 tr("错误"),
                                 tr("定时周期输入有误"));
            return;
        }

        timerId = startTimer(tick);
        ui->lineEditTime->setEnabled(false);
    }
    else
    {
        killTimer(timerId);
        timerId = 0;
        ui->lineEditTime->setEnabled(true);
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerId)
    {
        QString text = ui->lineEdit_2->text();
        if(text.isEmpty())
        {
            ui->checkBoxTimer->click();
            statusBar()->showMessage(tr("内容为空，定时发送已经取消"), 5000);

            return;
        }

        if(doSend(text) < 0)
        {
            ui->checkBoxTimer->click();
            statusBar()->showMessage(tr("发送失败，定时发送已经取消"), 5000);
        }
    }
}
