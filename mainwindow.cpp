#include <QDebug>
#include <QClipboard>
#include <QLibraryInfo>
#include <QTranslator>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCodec>
#include <QCloseEvent>
#include <QSettings>
#include <QGLWidget>
#include <QProgressBar>
#include <QTextEdit>
#include <QDockWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glmodel.h"



int langNo = EN;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initApp();
}

MainWindow::~MainWindow()
{
    if (ngObject)
        delete ngObject;
    delete ui;
}

void MainWindow::initApp(void)
{
    QTextCodec* codec =  QTextCodec::codecForName("UTF-8");
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
#endif
    QTextCodec::setCodecForLocale(codec);


    #ifndef Q_OS_LINUX
        setWindowIcon(QIcon(":/images/app.ico"));
    #endif

    setupRecentActions();
    readSettings();
    setupLanguage();
    createRecentMenu();


    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(false);
    setCentralWidget(tabWidget);
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTab(int)));

    terminal = new QTextEdit(this);
    terminal->setPalette(QPalette(QPalette::Base, QColor(255, 255, 255)));
    terminal->setReadOnly(true);
    terminal->setWordWrapMode(QTextOption::NoWrap);
    terminal->setFont(QFont("Courier"));



    //////////////////////
    dock = new QDockWidget(tr("Terminal"), this);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dock->setWidget(terminal);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    dock->setVisible(ui->action_Terminal->isChecked());
    dock->setWindowTitle(tr("Terminal"));
    connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(closeTerminal(void)));

    //////////////////////
    // Перехват cout и cerr
    wout = new QStdRedirector<>(std::cout, this);
    connect(wout, SIGNAL(messageChanged(QString)), this, SLOT(setMsgColor()));
    connect(wout, SIGNAL(messageChanged(QString)), terminal, SLOT(insertPlainText(QString)));

    werr = new QStdRedirector<>(std::cerr, this);
    connect(werr, SIGNAL(messageChanged(QString)), this, SLOT(setErrColor()));
    connect(werr, SIGNAL(messageChanged(QString)), terminal, SLOT(insertPlainText(QString)));
    //////////////////////
    // Скроллирование терминала вниз при добавлении данных
    connect(terminal, SIGNAL(textChanged()), this, SLOT(setScroll()));
    //////////////////////

    pb = new QProgressBar(statusBar());
    pb->setTextVisible(false);
    pb->hide();
    statusBar()->addPermanentWidget(pb);

    //////////////////////
    connect(ui->action_Terminal, SIGNAL(triggered(void)), this, SLOT(setTerminal(void)));
    connect(ui->action_Open, SIGNAL(triggered(void)), this, SLOT(openDocument(void)));
    connect(ui->action_Cut, SIGNAL(triggered(void)), this, SLOT(cutSelection(void)));
    connect(ui->action_Copy, SIGNAL(triggered(void)), this, SLOT(copySelection(void)));
    connect(ui->action_Paste, SIGNAL(triggered(void)), this, SLOT(pasteClipboard(void)));
    connect(ui->action_SelectAll, SIGNAL(triggered(void)), this, SLOT(selectAll(void)));
    connect(ui->action_Undo, SIGNAL(triggered(void)), this, SLOT(undo(void)));
    connect(ui->action_Redo, SIGNAL(triggered(void)), this, SLOT(redo(void)));
    connect(ui->action_Save, SIGNAL(triggered(void)), this, SLOT(save(void)));
    connect(ui->action_SaveAs, SIGNAL(triggered(void)), this, SLOT(saveAs(void)));
    connect(ui->action_NewCSG, SIGNAL(triggered(void)), this, SLOT(newCSG(void)));
    connect(ui->action_NewSTL, SIGNAL(triggered(void)), this, SLOT(newSTL(void)));
    connect(ui->action_Start, SIGNAL(triggered(void)), this, SLOT(startMesh(void)));
    connect(ui->action_Stop, SIGNAL(triggered(void)), this, SLOT(stopMesh(void)));


    ngObject = new NGInterface();

    checkMenuState();
}


