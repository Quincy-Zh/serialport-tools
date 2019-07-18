#include "dialogactivateplugin.h"
#include "ui_dialogactivateplugin.h"

#include "serialport_plugins.h"

#include <QDir>
#include <QFileInfoList>
#include <QDebug>
#include <QLibrary>
#include <QComboBox>

DialogActivatePlugin::DialogActivatePlugin(const QString &root, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogActivatePlugin),
    pluginPath("")
{
    ui->setupUi(this);

    QDir dir(root);

    foreach(QFileInfo inf, dir.entryInfoList(QStringList() << "*.dll"))
    {
        QLibrary lib(inf.absoluteFilePath());

        if(!lib.load())
        {
            qDebug() << lib.errorString();

            continue;
        }

        getPluginInstance instance = getPluginInstance(lib.resolve("getInstance"));
        if(!instance)
        {
            qDebug() << "Resolve FAIL.";

            lib.unload();
            continue;
        }

        SerialportPlugin *plugin = instance();
        if(!plugin)
        {
            qDebug() << "getInstance FAIL.";
            lib.unload();

            continue;
        }

        QString name = plugin->name();
        QString desc = plugin->description();
        int index = ui->comboBox->count();

        delete plugin;
        lib.unload();

        ui->comboBox->addItem(name);
        ui->comboBox->setItemData(index, desc, Qt::UserRole);
        ui->comboBox->setItemData(index, inf.absoluteFilePath(), Qt::UserRole+1);
    }
}

DialogActivatePlugin::~DialogActivatePlugin()
{
    delete ui;
}

void DialogActivatePlugin::on_buttonBox_accepted()
{
    pluginPath = ui->comboBox->currentData(Qt::UserRole+1).toString();

    QDialog::accept();
}

void DialogActivatePlugin::on_buttonBox_rejected()
{
    QDialog::reject();
}

void DialogActivatePlugin::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    ui->textBrowser->setPlainText(
                ui->comboBox->currentData(Qt::UserRole).toString());
}

const QString DialogActivatePlugin::getPluginPath()
{
    return pluginPath;
}
