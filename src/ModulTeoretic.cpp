#include "ModulTeoretic.h"

// Constructor implicit
ModulTeoretic::ModulTeoretic() : denumire("Necunoscut"), dificultate(1), puncteRecompensa(0), esteDeblocat(false) {}

// Constructor cu parametri
ModulTeoretic::ModulTeoretic(std::string denumire, int dificultate, int puncte, bool deblocat)
    : denumire(denumire), dificultate(dificultate), puncteRecompensa(puncte), esteDeblocat(deblocat) {}

// Constructor de copiere
ModulTeoretic::ModulTeoretic(const ModulTeoretic& altul) {
    denumire = altul.denumire;
    dificultate = altul.dificultate;
    puncteRecompensa = altul.puncteRecompensa;
    esteDeblocat = altul.esteDeblocat;
}

// Destructor
ModulTeoretic::~ModulTeoretic() {
    // Curățare automată la ieșirea din scope
}

// Operator de atribuire (=)
ModulTeoretic& ModulTeoretic::operator=(const ModulTeoretic& altul) {
    if (this != &altul) {
        denumire = altul.denumire;
        dificultate = altul.dificultate;
        puncteRecompensa = altul.puncteRecompensa;
        esteDeblocat = altul.esteDeblocat;
    }
    return *this;
}

// Getteri și Setteri
std::string ModulTeoretic::getDenumire() const { return denumire; }
void ModulTeoretic::setDenumire(const std::string& denumireNoua) { denumire = denumireNoua; }

int ModulTeoretic::getPuncte() const { return puncteRecompensa; }
void ModulTeoretic::setPuncte(int puncteNoi) { puncteRecompensa = puncteNoi; }

// Supraîncărcare operator << (afișare)
std::ostream& operator<<(std::ostream& out, const ModulTeoretic& modul) {
    out << "[Modul Electric] Titlu: " << modul.denumire 
        << " | Dificultate: " << modul.dificultate 
        << " | Puncte Recompensa: " << modul.puncteRecompensa 
        << " | Status: " << (modul.esteDeblocat ? "Deblocat" : "Blocat");
    return out;
}

// Supraîncărcare operator >> (citire)
std::istream& operator>>(std::istream& in, ModulTeoretic& modul) {
    std::cout << "Introdu denumirea modulului: ";
    in >> modul.denumire;
    std::cout << "Introdu dificultatea (1-5): ";
    in >> modul.dificultate;
    std::cout << "Introdu punctele de recompensa: ";
    in >> modul.puncteRecompensa;
    modul.esteDeblocat = true;
    return in;
}

// Operator ++ (adaugă 10 puncte bonus pentru viteză în treasure hunt)
ModulTeoretic& ModulTeoretic::operator++() {
    puncteRecompensa += 10;
    return *this;
}

// Metodă specifică
void ModulTeoretic::afiseazaDetaliiEtapa() const {
    std::cout << ">>> Bun venit in Ghidul Electricianului! Etapa curenta: " << denumire << "\n";
}