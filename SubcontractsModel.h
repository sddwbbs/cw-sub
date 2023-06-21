#ifndef SUBCONTRACTSMODEL_H
#define SUBCONTRACTSMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "SubcontractsClass.h"

/**
 * @brief Класс "MyTableModel" представляет модель таблицы для подрядов.
 *
 * Класс наследуется от QAbstractTableModel и предоставляет методы для отображения и редактирования
 * данных о подрядах в виде таблицы.
 */
class MyTableModel : public QAbstractTableModel {
    Q_OBJECT

private:
    QList<Subcontracts> subctr; /**< Список подрядов. */
    int numberOfRecords; /**< Количество записей. */
    const int columnsNum; /**< Количество столбцов. */

public:
    /**
     * @brief Конструктор класса.
     *
     * @param parent Родительский объект.
     */
    explicit MyTableModel(QObject *parent = nullptr);

    /**
     * @brief Возвращает количество строк в модели.
     *
     * @param parent Родительский индекс.
     * @return Количество строк.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Возвращает количество столбцов в модели.
     *
     * @param parent Родительский индекс.
     * @return Количество столбцов.
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Очищает список подрядов.
     */
    void clearList();

    /**
     * @brief Возвращает значение элемента из списка подрядов для заданной ячейки таблицы.
     *
     * @param index Индекс ячейки таблицы.
     * @param role Роль элемента.
     * @return Значение элемента.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Возвращает MIME-данные для перетаскивания ячеек таблицы.
     *
     * @param indexes Список индексов ячеек таблицы.
     * @return MIME-данные.
     */
    QMimeData* mimeData(const QModelIndexList &indexes) const override;

    /**
     * @brief Возвращает список поддерживаемых MIME-типов.
     *
     * @return Список MIME-типов.
     */
    QStringList mimeTypes() const override;

    /**
     * @brief Возвращает режим перетаскивания-перемещения.
     *
     * @return true, если включен режим перезаписи при перетаскивании, иначе false.
     */
    bool dragDropOverwtiteMode() const;

    /**
     * @brief Возвращает поддерживаемые действия перетаскивания.
     *
     * @return Поддерживаемые действия перетаскивания.
     */
    Qt::DropActions supportedDropActions() const override;

    /**
     * @brief Задает новое значение для элемента в указанной ячейке таблицы.
     *
     * @param index Индекс ячейки таблицы.
     * @param value Новое значение.
     * @param role Роль элемента.
     * @return true, если установка значения прошла успешно, иначе false.
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /**
     * @brief Возвращает ссылку на список подрядов.
     *
     * @return Ссылка на список подрядов.
     */
    const QList<Subcontracts> &getData() const;

    /**
     * @brief Вставляет новую строку со значением "value" в модель.
     *
     * @param position Позиция, на которую нужно вставить строку.
     * @param value Значение для новой строки.
     * @return true, если вставка прошла успешно, иначе false.
     */
    bool insertRow(int position, const Subcontracts &value);

    /**
     * @brief Удаляет строку с заданной позицией из модели.
     *
     * @param position Позиция строки для удаления.
     * @return true, если удаление прошло успешно, иначе false.
     */
    bool removeRow(const int position);

    /**
     * @brief Возвращает количество записей в модели.
     *
     * @return Количество записей.
     */
    int howManyRecords();

    /**
     * @brief Возвращает значение для заголовка таблицы.
     *
     * @param section Номер секции (столбца/строки).
     * @param orientation Ориентация (горизонтальная/вертикальная).
     * @param role Роль элемента.
     * @return Значение для заголовка.
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * @brief Возвращает флаги для редактируемого элемента.
     *
     * @param index Индекс элемента.
     * @return Флаги элемента.
     */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /**
     * @brief Возвращает индекс для указанной строки и столбца в родительском индексе.
     *
     * @param row Номер строки.
     * @param column Номер столбца.
     * @param parent Родительский индекс.
     * @return Индекс элемента.
     */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
};





#endif // SUBCONTRACTSMODEL_H
