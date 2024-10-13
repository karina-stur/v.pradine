#ifndef FUNKCIJOS_H
#define FUNKCIJOS_H

#include <vector>
#include <string>
#include "studentas.h"

void generuotiStudentuFailus();
void nuskaitytiStudentus(const std::string& failoPavadinimas, std::vector<Studentas>& studentai);
double skaiciuotiGalutiniVidurki(const std::vector<int>& namuDarbai, int egzaminas);
double skaiciuotiGalutiniMediana(const std::vector<int>& namuDarbai, int egzaminas);
double isaugotiStudentuGrupe(const std::vector<Studentas>& studentai, const std::string& failoPavadinimas, bool pagalVidurki);
void grupuotiIrIssaugotiStudentus(std::vector<Studentas>& studentai, bool pagalVidurki);

#endif // FUNKCIJOS_H
