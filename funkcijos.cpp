#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include "funkcijos.h"

void generuotiStudentuFailus() {
    std::vector<int> record_counts = { 1000, 10000, 100000, 1000000, 10000000 };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    for (int count : record_counts) {
        std::string filename = "studentai_" + std::to_string(count) + ".txt";
        std::ifstream infile(filename);
        if (infile.good()) {
            std::cout << "Failas " << filename << " jau egzistuoja. Jo sugeneruoti nereikia." << std::endl;
            continue;
        }
        infile.close();

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Nepavyko sukurti failo: " << filename << std::endl;
            continue;
        }

        file << std::left << std::setw(20) << "Vardas" << std::setw(20) << "Pavarde";
        for (int i = 1; i <= 15; ++i) file << std::setw(10) << ("ND" + std::to_string(i));
        file << "Egz" << std::endl;

        for (int i = 1; i <= count; ++i) {
            file << std::left << std::setw(20) << ("Vardas" + std::to_string(i))
                << std::setw(20) << ("Pavarde" + std::to_string(i));
            for (int j = 0; j < 15; ++j) file << std::setw(10) << dist(gen);
            file << dist(gen) << std::endl;
        }

        file.close();
        std::cout << "Sukurtas failas: " << filename << " su " << count << " irasu." << std::endl;
    }
}

void nuskaitytiStudentus(const std::string& failoPavadinimas, std::vector<Studentas>& studentai) {
    std::ifstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        std::cerr << "Nepavyko atidaryti failo: " << failoPavadinimas << std::endl;
        return;
    }

    std::string header;
    std::getline(failas, header);
    while (failas.good()) {
        Studentas studentas;
        failas >> studentas.vardas >> studentas.pavarde;

        studentas.namuDarbai.resize(15);
        for (int& nd : studentas.namuDarbai) failas >> nd;
        failas >> studentas.egzaminas;

        studentai.push_back(studentas);
    }
    failas.close();
}

double skaiciuotiGalutiniVidurki(const std::vector<int>& namuDarbai, int egzaminas) {
    double suma = std::accumulate(namuDarbai.begin(), namuDarbai.end(), 0.0);
    return (suma + egzaminas) / (namuDarbai.size() + 1);
}

double skaiciuotiGalutiniMediana(const std::vector<int>& namuDarbai, int egzaminas) {
    std::vector<double> viskas(namuDarbai.begin(), namuDarbai.end());
    viskas.push_back(static_cast<double>(egzaminas));

    std::sort(viskas.begin(), viskas.end());

    size_t size = viskas.size();
    if (size % 2 == 0) {
        return (viskas[size / 2 - 1] + viskas[size / 2]) / 2.0;
    }
    else {
        return viskas[size / 2];
    }
}

double isaugotiStudentuGrupe(const std::vector<Studentas>& studentai, const std::string& failoPavadinimas, bool pagalVidurki) {
    auto start = std::chrono::high_resolution_clock::now();

    std::ofstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        std::cerr << "Nepavyko sukurti failo: " << failoPavadinimas << std::endl;
        return 0.0;
    }

    failas << std::left << std::setw(20) << "Vardas" << std::setw(20) << "Pavarde"
        << std::setw(20) << "Galutinis (Vid.)" << "Galutinis (Med.)" << std::endl;
    failas << std::string(80, '-') << std::endl;

    for (const auto& studentas : studentai) {
        double galutinisVid = skaiciuotiGalutiniVidurki(studentas.namuDarbai, studentas.egzaminas);
        double galutinisMed = skaiciuotiGalutiniMediana(studentas.namuDarbai, studentas.egzaminas);

        failas << std::left << std::setw(20) << studentas.vardas
            << std::setw(20) << studentas.pavarde
            << std::setw(20) << galutinisVid
            << galutinisMed << std::endl;
    }

    failas.close();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

void grupuotiIrIssaugotiStudentus(std::vector<Studentas>& studentai, bool pagalVidurki) {
    std::vector<Studentas>::iterator it = std::stable_partition(studentai.begin(), studentai.end(),
        [pagalVidurki](const Studentas& studentas) {
            double galutinis = pagalVidurki ? skaiciuotiGalutiniVidurki(studentas.namuDarbai, studentas.egzaminas)
                : skaiciuotiGalutiniMediana(studentas.namuDarbai, studentas.egzaminas);
            return galutinis < 5;
        });

    std::vector<Studentas> nuskriaustukai(studentai.begin(), it);
    std::vector<Studentas> kietiakai(it, studentai.end());

    double timeNuskriaustukai = isaugotiStudentuGrupe(nuskriaustukai, "nuskriaustukai.txt", pagalVidurki);
    double timeKietiakai = isaugotiStudentuGrupe(kietiakai, "kietiakai.txt", pagalVidurki);

    std::cout << "Nuskriaustukai failo rasymas truko: " << timeNuskriaustukai << " sekundziu." << std::endl;
    std::cout << "Kietekai failo rasymas truko: " << timeKietiakai << " sekundziu." << std::endl;
}
