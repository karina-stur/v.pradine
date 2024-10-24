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

    testKonteinerius(failoPavadinimas, pagalVidurki);

    return 0;
}
