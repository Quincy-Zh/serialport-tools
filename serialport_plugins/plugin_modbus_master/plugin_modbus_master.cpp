#include "plugin_modbus_master.h"
#include "ui_form.h"

const QString ModbusMaster::_name("Modbus Master");
const QString ModbusMaster::_description("Modbus master plugin. A Modbus client.");

ModbusMaster::ModbusMaster() :
    form(new QWidget),
    ui(new Ui::Form)
{
    ui->setupUi(form);
}

ModbusMaster::~ModbusMaster()
{
    ;
}

const QString ModbusMaster::name()
{
    return _name;
}

const QString ModbusMaster::description()
{
    return _description;
}

QWidget * ModbusMaster::widget()
{
    return form;
}

void ModbusMaster::newDataArrived(const QByteArray &dat)
{
    emit requestSend(dat);
}

extern "C" SERIALPORT_PLUGIN_EXPORT SerialportPlugin * getInstance()
{
    return new ModbusMaster();
}
