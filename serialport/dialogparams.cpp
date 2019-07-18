#include "dialogparams.h"
#include "ui_dialogparams.h"

#include <QSerialPortInfo>
#include <QIcon>

DialogParams::DialogParams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogParams),
    name(""),
    baudrate(0),
    databits(0),
    stopbits(0),
    parity(0),
    flowctrl(0)
{
    ui->setupUi(this);

    ui->toolButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    refrushPortsInfo();
}

DialogParams::~DialogParams()
{
    delete ui;
}

void DialogParams::refrushPortsInfo()
{
    ui->comboBoxName->clear();

    foreach(QSerialPortInfo inf, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxName->addItem(inf.portName(), inf.description());
    }
}

void DialogParams::on_buttonBox_accepted()
{
    name = ui->comboBoxName->currentText();
    baudrate = ui->comboBoxBaudrate->currentText().toInt();
    databits = ui->comboBoxDatabits->currentText().toInt();
    stopbits = ui->comboBoxStopbits->currentIndex() + 1;
    parity = ui->comboBoxParity->currentIndex();
    if(parity > 0)
        parity += 1;

    flowctrl = ui->comboBoxFlowctrl->currentIndex();

    QDialog::accept();
}

void DialogParams::on_buttonBox_rejected()
{
    QDialog::reject();
}

void DialogParams::on_toolButton_clicked()
{
    refrushPortsInfo();
}

void DialogParams::on_comboBoxName_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    ui->labelPortDesc->setText(tr("# %1 %2")
                               .arg(ui->comboBoxName->currentText())
                               .arg(ui->comboBoxName->currentData().toString()));
}

int DialogParams::getParams(QString *name,
              int *baudrate,
              int *databits,
              int *stopbits,
              int *parity,
              int *flowctrl)
{
    *name = this->name;
    *baudrate = this->baudrate;
    *databits = this->databits;
    *stopbits = this->stopbits;
    *parity = this->parity;
    *flowctrl = this->flowctrl;

    return 0;
}
