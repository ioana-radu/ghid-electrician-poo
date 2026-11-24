#ifndef ACTUATOR_SERA_H
#define ACTUATOR_SERA_H

#include <string>

#include "ComponentaElectronica.h"

// Clasa derivata 3: Dispozitive de executie (pompe, electrovalve, servomotoare de aerisire)
class ActuatorSera : public ComponentaElectronica {
private:
    std::string rolSera;
    double putereNominalaW;
    int totalCicluriComutare;

    void valideazaParametriActuator() const;

protected:
    void afiseazaDetaliiSpecifice(std::ostream& out) const override;

public:
    ActuatorSera(std::string denumireComp, double tensiuneNominalaComp, double curentMaximComp,
                 bool functional, std::string rol, double putereW, int cicluriInitiale = 0);

    [[nodiscard]] std::unique_ptr<ComponentaElectronica> clone() const override;

    // Simulare tematica: Executa comanda de comutare/pompare si returneaza energia consumata
    [[nodiscard]] double simuleazaFunctionare(double parametruMediu, double tensiuneAlimentare) override;

    [[nodiscard]] std::string getTipComponenta() const override;

    // Metode specifice derivatei (folosite cu dynamic_cast)
    void executaCicluTestareFortata();
    void reseteazaContorCicluri();

    [[nodiscard]] const std::string& getRolSera() const;
    [[nodiscard]] double getPutereNominalaW() const;
    [[nodiscard]] int getTotalCicluriComutare() const;
};

#endif // ACTUATOR_SERA_H
