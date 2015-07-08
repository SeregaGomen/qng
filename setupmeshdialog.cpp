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
