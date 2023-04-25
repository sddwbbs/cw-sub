#include <QtWidgets>

#include "mdichildTable.h"

MdiChildTable::MdiChildTable(QWidget *parent):
    QTableView(parent)
    , tableModel(new MyTableModel(this))
{
    this->key = "10321";
}

// Стандартная загрузка файла
bool MdiChildTable::loadFile(const QString &fileName)
{
    setCurrentFile(fileName);
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,
                             tr("MDI"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Загружаем данные в текущее дочернее окно
    // Считываем данные до конца файла
    QTextStream in(&file);
    int row = tableModel->rowCount();
    QString keyCheck = in.readLine();
    if (keyCheck != this->key) {
        QApplication::restoreOverrideCursor();
        return false;
    }
    while (!in.atEnd())
    {
        // ... построчно
        QString line = in.readLine();
        QStringList fields;
        try {
            fields = line.split(";");
        } catch (...) {
            QMessageBox::warning(this,
                                 tr("MDI"),
                                 tr("Cannot read file %1:\n%2.")
                                     .arg(fileName)
                                     .arg(file.errorString()));
            return true;
        }
        Subcontracts newSubctr;

        newSubctr.setName(fields[0]);
        newSubctr.setNumberEmpl(fields[1].toInt());
        newSubctr.setWorkload(fields[2].toInt());
        newSubctr.setLocation(fields[3]);
        newSubctr.setAdditionalServ(fields[4].toInt());
        newSubctr.setPrice(fields[5].toInt());
        newSubctr.setExperience(fields[6].toInt());
        newSubctr.setCompletedProjects(fields[7].toInt());
        newSubctr.setRating(fields[8].toFloat());

        tableModel->insertRow(row, newSubctr);
        row++;
    }
    initTable();
    file.close();

    QApplication::restoreOverrideCursor();
    setWindowTitle(userFriendlyCurrentFile());

    this->setModel(tableModel);
    this->resizeColumnsToContents();

    // Ставим контекстное меню
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotCustomMenuRequested(QPoint)));
    return true;
}

void MdiChildTable::initTable() {
    QList<Subcontracts> subctr = tableModel->getData();

    for (int row = 0; row < tableModel->rowCount(); row++) {
        tableModel->setData(tableModel->index(row, 0), subctr[row].getName());
        tableModel->setData(tableModel->index(row, 1), subctr[row].getNumberEmpl());
        tableModel->setData(tableModel->index(row, 2), subctr[row].getWorkload());
        tableModel->setData(tableModel->index(row, 3), subctr[row].getLocation());
        tableModel->setData(tableModel->index(row, 4), subctr[row].getAdditionalServ());
        tableModel->setData(tableModel->index(row, 5), subctr[row].getPrice());
        tableModel->setData(tableModel->index(row, 6), subctr[row].getExperience());
        tableModel->setData(tableModel->index(row, 7), subctr[row].getCompletedProjects());
        tableModel->setData(tableModel->index(row, 8), subctr[row].getRating());
    }
}

// При сохранении файла надо выяснить есть ли у него название
bool MdiChildTable::save()
{
    return true;
}

// Сохранение файла через открытие диалогового окна сохранения файла,
// если у него не было имени и пути
bool MdiChildTable::saveAs()
{
    return true;
}

// Метод записывает файл из текстового потока
bool MdiChildTable::saveFile(const QString &fileName)
{
    return true;
}

// Оставим у файла только его имя. Убираем путь
QString MdiChildTable::userFriendlyCurrentFile()
{
    return QFileInfo(curFile).fileName();
}

// Если дочернее окно закрывается, то надо проверить
// был ли в нём отредактирован документ, и если да,
// то предложим пользователю его сохранить
void MdiChildTable::closeEvent(QCloseEvent *event)
{

}

// В классе QTextEdit, за работу с форматированными данными используется
// класс QTextDocument, у которого есть механизм определять,
// было ли редактирование текста или нет. Данный метод узнаёт это и
// передаёт эти сведенья текущему дочернему окну
void MdiChildTable::documentWasModified()
{

}

// Слот для вызова контекстного меню
void MdiChildTable::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * editDevice = new QAction(trUtf8("Редактировать"), this);
    QAction * deleteDevice = new QAction(trUtf8("Удалить"), this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEditRecord()));     // Обработчик вызова диалога редактирования
    connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); // Обработчик удаления записи
    /* Устанавливаем действия в меню */
    menu->addAction(editDevice);
    menu->addAction(deleteDevice);
    /* Вызываем контекстное меню */
    menu->popup(this->viewport()->mapToGlobal(pos));
}

// Проверка, надо ли сохранять файл
bool MdiChildTable::maybeSave()
{
    return true;
}

// Метод делает новый отредактированный файл текущим
// и даёт ему имя
void MdiChildTable::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    // Используя новое имя файла надо обновить признак редактирования
    // в заголовке дочернего окна
    setWindowTitle(userFriendlyCurrentFile());
}


