#include "PanouSolar.h"

#include <cmath>
#include <iostream>
#include <numbers>
#include <utility>

#include "EroriCircuit.h"

void PanouSolar::valideazaParametriPanou() const {
    if (eficientaFotovoltaica <= 0.0 || eficientaFotovoltaica > 1.0) {
        throw ConfiguratieInvalidaException("Eficienta panoului trebuie sa fie subunitara si pozitiva (ex: 0.20)!");
    }
    if (suprafataMp <= 0.0) {
        throw ConfiguratieInvalidaException("Suprafata panoului fotovoltaic trebuie sa fie strict pozitiva!");
    }
    if (unghiOrientareGrade < 0.0 || unghiOrientareGrade > 90.0) {
        throw ConfiguratieInvalidaException("Unghiul de orientare trebuie sa fie intre 0 si 90 grade!");
    }
}

PanouSolar::PanouSolar(std::string denumireComp, double tensiuneNominalaComp, double curentMaximComp,
                       bool functional, double eficienta, double suprafata, double unghi)
    : ComponentaElectronica(std::move(denumireComp), tensiuneNominalaComp, curentMaximComp, functional),
      eficientaFotovoltaica(eficienta),
      suprafataMp(suprafata),
      unghiOrientareGrade(unghi) {
    valideazaParametriPanou();
}

std::unique_ptr<ComponentaElectronica> PanouSolar::clone() const {
    return std::make_unique<PanouSolar>(*this);
}

double PanouSolar::simuleazaFunctionare(double parametruMediu, double tensiuneAlimentare) {
    if (!getEsteFunctionala()) {
        throw ComponentaDefectaException("Panoul solar '" + getDenumire() + "' este deteriorat si nu produce energie!");
    }
    if (parametruMediu < 0.0) {
        throw ConfiguratieInvalidaException("Radiatia solara incidenta nu poate fi negativa!");
    }
    if (!verificaCompatibilitateTensiune(tensiuneAlimentare)) {
        throw TensiuneInvalidaException("Tensiunea retelei de incarcare (" + std::to_string(tensiuneAlimentare) +
                                        " V) depaseste toleranta panoului (" +
                                        std::to_string(getTensiuneNominala()) + " V)!");
    }

    // parametruMediu = radiatia solara in W/m^2 (ex: 800 W/m^2 intr-o zi insorita)
    const double unghiRad = unghiOrientareGrade * (std::numbers::pi / 180.0);
    const double factorOrientare = std::cos(unghiRad);
    const double putereGenerata = parametruMediu * suprafataMp * eficientaFotovoltaica * factorOrientare;

    return putereGenerata;
}

std::string PanouSolar::getTipComponenta() const {
    return "Sursa Fotovoltaica";
}

void PanouSolar::afiseazaDetaliiSpecifice(std::ostream& out) const {
    out << "  - Eficienta celule: " << (eficientaFotovoltaica * 100.0) << " %\n"
        << "  - Suprafata activa: " << suprafataMp << " m^2\n"
        << "  - Unghi orientare solar: " << unghiOrientareGrade << " grade\n"
        << "  - Putere nominala maxima: " << calculeazaPutereMaximaTeoretica() << " W\n";
}

void PanouSolar::optimizeazaUnghiOrientare(double azimutOptim) {
    if (azimutOptim < 0.0 || azimutOptim > 90.0) {
        throw ConfiguratieInvalidaException("Azimutul optim calculat este in afara intervalului valid [0, 90]!");
    }
    unghiOrientareGrade = azimutOptim;
}

double PanouSolar::calculeazaPutereMaximaTeoretica() const {
    // Standard test condition: 1000 W/m^2 radiatie incidenta perpendiculara (cos 0 = 1)
    return 1000.0 * suprafataMp * eficientaFotovoltaica;
}

double PanouSolar::getEficientaFotovoltaica() const {
    return eficientaFotovoltaica;
}

double PanouSolar::getSuprafataMp() const {
    return suprafataMp;
}

double PanouSolar::getUnghiOrientareGrade() const {
    return unghiOrientareGrade;
}
