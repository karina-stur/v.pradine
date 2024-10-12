#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <stdexcept>
#include <limits>
#include <fstream>
#include "funkcijos.h"

int generuotiAtsitiktiniPazymi(int min = 1, int max = 10) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

void generuotiPazymius(Studentas& studentas, int namuDarbuKiekis) {
    for (int i = 0; i < namuDarbuKiekis; ++i) {
        studentas.namuDarbai.push_back(generuotiAtsitiktiniPazymi());
    }
    studentas.egzaminas = generuotiAtsitiktiniPazymi();
}

int readInteger() {
    int value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail() || value < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Klaida: Prasome ivesti teigiama skaiciu." << std::endl;
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    return value;
}

void isaugotiStudentuGrupe(const std::vector<Studentas>& studentai, const std::string& failoPavadinimas, bool pagalVidurki) {
    std::ofstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        std::cerr << "Nepavyko sukurti failo: " << failoPavadinimas << std::endl;
        return;
    }

    failas << std::left << std::setw(15) << "Vardas"
        << std::setw(15) << "Pavarde"
        << std::setw(20) << "Galutinis (Vid.)"
        << std::setw(20) << "Galutinis (Med.)"
        << std::endl;
    failas << "--------------------------------------------------------------" << std::endl;

    for (const auto& studentas : studentai) {
        double galutinisVid = skaiciuotiGalutiniVidurki(studentas.namuDarbai, studentas.egzaminas);
        double galutinisMed = skaiciuotiGalutiniMediana(studentas.namuDarbai, studentas.egzaminas);

        failas << std::left << std::setw(15) << studentas.vardas
            << std::setw(15) << studentas.pavarde
            << std::setw(20) << std::fixed << std::setprecision(2) << galutinisVid
            << std::setw(20) << std::fixed << std::setprecision(2) << galutinisMed
            << std::endl;
    }
}

void grupuotiIrIssaugotiStudentus(const std::vector<Studentas>& studentai, bool pagalVidurki) {
    std::vector<Studentas> nuskriaustukai;
    std::vector<Studentas> kietiakai;

    nuskriaustukai.reserve(studentai.size() / 2);
    kietiakai.reserve(studentai.size() / 2);

    for (const auto& studentas : studentai) {
        double galutinisBalas = pagalVidurki
            ? skaiciuotiGalutiniVidurki(studentas.namuDarbai, studentas.egzaminas)
            : skaiciuotiGalutiniMediana(studentas.namuDarbai, studentas.egzaminas);

        if (galutinisBalas < 5) {
            nuskriaustukai.push_back(studentas);
        }
        else {
            kietiakai.push_back(studentas);
        }
    }

    std::string failoPavadinimasNuskriaustukai = pagalVidurki ? "nuskriaustukai_vidurkis.txt" : "nuskriaustukai_mediana.txt";
    std::string failoPavadinimasKietiakai = pagalVidurki ? "kietiakai_vidurkis.txt" : "kietiakai_mediana.txt";

    isaugotiStudentuGrupe(nuskriaustukai, failoPavadinimasNuskriaustukai, pagalVidurki);
    isaugotiStudentuGrupe(kietiakai, failoPavadinimasKietiakai, pagalVidurki);
}

int main() {
    std::vector<Studentas> studentai;
    studentai.reserve(10000000);

    int pasirinkimas;

    std::cout << "Pasirinkite: 1 - generuoti failus, 2 - nuskaityti is failo: ";
    std::cin >> pasirinkimas;
    std::cin.ignore();

    if (pasirinkimas == 1) {
        generuotiStudentuFailus();
        return 0;
    }
    else if (pasirinkimas == 2) {
        std::string failoPavadinimas;
        std::cout << "Iveskite failo pavadinima: ";
        std::getline(std::cin, failoPavadinimas);

        try {
            nuskaititiIsFailo(failoPavadinimas, studentai);
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Klaida: " << e.what() << std::endl;
            return 1;
        }
    }
    else {
        std::cout << "Neteisingas pasirinkimas." << std::endl;
        return 1;
    }

    surikiuotiStudentus(studentai);

    std::cout << "Pasirinkite rusiavimo buda: 1 - pagal vidurki, 2 - pagal mediana: ";
    int rusiavimoPasirinkimas;
    std::cin >> rusiavimoPasirinkimas;

    bool pagalVidurki = (rusiavimoPasirinkimas == 1);
    grupuotiIrIssaugotiStudentus(studentai, pagalVidurki);

    return 0;
}