void MainWindow::setupRecentActions(void)
{
    for (int i = 0; i < maxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
}

bool MainWindow::closeTab(int nTab)
{
    int i = tabWidget->count() - 1;

    if (nTab != 0)
        tabWidget->removeTab(nTab);
    else
    {
        if (!canClose())
            return false;
        while (tabWidget->count() > 0)
            tabWidget->removeTab(i--);
        if (ngObject)
            delete ngObject;
        ngObject = new NGInterface();
        setWindowTitle("3D-mesh generator");
        isUntitled = true;
        checkMenuState();
    }
    return true;
}

void MainWindow::setTerminal(void)
{
    dock->setVisible(ui->action_Terminal->isChecked());
}

void MainWindow::closeTerminal(void)
{
    ui->action_Terminal->setChecked(dock->isVisible());
}

void MainWindow::setErrColor(void)
{
    terminal->setTextColor(QColor("red"));
}

void MainWindow::setMsgColor(void)
{
    terminal->setTextColor(QColor("black"));
}

void MainWindow::setScroll(void)
{
    terminal->moveCursor(QTextCursor::End);
}

void MainWindow::changeTab(int nTab)
{
    bool isGL = (qobject_cast<QGLWidget*>(tabWidget->widget(nTab)) == NULL) ? false : true;

    if (isGL)
        qobject_cast<QGLWidget*>(tabWidget->widget(nTab))->updateGL();

//    ui->actionInfo->setEnabled(isEnabled);
//    ui->actionRotate->setEnabled(isEnabled);
//    ui->actionScale->setEnabled(isEnabled);
//    ui->actionTranslate->setEnabled(isEnabled);
//    ui->actionRestore->setEnabled(isEnabled);
//    ui->actionAnalyse->setEnabled(femObject->isCalculated());
//    ui->actionSetupImage->setEnabled(isEnabled);
//    if (isEnabled && !isUntitled)
//    {
//        if (qobject_cast<GLWidget*>(tabWidget->widget(nTab))->getRotate())
//            setRotate();
//        else if (qobject_cast<GLWidget*>(tabWidget->widget(nTab))->getScale())
//            setScale();
//        else
//            setTranslate();
//    }
}

void MainWindow::readSettings(void)
{
    QSettings settings("ZNU","QNG");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    QString path = settings.value("path").toString();
    bool isTerminal = settings.value("terminal").toBool();
    int states = settings.value("state").toInt();

    ui->action_Terminal->setChecked(isTerminal);
    files = settings.value("recentFileList").toStringList();
    langNo = settings.value("lang").toInt();
    if ((Qt::WindowStates)states == Qt::WindowMaximized)
        this->setWindowState(Qt::WindowMaximized);
    else
    {
        move(pos);
        resize(size);
    }
    setWindowFilePath(path);
}

void MainWindow::writeSettings(void)
{
    QSettings settings("ZNU","QNG");

    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("path",windowFilePath());
    settings.setValue("state",(int)windowState());
    settings.setValue("terminal",ui->action_Terminal->isChecked());
    settings.setValue("recentFileList",files);
    settings.setValue("lang",langNo);
}

void MainWindow::openRecentFile(void)
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action)
        loadFile(action->data().toString());
}

void MainWindow::loadFile(const QString& fileName)
{
    bool isOk = false;

    if (!closeTab(0))
        return;
    if (QFileInfo(fileName).completeSuffix().toUpper() == "GEO")
        isOk = loadCSG(fileName);
    else if (QFileInfo(fileName).completeSuffix().toUpper() == "STL")
        isOk = loadSTL(fileName);

    if (isOk)
    {
        statusBar()->showMessage(tr("File successfully downloaded"),5000);
        updateRecentFileActions(fileName);
        setCurrentFile(fileName);
    }
    else
        statusBar()->showMessage(tr("Error opening file"),5000);
}

