#include <iostream>
#include "ModulTeoretic.h"

int main() {
    // Testăm constructorul cu parametri
    ModulTeoretic m1("Circuite Serie-Paralel", 2, 50, true);
    m1.afiseazaDetaliiEtapa();

    // Testăm operatorul <<
    std::cout << m1 << std::endl;

    // Testăm operatorul ++ (bonus de puncte)
    ++m1;
    std::cout << "Dupa acordarea punctelor bonus: " << m1 << std::endl;

    return 0;
}