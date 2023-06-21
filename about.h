#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class About;
}

/**
 * @brief Класс About представляет диалоговое окно "О программе".
 *
 * Он наследуется от класса QDialog.
 *
 * @note Важно подключить макрос Q_OBJECT для использования механизма сигналов и слотов в Qt.
 */
class About : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса About.
     *
     * @param parent Указатель на родительское виджет-окно (по умолчанию nullptr).
     */
    explicit About(QWidget *parent = nullptr);

    /**
     * @brief Виртуальный деструктор класса About.
     */
    ~About();

private slots:
    /**
     * @brief Слот, вызываемый при нажатии кнопки pushButton.
     */
    void on_pushButton_clicked();

private:
    Ui::About *ui; /**< Указатель на пользовательский интерфейс класса About. */
};


#endif // ABOUT_H
