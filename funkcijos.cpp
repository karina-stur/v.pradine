#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include "funkcijos.h"

void nuskaititiIsFailo(const std::string& failoPavadinimas, std::vector<Studentas>& studentai) {
    std::ifstream failas(failoPavadinimas, std::ios::in | std::ios::binary);
    if (!failas.is_open()) {
        throw std::runtime_error("Nepavyko atidaryti failo: " + failoPavadinimas);
    }

    std::string header;
    std::getline(failas, header);

    studentai.reserve(10000000);

    const size_t bufferSize = 32 * 1024 * 1024;
    std::vector<char> buffer(bufferSize);

    std::vector<int> pazymiai;
    pazymiai.reserve(100);

    while (failas.read(buffer.data(), buffer.size()) || failas.gcount()) {
        char* lineStart = buffer.data();
        char* lineEnd = lineStart;

        while ((lineEnd = std::strchr(lineStart, '\n'))) {
            *lineEnd = '\0';

            Studentas studentas;
            pazymiai.clear();

            char* context = nullptr;
            char* token = strtok_s(lineStart, " ", &context);
            if (token) studentas.vardas = token;

            token = strtok_s(nullptr, " ", &context);
            if (token) studentas.pavarde = token;

            int nd;
            while ((token = strtok_s(nullptr, " ", &context)) != nullptr) {
                nd = std::stoi(token);
                pazymiai.push_back(nd);
            }

            if (!pazymiai.empty()) {
                studentas.egzaminas = pazymiai.back();
                pazymiai.pop_back();
            }
            else {
                throw std::runtime_error("Nera namu darbu duomenu studentui: " + studentas.vardas + " " + studentas.pavarde);
            }

            studentas.namuDarbai = pazymiai;
            studentai.push_back(std::move(studentas));

            lineStart = lineEnd + 1;
        }
    }
}

double skaiciuotiGalutiniVidurki(const std::vector<int>& namuDarbai, int egzaminas) {
    if (namuDarbai.empty()) {
        throw std::runtime_error("Namu darbu sarasas negali buti tuscias.");
    }

    double vidurkis = 0;
    for (int nd : namuDarbai) {
        vidurkis += nd;
    }
    vidurkis /= namuDarbai.size();
    return 0.4 * vidurkis + 0.6 * egzaminas;
}

double skaiciuotiGalutiniMediana(std::vector<int> namuDarbai, int egzaminas) {
    if (namuDarbai.empty()) {
        throw std::runtime_error("Namu darbu sarasas negali buti tuscias.");
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
    return 0.4 * mediana + 0.6 * egzaminas;
}

void surikiuotiStudentus(std::vector<Studentas>& studentai) {
    std::sort(studentai.begin(), studentai.end(), [](const Studentas& a, const Studentas& b) {
        return a.pavarde < b.pavarde;
        });
}
