#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuDiagram->setEnabled(false);
    ui->menuEdit->setEnabled(false);

    connect(ui->actionOpen, &QAction::triggered,
            this, &MainWindow::open);
    connect(ui->actionNew, &QAction::triggered,
            this, &MainWindow::newFile);
    connect(ui->actionnewTextFile, &QAction::triggered,
            this, &MainWindow::newTextFile);
    connect(ui->actionSave, &QAction::triggered,
            this, &MainWindow::save);
    connect(ui->actionSaveAs, &QAction::triggered,
            this, &MainWindow::saveAs);

    connect(ui->actionCut, &QAction::triggered,
            this, &MainWindow::cut);
    connect(ui->actionCopy, &QAction::triggered,
            this, &MainWindow::copy);
    connect(ui->actionPaste, &QAction::triggered,
            this, &MainWindow::paste);

    connect(ui->actionExit_2, &QAction::triggered,
            qApp, &QApplication::closeAllWindows);

    connect(ui->mdiArea, &QMdiArea::subWindowActivated,
            this, &MainWindow::updateActions);

    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionPrint->setEnabled(false);
    ui->lineEdit->setClearButtonEnabled(true);
    ui->lineEdit->setPlaceholderText(tr("Type to search..."));
    ui->label->hide();
    ui->lineEdit->hide();

    ui->mainToolBar->setIconSize(QSize(29, 29));
    ui->mainToolBar->setMovable(true);

    // Задаём заголовок окна. Его так же можно через свойства формы
    // в файле mainwindow.ui задать
    setWindowTitle(tr("My MDI Application"));

    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);

    qmPath = "../cw-sub/translations";

    createLanguageMenu();

    readSettings();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

// Открываем новый пустой документ
// со стандартным названием untitled%1.txt
void MainWindow::newFile()
{
    // Создаём новое окно
    MdiChildTable *child = createMdiChildTable();
    // Задаём ему базовые параметры
    child->newFile();
    // Выводим его в объекте ui->mdiArea
    child->show();
}

void MainWindow::newTextFile()
{
    // Создаём новое окно
    MdiChild *child = createMdiChild();
    // Задаём ему базовые параметры
    child->newFile();
    // Выводим его в объекте ui->mdiArea
    child->show();
}

// Открываем файл
void MainWindow::open()
{
    QFileDialog *fDialog = new QFileDialog(this);
    fDialog->setAttribute(Qt::WA_DeleteOnClose);

    QString fileName = fDialog->getOpenFileName(this, tr("Open Document"), QDir::cleanPath("../cw-sub_mdi/data"),
                                                "Text Files (*.txt *.db)", nullptr, QFileDialog::DontUseNativeDialog);

    QFileInfo fileinfo(fileName);
    QString ext = fileinfo.suffix();

    if (fileName.isEmpty()) {
        return;
    }

    if (ext != "db") {
        // Выесняем, есть ли у нас уже документ с таким названием
        QMdiSubWindow *existing = findMdiChild(fileName, false);
        if (existing) {
            // Если есть, то делаем его активным и выходим из метода
            ui->mdiArea->setActiveSubWindow(existing);
            return;
        }

        // Если такого файла ещё нет, то создаём новое окно
        MdiChild *child = createMdiChild();
        // и загружаем файл
        if (child->loadFile(fileName)) {
            // О чём сообщаем пользователю через нижнюю часть окна
            ui->statusBar->showMessage(tr("File loaded"), 2000);
            // Показываем окно
            child->show();
        } else {
            // Если загрузить не удалось, то удаляем только что
            // созданный экземпляр окна
            child->close();
        }

    } else {
        // Выесняем, есть ли у нас уже документ с таким названием
        QMdiSubWindow *existing = findMdiChild(fileName, true);
        if (existing) {
            // Если есть, то делаем его активным и выходим из метода
            ui->mdiArea->setActiveSubWindow(existing);
            return;
        }

        // Если такого файла ещё нет, то создаём новое окно
        MdiChildTable *child = createMdiChildTable();
        // и загружаем файл
        if (child->loadFile(fileName)) {
            // О чём сообщаем пользователю через нижнюю часть окна
            ui->statusBar->showMessage(tr("File loaded"), 2000);
            // Показываем окно
            child->show();
        } else {
            // Если загрузить не удалось, то удаляем только что
            // созданный экземпляр окна
            child->close();
        }
    }
}

