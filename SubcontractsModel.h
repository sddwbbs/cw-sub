#ifndef SUBCONTRACTSMODEL_H
#define SUBCONTRACTSMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "SubcontractsClass.h"

class MyTableModel : public QAbstractTableModel
{
    // Если добавили после сборки проекта, то надо удалить старые файл проекта
    // и пересобрать заново, а то будет вылезать ошибка компиляции
    // "undefined reference to `vtable for MyTableModel'"
    Q_OBJECT

    const int columnsNum;

public:
    explicit MyTableModel(QObject *parent = nullptr);

    // Методы, возвращают количество элементов списка (строк) и столбцов
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void clearList();

    // Метод, возвращает значение элементов из списков в ячейку таблицы
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    Qt::DropActions supportedDropActions() const override;

    // Метод, возвращает новое значение элемента из ячейки в список
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role = Qt::EditRole) override;

    // Метод, вставляет новую строку "value" в модель
    bool insertRow(int position, const Subcontracts &value);
    bool removeRow(const int position);

    // Метод, возвращает значение в заголовок таблицы
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    const QList<Subcontracts> &getData() const;

    // Метод, возвращает параметры редактируемого элемента
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QList<Subcontracts> subctr;
};




#endif // SUBCONTRACTSMODEL_H
