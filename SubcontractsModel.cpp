#include "SubcontractsModel.h"
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItem>
#include <QMimeData>

using namespace std;

MyTableModel::MyTableModel(QObject *parent):
    QAbstractTableModel(parent)
    , columnsNum(10) // Задаём количество столбцов
    , numberOfRecords(0)
{

}

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : subctr.count();
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : columnsNum;
}

void MyTableModel::clearList() {
    subctr.clear();
}

// Этот метод вызывается для каждой ячейки и по каждому событию
// для правильной отрисовки объекта и взаимодействия с пользователем.
// Вызывает данный метод, как правило, объект класса "QTableView"
QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    // Работать имеет смысл только при корректном индексе
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    int row = index.row();
    int col = index.column();

    // Определяем, с какой ролью был вызван метод "data()", и что он должен
    // теперь сделать с ячейкой
    switch (role) {
    // Роль, которая отрабатывает, если ячейка отображается в таблице
    case Qt::DisplayRole :

        // Роль, которая отрабатывает при редактировании ячейки таблицы
    case Qt::EditRole :
        switch(col) {
        case 0: return subctr[row].getId();
        case 1: return subctr[row].getName();
        case 2: return subctr[row].getNumberEmpl();
        case 3: return subctr[row].getWorkload();
        case 4: return subctr[row].getLocation();
        case 5: return subctr[row].getAdditionalServ();
        case 6: return subctr[row].getPrice();
        case 7: return subctr[row].getExperience();
        case 8: return subctr[row].getCompletedProjects();
        case 9: return subctr[row].getRating();
        }

        // Роль для изменения фона ячейки
    case Qt::BackgroundRole :
        // Для всех остальных случаев выдать недопустимый вариант (invalid)
        return QVariant();

        // Для всех остальных случаев выдать недопустимый вариант (invalid)
    default:
        return QVariant();
    }

    // Для всех остальных случаев выдать недопустимый вариант (invalid)
    return QVariant();
}

//===============================================================================================

//QMimeData *MyTableModel::mimeData(const QModelIndexList &indexes) const
//{
//    QMimeData *mimeData = new QMimeData();
//    QByteArray encodedData;
//    QDataStream stream(&encodedData, QIODevice::WriteOnly);
//    foreach(const QModelIndex &index, indexes) {
//        if(index.isValid()) {
//            QString text = data(index, Qt::DisplayRole).toString();
//            stream << text;
//        }
//    }
//    mimeData->setData("text/plain", encodedData);
//    return mimeData;
//}

//QStringList MyTableModel::mimeTypes() const
//{
//    QStringList types;
//    types << "text/plain";
//    return types;
//}

//bool MyTableModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
//                                int row, int column, const QModelIndex &parent)
//{
//    qDebug() << action;
//    if(action == Qt::IgnoreAction)
//        return true;
//    if(!data->hasFormat("text/plain"))
//        return false;
//    if(column > 0)
//        return false;
//    int beginRow;
//    if(row != -1)
//        beginRow = row;
//    else if(parent.isValid())
//        beginRow = parent.row();
//    else
//        beginRow = rowCount(QModelIndex());
//    QByteArray encodedData = data->data("text/plain");
//    QDataStream stream(&encodedData, QIODevice::ReadOnly);
//    QStringList newItems;
//    int rows = 0;
//    while(!stream.atEnd()) {
//        QString text;
//        stream >> text;
//        newItems << text;
//        ++rows;
//    }
//    insertRows(beginRow, rows, QModelIndex());
//    foreach(const QString &text, newItems) {
//        QModelIndex idx = index(beginRow, column, QModelIndex());
//        setData(idx, text);
//        beginRow++;
//    }
//    return true;
//}

//bool MyTableModel::dragDropOverwtiteMode() const
//{
//    return false;
//}

//Qt::DropActions MyTableModel::supportedDropActions() const
//{
//    return Qt::CopyAction | Qt::MoveAction;
//}

//===============================================================================================

QStringList MyTableModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

QMimeData *MyTableModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    foreach(const QModelIndex &index, indexes) {
        if(index.isValid()) {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << text;
        }
    }
    mimeData->setData("text/plain", encodedData);
    return mimeData;
}

//QMimeData *MyTableModel::mimeData(const QModelIndexList &indexes) const
//{
//    QMimeData *mimeData = new QMimeData();
//    QByteArray encodedData;
//    QDataStream stream(&encodedData, QIODevice::WriteOnly);

