#include "AcumulatorLitiu.h"

#include <iostream>
#include <utility>

#include "EroriCircuit.h"

void AcumulatorLitiu::valideazaParametriAcumulator() const {
    if (capacitateAmperiOra <= 0.0) {
        throw ConfiguratieInvalidaException("Capacitatea acumulatorului trebuie sa fie strict pozitiva!");
    }
    if (nivelIncarcareProcent < 0.0 || nivelIncarcareProcent > 100.0) {
        throw ConfiguratieInvalidaException("Nivelul de incarcare trebuie sa fie intre 0% si 100%!");
    }
    if (cicluriIncarcare < 0) {
        throw ConfiguratieInvalidaException("Numarul de cicluri de incarcare nu poate fi negativ!");
    }
}

AcumulatorLitiu::AcumulatorLitiu(std::string denumireComp, double tensiuneNominalaComp,
                                 double curentMaximComp, bool functional,
                                 double capacitateAh, double nivelInitialProcent, int cicluriInitiale)
    : ComponentaElectronica(std::move(denumireComp), tensiuneNominalaComp, curentMaximComp, functional),
      capacitateAmperiOra(capacitateAh),
      nivelIncarcareProcent(nivelInitialProcent),
      cicluriIncarcare(cicluriInitiale) {
    valideazaParametriAcumulator();
}

std::unique_ptr<ComponentaElectronica> AcumulatorLitiu::clone() const {
    return std::make_unique<AcumulatorLitiu>(*this);
}

double AcumulatorLitiu::simuleazaFunctionare(double parametruMediu, double tensiuneAlimentare) {
    if (!getEsteFunctionala()) {
        throw ComponentaDefectaException("Bancul de baterii '" + getDenumire() + "' este defect sau celulele sunt desincronizate!");
    }
    if (!verificaCompatibilitateTensiune(tensiuneAlimentare)) {
        throw TensiuneInvalidaException("Tensiunea de incarcare a acumulatorului (" +
                                        std::to_string(tensiuneAlimentare) + " V) este periculoasa!");
    }

    // parametruMediu reprezinta consumul sau incarcarea neta (in Wh)
    if (parametruMediu < 0.0) {
        throw ConfiguratieInvalidaException("Parametrul de energie pentru acumulator nu poate fi negativ!");
    }

    ++cicluriIncarcare;
    // Returneaza energia disponibila in Wh = Ah * V * (SoC / 100)
    const double energieDisponibilaWh = capacitateAmperiOra * getTensiuneNominala() * (nivelIncarcareProcent / 100.0);
    return energieDisponibilaWh;
}

std::string AcumulatorLitiu::getTipComponenta() const {
    return "Acumulator Energie";
}

void AcumulatorLitiu::afiseazaDetaliiSpecifice(std::ostream& out) const {
    out << "  - Capacitate stocare: " << capacitateAmperiOra << " Ah\n"
        << "  - Nivel curent incarcare (SoC): " << nivelIncarcareProcent << " %\n"
        << "  - Total cicluri incarcare: " << cicluriIncarcare << "\n"
        << "  - Energie maxima stocabila: " << calculeazaEnergieTotalaWh() << " Wh\n";
}

void AcumulatorLitiu::reincarcaLaCapacitateMaxima() {
    nivelIncarcareProcent = 100.0;
}

double AcumulatorLitiu::calculeazaEnergieTotalaWh() const {
    return capacitateAmperiOra * getTensiuneNominala();
}

double AcumulatorLitiu::getCapacitateAmperiOra() const {
    return capacitateAmperiOra;
}

double AcumulatorLitiu::getNivelIncarcareProcent() const {
    return nivelIncarcareProcent;
}

int AcumulatorLitiu::getCicluriIncarcare() const {
    return cicluriIncarcare;
}
