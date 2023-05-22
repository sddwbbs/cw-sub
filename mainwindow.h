#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MdiChild;
class MdiChildTable;
class QMdiSubWindow;
class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Методы позволяющие создавать, открывать и сохранять файл
    void newFile();
    void open();
    void save();
    void saveAs();

    // Методы позволяющие вырезать, скопировать и вставлять текст
    void cut();
    void copy();
    void paste();

    // Метод создающий новое дочернее окно на основе динамического
    // экземпляра класса MdiChild
    MdiChild *createMdiChild();
    MdiChildTable *createMdiChildTable();

    void on_actionAbout_author_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_actionSaveAs_triggered();

    void switchLanguage(QAction *action);

    void diagram_triggered();

protected:
    // Для вопроса о сохранении файла при закрытии главного окна,
    // если в одном из дочерних окон файл был изменён
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    // Методы возвращающий указатель на активное дочернее окно
    MdiChild *activeMdiChild();
    MdiChildTable *activeMdiChildTable();
    void updateActions();
    // Методы поиска окна по имени файла
    QMdiSubWindow *findMdiChild(const QString &fileName);
    QMdiSubWindow *findMdiChildTable(const QString &fileName);
    
    // Методы для сохранения и чтения настроек
    void readSettings();
    void writeSettings();

    QTranslator appTranslator;
    QTranslator qtTranslator;

    // Список доступных переводов в меню будет формироваться динамически
    QActionGroup *languageActionGroup;
    /// Переменная где будет храниться путь к файлам перевода "*.qm"
    QString qmPath;

    /// Метод для создания языкового меню
    void createLanguageMenu();
};

#endif // MAINWINDOW_HPP
