#include <QColorDialog>
#include <math.h>

#include "setupimagedialog.h"
#include "ui_setupimagedialog.h"

SetupImageDialog::SetupImageDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SetupImageDialog)
{
    ui->setupUi(this);
    initParams();

    connect(ui->pbColor, SIGNAL(clicked()), this, SLOT(changeBkgColor()));
}

SetupImageDialog::~SetupImageDialog()
{
    delete ui;
}

void SetupImageDialog::changeBkgColor(void)
{
    QColorDialog* cDlg = new QColorDialog(bkgColor,this);

    if (cDlg->exec() == QDialog::Accepted)
    {
        bkgColor = cDlg->currentColor();
        ui->pbColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(bkgColor.red()).arg(bkgColor.green()).arg(bkgColor.blue()));
    }
    delete cDlg;
}

void SetupImageDialog::setup(bool isMesh)
{
    // Инициализация параметров визуализации
    ui->chbMesh->setChecked(params.isMesh);
    ui->chbVertex->setChecked(params.isVertex);
    ui->chbFace->setChecked(params.isFace);
    ui->chbLight->setChecked(params.isLight);
    ui->chbQuality->setChecked(params.isQuality);
    ui->chbCoord->setChecked(params.isCoord);
    ui->hsAlpha->setSliderPosition(int(params.alpha*10.0));
    ui->gbMesh->setEnabled(isMesh);
    // Цвет фона
    bkgColor = params.bkgColor;
    ui->pbColor->setAutoFillBackground(true);
    ui->pbColor->setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(params.bkgColor.red()).arg(params.bkgColor.green()).arg(params.bkgColor.blue()));


}

void SetupImageDialog::initParams(void)
{
    params.init();
}

void SetupImageDialog::clear(void)
{
    initParams();
    setup();
}


bool SetupImageDialog::reload(void)
{
    params.isVertex = ui->chbVertex->isChecked();
    params.isMesh = ui->chbMesh->isChecked();
    params.isFace = ui->chbFace->isChecked();
    params.isCoord = ui->chbCoord->isChecked();
    params.isLight = ui->chbLight->isChecked();
    params.isQuality = ui->chbQuality->isChecked();
    params.alpha = double(ui->hsAlpha->sliderPosition())*0.1;
    params.bkgColor = bkgColor;
    return true;
}

void SetupImageDialog::accept(void)
{
    if (reload())
        QDialog::accept();
}