// Сохраняем файл
void MainWindow::save()
{
    // Окон на области ui->mdiArea может быть много
    // определяем активное, так как скорее всего пользователь
    // про него говорил, когда выбирал пункт "Save".
    // У этого активного окна запускаем метод, который
    // сохранит файл
    if (activeMdiChildTable() && activeMdiChildTable()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
}

// Сохранить файл с другим названием
void MainWindow::saveAs()
{
    // Аналогично предыдущему варианту
    if (activeMdiChildTable() && activeMdiChildTable()->saveAs())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::cut()
{
    // Определяем текущее активное окно,
    // для использования метода cut().
    if (activeMdiChild())
        // Сам метод cut() прописан в базовом классе QTextEdit
        activeMdiChild()->cut();
}

void MainWindow::copy()
{
    if (activeMdiChild())
        activeMdiChild()->copy();
}

void MainWindow::paste()
{
    if (activeMdiChild())
        activeMdiChild()->paste();
}

// Метод по созданию дочернего окна в области ui->mdiArea
MdiChild *MainWindow::createMdiChild()
{
    // Создаём окно редактора
    MdiChild *child = new MdiChild;
    // Добавляем его в объект ui->mdiArea
    ui->mdiArea->addSubWindow(child);

    // По умолчанию пункты меню "Cut" и "Copy" отключены.
    // Связываем сигналы выделенного текста, для включения
    // этих пунктов в меню и выключения, если выделенного текста нет
    connect(child, &MdiChild::copyAvailable,
            ui->actionCut, &QAction::setEnabled);
    connect(child, &MdiChild::copyAvailable,
            ui->actionCopy, &QAction::setEnabled);

    return child;
}

MdiChildTable *MainWindow::createMdiChildTable()
{
    // Создаём окно таблицы
    MdiChildTable *child = new MdiChildTable;
    // Добавляем его в объект ui->mdiArea
    ui->mdiArea->addSubWindow(child);

    return child;
}

// Метод вытаскивает активное дочернее окно
MdiChild *MainWindow::activeMdiChild()
{
    // Проблема в том, что метод ui->mdiArea->activeSubWindow() возвращает
    // указатель типа QMdiSubWindow, а нам нужен MdiChild.
    // По этому вытаскиваем указатьель типа QMdiSubWindow активного окна
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        // и делаем приведение типа к указателю MdiChild.
        // Тут переход к указателю типа QWidget нужен по тому,
        // что тип QMdiSubWindow, это другая ветвь по иерархии наследования.
        // По этому нам надо поднятся вверх по иерархии к более общему классу
        return qobject_cast<MdiChild *>(activeSubWindow->widget());

    // На случай если активного окна не было
    return nullptr;
}

MdiChildTable *MainWindow::activeMdiChildTable()
{
    // Проблема в том, что метод ui->mdiArea->activeSubWindow() возвращает
    // указатель типа QMdiSubWindow, а нам нужен MdiChild.
    // По этому вытаскиваем указатьель типа QMdiSubWindow активного окна
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
        // и делаем приведение типа к указателю MdiChild.
        // Тут переход к указателю типа QWidget нужен по тому,
        // что тип QMdiSubWindow, это другая ветвь по иерархии наследования.
        // По этому нам надо поднятся вверх по иерархии к более общему классу
        return qobject_cast<MdiChildTable *>(activeSubWindow->widget());

    // На случай если активного окна не было
    return nullptr;
}

void MainWindow::updateActions()
{
    bool hasChild = ui->mdiArea->subWindowList().count() > 0;
    MdiChild *mdiChild = activeMdiChild();
    bool isTextEdit = mdiChild;
    //    QMdiSubWindow *existing = findMdiChildTable(fileName);

    ui->actionSave->setEnabled(hasChild);
    ui->actionSaveAs->setEnabled(hasChild);
    ui->menuDiagram->setEnabled(!isTextEdit && hasChild);
    ui->menuEdit->setEnabled(hasChild);
    ui->actionCut->setEnabled(isTextEdit);
    ui->actionCopy->setEnabled(isTextEdit);
    ui->actionPaste->setEnabled(isTextEdit);
    ui->actionPrint->setEnabled(!isTextEdit && hasChild);
    ui->lineEdit->setEnabled(!isTextEdit && hasChild);
    if (!isTextEdit && hasChild) {
        ui->label->show();
        ui->lineEdit->show();
    } else {
        ui->label->hide();
        ui->lineEdit->hide();
    }
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName, bool db)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    for (auto *window : ui->mdiArea->subWindowList()) {
        if (db) {
            MdiChildTable *mdiChildTable = qobject_cast<MdiChildTable *>(window->widget());
            if (mdiChildTable && mdiChildTable->currentFile() == canonicalFilePath) {
                return window;
            }
        } else {
            MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
            if (mdiChild && mdiChild->currentFile() == canonicalFilePath) {
                return window;
            }
        }
    }

    return nullptr;
}

void MainWindow::readSettings()
{
    QSettings settings("MySoft", "MyProgram");

    // Существует несколько способов работы с сохраняемыми параметрами
    // Первый, каждый параметр независим:
    //    restoreGeometry(settings.value("geometryMainWindow").toByteArray());
    // Второй, параметры объединяются в группы:

    // Для примера сделаем группу сохраняющую параметры окна MainWindow
    settings.beginGroup("MainWindowGeometry"); // начало группы
    resize(settings.value("size", QSize(400, 400)).toSize()); // сбрасываем размеры окна
    move(settings.value("position", QPoint(200, 200)).toPoint()); // сбрасываем крайнюю левую верхнюю точку
    settings.endGroup(); // конец группы
}

void MainWindow::writeSettings()
{
    QSettings settings("MySoft", "MyProgram");

    // Запись значений также как и чтение либо по одному, либо по группам
    // Пример по одному:
    //    settings.setValue("geometryMainWindow", saveGeometry());
    // Пример по группам MainWindowGeometry и MainWindowCheckBox:
    settings.beginGroup("MainWindowGeometry");
    settings.setValue("size", size());
    settings.setValue("position", pos());
    settings.endGroup();
}

void MainWindow::on_actionAbout_author_triggered()
{
    About aboutWnd(this);
    aboutWnd.exec();
}

void MainWindow::on_lineEdit_textChanged(const QString &text){
    MdiChildTable *myTable = activeMdiChildTable();

    if (!text.isEmpty()) {
        QModelIndex index = myTable->tableFind(text);
        myTable->setCurrentIndex(index);
    } else {
        myTable->resetFind();
    }
}


void MainWindow::createLanguageMenu()
{
    // Создаём список пунктов выбора языка в динамической памяти
    languageActionGroup = new QActionGroup(this);

    // Ставим связь пунктов меню со слотом смены языка приложения
    connect(languageActionGroup, &QActionGroup::triggered, this, &MainWindow::switchLanguage);

    // Определяем каталог, где лежат файлы переводов "*.qm"
    QDir dir(qmPath);

    QStringList fileNames =
        dir.entryList(QStringList("cw-sub_*.qm"));

    for (int i = 0; i < fileNames.size(); i++) {
        // Получаем i-ую локаль
        QString locale = fileNames[i];
        // Удаляем символы в строке сначала и по символ "_" включительно
        locale.remove(0, locale.indexOf('_') + 1);
        // Удаляем символы в строке начиная с символа точки
        locale.truncate(locale.lastIndexOf('.'));

        // Создаём временную переменную перевода для языкового пункта меню
        QTranslator translator;
        // Загружаем перевод из файла перевода "*.qm"
        translator.load(fileNames[i], qmPath);

        QString language = translator.translate("MainWindow", "English");
        QAction *action = new QAction(tr("&%1 %2").arg(i + 1).arg(language));

        action->setCheckable(true);
        // Задаём внутренние данные хранимые в i-ом пункте.
        // В нашем случае это локаль вида "ru_RU"
        action->setData(locale);

        // Добавляем i-ый пункт в меню на нашей форме "mainwindow.ui"
        ui->menuLanguage->addAction(action);
        // Добавляем i-ый пункт в единую группу пунктов
        languageActionGroup->addAction(action);

        // Задаём  английский язык в качестве выбранного по умолчанию
        if (language == "English")
            action->setChecked(true);
    }
}

void MainWindow::switchLanguage(QAction *action)
{
    QString locale = action->data().toString();

    // Загружаем тот перевод который выбрал пользователь
    appTranslator.load("cw-sub_" + locale, qmPath);
    locale.chop(3);

    qtTranslator.load("qt_" + locale + ".qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    ui->retranslateUi(this);
}

void MainWindow::on_actionShow_triggered()
{
    activeMdiChildTable()->showDiagram();
}

void MainWindow::on_actionPrint_triggered()
{
    if(!activeMdiChildTable()) {
        QMessageBox::warning(this, tr("Warning"), tr("There is no windows!"));
        return;
    }
    activeMdiChildTable()->printTable(activeMdiChildTable());
}
