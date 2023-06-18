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

#include "SubcontractsClass.h"
#include "SubcontractsModel.h"

class QSortFilterProxyModel;

// Воспользуемся классом для работы с текстом, но добавим несколько своих
// методов, которые реализуют дополнительный возможности нужные нам.
// На основе этого можно создать окно текстового редактора
class MdiChildTable : public QTableView
{
    Q_OBJECT

public:
    explicit MdiChildTable(QWidget *parent = nullptr);

    // Методы реализующие работу с файлами
    bool loadFile(const QString &fileName);

    void initTable();

    bool save();

    bool saveAs();

    bool saveFile(const QString &fileName);

    QString currentFile() { return curFile; }

    QModelIndex tableFind(const QString &text);

    void dragEnterEvent(QDragEnterEvent *event);

    void dropEvent(QDropEvent *event);

    void resetFind();

    void showDiagram();

    void printTable(QTableView* tableView);

    // Проверка нужно ли сохранить документ
    bool maybeSave();

    // Для вопроса о сохранении файла при закрытии дочернего окна,
    // если файл был изменён
    void closeEvent(QCloseEvent *event);

private slots:
    void slotClearCell();

    void slotAddRow();

    void slotDeleteRow();

    void slotCustomMenuRequested(QPoint pos);

    void onRowsInserted(const QModelIndex &, int, int);

    void onRowsRemoved(const QModelIndex &, int, int);

    void onDataChanged();

private:
    void keyPressEvent(QKeyEvent *event);

    // Метод делает имя файла как текущий файл
    void setCurrentFile(const QString &fileName);

    // Получить имя файла отдельно от пути
    QString userFriendlyCurrentFile();

    QString key;

    // Переменная хранит имя файла
    QString curFile;

    MyTableModel *tableModel;

    QSortFilterProxyModel *proxyModel;

    bool isModified;
};

#endif // MDICHILDTABLE_H
