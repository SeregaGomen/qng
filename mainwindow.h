#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qstdredirector.h"
#include "nglib.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
class QProgressBar;
class QTranslator;
QT_END_NAMESPACE


typedef enum { EN = 0, RU = 1 } Lang;
typedef enum { EMPTY = 0, CSG = 1, STL = 2 } FileType;

const int maxRecentFiles = 5;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    bool closeTab(int);
    void changeTab(int);
    void closeTerminal(void);
    void setTerminal(void);
    void setErrColor(void);
    void setMsgColor(void);
    void setScroll(void);
    void openRecentFile(void);
    void openDocument(void);
    void onTextChanged(void);
    void onCopyAvailable(bool);
    void onRedoAvailable(bool);
    void onUndoAvailable(bool);
    void cutSelection(void);
    void copySelection(void);
    void pasteClipboard(void);
    void selectAll(void);
    void newCSG(void);
    void newSTL(void);
    void undo(void);
    void redo(void);
    void save(void);
    void saveAs(void);
    void saveMesh(void);
    void startMesh(void);
    void stopMesh(void);
    void refinementMesh(void);
    void setRotate(void);
    void setScale(void);
    void setTranslate(void);
    void restoreImage(void);
    void imageParams(void);
    void meshParam(void);

private:
    bool isCSG = true;
    bool isUntitled = true;
    bool isGenMeshStarted = false;
    bool isMeshGenerated = false;
    FileType fType = EMPTY;
    Ui::MainWindow *ui;
    QTranslator *translator;
    QMenu *recentFilesMenu;
    QAction *recentFileActs[maxRecentFiles];
    QAction *separatorAct;
    QStringList files;
    QTabWidget *tabWidget;
    QTextEdit *terminal;
    QProgressBar* pb;
    QDockWidget* dock;
    QStdRedirector<> *wout;
    QStdRedirector<> *werr;
    QString curFile;
    NGInterface* ngObject = NULL;
    void initApp(void);
    void setupRecentActions(void);
    void readSettings(void);
    void setupLanguage(void);
    void createRecentMenu(void);
    void checkMenuState(void);
    void writeSettings(void);
    void loadFile(const QString &);
    void saveFile(const QString &);
    void setCurrentFile(const QString &);
    void updateRecentFileActions(const QString &);
    void genMeshSTL(void);
    void genMeshCSG(void);
    void showSTL(void);
    void showCSG(void);
    bool loadGeometry(const QString &);
    bool loadCSG(const QString &);
    bool loadSTL(const QString &);
    bool canClose(void);
};

#endif // MAINWINDOW_H
