#ifndef SUBCONTRACTSCLASS_H
#define SUBCONTRACTSCLASS_H

#include <QString>

using namespace std;

class Subcontracts {
private:
    int id;
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
    Subcontracts(int, const QString &, int, int, const QString &, int,
                 int, int, int, float);

    void setId(int id);
    void setName(const QString &n);
    void setNumberEmpl(int num);
    void setWorkload(int w);
    void setLocation(const QString &l);
    void setAdditionalServ(int a);
    void setPrice(int price);
    void setExperience(int e);
    void setCompletedProjects(int c);
    void setRating(float r);

    int getId() const;
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


#endif // SUBCONTRACTSCLASS_H
