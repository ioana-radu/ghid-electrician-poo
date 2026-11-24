#ifndef CIRCUIT_ELECTRIC_H
#define CIRCUIT_ELECTRIC_H

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "ComponentaElectronica.h"

// Clasa care detine atribute de tip pointer (smart pointers) la clasa de baza ComponentaElectronica
class CircuitElectric {
private:
    std::string denumireCircuit;
    double tensiuneAlimentare; // in Volti (V)
    std::unique_ptr<ComponentaElectronica> componentaPrincipala;
    std::vector<std::unique_ptr<ComponentaElectronica>> componenteSecundare;

    void valideazaCircuit() const;

public:
    // Constructori
    CircuitElectric(std::string denumire, double tensiune);

    // Constructor de copiere (Deep Copy prin intermediul constructorului virtual clone())
    CircuitElectric(const CircuitElectric& altul);

    // Swap noexcept pentru idiomul Copy-and-Swap
    friend void swap(CircuitElectric& first, CircuitElectric& second) noexcept;

    // Operator de atribuire prin Copy-and-Swap
    CircuitElectric& operator=(CircuitElectric altul);

    // Mutare (Move semantics)
    CircuitElectric(CircuitElectric&& altul) noexcept = default;
    CircuitElectric& operator=(CircuitElectric&& altul) noexcept = default;

    // Destructor
    ~CircuitElectric();

    // Conectare componente (proprietate transferata prin std::unique_ptr)
    void conecteazaComponentaPrincipala(std::unique_ptr<ComponentaElectronica> comp);
    void adaugaComponentaSecundara(std::unique_ptr<ComponentaElectronica> comp);

    // Apel polimorfic de nivel inalt: simuleaza intregul circuit electric
    [[nodiscard]] double simuleazaSarcinaCircuit(double parametruMediu) const;

    // Downcast cu sens folosind dynamic_cast pe pointer de clasa de baza
    void optimizeazaSauCalibreazaEchipamente();

    // Afisare schema circuit apeland interfata NVI a componentelor prin pointer de baza
    void afiseazaSchemaCircuit(std::ostream& out) const;

    // Getteri
    [[nodiscard]] const std::string& getDenumireCircuit() const;
    [[nodiscard]] double getTensiuneAlimentare() const;
    [[nodiscard]] const ComponentaElectronica* getComponentaPrincipala() const;
    [[nodiscard]] const std::vector<std::unique_ptr<ComponentaElectronica>>& getComponenteSecundare() const;

    // Operator de afisare
    friend std::ostream& operator<<(std::ostream& out, const CircuitElectric& circuit);
};

#endif // CIRCUIT_ELECTRIC_H
