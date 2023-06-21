#include <QtWidgets>

#include "mdichildTable.h"
#include "empdiagram.h"

MdiChildTable::MdiChildTable(QWidget *parent):
    QTableView(parent)
    , tableModel(new MyTableModel(this))
    , proxyModel(new QSortFilterProxyModel(this))
    , isModified(false)
    , isUntitled(true)
{
    this->key = "10321";
    /* Устанавливаем фокус на таблицу */
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->dragDropOverwriteMode();
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDefaultDropAction(Qt::MoveAction);

    /* Подключаем СЛОТ-обработчик для очистки ячейки */
    connect(this, &MdiChildTable::keyPressEvent,
            this, &MdiChildTable::slotClearCell);

    connect(tableModel, &QAbstractTableModel::dataChanged, this, &MdiChildTable::onDataChanged);
    connect(tableModel, &QAbstractTableModel::rowsInserted, this, &MdiChildTable::onRowsInserted);
    connect(tableModel, &QAbstractTableModel::rowsRemoved, this, &MdiChildTable::onRowsRemoved);

    QHeaderView* headerView = this->verticalHeader();
    headerView->setVisible(false);

    setWindowTitle(userFriendlyCurrentFile());
    this->setSortingEnabled(true);

    proxyModel->setSourceModel(tableModel);
    this->setModel(proxyModel);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MdiChildTable::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        slotClearCell();
    } else {
        QTableView::keyPressEvent(event);
    }
}

// Метод создаёт новый пустой файл
void MdiChildTable::newFile()
{
    // Для нумерации создаваемых файлов, пока им не присвоено имя
    static int sequenceNumber = 1;

    // Ставим метку, что файл без названия
    isUntitled = true;
    // Создаём имя документа по умолчанию, с использованием счётчика
    curFile = tr("untitled%1.db").arg(sequenceNumber++);
    // Включаем добавление символа "*" в заголовок дочернего окна,
    // в качестве признака редактирования файла
    setWindowTitle(curFile + "[*]");

    // Ставим контекстное меню для ячеек
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MdiChildTable::customContextMenuRequested,
            this, &MdiChildTable::slotCustomMenuRequested);
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
        QMessageBox::warning(this, tr("Error"), tr("Invalid key!"));
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

        newSubctr.setId(fields[0].toInt());
        newSubctr.setName(fields[1]);
        newSubctr.setNumberEmpl(fields[2].toInt());
        newSubctr.setWorkload(fields[3].toInt());
        newSubctr.setLocation(fields[4]);
        newSubctr.setAdditionalServ(fields[5].toInt());
        newSubctr.setPrice(fields[6].toInt());
        newSubctr.setExperience(fields[7].toInt());
        newSubctr.setCompletedProjects(fields[8].toInt());
        newSubctr.setRating(fields[9].toFloat());

        tableModel->insertRow(row, newSubctr);
        row++;
    }
    initTable();
    file.close();

    QApplication::restoreOverrideCursor();

    // Ставим контекстное меню для ячеек
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MdiChildTable::customContextMenuRequested,
            this, &MdiChildTable::slotCustomMenuRequested);

    isModified = false;
    return true;
}

void MdiChildTable::initTable() {
    QList<Subcontracts> subctr = tableModel->getData();

    for (int row = 0; row < tableModel->rowCount(); row++) {
        tableModel->setData(tableModel->index(row, 0), subctr[row].getId());
        tableModel->setData(tableModel->index(row, 1), subctr[row].getName());
        tableModel->setData(tableModel->index(row, 2), subctr[row].getNumberEmpl());
        tableModel->setData(tableModel->index(row, 3), subctr[row].getWorkload());
        tableModel->setData(tableModel->index(row, 4), subctr[row].getLocation());
        tableModel->setData(tableModel->index(row, 5), subctr[row].getAdditionalServ());
        tableModel->setData(tableModel->index(row, 6), subctr[row].getPrice());
        tableModel->setData(tableModel->index(row, 7), subctr[row].getExperience());
        tableModel->setData(tableModel->index(row, 8), subctr[row].getCompletedProjects());
        tableModel->setData(tableModel->index(row, 9), subctr[row].getRating());
    }
}

// При сохранении файла надо выяснить есть ли у него название
bool MdiChildTable::save()
{
    return saveFile(curFile);
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

QModelIndex MdiChildTable::tableFind(const QString &text)
{
    QModelIndex index = currentIndex();

    // Если в таблице нет выделенных ячеек, устанавливаем фильтр на указанный столбец
    if (!index.isValid()) {
        proxyModel->setFilterKeyColumn(selectedColumn);
    } else {
        selectedColumn = index.column();
        proxyModel->setFilterKeyColumn(selectedColumn);
    }

    proxyModel->setFilterFixedString(text);

    return index;

}

void MdiChildTable::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        event->acceptProposedAction();
    }
}

void MdiChildTable::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        QString text = QString::fromUtf8(event->mimeData()->data("text/plain"));
        QStringList textList = text.split(";");

        int row = this->indexAt(event->pos()).row();
        int col = this->indexAt(event->pos()).column();

        foreach (const QString &text, textList) {
            QModelIndex index = model()->index(row, col);

            if (index == QModelIndex()) {
                return;
            }

            model()->setData(index, text, Qt::EditRole);
            col++;
        }

        event->acceptProposedAction();
    }
}


