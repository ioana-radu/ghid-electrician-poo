#include "ActuatorSera.h"

#include <iostream>
#include <utility>

#include "EroriCircuit.h"

void ActuatorSera::valideazaParametriActuator() const {
    if (rolSera.empty()) {
        throw ConfiguratieInvalidaException("Rolul actuatorului in sera nu poate fi vid!");
    }
    if (putereNominalaW <= 0.0) {
        throw ConfiguratieInvalidaException("Puterea nominala a actuatorului trebuie sa fie strict pozitiva!");
    }
    if (totalCicluriComutare < 0) {
        throw ConfiguratieInvalidaException("Numarul de cicluri de comutare nu poate fi negativ!");
    }
}

ActuatorSera::ActuatorSera(std::string denumireComp, double tensiuneNominalaComp,
                           double curentMaximComp, bool functional,
                           std::string rol, double putereW, int cicluriInitiale)
    : ComponentaElectronica(std::move(denumireComp), tensiuneNominalaComp, curentMaximComp, functional),
      rolSera(std::move(rol)),
      putereNominalaW(putereW),
      totalCicluriComutare(cicluriInitiale) {
    valideazaParametriActuator();
}

std::unique_ptr<ComponentaElectronica> ActuatorSera::clone() const {
    return std::make_unique<ActuatorSera>(*this);
}

double ActuatorSera::simuleazaFunctionare(double parametruMediu, double tensiuneAlimentare) {
    if (!getEsteFunctionala()) {
        throw ComponentaDefectaException("Actuatorul '" + getDenumire() + "' este blocat mecanic sau ars!");
    }
    if (!verificaCompatibilitateTensiune(tensiuneAlimentare)) {
        throw TensiuneInvalidaException("Tensiunea de comanda (" + std::to_string(tensiuneAlimentare) +
                                        " V) depaseste specificatia actuatorului (" +
                                        std::to_string(getTensiuneNominala()) + " V)!");
    }

    // parametruMediu reprezinta durata activarii in minute (ex: irigare timp de 5 minute)
    if (parametruMediu < 0.0) {
        throw ConfiguratieInvalidaException("Durata activarii actuatorului nu poate fi negativa!");
    }

    ++totalCicluriComutare;
    // Calculeaza energia consumata in Wh: P (W) * (t / 60 ore)
    const double oreFunctionare = parametruMediu / 60.0;
    const double energieConsumataWh = putereNominalaW * oreFunctionare;

    return energieConsumataWh;
}

std::string ActuatorSera::getTipComponenta() const {
    return "Actuator Executie";
}

void ActuatorSera::afiseazaDetaliiSpecifice(std::ostream& out) const {
    out << "  - Rol functional: " << rolSera << "\n"
        << "  - Putere nominala: " << putereNominalaW << " W\n"
        << "  - Total cicluri comutare: " << totalCicluriComutare << "\n";
}

void ActuatorSera::executaCicluTestareFortata() {
    if (!getEsteFunctionala()) {
        throw ComponentaDefectaException("Nu se poate efectua testare fortata pe o componenta defecta!");
    }
    ++totalCicluriComutare;
}

void ActuatorSera::reseteazaContorCicluri() {
    totalCicluriComutare = 0;
}

const std::string& ActuatorSera::getRolSera() const {
    return rolSera;
}

double ActuatorSera::getPutereNominalaW() const {
    return putereNominalaW;
}

int ActuatorSera::getTotalCicluriComutare() const {
    return totalCicluriComutare;
}
