#include <QTimer>
#include <QDebug>
#include <QClipboard>
#include <QLibraryInfo>
#include <QTranslator>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
//#include <QTextCodec>
#include <QCloseEvent>
#include <QSettings>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QProgressBar>
#include <QTextEdit>
#include <QDockWidget>
#include "mainwindow.h"
#include "glwidget.h"
#include "setupimagedialog.h"
#include "setupmeshdialog.h"
#include "ui_mainwindow.h"


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
//    QTextCodec* codec =  QTextCodec::codecForName("UTF-8");
//#if QT_VERSION < 0x050000
//    QTextCodec::setCodecForTr(codec);
//    QTextCodec::setCodecForCStrings(codec);
//#endif
//    QTextCodec::setCodecForLocale(codec);


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
    connect(ui->action_Refinement, SIGNAL(triggered(void)), this, SLOT(refinementMesh(void)));
    connect(ui->action_Rotate, SIGNAL(triggered(void)), this, SLOT(setRotate(void)));
    connect(ui->action_Scale, SIGNAL(triggered(void)), this, SLOT(setScale(void)));
    connect(ui->action_Translate, SIGNAL(triggered(void)), this, SLOT(setTranslate(void)));
    connect(ui->action_Restore, SIGNAL(triggered(void)), this, SLOT(restoreImage(void)));
    connect(ui->action_Parameters, SIGNAL(triggered(void)), this, SLOT(imageParams(void)));
    connect(ui->action_SaveMesh, SIGNAL(triggered(void)), this, SLOT(saveMesh(void)));
    connect(ui->action_MeshOptions, SIGNAL(triggered(void)), this, SLOT(meshParam(void)));
    connect(ui->action_ShowModel, SIGNAL(triggered(void)), this, SLOT(isShowModel(void)));
    connect(ui->action_Close, SIGNAL(triggered(void)), this, SLOT(closeDoc(void)));

    ngObject = new NGInterface();

    checkMenuState();
}