void MainWindow::updateRecentFileActions(const QString& fileName)
{
    // Удаляем в меню старый список Recent-файлов
    for (int i = 0; i < files.size(); ++i)
        ui->menu_File->removeAction(recentFileActs[i]);

    // Модифицируем список Recent-файлов
    setWindowFilePath(fileName);
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > maxRecentFiles)
        files.removeLast();
    createRecentMenu();
}

void MainWindow::createRecentMenu(void)
{
    // Создаем в меню новый список Recent-файлов
    for (int i = 0; i < files.size(); ++i)
    {
        QString text = QString("&%1 %2").arg(i + 1).arg(QFileInfo(files[i]).fileName());

        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
        recentFileActs[i]->setStatusTip(files[i]);
        ui->menu_File->insertAction(ui->action_Exit, recentFileActs[i]);
    }
    if (files.size() > 0)
    {
        separatorAct = ui->menu_File->insertSeparator(ui->action_Exit);
        separatorAct->setVisible(files.size() > 0);
    }
}

void MainWindow::setCurrentFile(const QString& fileName)
{
    isUntitled = false;
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(QFileInfo(fileName).fileName() + " - 3D-mesh generator");
    setWindowModified(false);
    setWindowFilePath(curFile);
    checkMenuState();
}

bool MainWindow::loadGeometry(const QString& fileName)
{
    QFile file(fileName);
    QTextStream in;
    QString text;

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this, tr("Error"), tr("Error opening file %1").arg(fileName));
        return false;
    }

    in.setDevice(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    text = in.readAll();
    tabWidget->insertTab(0,new QTextEdit(this),tr("Geometry"));
    tabWidget->setTabsClosable(true);
    tabWidget->setCurrentIndex(0);
    qobject_cast<QTextEdit*>(tabWidget->widget(0))->setText(text);

    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));

    QApplication::restoreOverrideCursor();
    file.close();
    return true;
}

bool MainWindow::loadCSG(const QString& fileName)
{
    if (!loadGeometry(fileName))
        return false;
    fType = CSG;
    showCSG();
    return true;
}

bool MainWindow::loadSTL(const QString& fileName)
{
    if (!loadGeometry(fileName))
        return false;
    fType = STL;
    showSTL();
    return true;
}

void MainWindow::openDocument(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Opening a geometry"),windowFilePath(),tr("NETGEN Geometry files (*.geo);; STL files (*.stl);; All files (*.*)"));

    if (!fileName.isEmpty())
        loadFile(fileName);
}