//    foreach(const QModelIndex &index, indexes) {
//        if(index.isValid()) {
//            QString text = data(index, Qt::DisplayRole).toString();
//            stream << text;

//            // Добавляем информацию о позиции ячейки
//            stream << index.row() << index.column();
//        }
//    }

//    mimeData->setData("text/plain", encodedData);
//    return mimeData;
//}

Qt::DropActions MyTableModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

bool MyTableModel::dragDropOverwtiteMode() const
{
    return false;
}

// Делаем вставку, введённых пользователем данных из ячейки
// в контейнеры "listStrA" и "listStrB",
// и в переменную "time" записываем текущее время системы
bool MyTableModel::setData(const QModelIndex &index,
                           const QVariant &value,
                           int role)
{
    // Проверим на корректность "index"
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return false;

    // Определяем текущий столбец
    //    int row = index.row();
    int col = index.column();

    // Определяем текущую роль. Тут почти всегда будет "EditRole".
    // Другие роли будут, если поменяется поле ввода с текстовой на "QCheckBox"
    if (role == Qt::EditRole) {
        switch(col) {
        case 1: subctr[index.row()].setName(value.toString()); break;
        case 2: subctr[index.row()].setNumberEmpl(value.toInt()); break;
        case 3: subctr[index.row()].setWorkload(value.toInt()); break;
        case 4: subctr[index.row()].setLocation(value.toString()); break;
        case 5: subctr[index.row()].setAdditionalServ(value.toInt()); break;
        case 6: subctr[index.row()].setPrice(value.toInt()); break;
        case 7: subctr[index.row()].setExperience(value.toInt()); break;
        case 8: subctr[index.row()].setCompletedProjects(value.toInt()); break;
        case 9: subctr[index.row()].setRating(value.toFloat()); break;
        }

        // Если модель используют несколько представлений,
        // то необходимо испустить сигнал "dataChanged()",
        // чтобы проинформировать об изменениях другие представления
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
        // Если всё хорошо, то вызывающему методу сообщаем об этом
        return true;
    }

    // Если вставить данные не удалось
    return false;
}

const QList<Subcontracts> &MyTableModel::getData() const
{
    // Возвращаем содержимое списка
    return subctr;
}

// Слот вставляет новые значения в таблицу
bool MyTableModel::insertRow(int position, const Subcontracts &value)
{
    beginInsertRows(QModelIndex(), position, position);

    subctr.insert(position, value);

    numberOfRecords++;

    endInsertRows();

    return true;
}

bool MyTableModel::removeRow(const int position)
{
    if (numberOfRecords == 0) {
        return false;
    }

    beginRemoveRows(QModelIndex(), position, position);

    subctr.removeAt(position);

    numberOfRecords--;

    endRemoveRows();

    return true;
}

int MyTableModel::howManyRecords() {
    if (numberOfRecords == 0) {
        numberOfRecords++;
        return 0;
    }
    numberOfRecords = subctr.last().getId();
    return numberOfRecords;
}

// Задаём заголовки для столбцов и строк таблицы
QVariant MyTableModel::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const
{
    // Задаём заголовки для столбцов при их отображении
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        // Определяем какой сейчас столбец. Отсчёт идёт от нуля
        switch (section) {
        case 0: return QString(tr("ID"));
        case 1: return QString(tr("Name"));
        case 2: return QString(tr("Number of Employees"));
        case 3: return QString(tr("Workload"));
        case 4: return QString(tr("Location"));
        case 5: return QString(tr("Additional Services"));
        case 6: return QString(tr("Price"));
        case 7: return QString(tr("Experience"));
        case 8: return QString(tr("Completed Projects"));
        case 9: return QString(tr("Rating"));
        }
    }

    // Задаём заголовки для строк при их отображении
    if (orientation == Qt::Vertical && role == Qt::DisplayRole)
        // Поскольку строки просто нумеруются, можно просто передать значение
        // "section + 1" и преобразовать его из "int" в "QString"
        return QString::number(section + 1);

    // Для всех остальных случаев выдать недопустимый вариант (invalid)
    return QVariant();
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid() && index.column() != 0)
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QModelIndex MyTableModel::index(int row, int column, const QModelIndex &) const {
    return createIndex(row, column);
}










