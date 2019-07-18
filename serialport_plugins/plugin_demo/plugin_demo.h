#ifndef PLUGIN_DEMO_H
#define PLUGIN_DEMO_H

#include "serialport_plugins.h"

class Demo: public SerialportPlugin
{
    Q_OBJECT

public:
    Demo();
    ~Demo();

    const QString name();
    const QString description();
    QWidget * widget();

public Q_SLOTS:
    void newDataArrived(const QByteArray &dat);

private:
    static const QString _name;
    static const QString _description;
};

#endif // PLUGIN_DEMO_H
