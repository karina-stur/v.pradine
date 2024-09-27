#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <stdexcept>
#include <limits>
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

int main() {
    std::vector<Studentas> studentai;
    int pasirinkimas;

    std::cout << "Pasirinkite: 1 - ivesti duomenis, 2 - nuskaityti is failo: ";
    std::cin >> pasirinkimas;
    std::cin.ignore();

    if (pasirinkimas == 1) {
        int kiekMokiniu;
        std::cout << "Kiek studentu norite ivesti? ";
        kiekMokiniu = readInteger();

        for (int i = 0; i < kiekMokiniu; ++i) {
            Studentas studentas;
            std::cout << "Iveskite " << i + 1 << "-ojo studento varda: ";
            std::getline(std::cin, studentas.vardas);
            std::cout << "Iveskite " << i + 1 << "-ojo studento pavarde: ";
            std::getline(std::cin, studentas.pavarde);

            int ndKiekis;
            std::cout << "Kiek namu darbu pazymiu norite sugeneruoti? ";
            ndKiekis = readInteger();

            generuotiPazymius(studentas, ndKiekis);
            studentai.push_back(studentas);
        }
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

    std::cout << std::left << std::setw(15) << "Vardas"
        << std::setw(15) << "Pavarde"
        << std::setw(20) << "Galutinis (Vid.)"
        << std::setw(20) << "Galutinis (Med.)"
        << std::endl;

    std::cout << "--------------------------------------------------------------" << std::endl;

    for (const auto& studentas : studentai) {
        double galutinisVid = skaiciuotiGalutiniVidurki(studentas.namuDarbai, studentas.egzaminas);
        double galutinisMed = skaiciuotiGalutiniMediana(studentas.namuDarbai, studentas.egzaminas);

        std::cout << std::left << std::setw(15) << studentas.vardas
            << std::setw(15) << studentas.pavarde
            << std::setw(20) << std::fixed << std::setprecision(2) << galutinisVid
            << std::setw(20) << std::fixed << std::setprecision(2) << galutinisMed
            << std::endl;
    }

    return 0;
}
