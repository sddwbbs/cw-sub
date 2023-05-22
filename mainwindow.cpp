#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QtWidgets>
#include <QTextStream>
#include <QApplication>
#include <QTableView>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include <QSettings>
#include <QTranslator>
#include <QActionGroup>

#include "mdichild.h"
#include "mdichildTable.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , languageActionGroup(nullptr)
{
    ui->setupUi(this);

    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);

    qmPath = qApp->applicationDirPath() + "/translations";

    createLanguageMenu();

    // Соединяем пункты меню со слотами

    connect(ui->actionOpen, &QAction::triggered,
            this, &MainWindow::open);
    connect(ui->actionNew, &QAction::triggered,
            this, &MainWindow::newFile);
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

    connect(ui->menuDiagram, &QAction::triggered,
            this, &MainWindow::diagram_triggered);

    connect(ui->actionExit, &QAction::triggered,
            qApp, &QApplication::closeAllWindows);

    connect(ui->mdiArea, &QMdiArea::subWindowActivated,
            this, &MainWindow::updateActions);

    // Задаём заголовок окна. Его так же можно через свойства формы
    // в файле mainwindow.ui задать
    setWindowTitle(tr("My MDI Application"));

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

    if (ext != "db") {
        if (!fileName.isEmpty()) {
            // Выесняем, есть ли у нас уже документ с таким названием
            QMdiSubWindow *existing = findMdiChild(fileName);
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
        }
    } else {
        if (!fileName.isEmpty()) {
            // Выесняем, есть ли у нас уже документ с таким названием
            QMdiSubWindow *existing = findMdiChildTable(fileName);
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

// Так как в этом примере на одном главном окне располагается много
// отдельных окон в области ui->mdiArea, то перед закрытием главного
// окна их все надо позакрывать
void MainWindow::closeEvent(QCloseEvent *event)
{
    // Пытаемся закрыть все дочерние окна
    ui->mdiArea->closeAllSubWindows();
    // Проверяем закрылись ли все дочерние окна.
    if (ui->mdiArea->currentSubWindow())
        // Если есть какое-то активное окно,
        // то ждём пока его закроет пользователь
        event->ignore();
    else
        // Если все окна закрылись, то выходим
        event->accept();
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

void MainWindow::updateActions() {
    bool hasChild = ui->mdiArea->subWindowList().count() > 0;

    ui->lineEdit->setEnabled(hasChild);
    ui->actionSave->setEnabled(hasChild);
    ui->actionSaveAs->setEnabled(hasChild);
}

// Метод поиска окна по имени файла
QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
    // Вначале определим полный путь к файлу и его имя,
    // что бы не было ошибки и путаницы. Вдруг есть ещё один файл с таким же
    // именем но в другом каталоге
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    // Теперь переберём все дочерние окна в поисках окна с
    for (auto *window : ui->mdiArea->subWindowList()) {
        // Полученный в текущей итерации цикла указатель приводим
        // к типу MdiChild
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        // Методом сравнения определяем, является ли имя файла
        // в текущем окне искомым именем.
        if (mdiChild->currentFile() == canonicalFilePath)
            // Если совпадают, значит окно найдено и мы
            // возвращаем указатель на него
            return window;
    }

    // На случай если окно не нашлось
    return nullptr;
}

// Метод поиска окна по имени файла
QMdiSubWindow *MainWindow::findMdiChildTable(const QString &fileName)
{
    // Вначале определим полный путь к файлу и его имя,
    // что бы не было ошибки и путаницы. Вдруг есть ещё один файл с таким же
    // именем но в другом каталоге
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    // Теперь переберём все дочерние окна в поисках окна с
    for (auto *window : ui->mdiArea->subWindowList()) {
        // Полученный в текущей итерации цикла указатель приводим
        // к типу MdiChild
        MdiChildTable *mdiChildTable = qobject_cast<MdiChildTable *>(window->widget());
        // Методом сравнения определяем, является ли имя файла
        // в текущем окне искомым именем.
        if (mdiChildTable->currentFile() == canonicalFilePath)
            // Если совпадают, значит окно найдено и мы
            // возвращаем указатель на него
            return window;
    }

    // На случай если окно не нашлось
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


void MainWindow::on_lineEdit_textChanged(const QString &text)
{
    MdiChildTable *myTable = activeMdiChildTable();

    if (!text.isEmpty()) {
        if (myTable->currentIndex().isValid()) {
            QModelIndex index = myTable->tableFind(text);
            myTable->setCurrentIndex(index);
        }
    }
    else
        myTable->resetFind();
}


void MainWindow::on_actionSaveAs_triggered()
{
    MdiChildTable *myTable = activeMdiChildTable();
    myTable->saveAs();
}

void MainWindow::createLanguageMenu()
{
    /// Создаём список пунктов выбора языка в динамической памяти
    languageActionGroup = new QActionGroup(this);

    /// Ставим связь пунктов меню со слотом смены языка приложения
    connect(languageActionGroup, &QActionGroup::triggered,
            this, &MainWindow::switchLanguage);

    /// Определяем каталог, где лежат файлы переводов "*.qm"
    QDir dir(qmPath);

    /**
     * Получаем список файлов "*.qm" в каталоге, которые относятся
     * к нашей программе по шаблону "multilang_*.qm",
     * где "multilang" - название нашего приложения
     * "_" - разделитель
     * "*" - означает любой символ или группа символ произвольной длинны
     * ".qm" - расширение файла
     */
    QStringList fileNames =
        dir.entryList(QStringList("multilang_*.qm"));

    /**
     * Количество пунктов меню нам заранее не известно,
     * но так как оно зависит от количества файлов "*.qm"
     * в каталоге "translations",
     * то мы можем сформировать в меню столько пунктов,
     * сколько файлов у нас, используя цикл.
     */
    for (int i = 0; i < fileNames.size(); i++) {
        /// Получаем i-ую локаль
        QString locale = fileNames[i];
        /// Удаляем символы в строке сначала и по символ "_" включительно
        locale.remove(0, locale.indexOf('_') + 1);
        /// Удаляем символы в строке начиная с символа точки
        locale.truncate(locale.lastIndexOf('.'));

        /// Создаём временную переменную перевода для языкового пункта меню
        QTranslator translator;
        /// Загружаем перевод из файла перевода "*.qm"
        translator.load(fileNames[i], qmPath);

        /**
         * Создаём имя i-ого пункта меню с названием языка путём
         * перевода в linguist заменив English на соответствующий язык
         */
        QString language = translator.translate("MainWindow",
                                                "English");

        /**
         * Создаём пункт в меню с i-ым языком по маске "&%1 %2",
         * где "&" - символ быстрого доступа к пункту меню через сочетание
         * клавиш Alt+символ_перед_которым_стоит_&;
         * "%1" - номер i-ого пункта меню по порядку.
         * "%2" - название языка, которое будет отображаться в меню.
         */
        QAction *action = new QAction(tr("&%1 %2")
                                          .arg(i + 1)
                                          .arg(language),
                                      this);

        /**
         * Задаём свойства для i-ого пункта меню.
         * Возможность держать пункт меню выбранным
         * пока пользователем не будет выбран другой пункт меню.
         */
        action->setCheckable(true);
        // Задаём внутренние данные хранимые в i-ом пункте.
        // В нашем случае это локаль вида "ru_RU"
        action->setData(locale);

        /// Добавляем i-ый пункт в меню на нашей форме "mainwindow.ui"
        ui->menuLanguage->addAction(action);
        /// Добавляем i-ый пункт в единую группу пунктов
        languageActionGroup->addAction(action);

        /// Задаём  английский язык в качестве выбранного по умолчанию
        if (language == "English")
            action->setChecked(true);
    }
}

void MainWindow::switchLanguage(QAction *action)
{
    /**
     * Определяем локаль которую выбрал пользователь.
     * Например "ru_RU" для русского языка в России или
     * "en_US" для английского в США
     */
    QString locale = action->data().toString();

    /// Загружаем тот перевод который выбрал пользователь
    appTranslator.load("multilang_" + locale, qmPath);
    locale.chop(3);
    /**
     * Для переводов стандартных сообщений Qt можно запросить у системы
     * их местонахождение через вызов "QLibraryInfo::TranslationsPath"
     */
    qtTranslator.load("qt_" + locale + ".qm", QLibraryInfo::location(
                                                  QLibraryInfo::TranslationsPath));



    /**
     * Взываем "retranslateUi()" для обновления интерфейса приложения
     * используя загруженный из соответствующего файла "qm" язык
     */
    ui->retranslateUi(this);

    // Для элемента созданного динамический, но не на форме,
    // надо заново задать текст, который сработает при переключении языка
}

void MainWindow::diagram_triggered() {
    activeMdiChildTable()->showDiagram();
}

