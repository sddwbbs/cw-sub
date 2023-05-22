#include "diagram.h"
#include "ui_diagram.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QDesktopWidget>
#include <QMessageBox>
#include <map>

using namespace QtCharts;

AddServDiagram::AddServDiagram(QWidget *parent, QList<Subcontracts> values) :
    QMainWindow(parent),
    ui(new Ui::AddServDiagram)
{
    ui->setupUi(this);

    QRect availableGeometry = QApplication::desktop()->availableGeometry(this);

    int x = (availableGeometry.width() - this->width()) / 2;
    int y = (availableGeometry.height() - this->height()) / 2;
    this->move(x, y);

    setWindowTitle(tr("График соотношения дополнительных услуг"));

    createDiagram(values);
}

AddServDiagram::~AddServDiagram()
{
    delete ui;
}

void AddServDiagram::createDiagram(QList<Subcontracts> values)
{

    std::map<bool, float> freq;

//    foreach (auto el, values) {
//        if (el.getWarehouse())
//            freq[true]++;
//        else
//            freq[false]++;
//    }

    QPieSeries *series = new QPieSeries();
    series->append(tr("Имеют склад"), (freq[true] / values.length()) * 100);
    series->append(tr("Не имеют склад"), (freq[false] / values.length()) * 100);

    // Создаем объект графика
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Наличие складов у фирм"));

    // Создаем объект представления графика
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Создаем объект главного окна и добавляем в него представление графика
    this->setCentralWidget(chartView);
    this->resize(500, 400);
}
