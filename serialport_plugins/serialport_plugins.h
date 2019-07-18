#ifndef SERIALPORT_PLUGINS_H
#define SERIALPORT_PLUGINS_H

#include <QtCore/qglobal.h>
#include<QObject>

class QWidget;

#if defined(SERIALPORT_PLUGIN)
#  define SERIALPORT_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define SERIALPORT_PLUGIN_EXPORT Q_DECL_IMPORT
#endif

class SerialportPlugin : public QObject
{
    Q_OBJECT

public:
    virtual ~SerialportPlugin() {}

    virtual const QString name() = 0;
    virtual const QString description() = 0;

    virtual QWidget * widget() = 0;

public Q_SLOTS:
    virtual Q_INVOKABLE void newDataArrived(const QByteArray &dat) = 0;

signals:
    void requestSend(const QByteArray &dat);
};

typedef SerialportPlugin *(*getPluginInstance)();

#endif // SERIALPORT_PLUGINS_H
