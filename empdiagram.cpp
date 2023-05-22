#include "empdiagram.h"
#include "ui_empdiagram.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QDesktopWidget>
#include <QMessageBox>
//#include <map>
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

    setWindowTitle(tr("Диаграмма количества работников"));

    createDiagram(values);
}

EmpDiagram::~EmpDiagram()
{
    delete ui;
}

void EmpDiagram::createDiagram(QList<Subcontracts> values) {
    std::vector<int> employees(5, 0);

    foreach (auto elem, values) {
        int temp = elem.getNumberEmpl();

        if (temp >= 0 && temp <= 10) {
            employees[0]++;
            break;
        }

        if (temp > 10 && temp <= 20) {
            employees[1]++;
            break;
        }

        if (temp > 20 && temp <= 30) {
            employees[2]++;
            break;
        }

        if (temp > 30 && temp <= 40) {
            employees[3]++;
            break;
        }

        employees[4]++;
    }

    QPieSeries *series = new QPieSeries();
    series->append(tr("от 0 до 10 работников"), (employees[0] / values.length()) * 100);
    series->append(tr("от 10 до 20 работников"), (employees[1] / values.length()) * 100);
    series->append(tr("от 20 до 30 работников"), (employees[2] / values.length()) * 100);
    series->append(tr("от 30 до 40 работников"), (employees[3] / values.length()) * 100);
    series->append(tr("от 40 и более"), (employees[4] / values.length()) * 100);

    // Создаем объект графика
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Количество работников у субподрядчиков"));

    // Создаем объект представления графика
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Создаем объект главного окна и добавляем в него представление графика
    this->setCentralWidget(chartView);
    this->resize(500, 400);
}
