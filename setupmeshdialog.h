#ifndef SETUPMESHDIALOG_H
#define SETUPMESHDIALOG_H

#include <QDialog>

namespace Ui {
class SetupMeshDialog;
}

class SetupMeshDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupMeshDialog(QWidget *parent = 0);
    ~SetupMeshDialog();
    void set(double*);
    void get(double*);

private slots:
    void changeRadius(int);
    void changeEdge(int);

private:
    Ui::SetupMeshDialog *ui;
};

#endif // SETUPMESHDIALOG_H