void MainWindow::setupLanguage(void)
{
    QString translatorFileName = QLatin1String("qt_") + QLocale::system().name(),
            translatorFile;
    QTranslator* dlgTranslator = new QTranslator(qApp);

    // Локализация (стандартных диалогов, e.t.c, ...)
    if (dlgTranslator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        qApp->installTranslator(dlgTranslator);

    translator = new QTranslator(qApp);
    // Перевод на указанный язык
    switch (langNo)
    {
        case EN: // English
            translatorFile = "";
            break;
        case RU: // Russian
            translatorFile = "qng_ru";
    }

    if (translator->load(translatorFile))
    {
        qApp->installTranslator(translator);
        ui->retranslateUi(this);
    }
}

void MainWindow::checkMenuState(void)
{
    bool isEditor = (qobject_cast<QTextEdit *>(tabWidget->currentWidget()) == NULL) ? false : true,
         isPaste = (isEditor) ? QApplication::clipboard()->text().length() : false;

    ui->action_SelectAll->setEnabled(isEditor);
    ui->action_Paste->setEnabled(isPaste);
    ui->action_Close->setEnabled(!isUntitled);
    ui->action_SaveAs->setEnabled(!isUntitled);
//    ui->actionRotate->setEnabled(!isUntitled && isEnabled);
//    ui->actionScale->setEnabled(!isUntitled && isEnabled);
//    ui->actionTranslate->setEnabled(!isUntitled && isEnabled);
//    ui->actionRestore->setEnabled(!isUntitled);
//    ui->actionObjectParameters->setEnabled(!isUntitled);
    ui->action_Start->setEnabled(!isUntitled && !isGenMeshStarted);
    ui->action_Stop->setEnabled(!isUntitled && isGenMeshStarted);
//    ui->actionSetupImage->setEnabled(!isUntitled && isEnabled);
//    ui->actionInfo->setEnabled(!isUntitled);
//    ui->actionSaveResults->setEnabled(!isUntitled && femObject->isCalculated());
}

void MainWindow::onTextChanged(void)
{
    ui->action_Save->setEnabled(true);
}

void MainWindow::cutSelection(void)
{
    qobject_cast<QTextEdit*>(tabWidget->currentWidget())->cut();
}

void MainWindow::copySelection(void)
{
    qobject_cast<QTextEdit*>(tabWidget->currentWidget())->copy();
}

void MainWindow::pasteClipboard(void)
{
    qobject_cast<QTextEdit*>(tabWidget->currentWidget())->paste();
}

void MainWindow::selectAll(void)
{
    qobject_cast<QTextEdit*>(tabWidget->currentWidget())->selectAll();
}

void MainWindow::onCopyAvailable(bool isAvailable)
{
    ui->action_Cut->setEnabled(isAvailable);
    ui->action_Copy->setEnabled(isAvailable);
}

void MainWindow::onRedoAvailable(bool isAvailable)
{
    ui->action_Redo->setEnabled(isAvailable);
}

void MainWindow::onUndoAvailable(bool isAvailable)
{
    ui->action_Undo->setEnabled(isAvailable);
}

void MainWindow::undo(void)
{
    qobject_cast<QTextEdit*>(tabWidget->currentWidget())->undo();
}

void MainWindow::redo(void)
{
    qobject_cast<QTextEdit*>(tabWidget->currentWidget())->redo();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!canClose())
    {
        event->ignore();
        return;
    }
    writeSettings();
    event->accept();
}

void MainWindow::save(void)
{
    if (!isUntitled)
        saveFile(curFile);
    else
        saveAs();
}

void MainWindow::saveAs(void)
{
    QString fileType = (fType == STL) ? tr("STL files (*.stl)") : tr("NETGEN Geometry files (*.geo)"),
            fileName = QFileDialog::getSaveFileName(this,tr("Saving a geometry"),windowFilePath(),fileType);

    if (!fileName.isEmpty())
    {
        saveFile(fileName);
        updateRecentFileActions(fileName);
    }
}

void MainWindow::saveFile(const QString & fileName)
{
    QFile file(fileName);
    QTextStream out;

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this, tr("Error"), tr("Error writing file %1").arg(fileName));
        return;
    }
    out.setDevice(&file);
    out << qobject_cast<QTextEdit *>(tabWidget->widget(0))->toPlainText();
    file.close();
    statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::newCSG(void)
{
    if (!closeTab(0))
        return;
    fType = CSG;
    tabWidget->insertTab(0,new QTextEdit(this),tr("Geometry"));
    tabWidget->setTabsClosable(true);
    tabWidget->setCurrentIndex(0);
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
    checkMenuState();
}

