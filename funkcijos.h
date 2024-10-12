#ifndef FUNKCIJOS_H
#define FUNKCIJOS_H

#include <vector>
#include <string>
#include "studentas.h"

void nuskaititiIsFailo(const std::string& failoPavadinimas, std::vector<Studentas>& studentai);
double skaiciuotiGalutiniVidurki(const std::vector<int>& namuDarbai, int egzaminas);
double skaiciuotiGalutiniMediana(std::vector<int> namuDarbai, int egzaminas);
void surikiuotiStudentus(std::vector<Studentas>& studentai);
void generuotiStudentuFailus();

#endif // FUNKCIJOS_H

