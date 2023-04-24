#include "SubcontractsClass.h"

Subcontracts::Subcontracts() {};

Subcontracts::Subcontracts(const QString &n, int num, int w, const QString &l, int a,
                           int p, int e, int c, float r) {
    this->name = n;
    this->numberEmpl = num;
    this->workload = w;
    this->location = l;
    this->additionalServ = a;
    this->price = p;
    this->experience = e;
    this->completedProjects = c;
    this->rating = r;
}

void Subcontracts::setName(const QString &n) { this->name = n; }
void Subcontracts::setNumberEmpl(int num) { this->numberEmpl = num; }
void Subcontracts::setWorkload(int w) { this->workload = w; }
void Subcontracts::setLocation(const QString &l) { this->location = l; }
void Subcontracts::setAdditionalServ(int a) { this->additionalServ = a; }
void Subcontracts::setPrice(int p) { this->price = p; }
void Subcontracts::setExperience(int e) { this->experience = e; }
void Subcontracts::setCompletedProjects(int c) { this->completedProjects = c; }
void Subcontracts::setRating(float r) { this->rating = r; }

QString Subcontracts::getName() const { return this->name; }
int Subcontracts::getNumberEmpl() const { return this->numberEmpl; }
int Subcontracts::getWorkload() const { return this->workload; }
QString Subcontracts::getLocation() const { return this->location; }
int Subcontracts::getAdditionalServ() const { return this->additionalServ; }
int Subcontracts::getPrice() const { return this->price; }
int Subcontracts::getExperience() const { return this->experience; }
int Subcontracts::getCompletedProjects() const { return this->completedProjects; }
float Subcontracts::getRating() const { return this->rating; }
