#ifndef MDICHILD_H
#define MDICHILD_H

#include <QString>
#include <QCloseEvent>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMdiSubWindow>

// Воспользуемся классом для работы с текстом, но добавим несколько своих
// методов, которые реализуют дополнительный возможности нужные нам.
// На основе этого можно создать окно текстового редактора
class MdiChild : public QTextEdit
{
    Q_OBJECT

public:
    explicit MdiChild(QWidget *parent = nullptr);

    // Методы реализующие работу с файлами
    void newFile();

    bool loadFile(const QString &fileName);

    bool save();

    bool saveAs();

    bool saveFile(const QString &fileName);

    QString currentFile() { return curFile; }

protected:
    // Для вопроса о сохранении файла при закрытии дочернего окна,
    // если файл был изменён
    void closeEvent(QCloseEvent *event);

private slots:
    // Метод проверяет было ли редактирование текста
    void documentWasModified();

private:
    // Проверка нужно ли сохранить документ
    bool maybeSave();
    // Метод делает имя файла как текущий файл
    void setCurrentFile(const QString &fileName);
    // Получить имя файла отдельно от пути
    QString userFriendlyCurrentFile();

    // Переменная хранит имя файла
    QString curFile;
    // С помощью этой переменной можно определить
    // имеется ли имя у файла или он использует имя
    // по умолчанию "untitled%1.txt"
    bool isUntitled;
};

#endif // MDICHILD_H