void MdiChildTable::resetFind()
{
    proxyModel->setFilterFixedString(QString());

    // Если в таблице нет выделенных ячеек, сбрасываем фильтр на все столбцы
    if (!currentIndex().isValid()) {
        selectedColumn = -1;
        proxyModel->setFilterKeyColumn(selectedColumn);
    }
}


// Оставим у файла только его имя. Убираем путь
QString MdiChildTable::userFriendlyCurrentFile()
{
    return QFileInfo(curFile).fileName();
}

void MdiChildTable::closeEvent(QCloseEvent *event)
{
    // Если пользователь нажмёт "Cancel",
    // то дочернее окно не закроется
    if (maybeSave())
        // Окно закрывается
        event->accept();
    else
        // Окно продолжит работать
        event->ignore();
}

// Проверка, надо ли сохранять файл
bool MdiChildTable::maybeSave()
{
    //     Если документ был изменён
    if (isModified) {
        // То предложим пользователю его сохранить
        // или выйти без сохранения
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this,
                                   tr("MDI"),
                                   tr("'%1' has been modified.\n"
                                      "Do you want to save your changes?")
                                       .arg(userFriendlyCurrentFile()),
                                   QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
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
    newSubctr.setId(tableModel->howManyRecords() + 1);
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
    QModelIndex index = this->currentIndex();
    int sourceRow = proxyModel->mapToSource(index).row();
    if (sourceRow >= 0) {
        tableModel->removeRow(sourceRow);
        initTable();
    }
    //statusBar()->showMessage(tr("Row deleted"));
}

// Слот для вызова контекстного меню ячейки
void MdiChildTable::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * clearCell = new QAction(("Clear cell"), this);
    QAction * addRow = new QAction(("Add row"), this);
    QAction * deleteRow = new QAction(("Delete row"), this);

    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(clearCell, &QAction::triggered,
            this, &MdiChildTable::slotClearCell);
    connect(addRow, &QAction::triggered,
            this, &MdiChildTable::slotAddRow);
    connect(deleteRow, &QAction::triggered,
            this, &MdiChildTable::slotDeleteRow);

    /* Устанавливаем действия в меню */
    menu->addAction(clearCell);
    menu->addAction(addRow);
    menu->addAction(deleteRow);

    /* Вызываем контекстное меню */
    menu->popup(this->viewport()->mapToGlobal(pos));
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

void MdiChildTable::showDiagram()
{
    QList<Subcontracts> subctr = tableModel->getData();

    if (subctr.length() == 0) {
        QMessageBox::warning(this, "", tr("There is no data for create chart!"));
        return;
    }

    EmpDiagram *newDiagram = new EmpDiagram(this, subctr);
    newDiagram->show();
}

void MdiChildTable::printTable(QTableView* tableView) {
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QDir::currentPath() + "/print.pdf");

    QList<Subcontracts> subctr = tableModel->getData();

    // Создаем диалоговое окно печати и устанавливаем настройки принтера
    QPrintDialog printDialog(&printer, nullptr);
    if (printDialog.exec() == QDialog::Rejected) {
        return;
    }

    QString htmlTable;
    htmlTable = "<table border=1 cellspacing=0>\n"
                "<caption>Таблица субподрядчиков строительной фирмы</caption>\n"
                "<tr>\n"
                "<th>ID</th>\n"
                "<th>Название</th>\n"
                "<th>Количество работников</th>\n"
                "<th>Нагрузка</th>\n"
                "<th>Местоположение</th>\n"
                "<th>Дополнительные услуги</th>\n"
                "<th>Цена</th>\n"
                "<th>Опыт</th>\n"
                "<th>Завершенных проектов</th>\n"
                "<th>Рейтинг</th>\n"
                "</tr>\n";


    for (const auto& temp : subctr) {
        QString id = QString::number(temp.getId());
        QString numberEmp = QString::number(temp.getNumberEmpl());
        QString workload = QString::number(temp.getWorkload());
        QString additionalServ = QString::number(temp.getAdditionalServ());
        QString price = QString::number(temp.getPrice());
        QString experience = QString::number(temp.getExperience());
        QString completedProjects = QString::number(temp.getCompletedProjects());
        QString rating = QString::number(temp.getRating());

        htmlTable += "<tr><td>" + id + "</td><td>" +
                     temp.getName() + "</td><td>" +
                     numberEmp + "</td><td>" +
                     workload + "</td><td>" +
                     temp.getLocation() + "</td><td>" +
                     additionalServ + "</td><td>" +
                     price + "</td><td>" +
                     experience + "</td><td>" +
                     completedProjects + "</td><td>" +
                     rating + "</td></tr>\n";
    }
    htmlTable += "</table>\n<br>\n";

    QTextDocument document;
    document.setHtml(htmlTable);
    document.print(&printer);

    document.end();

    return;
}

void MdiChildTable::onRowsInserted(const QModelIndex &, int, int) { isModified =  true; }

void MdiChildTable::onRowsRemoved(const QModelIndex &, int, int) { isModified = true; }

void MdiChildTable::onDataChanged() { isModified = true; }
