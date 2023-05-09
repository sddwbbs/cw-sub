#include <QtWidgets>

#include "mdichildTable.h"

MdiChildTable::MdiChildTable(QWidget *parent):
    QTableView(parent)
    , tableModel(new MyTableModel(this))
    , proxyModel(new QSortFilterProxyModel(this))
{
    this->key = "10321";
    /* Устанавливаем фокус на таблицу */
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    /* Подключаем СЛОТ-обработчик для удаления записи */
    connect(this, SIGNAL(deleteKeyPressed()), this, SLOT(slotRemoveRecord()));
}

void MdiChildTable::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        slotClearCell();
    } else {
        QTableView::keyPressEvent(event);
    }
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
    this->setSortingEnabled(true);

    proxyModel->setSourceModel(tableModel);
    this->setModel(proxyModel);
    this->resizeColumnsToContents();

    // Ставим контекстное меню для ячеек
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotCustomMenuRequested(QPoint)));

    // Ставим контекстное меню для строк
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->horizontalHeader(), SIGNAL(customContextMenuRequestedRow(QPoint)),
            this, SLOT(slotCustomMenuRequestedRow(QPoint)));

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
    return saveFile(curFile);;
}

// Сохранение файла через открытие диалогового окна сохранения файла,
// если у него не было имени и пути
bool MdiChildTable::saveAs()
{
    QFileDialog *fDialog = new QFileDialog(this);
    fDialog->setAttribute(Qt::WA_DeleteOnClose);

    QString fileName = fDialog->getSaveFileName(this, tr("Save Document"), QDir::cleanPath("./.."), "Text Files (*.txt *.db)", nullptr, QFileDialog::DontUseNativeDialog);

    if (fileName.isEmpty())
        return false;

    // Путь и имя получили, теперь делаем сохранение файла
    return saveFile(fileName);
}

// Метод записывает файл из текстового потока
bool MdiChildTable::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,
                             tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                                 .arg(fileName).arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    QList<Subcontracts> subctr = tableModel->getData();

    out << "10321" << "\n";

    for (Subcontracts temp : subctr) {
        out << temp.getName() << ";"
            << temp.getNumberEmpl() << ";"
            << temp.getWorkload() << ';'
            << temp.getLocation() << ";"
            << temp.getAdditionalServ() << ";"
            << temp.getPrice() << ";"
            << temp.getExperience() << ";"
            << temp.getCompletedProjects() << ";"
            << temp.getRating() << "\n";
    }

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;

}

// Метод реализующий поиск по таблице
QModelIndex MdiChildTable::tableFind(const QString &text)
{
    QModelIndex index = currentIndex();

    proxyModel->setFilterKeyColumn(index.column());
    proxyModel->setFilterFixedString(text);

    return index;
}

void MdiChildTable::resetFind()
{
    proxyModel->setFilterFixedString(QString());
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

void MdiChildTable::slotClearCell()
{
    /* Получаем индекс выбранной ячейки */
    QModelIndex index = this->currentIndex();

    /* Проверяем, что ячейка действительно выбрана */
    if (index.isValid()) {
        /* Очищаем содержимое ячейки */
        this->model()->setData(index, QVariant());
    }
}

void MdiChildTable::slotAddRow()
{
    int row = tableModel->rowCount();
    Subcontracts newSubctr;
    newSubctr.setName("");
    newSubctr.setNumberEmpl(0);
    newSubctr.setWorkload(0);
    newSubctr.setLocation("");
    newSubctr.setAdditionalServ(0);
    newSubctr.setPrice(0);
    newSubctr.setExperience(0);
    newSubctr.setCompletedProjects(0);
    newSubctr.setRating(0);

    tableModel->insertRow(row, newSubctr);
    initTable();
}

void MdiChildTable::slotDeleteRow()
{
    int row = this->currentIndex().row();
    tableModel->removeRow(row);
    initTable();
}

// Слот для вызова контекстного меню ячейки
void MdiChildTable::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * clearCell = new QAction(trUtf8("Очистить ячейку"), this);
    QAction * addRow = new QAction(trUtf8("Добавить строку"), this);
    QAction * deleteRow = new QAction(trUtf8("Удалить строку"), this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(clearCell, SIGNAL(triggered()), this, SLOT(slotClearCell()));// Обработчик удаления записи
    connect(addRow, SIGNAL(triggered()), this, SLOT(slotAddRow()));
    connect(deleteRow, SIGNAL(triggered()), this, SLOT(slotDeleteRow()));
    /* Устанавливаем действия в меню */
    menu->addAction(clearCell);
    menu->addAction(addRow);
    menu->addAction(deleteRow);
    /* Вызываем контекстное меню */
    menu->popup(this->viewport()->mapToGlobal(pos));
}

// Слот для вызова контекстного меню строки
void MdiChildTable::slotCustomMenuRequestedRow(QPoint pos)
{
    // Получаем номер выбранной строки
    int row = this->rowAt(pos.y());

    // Если строка выбрана, то показываем контекстное меню
    QMenu *menu = new QMenu(this);
    QAction * deleteRow = new QAction(trUtf8("Удалить"), this);
    connect(deleteRow, SIGNAL(triggered()), this, SLOT(slotRemoveRecord()));
    menu->addAction(deleteRow);
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


