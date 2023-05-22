#ifndef EMPDIAGRAM_H
#define EMPDIAGRAM_H

#include <QMainWindow>
#include <SubcontractsClass.h>

namespace Ui {
class EmpDiagram;
}

class EmpDiagram : public QMainWindow
{
    Q_OBJECT

public:
    explicit EmpDiagram(QWidget *parent = nullptr, QList<Subcontracts> values = QList<Subcontracts>());
    ~EmpDiagram();

    void createDiagram(QList<Subcontracts>);

private:
    Ui::EmpDiagram *ui;
};

#endif // EMPDIAGRAM_H
