#ifndef PLUGIN_MODBUS_MASTER_H
#define PLUGIN_MODBUS_MASTER_H

#include "serialport_plugins.h"
#include <QWidget>

namespace Ui {
    class Form;
}


class ModbusMaster: public SerialportPlugin
{
    Q_OBJECT

public:
    ModbusMaster();
    ~ModbusMaster();

    const QString name();
    const QString description();
    QWidget * widget();

public Q_SLOTS:
    void newDataArrived(const QByteArray &dat);

private:
    QWidget *form;
    Ui::Form *ui;

    static const QString _name;
    static const QString _description;
};

#endif // PLUGIN_MODBUS_MASTER_H
