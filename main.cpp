#include <iostream>
#include <vector>
#include <chrono>
#include "funkcijos.h"

int main() {
    char generateOption;
    std::cout << "Norite sugeneruoti studentu failus? (y/n): ";
    std::cin >> generateOption;

    if (generateOption == 'y' || generateOption == 'Y') {
        generuotiStudentuFailus();
    }

    std::string failoPavadinimas;
    std::cout << "Iveskite failo pavadinima: ";
    std::cin >> failoPavadinimas;

    bool pagalVidurki;
    std::cout << "Pasirinkite rusiavimo kriteriju (0 - mediana, 1 - vidurkis): ";
    std::cin >> pagalVidurki;

    std::vector<Studentas> studentai;
    auto startRead = std::chrono::high_resolution_clock::now();
    nuskaitytiStudentus(failoPavadinimas, studentai);
    auto endRead = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> readingTime = endRead - startRead;

    auto startSort = std::chrono::high_resolution_clock::now();
    grupuotiIrIssaugotiStudentus(studentai, pagalVidurki);
    auto endSort = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sortingTime = endSort - startSort;

    std::cout << "Duomenu nuskaitymas truko: " << readingTime.count() << " sekundziu." << std::endl;
    std::cout << "Studentu rusiavimas truko: " << sortingTime.count() << " sekundziu." << std::endl;
    std::cout << "Bendra programos trukme: " << (readingTime.count() + sortingTime.count()) << " sekundziu." << std::endl;

    return 0;
}
