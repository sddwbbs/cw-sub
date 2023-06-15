#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTranslator>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QtWidgets>
#include <QTextStream>
#include <QApplication>
#include <QTableView>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include <QSettings>
#include <QActionGroup>

#include "mdichild.h"
#include "mdichildTable.h"
#include "about.h"

namespace Ui {
class MainWindow;
}

class MdiChild;
class MdiChildTable;
class QMdiSubWindow;
class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newFile();

    void open();

    void save();

    void saveAs();

    void cut();

    void copy();

    void paste();

    MdiChild *createMdiChild();

    MdiChildTable *createMdiChildTable();

    void on_actionAbout_author_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_actionSaveAs_triggered();

    void switchLanguage(QAction *action);

    void on_actionShow_triggered();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    MdiChild *activeMdiChild();

    MdiChildTable *activeMdiChildTable();

    void updateActions();

    QMdiSubWindow *findMdiChild(const QString &fileName);

    QMdiSubWindow *findMdiChildTable(const QString &fileName);

    void readSettings();

    void writeSettings();

    QTranslator appTranslator;

    QTranslator qtTranslator;

    QActionGroup *languageActionGroup;

    QString qmPath;

    void createLanguageMenu();
};

#endif // MAINWINDOW_HPP
