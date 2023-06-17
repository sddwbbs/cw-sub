#include <QtWidgets>

#include "mdichildTable.h"
#include "empdiagram.h"

MdiChildTable::MdiChildTable(QWidget *parent):
    QTableView(parent)
    , tableModel(new MyTableModel(this))
    , proxyModel(new QSortFilterProxyModel(this))
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

// Метод реализующий поиск по таблице
QModelIndex MdiChildTable::tableFind(const QString &text)
{
    QModelIndex index = currentIndex();

    proxyModel->setFilterKeyColumn(index.column());
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
        QByteArray encodedData = event->mimeData()->data("text/plain");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        int row = this->indexAt(event->pos()).row();
        int col = this->indexAt(event->pos()).column();
        while (!stream.atEnd()) {
            QString text;
            stream >> text;
            QModelIndex index = model()->index(row, col);
            model()->setData(index, text, Qt::EditRole);
            row++;
        }
        event->acceptProposedAction();
    }
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
    QAction * clearCell = new QAction(("Очистить ячейку"), this);
    QAction * addRow = new QAction(("Добавить строку"), this);
    QAction * deleteRow = new QAction(("Удалить строку"), this);

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

void MdiChildTable::printTable(QTableView* tableView)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));

    // Открываем диалог печати, чтобы пользователь мог выбрать принтер и настройки печати
    QPrintDialog dialog(&printer);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    // Создаем объект QPainter для рисования на принтере
    QPainter painter(&printer);

    // Получаем размеры таблицы и количество строк и столбцов
    int rows = tableView->model()->rowCount();
    int cols = tableView->model()->columnCount();
    int w = tableView->viewport()->width();
    int h = tableView->viewport()->height();

    // Вычисляем масштаб, чтобы таблица занимала всю страницу
    double xscale = double(printer.pageRect().width()) / double(w);
    double yscale = double(printer.pageRect().height()) / double(h);
    double scale = qMin(xscale, yscale);

    // Устанавливаем масштабирование и перенос строк внутри ячеек
    painter.translate(printer.paperRect().x(), printer.paperRect().y());
    painter.scale(scale, scale);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Рисуем таблицу
    tableView->render(&painter);

    // Завершаем рисование
    painter.end();
}