void MainWindow::newSTL(void)
{
    if (!closeTab(0))
        return;
    fType = STL;
    tabWidget->insertTab(0,new QTextEdit(this),tr("Geometry"));
    tabWidget->setTabsClosable(true);
    tabWidget->setCurrentIndex(0);
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
    connect(qobject_cast<QTextEdit *>(tabWidget->widget(0)), SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
    checkMenuState();
}

bool MainWindow::canClose(void)
{
    QMessageBox msgBox;

    if (!qobject_cast<QTextEdit *>(tabWidget->widget(0)))
        return true;
    if (!qobject_cast<QTextEdit *>(tabWidget->widget(0))->document()->isModified())
        return true;
    msgBox.setText(tr("The geometry file has been modified"));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    switch (msgBox.exec())
    {
        case QMessageBox::Save:
            save();
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return false;
    }
    return true;
}

void MainWindow::startMesh(void)
{
    ui->action_Stop->setEnabled(true);
    isGenMeshStarted = true;
    switch (fType)
    {
        case STL:
            genMeshSTL();
            break;
        case CSG:
            genMeshCSG();
        default:
            break;
    }
    isGenMeshStarted = false;
    ui->action_Stop->setEnabled(false);
    checkMenuState();
}

void MainWindow::stopMesh(void)
{

}

void MainWindow::genMeshSTL(void)
{
    bool isFind = false;

    if (!ngObject->genMeshSTL(qobject_cast<QTextEdit*>(tabWidget->widget(0))->toPlainText().toStdString()))
        return;

    // Обновление визуализации
    for (int i = 0; i < tabWidget->count(); i++)
        if (tabWidget->tabText(i).replace("&","") == tr("Mesh"))
        {
            isFind = true;
            tabWidget->widget(i)->repaint();
            tabWidget->setCurrentIndex(i);
            break;
        }
    if (!isFind)
    {
        tabWidget->addTab(new GLModelWidget(ngObject,MESH_MODEL,this),tr("Mesh"));
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
    }


//    Ng_SaveMesh(mesh,"test_ref.vol");
}

void MainWindow::showSTL(void)
{
    bool isFind = false;

    if (!ngObject->loadSTL(qobject_cast<QTextEdit*>(tabWidget->widget(0))->toPlainText().toStdString()))
    {
        cout << "Error reading in current STL data" << endl;
        return;
    }
    cout << "Successfully loaded STL data" << endl;
    if (!ngObject->initSTL())
    {
        cout << "Error Initialising the STL Geometry....Aborting!!" << endl;
        return;
    }

    // Проверка наличия такой закладки
    for (int i = 0; i < tabWidget->count(); i++)
        if (tabWidget->tabText(i).replace("&","") == tr("Model"))
        {
            isFind = true;
            tabWidget->setCurrentIndex(i);
            break;
        }
    if (!isFind)
    {
        tabWidget->addTab(new GLModelWidget(ngObject,STL_MODEL,this),tr("Model"));
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
    }
}

void MainWindow::genMeshCSG(void)
{
    bool isFind = false;

    if (!ngObject->genMeshCSG(qobject_cast<QTextEdit*>(tabWidget->widget(0))->toPlainText().toStdString()))
        return;

    // Обновление визуализации
    for (int i = 0; i < tabWidget->count(); i++)
        if (tabWidget->tabText(i).replace("&","") == tr("Mesh"))
        {
            isFind = true;
            tabWidget->widget(i)->repaint();
            tabWidget->setCurrentIndex(i);
            break;
        }
    if (!isFind)
    {
        tabWidget->addTab(new GLModelWidget(ngObject,MESH_MODEL,this),tr("Mesh"));
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
    }
}

void MainWindow::showCSG(void)
{
    bool isFind = false;

    if (!ngObject->loadCSG(qobject_cast<QTextEdit*>(tabWidget->widget(0))->toPlainText().toStdString()))
    {
        cout << "Error reading in current CSG data" << endl;
        return;
    }
    cout << "Successfully loaded CSG data" << endl;
    // Проверка наличия такой закладки
    for (int i = 0; i < tabWidget->count(); i++)
        if (tabWidget->tabText(i).replace("&","") == tr("Model"))
        {
            isFind = true;
            tabWidget->setCurrentIndex(i);
            break;
        }
    if (!isFind)
    {
        tabWidget->addTab(new GLModelWidget(ngObject,CSG_MODEL,this),tr("Model"));
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
    }
}
