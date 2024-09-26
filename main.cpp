#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <stdexcept>
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

int main() {
    std::vector<Studentas> studentai;
    int pasirinkimas;

    try {
        std::cout << "Pasirinkite: 1 - Įvesti duomenis, 2 - nuskaityti iš failo: ";
        std::cin >> pasirinkimas;
        std::cin.ignore(); // Clear newline character

        if (pasirinkimas == 1) {
            // Manual input with random grade generation
            int kiekMokiniu;
            std::cout << "Kiek studentų norite įvesti? ";
            std::cin >> kiekMokiniu;
            std::cin.ignore();

            for (int i = 0; i < kiekMokiniu; ++i) {
                Studentas studentas;
                std::cout << "Įveskite " << i + 1 << "-ojo studento vardą: ";
                std::getline(std::cin, studentas.vardas);
                std::cout << "Įveskite " << i + 1 << "-ojo studento pavardę: ";
                std::getline(std::cin, studentas.pavarde);

                // Ask how many homework grades to generate
                int ndKiekis;
                std::cout << "Kiek namų darbų pažymių norite sugeneruoti? ";
                std::cin >> ndKiekis;
                std::cin.ignore();

                generuotiPazymius(studentas, ndKiekis); // Generate random grades

                studentai.push_back(studentas);
            }
        }
        else if (pasirinkimas == 2) {
            // Read from file
            std::string failoPavadinimas;
            std::cout << "Įveskite failo pavadinimą: ";
            std::getline(std::cin, failoPavadinimas);
            nuskaititiIsFailo(failoPavadinimas, studentai);
        }
        else {
            std::cout << "Neteisingas pasirinkimas." << std::endl;
            return 1;
        }

        surikiuotiStudentus(studentai); // Sort students by last name

        // Print headers for both average and median
        std::cout << std::left << std::setw(15) << "Vardas"
            << std::setw(15) << "Pavarde"
            << std::setw(20) << "Galutinis (Vid.)"
            << std::setw(20) << "Galutinis (Med.)"
            << std::endl;

        std::cout << "--------------------------------------------------------------" << std::endl;

        // Iterate through each student to calculate and display scores
        for (const auto& studentas : studentai) {
            try {
                double galutinisVid = skaiciuotiGalutiniVidurki(studentas.namuDarbai, studentas.egzaminas);
                double galutinisMed = skaiciuotiGalutiniMediana(studentas.namuDarbai, studentas.egzaminas);

                // Print student data
                std::cout << std::left << std::setw(15) << studentas.vardas
                    << std::setw(15) << studentas.pavarde
                    << std::setw(20) << std::fixed << std::setprecision(2) << galutinisVid
                    << std::setw(20) << std::fixed << std::setprecision(2) << galutinisMed
                    << std::endl;
            }
            catch (const std::runtime_error& e) {
                std::cerr << "Klaida studentui " << studentas.vardas << " " << studentas.pavarde << ": " << e.what() << std::endl;
            }
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Klaida: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Įvyko nežinoma klaida." << std::endl;
    }

    return 0;
}

