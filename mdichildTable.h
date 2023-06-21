#ifndef MDICHILDTABLE_H
#define MDICHILDTABLE_H

#include <QString>
#include <QCloseEvent>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTableView>
#include <QMdiSubWindow>
#include <QMetaObject>
#include <QSortFilterProxyModel>

#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>

#include "SubcontractsModel.h"

class QSortFilterProxyModel;

// Воспользуемся классом для работы с текстом, но добавим несколько своих
// методов, которые реализуют дополнительный возможности нужные нам.
// На основе этого можно создать окно текстового редактора
/**
 * @brief Класс MdiChildTable - наследник QTableView, представляет собой таблицу
 * @details Класс реализует работу с файлами, поиск, контекстное меню, добавление и удаление строк,
 *          отслеживание изменений в таблице, печать таблицы, отображение диаграммы
 */
class MdiChildTable : public QTableView
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса
     * @param parent - указатель на родительский виджет
     */
    explicit MdiChildTable(QWidget *parent = nullptr);

    // Методы реализующие работу с файлами

    /**
     * @brief Создание нового файла
     */
    void newFile();

    /**
     * @brief Загрузка файла
     * @param fileName - имя файла
     * @return true, если загрузка прошла успешно, иначе false
     */
    bool loadFile(const QString &fileName);

    /**
     * @brief Инициализация таблицы
     */
    void initTable();

    /**
     * @brief Сохранение файла
     * @return true, если сохранение прошло успешно, иначе false
     */
    bool save();

    /**
     * @brief Сохранение файла с указанием имени
     * @param fileName - имя файла
     * @return true, если сохранение прошло успешно, иначе false
     */
    bool saveAs();

    /**
     * @brief Сохранение файла
     * @param fileName - имя файла
     * @return true, если сохранение прошло успешно, иначе false
     */
    bool saveFile(const QString &fileName);

    /**
     * @brief Поиск в таблице
     * @param text - текст для поиска
     * @return индекс найденной ячейки
     */
    QModelIndex tableFind(const QString &text);

    /**
     * @brief Обработчик события перетаскивания
     * @param event - указатель на событие перетаскивания
     */
    void dragEnterEvent(QDragEnterEvent *event);

    /**
     * @brief Обработчик события отпускания кнопки мыши после перетаскивания
     * @param event - указатель на событие отпускания кнопки мыши
     */
    void dropEvent(QDropEvent *event);

    /**
     * @brief Сброс поиска
     */
    void resetFind();

    /**
     * @brief Отображение диаграммы
     */
    void showDiagram();

    /**
     * @brief Печать таблицы
     * @param tableView - указатель на таблицу
     */
    void printTable(QTableView* tableView);

    /**
     * @brief Проверка, нужно ли сохранить документ
     * @return true, если нужно сохранить документ, иначе false
     */
    bool maybeSave();

    /**
     * @brief Обработчик события закрытия окна
     * @param event - указатель на событие закрытия окна
     */
    void closeEvent(QCloseEvent *event);

    /**
     * @brief Возвращает текущий файл
     * @return имя текущего файла
     * @deprecated Функция устарела и будет удалена в будущих версиях
     */
    QString currentFile() { return curFile; }

private slots:
    /**
     * @brief Очистка ячейки
     */
    void slotClearCell();

    /**
     * @brief Добавление строки
     */
    void slotAddRow();

    /**
     * @brief Удаление строки
     */
    void slotDeleteRow();

    /**
     * @brief Обработчик события вызова контекстного меню
     * @param pos - позиция курсора
     */
    void slotCustomMenuRequested(QPoint pos);

    /**
     * @brief Обработчик события добавления строк
     * @param parent - индекс родительской строки
     * @param first - номер первой добавленной строки
     * @param last - номер последней добавленной строки
     */
    void onRowsInserted(const QModelIndex &, int, int);

    /**
     * @brief Обработчик события удаления строк
     * @param parent - индекс родительской строки
     * @param first - номер первой удаленной строки
     * @param last - номер последней удаленной строки
     */
    void onRowsRemoved(const QModelIndex &, int, int);

    /**
     * @brief Обработчик события изменения данных в таблице
     */
    void onDataChanged();

private:
    /**
     * @brief Обработчик события нажатия клавиши
     * @param event - указатель на событие нажатия клавиши
     */
    void keyPressEvent(QKeyEvent *event);

    /**
     * @brief Установка текущего файла
     * @param fileName - имя файла
     */
    void setCurrentFile(const QString &fileName);

    /**
     * @brief Получение имени файла отдельно от пути
     * @return имя файла
     */
    QString userFriendlyCurrentFile();

    QString key; ///< Ключ
    QString curFile; ///< Имя файла
    MyTableModel *tableModel; ///< Модель таблицы
    QSortFilterProxyModel *proxyModel; ///< Прокси-модель
    bool isModified; ///< Флаг изменений в таблице
    bool isUntitled; ///< Флаг, указывающий, является ли файл безымянным
    int selectedColumn; ///< Номер выбранного столбца
};

#endif // MDICHILDTABLE_H
