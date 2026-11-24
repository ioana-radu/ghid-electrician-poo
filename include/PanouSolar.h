#ifndef PANOU_SOLAR_H
#define PANOU_SOLAR_H

#include "ComponentaElectronica.h"

// Clasa derivata 1: Sursa de alimentare fotovoltaica pentru sera
class PanouSolar : public ComponentaElectronica {
private:
    double eficientaFotovoltaica; // Randament (ex: 0.21 pentru 21%)
    double suprafataMp;           // Suprafata panoului in m^2
    double unghiOrientareGrade;   // Unghiul fata de soare (0 - 90 grade)

    void valideazaParametriPanou() const;

protected:
    void afiseazaDetaliiSpecifice(std::ostream& out) const override;

public:
    PanouSolar(std::string denumireComp, double tensiuneNominalaComp, double curentMaximComp,
               bool functional, double eficienta, double suprafata, double unghi);

    [[nodiscard]] std::unique_ptr<ComponentaElectronica> clone() const override;

    // Simulare tematica: Calculeaza puterea electrica produsa (W) in functie de radiatia solara (W/m^2)
    [[nodiscard]] double simuleazaFunctionare(double parametruMediu, double tensiuneAlimentare) override;

    [[nodiscard]] std::string getTipComponenta() const override;

    // Metode specifice derivatei (folosite cu dynamic_cast)
    void optimizeazaUnghiOrientare(double azimutOptim);
    [[nodiscard]] double calculeazaPutereMaximaTeoretica() const;

    [[nodiscard]] double getEficientaFotovoltaica() const;
    [[nodiscard]] double getSuprafataMp() const;
    [[nodiscard]] double getUnghiOrientareGrade() const;
};

#endif // PANOU_SOLAR_H
