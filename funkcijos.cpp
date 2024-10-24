#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <list>
#include <vector>
#include <sstream>
#include "studentas.h"

template <typename T>
std::string to_string_custom(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void generuotiStudentuFailus() {
    std::vector<int> record_counts = { 1000, 10000, 100000, 1000000, 10000000 };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    for (int count : record_counts) {
        std::string filename = "studentai_" + to_string_custom(count) + ".txt";
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
        for (int i = 1; i <= 15; ++i) file << std::setw(10) << ("ND" + to_string_custom(i));
        file << "Egz" << std::endl;

        for (int i = 1; i <= count; ++i) {
            file << std::left << std::setw(20) << ("Vardas" + to_string_custom(i))
                << std::setw(20) << ("Pavarde" + to_string_custom(i));
            for (int j = 0; j < 15; ++j) file << std::setw(10) << dist(gen);
            file << dist(gen) << std::endl;
        }

        file.close();
        std::cout << "Sukurtas failas: " << filename << " su " << count << " irasu." << std::endl;
    }
}

template <typename Container>
double nuskaitytiStudentus(const std::string& failoPavadinimas, Container& studentai) {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        std::cerr << "Nepavyko atidaryti failo: " << failoPavadinimas << std::endl;
        return 0.0;
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

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
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

template <typename Container>
double isaugotiStudentuGrupe(const Container& studentai, const std::string& failoPavadinimas, bool pagalVidurki) {
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

template <typename Container>
double grupuotiIrIssaugotiStudentus(Container& studentai, bool pagalVidurki, double& partitionTime, double& nuskriaustukaiSaveTime, double& kietiakaiSaveTime) {
    auto startPartition = std::chrono::high_resolution_clock::now();

    auto it = std::stable_partition(studentai.begin(), studentai.end(),
        [pagalVidurki](const Studentas& studentas) {
            double galutinis = pagalVidurki ? skaiciuotiGalutiniVidurki(studentas.namuDarbai, studentas.egzaminas)
                : skaiciuotiGalutiniMediana(studentas.namuDarbai, studentas.egzaminas);
            return galutinis < 5;
        });

    Container nuskriaustukai(studentai.begin(), it);

    Container kietiakai(it, studentai.end());

    auto endPartition = std::chrono::high_resolution_clock::now();
    partitionTime = std::chrono::duration<double>(endPartition - startPartition).count();

    nuskriaustukaiSaveTime = isaugotiStudentuGrupe(nuskriaustukai, "nuskriaustukai.txt", pagalVidurki);
    kietiakaiSaveTime = isaugotiStudentuGrupe(kietiakai, "kietiakai.txt", pagalVidurki);

    return partitionTime + nuskriaustukaiSaveTime + kietiakaiSaveTime;
}

void testKonteinerius(const std::string& failoPavadinimas, bool pagalVidurki) {
    std::vector<Studentas> studentaiVektorius;
    std::list<Studentas> studentaiSarasas;

    double partitionTime, nuskriaustukaiSaveTime, kietiakaiSaveTime, totalTime;

    std::cout << "Testuojama su std::vector<Studentas>..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    double readTimeVec = nuskaitytiStudentus(failoPavadinimas, studentaiVektorius);
    totalTime = grupuotiIrIssaugotiStudentus(studentaiVektorius, pagalVidurki, partitionTime, nuskriaustukaiSaveTime, kietiakaiSaveTime);
    auto end = std::chrono::high_resolution_clock::now();

    double totalVecTime = std::chrono::duration<double>(end - start).count();
    std::cout << "std::vector<Studentas> skaitymas uztruko: " << readTimeVec << " sekundziu." << std::endl;
    std::cout << "std::vector<Studentas> grupavimas uztruko: " << partitionTime << " sekundziu." << std::endl;
    std::cout << "std::vector<Studentas> nuskriaustukai isaugoti per: " << nuskriaustukaiSaveTime << " sekundziu." << std::endl;
    std::cout << "std::vector<Studentas> kietiakai isaugoti per: " << kietiakaiSaveTime << " sekundziu." << std::endl;
    std::cout << "std::vector<Studentas> bendrai uztruko: " << totalVecTime << " sekundziu." << std::endl;

    std::cout << "Testuojama su std::list<Studentas>..." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    double readTimeList = nuskaitytiStudentus(failoPavadinimas, studentaiSarasas);
    totalTime = grupuotiIrIssaugotiStudentus(studentaiSarasas, pagalVidurki, partitionTime, nuskriaustukaiSaveTime, kietiakaiSaveTime);
    end = std::chrono::high_resolution_clock::now();

    double totalListTime = std::chrono::duration<double>(end - start).count();
    std::cout << "std::list<Studentas> skaitymas uztruko: " << readTimeList << " sekundziu." << std::endl;
    std::cout << "std::list<Studentas> grupavimas uztruko: " << partitionTime << " sekundziu." << std::endl;
    std::cout << "std::list<Studentas> nuskriaustukai isaugoti per: " << nuskriaustukaiSaveTime << " sekundziu." << std::endl;
    std::cout << "std::list<Studentas> kietiakai isaugoti per: " << kietiakaiSaveTime << " sekundziu." << std::endl;
    std::cout << "std::list<Studentas> bendrai uztruko: " << totalListTime << " sekundziu." << std::endl;
}
