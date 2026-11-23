#ifndef ACUMULATOR_LITIU_H
#define ACUMULATOR_LITIU_H

#include "ComponentaElectronica.h"

// Clasa derivata 4: Banc de acumulatori litiu-fier-fosfat (LiFePO4) pentru stocarea energiei solare
class AcumulatorLitiu : public ComponentaElectronica {
private:
    double capacitateAmperiOra;   // Capacitate nominala in Ah (ex: 100 Ah)
    double nivelIncarcareProcent; // Stare de incarcare (SoC) intre 0% si 100%
    int cicluriIncarcare;

    void valideazaParametriAcumulator() const;

protected:
    void afiseazaDetaliiSpecifice(std::ostream& out) const override;

public:
    AcumulatorLitiu(std::string denumireComp, double tensiuneNominalaComp, double curentMaximComp,
                    bool functional, double capacitateAh, double nivelInitialProcent = 100.0,
                    int cicluriInitiale = 0);

    [[nodiscard]] std::unique_ptr<ComponentaElectronica> clone() const override;

    // Simulare tematica: Calculeaza energia utila stocata (Wh) si actualizeaza ciclul de functionare
    [[nodiscard]] double simuleazaFunctionare(double parametruMediu, double tensiuneAlimentare) override;

    [[nodiscard]] std::string getTipComponenta() const override;

    // Metode specifice derivatei
    void reincarcaLaCapacitateMaxima();
    [[nodiscard]] double calculeazaEnergieTotalaWh() const;

    [[nodiscard]] double getCapacitateAmperiOra() const;
    [[nodiscard]] double getNivelIncarcareProcent() const;
    [[nodiscard]] int getCicluriIncarcare() const;
};

#endif // ACUMULATOR_LITIU_H
