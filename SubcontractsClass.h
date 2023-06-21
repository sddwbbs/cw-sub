#ifndef SUBCONTRACTSCLASS_H
#define SUBCONTRACTSCLASS_H

#include <QString>

using namespace std;

/**
 * @brief Класс "Subcontracts" представляет подряды.
 *
 * Класс содержит информацию о субподрячике, включая идентификатор, имя, количество сотрудников,
 * загрузку работы, местоположение, дополнительные услуги, цену, опыт, количество завершенных
 * проектов и рейтинг.
 */
class Subcontracts {
private:
    int id; /**< Идентификатор подряда. */
    QString name; /**< Название подряда. */
    int numberEmpl; /**< Количество сотрудников. */
    int workload; /**< Загрузка работы. */
    QString location; /**< Местоположение. */
    int additionalServ; /**< Дополнительные услуги. */
    long price; /**< Цена. */
    int experience; /**< Опыт. */
    int completedProjects; /**< Количество завершенных проектов. */
    float rating; /**< Рейтинг. */

public:
    /**
     * @brief Конструктор по умолчанию.
     */
    Subcontracts();

    /**
     * @brief Конструктор с параметрами.
     *
     * @param id Идентификатор подряда.
     * @param name Название подряда.
     * @param numberEmpl Количество сотрудников.
     * @param workload Загрузка работы.
     * @param location Местоположение.
     * @param additionalServ Дополнительные услуги.
     * @param price Цена.
     * @param experience Опыт.
     * @param completedProjects Количество завершенных проектов.
     * @param rating Рейтинг.
     */
    Subcontracts(int id, const QString &name, int numberEmpl, int workload, const QString &location,
                 int additionalServ, int price, int experience, int completedProjects, float rating);

    /**
     * @brief Устанавливает идентификатор подряда.
     *
     * @param id Идентификатор подряда.
     */
    void setId(int id);

    /**
     * @brief Устанавливает название подряда.
     *
     * @param n Название подряда.
     */
    void setName(const QString &n);

    /**
     * @brief Устанавливает количество сотрудников.
     *
     * @param num Количество сотрудников.
     */
    void setNumberEmpl(int num);

    /**
     * @brief Устанавливает загрузку работы.
     *
     * @param w Загрузка работы.
     */
    void setWorkload(int w);

    /**
     * @brief Устанавливает местоположение.
     *
     * @param l Местоположение.
     */
    void setLocation(const QString &l);

    /**
     * @brief Устанавливает дополнительные услуги.
     *
     * @param a Дополнительные услуги.
     */
    void setAdditionalServ(int a);

    /**
     * @brief Устанавливает цену.
     *
     * @param price Цена.
     */
    void setPrice(int price);

    /**
     * @brief Устанавливает опыт.
     *
     * @param e Опыт.
     */
    void setExperience(int e);

    /**
     * @brief Устанавливает количество завершенных проектов.
     *
     * @param c Количество завершенных проектов.
     */
    void setCompletedProjects(int c);

    /**
     * @brief Устанавливает рейтинг.
     *
     * @param r Рейтинг.
     */
    void setRating(float r);

    /**
     * @brief Возвращает идентификатор подряда.
     *
     * @return Идентификатор подряда.
     */
    int getId() const;

    /**
     * @brief Возвращает название подряда.
     *
     * @return Название подряда.
     */
    QString getName() const;

    /**
     * @brief Возвращает количество сотрудников.
     *
     * @return Количество сотрудников.
     */
    int getNumberEmpl() const;

    /**
     * @brief Возвращает загрузку работы.
     *
     * @return Загрузка работы.
     */
    int getWorkload() const;

    /**
     * @brief Возвращает местоположение.
     *
     * @return Местоположение.
     */
    QString getLocation() const;

    /**
     * @brief Возвращает дополнительные услуги.
     *
     * @return Дополнительные услуги.
     */
    int getAdditionalServ() const;

    /**
     * @brief Возвращает цену.
     *
     * @return Цена.
     */
    int getPrice() const;

    /**
     * @brief Возвращает опыт.
     *
     * @return Опыт.
     */
    int getExperience() const;

    /**
     * @brief Возвращает количество завершенных проектов.
     *
     * @return Количество завершенных проектов.
     */
    int getCompletedProjects() const;

    /**
     * @brief Возвращает рейтинг.
     *
     * @return Рейтинг.
     */
    float getRating() const;
};



#endif // SUBCONTRACTSCLASS_H
