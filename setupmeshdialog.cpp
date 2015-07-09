#include <QMessageBox>
#include "setupmeshdialog.h"
#include "ui_setupmeshdialog.h"

SetupMeshDialog::SetupMeshDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupMeshDialog)
{
    ui->setupUi(this);

    connect(ui->hsRadius, SIGNAL(valueChanged  (int)),this, SLOT(changeRadius(int)));
    connect(ui->hsEdge, SIGNAL(valueChanged  (int)),this, SLOT(changeEdge(int)));
}

SetupMeshDialog::~SetupMeshDialog()
{
    delete ui;
}

void SetupMeshDialog::changeRadius(int pos)
{
    ui->laRadius->setText(tr("%1").arg(float(pos)*0.2,3,'f',1));
}

void SetupMeshDialog::changeEdge(int pos)
{
    ui->laEdge->setText(tr("%1").arg(float(pos)*0.2,3,'f',1));
}

void SetupMeshDialog::set(double *params)
{
    ui->leFacets->setText(QString("%1").arg(params[0]));
    ui->leDetail->setText(QString("%1").arg(params[1]));
    ui->leMinX->setText(QString("%1").arg(params[2]));
    ui->leMaxX->setText(QString("%1").arg(params[3]));
    ui->leMinY->setText(QString("%1").arg(params[4]));
    ui->leMaxY->setText(QString("%1").arg(params[5]));
    ui->leMinZ->setText(QString("%1").arg(params[6]));
    ui->leMaxZ->setText(QString("%1").arg(params[7]));
    ui->leMin->setText(QString("%1").arg(params[8]));
    ui->leMax->setText(QString("%1").arg(params[9]));
    ui->leGrading->setText(QString("%1").arg(params[10]));
    ui->hsRadius->setValue(int(params[11]*5));
    ui->hsEdge->setValue(int(params[12]*5));
}

void SetupMeshDialog::get(double *params)
{
    params[0] = ui->leFacets->text().toDouble();
    params[1] = ui->leDetail->text().toDouble();
    params[2] = ui->leMinX->text().toDouble();
    params[3] = ui->leMaxX->text().toDouble();
    params[4] = ui->leMinY->text().toDouble();
    params[5] = ui->leMaxY->text().toDouble();
    params[6] = ui->leMinZ->text().toDouble();
    params[7] = ui->leMaxZ->text().toDouble();
    params[8] = ui->leMin->text().toDouble();
    params[9] = ui->leMax->text().toDouble();
    params[10] = ui->leGrading->text().toDouble();
    params[11] = double(ui->hsRadius->value())*0.2;
    params[12] = double(ui->hsEdge->value())*0.2;
}

bool SetupMeshDialog::checkValues(void)
{
    bool isOk;

    ui->leFacets->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(0);
        ui->leFacets->setFocus();
        return false;
    }
    ui->leDetail->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(0);
        ui->leDetail->setFocus();
        return false;
    }
    ui->leMinX->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(0);
        ui->leMinX->setFocus();
        return false;
    }
    ui->leMaxX->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(0);
        ui->leMaxX->setFocus();
        return false;
    }
    ui->leMinY->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(0);
        ui->leMinY->setFocus();
        return false;
    }
    ui->leMaxY->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(0);
        ui->leMaxY->setFocus();
        return false;
    }
    ui->leMinZ->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(0);
        ui->leMinZ->setFocus();
        return false;
    }
    ui->leMaxZ->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(0);
        ui->leMaxZ->setFocus();
        return false;
    }
    ui->leMin->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->leMin->setFocus();
        return false;
    }
    ui->leMax->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->leMax->setFocus();
        return false;
    }
    ui->leGrading->text().toDouble(&isOk);
    if (!isOk)
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->leGrading->setFocus();
    }
    return isOk;
}

void SetupMeshDialog::accept(void)
{
    if (checkValues())
        QDialog::accept();
    else
        QMessageBox::information(this, tr("Error"), tr("Error floating value"));
}
