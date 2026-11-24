#ifndef SENZOR_MONITORIZARE_H
#define SENZOR_MONITORIZARE_H

#include <string>

#include "ComponentaElectronica.h"

// Clasa derivata 2: Senzori pentru monitorizarea parametrilor serei (umiditate, temperatura)
class SenzorMonitorizare : public ComponentaElectronica {
private:
    std::string marimeMasurata;
    double pragAlerta;
    double valoareCalibrareOffset;

    void valideazaParametriSenzor() const;

protected:
    void afiseazaDetaliiSpecifice(std::ostream& out) const override;

public:
    SenzorMonitorizare(std::string denumireComp, double tensiuneNominalaComp, double curentMaximComp,
                       bool functional, std::string marime, double prag, double calibrareOffset = 0.0);

    [[nodiscard]] std::unique_ptr<ComponentaElectronica> clone() const override;

    // Simulare tematica: Converteste marimea fizica de intrare in semnal masurat si consuma putere de semnal
    [[nodiscard]] double simuleazaFunctionare(double parametruMediu, double tensiuneAlimentare) override;

    [[nodiscard]] std::string getTipComponenta() const override;

    // Metode specifice derivatei (folosite cu dynamic_cast)
    void autocalibrare(double referintaZero);
    [[nodiscard]] bool esteStareCritica(double citireMediu) const;

    [[nodiscard]] const std::string& getMarimeMasurata() const;
    [[nodiscard]] double getPragAlerta() const;
    [[nodiscard]] double getValoareCalibrareOffset() const;
};

#endif // SENZOR_MONITORIZARE_H
