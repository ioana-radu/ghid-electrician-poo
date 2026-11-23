#ifndef COMPONENTA_ELECTRONICA_H
#define COMPONENTA_ELECTRONICA_H

#include <iosfwd>
#include <memory>
#include <string>

// Clasa de baza abstracta pentru toate componentele din sera inteligenta
class ComponentaElectronica {
private:
    static int contorComponente;

    std::string denumire;
    double tensiuneNominala; // in Volti (V)
    double curentMaxim;      // in Amperi (A)
    bool esteFunctionala;

    void valideazaParametriBaza() const;

protected:
    // Metoda virtuala pura pentru afisarea specifica a fiecarei clase derivate (NVI)
    virtual void afiseazaDetaliiSpecifice(std::ostream& out) const = 0;

public:
    // Constructori si Destructor virtual
    ComponentaElectronica(std::string denumireComp, double tensiuneNominalaComp,
                          double curentMaximComp, bool functional = true);
    ComponentaElectronica(const ComponentaElectronica& altul);
    ComponentaElectronica& operator=(const ComponentaElectronica& altul);
    virtual ~ComponentaElectronica();

    // Constructor virtual (Pattern-ul Prototype / Clone)
    [[nodiscard]] virtual std::unique_ptr<ComponentaElectronica> clone() const = 0;

    // Metoda virtuala pura specifica temei (simularea comportamentului in circuit)
    [[nodiscard]] virtual double simuleazaFunctionare(double parametruMediu, double tensiuneAlimentare) = 0;

    // Identificator textual al tipului de componenta
    [[nodiscard]] virtual std::string getTipComponenta() const = 0;

    // Interfata Non-Virtuala (NVI) pentru afisare polimorfica
    void afiseaza(std::ostream& out) const;

    // Metode de business
    void reparaSauCalibreaza();
    void marcheazaDefecta();
    [[nodiscard]] bool verificaCompatibilitateTensiune(double tensiuneCircuit) const;

    // Getteri
    [[nodiscard]] const std::string& getDenumire() const;
    [[nodiscard]] double getTensiuneNominala() const;
    [[nodiscard]] double getCurentMaxim() const;
    [[nodiscard]] bool getEsteFunctionala() const;

    // Membri statici
    [[nodiscard]] static int getContorComponente();
    [[nodiscard]] static bool esteTensiuneInGamaStandard(double tensiune);

    // Operator de afisare prieten (apeleaza afiseaza - NVI)
    friend std::ostream& operator<<(std::ostream& out, const ComponentaElectronica& comp);
};

#endif // COMPONENTA_ELECTRONICA_H
