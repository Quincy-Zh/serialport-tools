#ifndef DIALOGACTIVATEPLUGIN_H
#define DIALOGACTIVATEPLUGIN_H

#include <QDialog>

namespace Ui {
class DialogActivatePlugin;
}

class DialogActivatePlugin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogActivatePlugin(const QString &root, QWidget *parent = nullptr);
    ~DialogActivatePlugin();

    const QString getPluginPath();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::DialogActivatePlugin *ui;

    QString pluginPath;
};

#endif // DIALOGACTIVATEPLUGIN_H
