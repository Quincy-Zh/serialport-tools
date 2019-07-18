#ifndef DIALOGPARAMS_H
#define DIALOGPARAMS_H

#include <QDialog>

namespace Ui {
class DialogParams;
}

class DialogParams : public QDialog
{
    Q_OBJECT

public:
    explicit DialogParams(QWidget *parent = nullptr);
    ~DialogParams();

    int getParams(QString *name,
                  int *baudrate,
                  int *databits,
                  int *stopbits,
                  int *parity,
                  int *flowctrl);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_toolButton_clicked();

    void on_comboBoxName_currentIndexChanged(int index);

private:
    void refrushPortsInfo();

private:
    Ui::DialogParams *ui;
    QString name;
    int baudrate;
    int databits;
    int stopbits;
    int parity;
    int flowctrl;
};

#endif // DIALOGPARAMS_H
