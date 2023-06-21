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
/**
 * @brief Класс MdiChild представляет дочернее окно в MDI-интерфейсе для редактирования текста.
 *
 * Он наследуется от класса QTextEdit.
 */
class MdiChild : public QTextEdit
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса MdiChild.
     *
     * @param parent Указатель на родительский виджет (по умолчанию nullptr).
     */
    explicit MdiChild(QWidget *parent = nullptr);

    // Методы реализующие работу с файлами

    /**
     * @brief Создает новый файл.
     */
    void newFile();

    /**
     * @brief Загружает файл по указанному имени.
     *
     * @param fileName Имя файла для загрузки.
     * @return true, если загрузка прошла успешно, иначе false.
     */
    bool loadFile(const QString &fileName);

    /**
     * @brief Сохраняет текущий файл.
     *
     * @return true, если сохранение прошло успешно, иначе false.
     */
    bool save();

    /**
     * @brief Сохраняет текущий файл с указанием пути.
     *
     * @return true, если сохранение прошло успешно, иначе false.
     */
    bool saveAs();

    /**
     * @brief Сохраняет файл по указанному имени.
     *
     * @param fileName Имя файла для сохранения.
     * @return true, если сохранение прошло успешно, иначе false.
     */
    bool saveFile(const QString &fileName);

    /**
     * @brief Возвращает текущий файл.
     *
     * @return Имя текущего файла.
     */
    QString currentFile() { return curFile; }

protected:
    /**
     * @brief Обработчик события закрытия окна.
     *
     * @param event Событие закрытия окна.
     */
    void closeEvent(QCloseEvent *event) override;

private slots:
    /**
     * @brief Проверяет, было ли редактирование текста.
     */
    void documentWasModified();

private:
    /**
     * @brief Проверяет, нужно ли сохранить документ.
     *
     * @return true, если необходимо сохранить документ, иначе false.
     */
    bool maybeSave();

    /**
     * @brief Устанавливает текущий файл.
     *
     * @param fileName Имя файла.
     */
    void setCurrentFile(const QString &fileName);

    /**
     * @brief Возвращает имя файла без пути.
     *
     * @return Имя файла без пути.
     */
    QString userFriendlyCurrentFile();

    QString curFile; /**< Имя текущего файла. */
    bool isUntitled; /**< Флаг, указывающий, является ли файл безымянным. */
};


#endif // MDICHILD_H

