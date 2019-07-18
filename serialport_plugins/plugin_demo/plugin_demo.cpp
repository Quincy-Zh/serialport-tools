#include "plugin_demo.h"


const QString Demo::_name("Demo");
const QString Demo::_description("Serialport-plugin \"DEMO\".");

Demo::Demo()
{
}

Demo::~Demo()
{
    ;
}

const QString Demo::name()
{
    return _name;
}

const QString Demo::description()
{
    return _description;
}

QWidget * Demo::widget()
{
    return nullptr;
}

void Demo::newDataArrived(const QByteArray &dat)
{
    // Q_UNUSED(dat)

    emit requestSend(dat);
}

extern "C" SERIALPORT_PLUGIN_EXPORT SerialportPlugin * getInstance()
{
    return new Demo();
}
