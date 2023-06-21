#ifndef EMPDIAGRAM_H
#define EMPDIAGRAM_H

#include <QMainWindow>
#include <SubcontractsClass.h>

namespace Ui {
class EmpDiagram;
}

/**
 * @brief Класс EmpDiagram представляет окно приложения для отображения диаграммы сотрудников.
 *
 * Он наследуется от класса QMainWindow.
 */
class EmpDiagram : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса EmpDiagram.
     *
     * @param parent Указатель на родительский виджет (по умолчанию nullptr).
     * @param values Список подрядчиков (по умолчанию пустой список).
     */
    explicit EmpDiagram(QWidget *parent = nullptr, QList<Subcontracts> values = QList<Subcontracts>());

    /**
     * @brief Виртуальный деструктор класса EmpDiagram.
     */
    ~EmpDiagram();

    /**
     * @brief Создает диаграмму сотрудников.
     *
     * @param values Список подрядчиков для отображения на диаграмме.
     */
    void createDiagram(QList<Subcontracts> values);

private:
    Ui::EmpDiagram *ui; /**< Указатель на пользовательский интерфейс класса EmpDiagram. */
};

#endif // EMPDIAGRAM_H
