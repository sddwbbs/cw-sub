#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MdiChild;
class MdiChildTable;
class QMdiSubWindow;

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

protected:
    // Для вопроса о сохранении файла при закрытии главного окна,
    // если в одном из дочерних окон файл был изменён
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    // Методы возвращающий указатель на активное дочернее окно
    MdiChild *activeMdiChild();
    MdiChildTable *activeMdiChildTable();
    // Методы поиска окна по имени файла
    QMdiSubWindow *findMdiChild(const QString &fileName);
    QMdiSubWindow *findMdiChildTable(const QString &fileName);
    // Методы для сохранения и чтения настроек
    void readSettings();
    void writeSettings();
};

#endif // MAINWINDOW_HPP
