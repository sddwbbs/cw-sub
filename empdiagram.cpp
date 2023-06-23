#include "empdiagram.h"
#include "ui_empdiagram.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QDesktopWidget>
#include <QMessageBox>
#include <vector>
#include <QVBoxLayout>

using namespace QtCharts;

EmpDiagram::EmpDiagram(QWidget *parent, QList<Subcontracts> values) :
    QMainWindow(parent),
    ui(new Ui::EmpDiagram)
{
    ui->setupUi(this);

    QRect availableGeometry = QApplication::desktop()->availableGeometry(this);

    int x = (availableGeometry.width() - this->width()) / 2;
    int y = (availableGeometry.height() - this->height()) / 2;
    this->move(x, y);

    setWindowTitle(tr("Diagram"));

    createDiagram(values);
}

EmpDiagram::~EmpDiagram()
{
    delete ui;
}

void EmpDiagram::createDiagram(QList<Subcontracts> values) {
    std::vector<int> employees(5, 0);

    for (const auto &elem : values) {
        int temp = elem.getNumberEmpl();

        if (temp >= 0 && temp <= 10) {
            employees[0]++;
        } else if (temp > 10 && temp <= 20) {
            employees[1]++;
        } else if (temp > 20 && temp <= 30) {
            employees[2]++;
        } else if (temp > 30 && temp <= 40) {
            employees[3]++;
        } else {
            employees[4]++;
        }
    }

    QPieSeries *series = new QPieSeries();
    series->append(tr("from 0 to 10 employees"), (employees[0] / static_cast<double>(values.length())) * 100.0);
    series->append(tr("from 10 to 20 employees"), (employees[1] / static_cast<double>(values.length())) * 100.0);
    series->append(tr("from 20 to 30 employees"), (employees[2] / static_cast<double>(values.length())) * 100.0);
    series->append(tr("from 30 to 40 employees"), (employees[3] / static_cast<double>(values.length())) * 100.0);
    series->append(tr("from 40 and more"), (employees[4] / static_cast<double>(values.length())) * 100.0);

    // Создаем объект графика
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Number of employees of subcontractors"));

    // Создаем объект представления графика
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Создаем объект главного окна и добавляем в него представление графика
    this->setCentralWidget(chartView);
    this->resize(800, 600);
}
