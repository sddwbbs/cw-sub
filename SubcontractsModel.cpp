#include "SubcontractsModel.h"
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItem>

using namespace std;

MyTableModel::MyTableModel(QObject *parent):
    QAbstractTableModel(parent)
    , columnsNum(9) // Задаём количество столбцов
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
        case 0: return subctr[row].getName();
        case 1: return subctr[row].getNumberEmpl();
        case 2: return subctr[row].getWorkload();
        case 3: return subctr[row].getLocation();
        case 4: return subctr[row].getAdditionalServ();
        case 5: return subctr[row].getPrice();
        case 6: return subctr[row].getExperience();
        case 7: return subctr[row].getCompletedProjects();
        case 8: return subctr[row].getRating();
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


Qt::DropActions MyTableModel::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
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
        case 0: subctr[index.row()].setName(value.toString()); break;
        case 1: subctr[index.row()].setNumberEmpl(value.toInt()); break;
        case 2: subctr[index.row()].setWorkload(value.toInt()); break;
        case 3: subctr[index.row()].setLocation(value.toString()); break;
        case 4: subctr[index.row()].setAdditionalServ(value.toInt()); break;
        case 5: subctr[index.row()].setPrice(value.toInt()); break;
        case 6: subctr[index.row()].setExperience(value.toInt()); break;
        case 7: subctr[index.row()].setCompletedProjects(value.toInt()); break;
        case 8: subctr[index.row()].setRating(value.toFloat()); break;
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


// Слот вставляет новые значения в таблицу
bool MyTableModel::insertRow(int position, const Subcontracts &value)
{
    beginInsertRows(QModelIndex(), position, position);

    subctr.insert(position, value);

    endInsertRows();

    return true;
}

bool MyTableModel::removeRow(const int position)
{
    beginRemoveRows(QModelIndex(), position, position);

    subctr.removeAt(position);

    endRemoveRows();

    return true;
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
        case 0: return QString("Name");
        case 1: return QString("Number of Employees");
        case 2: return QString("Workload");
        case 3: return QString("Location");
        case 4: return QString("Additional Services");
        case 5: return QString("Price");
        case 6: return QString("Experience");
        case 7: return QString("Completed Projects");
        case 8: return QString("Rating");
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

const QList<Subcontracts> &MyTableModel::getData() const
{
    // Возвращаем содержимое списка
    return subctr;
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const
{
    // Проверяем на корректность текущий index
    if (index.isValid())
        // Если индекс правильный, то устанавливаем для него
        // следующие стандартные возможности:
        // ItemIsEnabled - включить текущую ячейку;
        // ItemIsSelectable - текущая ячейка может быть выделяемой;
        // ItemIsEditable - текущая ячейка может быть редактируемой;
        // ItemIsDragEnabled - разрешить перемещать данные через захват мышью
        return Qt::ItemIsEnabled |
               Qt::ItemIsSelectable |
               Qt::ItemIsEditable |
               Qt::ItemIsDragEnabled;
    else
        // Если индекс некорректный, то отправляем нулевой флаг
        return Qt::NoItemFlags;
}









