<<<<<<< HEAD
#ifndef SUBCONTRACTSCLASS_H
#define SUBCONTRACTSCLASS_H
=======
#ifndef SUBCONTRACTS_H
#define SUBCONTRACTS_H
>>>>>>> c5299f1 (Рефакторинг кода)

#include <QString>

using namespace std;

class Subcontracts {
private:
    QString name;
    int numberEmpl;
    int workload;
    QString location;
    int additionalServ;
    long price;
    int experience;
    int completedProjects;
    float rating;

public:
    Subcontracts();
    Subcontracts(const QString &, int, int, const QString &, int,
                 int, int, int, float);

    void setName(const QString &n);
    void setNumberEmpl(int num);
    void setWorkload(int w);
    void setLocation(const QString &l);
    void setAdditionalServ(int a);
    void setPrice(int price);
    void setExperience(int e);
    void setCompletedProjects(int c);
    void setRating(float r);

    QString getName() const;
    int getNumberEmpl() const;
    int getWorkload() const;
    QString getLocation() const;
    int getAdditionalServ() const;
    int getPrice() const;
    int getExperience() const;
    int getCompletedProjects() const;
    float getRating() const;
};

<<<<<<< HEAD

#endif // SUBCONTRACTSCLASS_H
=======
#endif // SUBCONTRACTS_H
>>>>>>> c5299f1 (Рефакторинг кода)
