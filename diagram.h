#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QMainWindow>
#include <SubcontractsClass.h>

namespace Ui {
class AddServDiagram;
}

class AddServDiagram : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddServDiagram(QWidget *parent = nullptr, QList<Subcontracts> values = QList<Subcontracts>());
    ~AddServDiagram();

    void createDiagram(QList<Subcontracts>);

private:
    Ui::AddServDiagram *ui;
};


#endif // DIAGRAM_H

