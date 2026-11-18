#include "ComponentaElectronica.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>

void ComponentaElectronica::valideaza() const {
    if (denumire.empty()) {
        throw std::invalid_argument("Denumirea componentei nu poate fi vida!");
    }
    if (tip.empty()) {
        throw std::invalid_argument("Tipul componentei nu poate fi vid!");
    }
    if (valoareNominala < 0.0) {
        throw std::invalid_argument("Valoarea nominala nu poate fi negativa!");
    }
    if (tensiuneMaximaSuportata <= 0.0) {
        throw std::invalid_argument("Tensiunea maxima trebuie sa fie strict pozitiva!");
    }
}

ComponentaElectronica::ComponentaElectronica() = default;

ComponentaElectronica::ComponentaElectronica(std::string denumireComp, std::string tipComp,
                                             double valoareNominalaComp, std::string unitateMasuraComp,
                                             double tensiuneMax, bool functionala)
    : denumire(std::move(denumireComp)),
      tip(std::move(tipComp)),
      valoareNominala(valoareNominalaComp),
      unitateMasura(std::move(unitateMasuraComp)),
      tensiuneMaximaSuportata(tensiuneMax),
      esteFunctionala(functionala) {
    valideaza();
}

const std::string& ComponentaElectronica::getDenumire() const {
    return denumire;
}

void ComponentaElectronica::setDenumire(std::string denumireNoua) {
    if (denumireNoua.empty()) {
        throw std::invalid_argument("Denumirea componentei nu poate fi vida!");
    }
    denumire = std::move(denumireNoua);
}

const std::string& ComponentaElectronica::getTip() const {
    return tip;
}

void ComponentaElectronica::setTip(std::string tipNou) {
    if (tipNou.empty()) {
        throw std::invalid_argument("Tipul componentei nu poate fi vid!");
    }
    tip = std::move(tipNou);
}

double ComponentaElectronica::getValoareNominala() const {
    return valoareNominala;
}

void ComponentaElectronica::setValoareNominala(double valoareNoua) {
    if (valoareNoua < 0.0) {
        throw std::invalid_argument("Valoarea nominala nu poate fi negativa!");
    }
    valoareNominala = valoareNoua;
}

const std::string& ComponentaElectronica::getUnitateMasura() const {
    return unitateMasura;
}

void ComponentaElectronica::setUnitateMasura(std::string unitateNoua) {
    unitateMasura = std::move(unitateNoua);
}

double ComponentaElectronica::getTensiuneMaxima() const {
    return tensiuneMaximaSuportata;
}

void ComponentaElectronica::setTensiuneMaxima(double tensiuneNoua) {
    if (tensiuneNoua <= 0.0) {
        throw std::invalid_argument("Tensiunea maxima trebuie sa fie strict pozitiva!");
    }
    tensiuneMaximaSuportata = tensiuneNoua;
}

bool ComponentaElectronica::getEsteFunctionala() const {
    return esteFunctionala;
}

void ComponentaElectronica::setEsteFunctionala(bool stare) {
    esteFunctionala = stare;
}

bool ComponentaElectronica::verificaSigurantaFunctionare(double tensiuneCircuit) const {
    if (tensiuneCircuit < 0.0) {
        throw std::invalid_argument("Tensiunea nu poate fi negativa!");
    }
    return esteFunctionala && (tensiuneCircuit <= tensiuneMaximaSuportata);
}

void ComponentaElectronica::reparaSauCalibreaza() {
    esteFunctionala = true;
}

double ComponentaElectronica::calculeazaCurentMaxim(double rezistentaCircuit) const {
    if (rezistentaCircuit <= 0.0) {
        throw std::invalid_argument("Rezistenta circuitului trebuie sa fie strict pozitiva!");
    }
    return tensiuneMaximaSuportata / rezistentaCircuit;
}

std::ostream& operator<<(std::ostream& out, const ComponentaElectronica& comp) {
    out << "[Componenta] " << comp.denumire << " (" << comp.tip << ") | Valoare: "
        << comp.valoareNominala << " " << comp.unitateMasura
        << " | Tensiune Max: " << comp.tensiuneMaximaSuportata
        << "V | Stare: " << (comp.esteFunctionala ? "Functionala" : "Defecta");
    return out;
}

std::istream& operator>>(std::istream& in, ComponentaElectronica& comp) {
    std::string denNou;
    std::string tipNou;
    double valNou = 0.0;
    std::string unitNou;
    double tensNou = 0.0;
    bool functNou = false;

    if (in >> denNou >> tipNou >> valNou >> unitNou >> tensNou >> functNou) {
        if (denNou.empty() || tipNou.empty() || valNou < 0.0 || tensNou <= 0.0) {
            in.setstate(std::ios::failbit);
        } else {
            comp.denumire = std::move(denNou);
            comp.tip = std::move(tipNou);
            comp.valoareNominala = valNou;
            comp.unitateMasura = std::move(unitNou);
            comp.tensiuneMaximaSuportata = tensNou;
            comp.esteFunctionala = functNou;
        }
    }
    return in;
}
