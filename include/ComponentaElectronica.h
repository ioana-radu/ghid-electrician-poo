#ifndef COMPONENTA_ELECTRONICA_H
#define COMPONENTA_ELECTRONICA_H

#include <iosfwd>
#include <string>

class ComponentaElectronica {
private:
    std::string denumire = "Componenta Generica";
    std::string tip = "Pasiv"; // Pasiv, Semiconductor, Sursa, Protectie, Senzor
    double valoareNominala = 0.0;
    std::string unitateMasura = "Ohm";
    double tensiuneMaximaSuportata = 12.0;
    bool esteFunctionala = true;

    void valideaza() const;

public:
    // Constructori
    ComponentaElectronica();
    ComponentaElectronica(std::string denumireComp, std::string tipComp,
                          double valoareNominalaComp, std::string unitateMasuraComp,
                          double tensiuneMax, bool functionala);

    // Getteri și Setteri
    [[nodiscard]] const std::string& getDenumire() const;
    void setDenumire(std::string denumireNoua);

    [[nodiscard]] const std::string& getTip() const;
    void setTip(std::string tipNou);

    [[nodiscard]] double getValoareNominala() const;
    void setValoareNominala(double valoareNoua);

    [[nodiscard]] const std::string& getUnitateMasura() const;
    void setUnitateMasura(std::string unitateNoua);

    [[nodiscard]] double getTensiuneMaxima() const;
    void setTensiuneMaxima(double tensiuneNoua);

    [[nodiscard]] bool getEsteFunctionala() const;
    void setEsteFunctionala(bool stare);

    // Metode specifice temei
    [[nodiscard]] bool verificaSigurantaFunctionare(double tensiuneCircuit) const;
    void reparaSauCalibreaza();
    [[nodiscard]] double calculeazaCurentMaxim(double rezistentaCircuit) const;

    // Operatori
    friend std::ostream& operator<<(std::ostream& out, const ComponentaElectronica& comp);
    friend std::istream& operator>>(std::istream& in, ComponentaElectronica& comp);
};

#endif // COMPONENTA_ELECTRONICA_H
