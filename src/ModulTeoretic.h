#ifndef MODUL_TEORETIC_H
#define MODUL_TEORETIC_H

#include <string>
#include <iostream>

class ModulTeoretic {
private:
    std::string denumire;
    int dificultate; // de la 1 la 5
    int puncteRecompensa;
    bool esteDeblocat;

public:
    // 1. Constructori și Destructor
    ModulTeoretic(); // Constructor implicit
    ModulTeoretic(std::string denumire, int dificultate, int puncte, bool deblocat); // Constructor cu parametri
    ModulTeoretic(const ModulTeoretic& altul); // Constructor de copiere
    ~ModulTeoretic(); // Destructor

    // 2. Operator de atribuire (=)
    ModulTeoretic& operator=(const ModulTeoretic& altul);

    // 3. Getteri și Setteri (pentru încapsulare)
    std::string getDenumire() const;
    void setDenumire(const std::string& denumireNoua);
    
    int getPuncte() const;
    void setPuncte(int puncteNoi);

    // 4. Supraîncărcare operatori de citire/afișare (<< și >>)
    friend std::ostream& operator<<(std::ostream& out, const ModulTeoretic& modul);
    friend std::istream& operator>>(std::istream& in, ModulTeoretic& modul);

    // 5. Operator suplimentar (ex: operator++ pentru a crește dificultatea sau punctele)
    ModulTeoretic& operator++(); // Pre-incrementare (adaugă 10 puncte bonus)

    // 6. Metodă specifică temei
    void afiseazaDetaliiEtapa() const;
};

#endif // MODUL_TEORETIC_H