#include "CircuitElectric.h"

#include <iostream>
#include <numeric>
#include <utility>

#include "ActuatorSera.h"
#include "EroriCircuit.h"
#include "PanouSolar.h"
#include "SenzorMonitorizare.h"

void CircuitElectric::valideazaCircuit() const {
    if (denumireCircuit.empty()) {
        throw ConfiguratieInvalidaException("Denumirea circuitului nu poate fi vida!");
    }
    if (tensiuneAlimentare <= 0.0) {
        throw TensiuneInvalidaException("Tensiunea de alimentare a circuitului trebuie sa fie strict pozitiva!");
    }
}

CircuitElectric::CircuitElectric(std::string denumire, double tensiune)
    : denumireCircuit(std::move(denumire)),
      tensiuneAlimentare(tensiune) {
    valideazaCircuit();
}

CircuitElectric::CircuitElectric(const CircuitElectric& altul)
    : denumireCircuit(altul.denumireCircuit),
      tensiuneAlimentare(altul.tensiuneAlimentare),
      componentaPrincipala(altul.componentaPrincipala ? altul.componentaPrincipala->clone() : nullptr) {
    componenteSecundare.reserve(altul.componenteSecundare.size());
    for (const auto& comp : altul.componenteSecundare) {
        if (comp) {
            componenteSecundare.push_back(comp->clone());
        }
    }
}

void swap(CircuitElectric& first, CircuitElectric& second) noexcept {
    using std::swap;
    swap(first.denumireCircuit, second.denumireCircuit);
    swap(first.tensiuneAlimentare, second.tensiuneAlimentare);
    swap(first.componentaPrincipala, second.componentaPrincipala);
    swap(first.componenteSecundare, second.componenteSecundare);
}

CircuitElectric& CircuitElectric::operator=(CircuitElectric altul) {
    swap(*this, altul);
    return *this;
}

CircuitElectric::~CircuitElectric() = default;

void CircuitElectric::conecteazaComponentaPrincipala(std::unique_ptr<ComponentaElectronica> comp) {
    if (!comp) {
        throw ConfiguratieInvalidaException("Nu se poate conecta un pointer null ca si componenta principala!");
    }
    componentaPrincipala = std::move(comp);
}

void CircuitElectric::adaugaComponentaSecundara(std::unique_ptr<ComponentaElectronica> comp) {
    if (!comp) {
        throw ConfiguratieInvalidaException("Nu se poate adauga un pointer null in componentele secundare!");
    }
    componenteSecundare.push_back(std::move(comp));
}

double CircuitElectric::simuleazaSarcinaCircuit(double parametruMediu) const {
    if (!componentaPrincipala) {
        throw ComponentaDefectaException("Circuitul '" + denumireCircuit + "' nu are nicio componenta principala conectata!");
    }

    // Apel virtual polimorfic prin pointerul de baza la clasa abstracta
    double bilantEnergeticW = componentaPrincipala->simuleazaFunctionare(parametruMediu, tensiuneAlimentare);

    // Parcurgere colectie STL de smart pointers la clasa de baza
    for (const auto& comp : componenteSecundare) {
        if (comp) {
            bilantEnergeticW += comp->simuleazaFunctionare(parametruMediu, tensiuneAlimentare);
        }
    }

    return bilantEnergeticW;
}

void CircuitElectric::optimizeazaSauCalibreazaEchipamente() {
    auto calibreazaIndividual = [](ComponentaElectronica* comp) {
        if (!comp) {
            return;
        }

        // Downcasting cu dynamic_cast: verificam tipul real al obiectului la runtime
        if (auto* panou = dynamic_cast<PanouSolar*>(comp)) {
            // Actiune specifica doar panourilor solare: aliniere la unghiul optim de 45 grade
            panou->optimizeazaUnghiOrientare(45.0);
        } else if (auto* senzor = dynamic_cast<SenzorMonitorizare*>(comp)) {
            // Actiune specifica doar senzorilor: recalibrare punct zero
            senzor->autocalibrare(0.0);
        } else if (auto* actuator = dynamic_cast<ActuatorSera*>(comp)) {
            // Actiune specifica doar actuatoarelor: ciclu de testare mecanica
            actuator->executaCicluTestareFortata();
        }
    };

    calibreazaIndividual(componentaPrincipala.get());
    for (const auto& comp : componenteSecundare) {
        calibreazaIndividual(comp.get());
    }
}

void CircuitElectric::afiseazaSchemaCircuit(std::ostream& out) const {
    out << "========================================================\n"
        << "  SCHEMA CIRCUIT: " << denumireCircuit << "\n"
        << "  Tensiune de lucru retea: " << tensiuneAlimentare << " V\n"
        << "========================================================\n";

    out << "-> Componenta Principala:\n";
    if (componentaPrincipala) {
        // Apel prin pointer de baza catre interfata Non-Virtuala afiseaza()
        componentaPrincipala->afiseaza(out);
    } else {
        out << "   [Slot liber - nicio componenta montata]\n";
    }

    out << "-> Componente Secundare (" << componenteSecundare.size() << " montate):\n";
    for (std::size_t i = 0; i < componenteSecundare.size(); ++i) {
        out << "  #" << (i + 1) << " ";
        if (componenteSecundare[i]) {
            componenteSecundare[i]->afiseaza(out);
        }
    }
    out << "========================================================\n";
}

const std::string& CircuitElectric::getDenumireCircuit() const {
    return denumireCircuit;
}

double CircuitElectric::getTensiuneAlimentare() const {
    return tensiuneAlimentare;
}

const ComponentaElectronica* CircuitElectric::getComponentaPrincipala() const {
    return componentaPrincipala.get();
}

const std::vector<std::unique_ptr<ComponentaElectronica>>& CircuitElectric::getComponenteSecundare() const {
    return componenteSecundare;
}

std::ostream& operator<<(std::ostream& out, const CircuitElectric& circuit) {
    circuit.afiseazaSchemaCircuit(out);
    return out;
}
