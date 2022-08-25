#ifndef REJSTER_H
#define REJSTER_H

#include <QDialog>
#include <QJsonObject>
#include "conaunicate.h"
#include "../commo.h"
#include <QMessageBox>
namespace Ui {
class rejster;
}

class rejster : public QDialog
{
    Q_OBJECT

public:
    explicit rejster(Conaunicate *com, QWidget *parent = 0);
    ~rejster();

private slots:
    void on_rejsterpushButton_clicked();

    void on_brankpushButton_clicked();

    void rstResult(int ret);

private:
    Ui::rejster *ui;
    Conaunicate *m_com;
};

#endif // REJSTER_H
