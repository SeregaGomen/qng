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

void SetupMeshDialog::set(bool isCSG,double *params)
{
    ui->cbIsCSG->setChecked(isCSG);
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
    ui->hsRadius->setValue(int(params[11]*5);
    ui->hsEdge->setValue(int(params[12]*5);
}
