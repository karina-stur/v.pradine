#ifndef FUNKCIJOS_H
#define FUNKCIJOS_H

#include <vector>
#include <list>
#include <string>
#include "studentas.h"

void generuotiStudentuFailus();

template <typename Container>
void nuskaitytiStudentus(const std::string& failoPavadinimas, Container& studentai);

double skaiciuotiGalutiniVidurki(const std::vector<int>& namuDarbai, int egzaminas);
double skaiciuotiGalutiniMediana(const std::vector<int>& namuDarbai, int egzaminas);

template <typename Container>
double isaugotiStudentuGrupe(const Container& studentai, const std::string& failoPavadinimas, bool pagalVidurki);

template <typename Container>
void grupuotiIrIssaugotiStudentus(Container& studentai, bool pagalVidurki);

void testKonteinerius(const std::string& failoPavadinimas, bool pagalVidurki);


#endif // FUNKCIJOS_H
