#include "SenzorMonitorizare.h"

#include <iostream>
#include <utility>

#include "EroriCircuit.h"

void SenzorMonitorizare::valideazaParametriSenzor() const {
    if (marimeMasurata.empty()) {
        throw ConfiguratieInvalidaException("Marimea fizica masurata de senzor nu poate fi vida!");
    }
}

SenzorMonitorizare::SenzorMonitorizare(std::string denumireComp, double tensiuneNominalaComp,
                                       double curentMaximComp, bool functional,
                                       std::string marime, double prag, double calibrareOffset)
    : ComponentaElectronica(std::move(denumireComp), tensiuneNominalaComp, curentMaximComp, functional),
      marimeMasurata(std::move(marime)),
      pragAlerta(prag),
      valoareCalibrareOffset(calibrareOffset) {
    valideazaParametriSenzor();
}

std::unique_ptr<ComponentaElectronica> SenzorMonitorizare::clone() const {
    return std::make_unique<SenzorMonitorizare>(*this);
}

double SenzorMonitorizare::simuleazaFunctionare(double parametruMediu, double tensiuneAlimentare) {
    if (!getEsteFunctionala()) {
        throw ComponentaDefectaException("Senzorul '" + getDenumire() + "' este defect si genereaza citiri eronate!");
    }
    if (!verificaCompatibilitateTensiune(tensiuneAlimentare)) {
        throw TensiuneInvalidaException("Tensiunea de alimentare a senzorului (" +
                                        std::to_string(tensiuneAlimentare) + " V) este periculoasa!");
    }

    // Calculeaza valoarea masurata corectata cu offset-ul de calibrare
    const double valoareCorectata = parametruMediu + valoareCalibrareOffset;

    // Intoarce puterea mica de semnal consumata de senzor (P = U * I_standby, e.g. 0.05 W per Volt)
    const double consumSemnalW = tensiuneAlimentare * 0.015;
    (void)valoareCorectata;
    return consumSemnalW;
}

std::string SenzorMonitorizare::getTipComponenta() const {
    return "Senzor Monitorizare";
}

void SenzorMonitorizare::afiseazaDetaliiSpecifice(std::ostream& out) const {
    out << "  - Marime monitorizata: " << marimeMasurata << "\n"
        << "  - Prag de alerta: " << pragAlerta << "\n"
        << "  - Offset calibrare: " << valoareCalibrareOffset << "\n";
}

void SenzorMonitorizare::autocalibrare(double referintaZero) {
    valoareCalibrareOffset = -referintaZero;
}

bool SenzorMonitorizare::esteStareCritica(double citireMediu) const {
    return (citireMediu + valoareCalibrareOffset) >= pragAlerta;
}

const std::string& SenzorMonitorizare::getMarimeMasurata() const {
    return marimeMasurata;
}

double SenzorMonitorizare::getPragAlerta() const {
    return pragAlerta;
}

double SenzorMonitorizare::getValoareCalibrareOffset() const {
    return valoareCalibrareOffset;
}