void MainWindow::closeDoc(void)
{
    closeTab(0);
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
        isMeshGenerated = false;
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
    bool isGL = (qobject_cast<QOpenGLWidget*>(tabWidget->widget(nTab)) == NULL) ? false : true;

    if (isGL)
        qobject_cast<QOpenGLWidget*>(tabWidget->widget(nTab))->update();

    ui->action_Rotate->setEnabled(isGL);
    ui->action_Scale->setEnabled(isGL);
    ui->action_Translate->setEnabled(isGL);
    ui->action_Restore->setEnabled(isGL);
    ui->action_Parameters->setEnabled(isGL);
    if (isGL)
    {
        if (qobject_cast<GLWidget*>(tabWidget->widget(nTab))->getRotate())
            setRotate();
        else if (qobject_cast<GLWidget*>(tabWidget->widget(nTab))->getScale())
            setScale();
        else
            setTranslate();
    }
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
    else if (QFileInfo(fileName).completeSuffix().toUpper() == "VOL")
        isOk = loadVOL(fileName);

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

bool MainWindow::loadMesh(const QString& fileName)
{
    ngObject->loadMesh(fileName.toStdString());
    return (ngObject->getMesh()) ? true : false;
}

bool MainWindow::loadCSG(const QString& fileName)
{
    if (!loadGeometry(fileName))
        return false;
    fType = CSG;
    if (isModel)
        showCSG();
    return true;
}

bool MainWindow::loadSTL(const QString& fileName)
{
    if (!loadGeometry(fileName))
        return false;
    fType = STL;
    if (isModel)
        showSTL();
    return true;
}

bool MainWindow::loadVOL(const QString& fileName)
{
    if (!loadMesh(fileName))
        return false;
    fType = VOL;
    tabWidget->addTab(new GLWidget(ngObject,MESH_MODEL,this),tr("Mesh"));
    tabWidget->setCurrentIndex(tabWidget->count() - 1);

    return true;
}


void MainWindow::openDocument(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Opening a geometrical model"),windowFilePath(),tr("Netgen Geometry files (*.geo);; STL files (*.stl);; Netgen Mesh files (*.vol);; All files (*.*)"));

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
         isPaste = (isEditor) ? QApplication::clipboard()->text().length() : false,
         isGL = (qobject_cast<QOpenGLWidget*>(tabWidget->currentWidget()) == NULL) ? false : true;

    ui->action_SelectAll->setEnabled(isEditor);
    ui->action_Paste->setEnabled(isPaste);
    ui->action_Close->setEnabled(!isUntitled);
    ui->action_SaveAs->setEnabled(!isUntitled && fType != VOL);

    ui->action_Rotate->setEnabled(!isUntitled && isGL);
    ui->action_Scale->setEnabled(!isUntitled && isGL);
    ui->action_Translate->setEnabled(!isUntitled && isGL);
    ui->action_Restore->setEnabled(!isUntitled && isGL);
    ui->action_Parameters->setEnabled(!isUntitled && isGL);

    ui->action_Start->setEnabled(!isUntitled && !isGenMeshStarted && fType != VOL);
    ui->action_Stop->setEnabled(!isUntitled && isGenMeshStarted);
    ui->action_Refinement->setEnabled(!isUntitled && isMeshGenerated);
    ui->action_SaveMesh->setEnabled(!isUntitled && isMeshGenerated);
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
    isUntitled = false;
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
    isUntitled = false;
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


void MainWindow::updateTimer(void)
{
    pb->update();
    QCoreApplication::processEvents();
}

void MainWindow::startMesh(void)
{
//    QTimer *timer = new QTimer(this);
    QNGThread *thread = new QNGThread(ngObject,fType,qobject_cast<QTextEdit*>(tabWidget->widget(0))->toPlainText().toStdString());
    bool isFind = false;

//    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
//    timer->start(100);

    pb->setMinimum(0);
    pb->setMaximum(0);
    pb->setValue(0);
    pb->show();
    ui->action_Start->setEnabled(false);
    ui->action_Stop->setEnabled(true);
    isGenMeshStarted = true;
    isGenMeshCanceled = false;

    thread->start();
    while (thread->isRunning())
    {
        if (isGenMeshCanceled)
            thread->terminate();
        QCoreApplication::processEvents();
    }
    pb->hide();
//    timer->stop();

    if (isGenMeshCanceled)
        cerr << endl << tr("Process aborted by user!").toStdString().c_str() << endl;

    isMeshGenerated = thread->getIsGenerated();
    isGenMeshStarted = isGenMeshCanceled = false;
    ui->action_Stop->setEnabled(false);
    ui->action_Start->setEnabled(true);
    checkMenuState();
    delete thread;
//    delete timer;

    if (!isMeshGenerated)
        return;

    // Обновление визуализации
    for (int i = 0; i < tabWidget->count(); i++)
        if (tabWidget->tabText(i).replace("&","") == tr("Mesh"))
        {
            isFind = true;
            qobject_cast<GLWidget*>(tabWidget->widget(i))->repaint();
            tabWidget->setCurrentIndex(i);
            break;
        }
    if (!isFind)
    {
        tabWidget->addTab(new GLWidget(ngObject,MESH_MODEL,this),tr("Mesh"));
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
    }


}

void MainWindow::stopMesh(void)
{
    isGenMeshCanceled = true;
}

void MainWindow::genMeshSTL(void)
{
    isMeshGenerated = (ngObject->genMeshSTL(qobject_cast<QTextEdit*>(tabWidget->widget(0))->toPlainText().toStdString())) ? true : false;
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
        tabWidget->addTab(new GLWidget(ngObject,STL_MODEL,this),tr("Model"));
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
    }
}

void MainWindow::genMeshCSG(void)
{
    isMeshGenerated = (ngObject->genMeshCSG(qobject_cast<QTextEdit*>(tabWidget->widget(0))->toPlainText().toStdString())) ? true : false;
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
        tabWidget->addTab(new GLWidget(ngObject,CSG_MODEL,this),tr("Model"));
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
    }
}

void MainWindow::refinementMesh(void)
{
    bool isFind = false;

    ngObject->refinementMesh();
    // Обновление визуализации
    for (int i = 0; i < tabWidget->count(); i++)
        if (tabWidget->tabText(i).replace("&","") == tr("Mesh"))
        {
            isFind = true;
            qobject_cast<GLWidget*>(tabWidget->widget(i))->repaint();
            tabWidget->setCurrentIndex(i);
            break;
        }
    if (!isFind)
    {
        tabWidget->addTab(new GLWidget(ngObject,MESH_MODEL,this),tr("Mesh"));
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
    }
}

void MainWindow::setScale(void)
{
    ui->action_Rotate->setChecked(false);
    ui->action_Scale->setChecked(true);
    ui->action_Translate->setChecked(false);
    qobject_cast<GLWidget*>(tabWidget->currentWidget())->setScale();
}

void MainWindow::setTranslate(void)
{
    ui->action_Rotate->setChecked(false);
    ui->action_Scale->setChecked(false);
    ui->action_Translate->setChecked(true);
    qobject_cast<GLWidget*>(tabWidget->currentWidget())->setTranslate();
}

