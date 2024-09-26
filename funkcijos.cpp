#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include "funkcijos.h"

void nuskaititiIsFailo(const std::string& failoPavadinimas, std::vector<Studentas>& studentai) {
    std::ifstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        throw std::runtime_error("Nepavyko atidaryti failo: " + failoPavadinimas);
    }

    std::string header;
    std::getline(failas, header); // Read the first line (header)

    Studentas studentas;
    std::string eilute;

    while (std::getline(failas, eilute)) {
        std::istringstream iss(eilute);
        iss >> studentas.vardas >> studentas.pavarde;

        studentas.namuDarbai.clear();
        int nd;
        while (iss >> nd) {
            studentas.namuDarbai.push_back(nd);
        }

        if (studentas.namuDarbai.empty()) {
            throw std::runtime_error("N?ra nam? darb? duomen? studentui: " + studentas.vardas + " " + studentas.pavarde);
        }

        studentas.egzaminas = studentas.namuDarbai.back();
        studentas.namuDarbai.pop_back();

        studentai.push_back(studentas);
    }
}

double skaiciuotiGalutiniVidurki(const std::vector<int>& namuDarbai, int egzaminas) {
    if (namuDarbai.empty()) {
        throw std::runtime_error("Namu darb? s?ra�as negali b?ti tu�?ias.");
    }

    double vidurkis = 0;
    for (int nd : namuDarbai) {
        vidurkis += nd;
    }
    vidurkis /= namuDarbai.size();
    return 0.5 * vidurkis + 0.5 * egzaminas;
}

double skaiciuotiGalutiniMediana(std::vector<int> namuDarbai, int egzaminas) {
    if (namuDarbai.empty()) {
        throw std::runtime_error("Namu darb? s?ra�as negali b?ti tu�?ias.");
    }

    std::sort(namuDarbai.begin(), namuDarbai.end());

    double mediana;
    int size = namuDarbai.size();
    if (size % 2 == 0) {
        mediana = (namuDarbai[size / 2 - 1] + namuDarbai[size / 2]) / 2.0;
    }
    else {
        mediana = namuDarbai[size / 2];
    }
    return 0.5 * mediana + 0.5 * egzaminas;
}

void surikiuotiStudentus(std::vector<Studentas>& studentai) {
    std::sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
        return a.pavarde < b.pavarde; 
        });
}
