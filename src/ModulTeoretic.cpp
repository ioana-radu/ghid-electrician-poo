#include "ModulTeoretic.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>

void ModulTeoretic::valideaza() const {
    if (denumire.empty()) {
        throw std::invalid_argument("Denumirea modulului nu poate fi vida!");
    }
    if (dificultate < dificultateMin || dificultate > dificultateMax) {
        throw std::invalid_argument("Dificultatea trebuie sa fie intre 1 si 5!");
    }
    if (puncteRecompensa < 0) {
        throw std::invalid_argument("Punctele de recompensa nu pot fi negative!");
    }
}

// 1. Constructori și Destructor (Regula celor 3)
ModulTeoretic::ModulTeoretic()
    : denumire("Necunoscut"),
      dificultate(1),
      puncteRecompensa(0),
      esteDeblocat(false) {}

ModulTeoretic::ModulTeoretic(std::string denumireModul, int dificultateModul, int puncte, bool deblocat)
    : denumire(std::move(denumireModul)),
      dificultate(dificultateModul),
      puncteRecompensa(puncte),
      esteDeblocat(deblocat) {
    valideaza();
}

ModulTeoretic::ModulTeoretic(const ModulTeoretic& altul)
    : denumire(altul.denumire),
      dificultate(altul.dificultate),
      puncteRecompensa(altul.puncteRecompensa),
      esteDeblocat(altul.esteDeblocat) {}

ModulTeoretic::~ModulTeoretic() = default;

// 2. Operator de atribuire (=)
ModulTeoretic& ModulTeoretic::operator=(const ModulTeoretic& altul) {
    if (this != &altul) {
        denumire = altul.denumire;
        dificultate = altul.dificultate;
        puncteRecompensa = altul.puncteRecompensa;
        esteDeblocat = altul.esteDeblocat;
    }
    return *this;
}

// 3. Getteri și Setteri
void swap(ModulTeoretic& first, ModulTeoretic& second) noexcept {
    using std::swap;
    swap(first.denumire, second.denumire);
    swap(first.dificultate, second.dificultate);
    swap(first.puncteRecompensa, second.puncteRecompensa);
    swap(first.esteDeblocat, second.esteDeblocat);
}

const std::string& ModulTeoretic::getDenumire() const {
    return denumire;
}

void ModulTeoretic::setDenumire(std::string denumireNoua) {
    if (denumireNoua.empty()) {
        throw std::invalid_argument("Denumirea modulului nu poate fi vida!");
    }
    denumire = std::move(denumireNoua);
}

int ModulTeoretic::getDificultate() const {
    return dificultate;
}

void ModulTeoretic::setDificultate(int dificultateNoua) {
    if (dificultateNoua < dificultateMin || dificultateNoua > dificultateMax) {
        throw std::invalid_argument("Dificultatea trebuie sa fie intre 1 si 5!");
    }
    dificultate = dificultateNoua;
}

int ModulTeoretic::getPuncte() const {
    return puncteRecompensa;
}

void ModulTeoretic::setPuncte(int puncteNoi) {
    if (puncteNoi < 0) {
        throw std::invalid_argument("Punctele de recompensa nu pot fi negative!");
    }
    puncteRecompensa = puncteNoi;
}

bool ModulTeoretic::getEsteDeblocat() const {
    return esteDeblocat;
}

void ModulTeoretic::setEsteDeblocat(bool deblocatNou) {
    esteDeblocat = deblocatNou;
}

// 4. Operatori de afișare și citire (<< și >>)
std::ostream& operator<<(std::ostream& out, const ModulTeoretic& modul) {
    out << "[Modul Electric] Titlu: " << modul.denumire
        << " | Dificultate: " << modul.dificultate
        << " | Puncte Recompensa: " << modul.puncteRecompensa
        << " | Status: " << (modul.esteDeblocat ? "Deblocat" : "Blocat");
    return out;
}

std::istream& operator>>(std::istream& in, ModulTeoretic& modul) {
    std::string denumireNoua;
    int dif = 0;
    int puncte = 0;
    bool deblocat = false;
    if (in >> denumireNoua >> dif >> puncte >> deblocat) {
        if (denumireNoua.empty() || dif < ModulTeoretic::dificultateMin ||
            dif > ModulTeoretic::dificultateMax || puncte < 0) {
            in.setstate(std::ios::failbit);
        } else {
            modul.denumire = std::move(denumireNoua);
            modul.dificultate = dif;
            modul.puncteRecompensa = puncte;
            modul.esteDeblocat = deblocat;
        }
    }
    return in;
}

// 5. Operator de pre-incrementare
ModulTeoretic& ModulTeoretic::operator++() {
    puncteRecompensa += puncteBonusPerIncrement;
    return *this;
}

// 6. Metode specifice temei
void ModulTeoretic::deblocheazaModul() {
    esteDeblocat = true;
}

int ModulTeoretic::calculeazaPunctajFinal(int timpSecunde, int incercariGresite) const {
    if (timpSecunde < 0 || incercariGresite < 0) {
        throw std::invalid_argument("Timpul si incercarile gresite nu pot fi negative!");
    }
    const int scorBaza = puncteRecompensa * dificultate;
    const int penalizare = incercariGresite * 5;
    int scor = scorBaza - penalizare;
    if (timpSecunde < 60) {
        scor += 15;
    }
    if (scor < 0) {
        scor = 0;
    }
    return scor;
}

bool ModulTeoretic::esteRecomandatPentruIncepatori() const {
    return dificultate <= 2;
}

void ModulTeoretic::afiseazaDetaliiEtapa() const {
    afiseazaDetaliiEtapa(std::cout);
}

void ModulTeoretic::afiseazaDetaliiEtapa(std::ostream& out) const {
    out << ">>> Bun venit in Ghidul Electricianului! Etapa curenta: " << denumire << "\n";
}