void MainWindow::setRotate(void)
{
    ui->action_Rotate->setChecked(true);
    ui->action_Scale->setChecked(false);
    ui->action_Translate->setChecked(false);

    qobject_cast<GLWidget*>(tabWidget->currentWidget())->setRotate();
}

void MainWindow::restoreImage(void)
{
    qobject_cast<GLWidget*>(tabWidget->currentWidget())->restore();
}

void MainWindow::imageParams(void)
{
    bool isMesh = (tabWidget->tabText(tabWidget->currentIndex()).replace("&","") == tr("Mesh")) ? true : false;
    SetupImageDialog *iDlg = new SetupImageDialog(this);

    iDlg->setImageParams(qobject_cast<GLWidget*>(tabWidget->currentWidget())->getImageParams(),isMesh);
    if (iDlg->exec() == QDialog::Accepted)
    {
        qobject_cast<GLWidget*>(tabWidget->currentWidget())->setImageParams(iDlg->getImageParams());
        qobject_cast<GLWidget*>(tabWidget->currentWidget())->repaint();
    }
    delete iDlg;
}

void MainWindow::saveMesh(void)
{
//    QString fileName = QFileDialog::getSaveFileName(this,tr("Saving a mesh"),windowFilePath(),tr("Mesh files (*.vol)"));
    QFileInfo fi(windowFilePath());
    QString path = fi.absolutePath() + "/" + fi.baseName() + QString(".vol"),
            fileName = QFileDialog::getSaveFileName(this,tr("Saving a mesh"),path,tr("Mesh files (*.vol)"));

    if (fileName.isEmpty())
        return;
    ngObject->saveMesh(fileName.toStdString());
}

void MainWindow::meshParam(void)
{
    SetupMeshDialog *mDlg = new SetupMeshDialog(this);
    double params[19];

    params[0] = ngObject->getFacets();
    params[1] = ngObject->getDetail();
    params[2] = ngObject->getMinX();
    params[3] = ngObject->getMaxX();
    params[4] = ngObject->getMinY();
    params[5] = ngObject->getMaxY();
    params[6] = ngObject->getMinZ();
    params[7] = ngObject->getMaxZ();
    params[8] = ngObject->getMinMeshSize();
    params[9] = ngObject->getMaxMeshSize();
    params[10] = ngObject->getMeshSizeGrading();
    params[11] = ngObject->getEpRadius();
    params[12] = ngObject->getEpEdge();
    params[13] = ngObject->getChartDist();
    params[14] = ngObject->getLineLength();
    params[15] = ngObject->getCloseEdges();
    params[16] = ngObject->getSurfaceCurvature();
    params[17] = ngObject->getEdgeAngle();
    params[18] = ngObject->getSurfaceMeshCurv();

    mDlg->set(params);
    if (mDlg->exec() != QDialog::Accepted)
        return;

    mDlg->get(params);
    ngObject->setFacets(params[0]);
    ngObject->setDetail(params[1]);
    ngObject->setMinX(params[2]);
    ngObject->setMaxX(params[3]);
    ngObject->setMinY(params[4]);
    ngObject->setMaxY(params[5]);
    ngObject->setMinZ(params[6]);
    ngObject->setMaxZ(params[7]);
    ngObject->setMinMeshSize(params[8]);
    ngObject->setMaxMeshSize(params[9]);
    ngObject->setMeshSizeGrading(params[10]);
    ngObject->setEpRadius(params[11]);
    ngObject->setEpEdge(params[12]);
    ngObject->setChartDist(params[13]);
    ngObject->setLineLength(params[14]);
    ngObject->setCloseEdges(params[15]);
    ngObject->setSurfaceCurvature(params[16]);
    ngObject->setEdgeAngle(params[17]);
    ngObject->setSurfaceMeshCurv(params[18]);


    delete mDlg;
}

void MainWindow::isShowModel(void)
{
    isModel = !isModel;
    ui->action_ShowModel->setChecked(isModel);

    if (isModel)
    {
        if (fType == CSG)
            showCSG();
        else if (fType == STL)
            showSTL();
    }
    else
        for (int i = 0; i < tabWidget->count(); i++)
            if (tabWidget->tabText(i).replace("&","") == tr("Model"))
            {
                closeTab(i);
                break;
            }
}

// Запуск расчета
void QNGThread::run(void)
{
    isGenerated = false;
    switch (fType)
    {
        case STL:
            isGenerated = (ngObject->genMeshSTL(geomData)) ? true : false;
            break;
        case CSG:
            isGenerated = (ngObject->genMeshCSG(geomData)) ? true : false;
        default:
            break;
    }
}
