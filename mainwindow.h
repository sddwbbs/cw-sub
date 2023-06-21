#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTranslator>
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
#include <QActionGroup>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>

#include "mdichild.h"
#include "mdichildTable.h"
#include "about.h"

namespace Ui {
class MainWindow;
}

class MdiChild;
class MdiChildTable;
class QMdiSubWindow;
class QActionGroup;

/**
 * @brief Класс MainWindow представляет главное окно приложения.
 *
 * Он наследуется от класса QMainWindow.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса MainWindow.
     *
     * @param parent Указатель на родительский виджет (по умолчанию nullptr).
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Виртуальный деструктор класса MainWindow.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Слот для создания нового файла.
     */
    void newFile();

    /**
     * @brief Слот для создания нового текстового файла.
     */
    void newTextFile();

    /**
     * @brief Слот для открытия файла.
     */
    void open();

    /**
     * @brief Слот для сохранения файла.
     */
    void save();

    /**
     * @brief Слот для сохранения файла с указанием пути.
     */
    void saveAs();

    /**
     * @brief Слот для вырезания выделенного текста.
     */
    void cut();

    /**
     * @brief Слот для копирования выделенного текста.
     */
    void copy();

    /**
     * @brief Слот для вставки скопированного/вырезанного текста.
     */
    void paste();

    /**
     * @brief Создает объект класса MdiChild.
     *
     * @return Указатель на созданный объект MdiChild.
     */
    MdiChild *createMdiChild();

    /**
     * @brief Создает объект класса MdiChildTable.
     *
     * @return Указатель на созданный объект MdiChildTable.
     */
    MdiChildTable *createMdiChildTable();

    /**
     * @brief Слот для отображения информации об авторе.
     */
    void on_actionAbout_author_triggered();

    /**
     * @brief Слот для обработки изменений в текстовом поле lineEdit.
     *
     * @param arg1 Измененный текст.
     */
    void on_lineEdit_textChanged(const QString &arg1);

    /**
     * @brief Переключает язык приложения.
     *
     * @param action Выбранное действие для переключения языка.
     */
    void switchLanguage(QAction *action);

    /**
     * @brief Слот для отображения окна.
     */
    void on_actionShow_triggered();

    /**
     * @brief Слот для печати содержимого.
     */
    void on_actionPrint_triggered();

protected:

private:
    Ui::MainWindow *ui; /**< Указатель на пользовательский интерфейс класса MainWindow. */

    /**
     * @brief Возвращает активный объект класса MdiChild.
     *
     * @return Указатель на активный объект MdiChild.
     */
    MdiChild *activeMdiChild();

    /**
     * @brief Возвращает активный объект класса MdiChildTable.
     *
     * @return Указатель на активный объект MdiChildTable.
     */
    MdiChildTable *activeMdiChildTable();

    /**
     * @brief Обновляет состояние действий.
     */
    void updateActions();

    /**
     * @brief Находит объект класса QMdiSubWindow по имени файла.
     *
     * @param fileName Имя файла.
     * @param db Флаг, указывающий, является ли объект QMdiSubWindow документом базы данных.
     * @return Указатель на объект QMdiSubWindow или nullptr, если не найден.
     */
    QMdiSubWindow *findMdiChild(const QString &fileName, bool db);

    /**
     * @brief Загружает настройки приложения.
     */
    void readSettings();

    /**
     * @brief Сохраняет настройки приложения.
     */
    void writeSettings();

    QTranslator appTranslator; /**< Переводчик приложения. */
    QTranslator qtTranslator; /**< Переводчик Qt. */

    QActionGroup *languageActionGroup; /**< Группа действий для выбора языка. */

    QString qmPath; /**< Путь к файлам перевода. */

    /**
     * @brief Создает меню выбора языка.
     */
    void createLanguageMenu();
};


#endif // MAINWINDOW_HPP
