#ifndef SETUPIMAGEDIALOG_H
#define SETUPIMAGEDIALOG_H

#include <QDialog>
#include <fstream>
#include "imageparams.h"

class QXmlStreamWriter;

namespace Ui {
    class SetupImageDialog;
}

using namespace std;

class SetupImageDialog : public QDialog
{
    Q_OBJECT

public slots:
    void accept(void);
    void changeBkgColor(void);

public:
    explicit SetupImageDialog(QWidget *parent = 0);
    ~SetupImageDialog();
    void setup(bool isMesh = false);
    void clear(void);
    void initParams(void);
    void write(QDataStream& file)
    {
        params.write(file);
    }
    void read(QDataStream& file)
    {
        params.read(file);
    }
    ImageParams& getImageParams(void)
    {
        return params;
    }
    void setImageParams(ImageParams& r, bool isMesh = false)
    {
        params = r;
        setup(isMesh);
    }
private:
    QColor bkgColor;
    Ui::SetupImageDialog *ui;
    ImageParams params;
    bool reload(void);
};

#endif // SETUPIMAGEDIALOG_H
