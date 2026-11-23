#include "ComponentaElectronica.h"

#include <iostream>
#include <utility>

#include "EroriCircuit.h"

int ComponentaElectronica::contorComponente = 0;

void ComponentaElectronica::valideazaParametriBaza() const {
    if (denumire.empty()) {
        throw ConfiguratieInvalidaException("Denumirea componentei nu poate fi vida!");
    }
    if (tensiuneNominala <= 0.0) {
        throw TensiuneInvalidaException("Tensiunea nominala trebuie sa fie strict pozitiva!");
    }
    if (curentMaxim <= 0.0) {
        throw ConfiguratieInvalidaException("Curentul maxim suportat trebuie sa fie strict pozitiv!");
    }
}

ComponentaElectronica::ComponentaElectronica(std::string denumireComp, double tensiuneNominalaComp,
                                             double curentMaximComp, bool functional)
    : denumire(std::move(denumireComp)),
      tensiuneNominala(tensiuneNominalaComp),
      curentMaxim(curentMaximComp),
      esteFunctionala(functional) {
    valideazaParametriBaza();
    ++contorComponente;
}

ComponentaElectronica::ComponentaElectronica(const ComponentaElectronica& altul)
    : denumire(altul.denumire),
      tensiuneNominala(altul.tensiuneNominala),
      curentMaxim(altul.curentMaxim),
      esteFunctionala(altul.esteFunctionala) {
    ++contorComponente;
}

ComponentaElectronica& ComponentaElectronica::operator=(const ComponentaElectronica& altul) {
    if (this != &altul) {
        denumire = altul.denumire;
        tensiuneNominala = altul.tensiuneNominala;
        curentMaxim = altul.curentMaxim;
        esteFunctionala = altul.esteFunctionala;
    }
    return *this;
}

ComponentaElectronica::~ComponentaElectronica() {
    --contorComponente;
}

void ComponentaElectronica::afiseaza(std::ostream& out) const {
    out << "[" << getTipComponenta() << "] " << denumire << "\n"
        << "  - Tensiune nominala: " << tensiuneNominala << " V\n"
        << "  - Curent maxim admis: " << curentMaxim << " A\n"
        << "  - Stare tehnica: " << (esteFunctionala ? "FUNCTIONALA (Gata de montaj)" : "DEFECTA / ARSA") << "\n";
    afiseazaDetaliiSpecifice(out);
}

void ComponentaElectronica::reparaSauCalibreaza() {
    esteFunctionala = true;
}

void ComponentaElectronica::marcheazaDefecta() {
    esteFunctionala = false;
}

bool ComponentaElectronica::verificaCompatibilitateTensiune(double tensiuneCircuit) const {
    if (tensiuneCircuit < 0.0) {
        throw TensiuneInvalidaException("Tensiunea aplicata in circuit nu poate fi negativa!");
    }
    // O marja de toleranta sigura de maxim +15% peste tensiunea nominala
    return tensiuneCircuit <= tensiuneNominala * 1.15;
}

const std::string& ComponentaElectronica::getDenumire() const {
    return denumire;
}

double ComponentaElectronica::getTensiuneNominala() const {
    return tensiuneNominala;
}

double ComponentaElectronica::getCurentMaxim() const {
    return curentMaxim;
}

bool ComponentaElectronica::getEsteFunctionala() const {
    return esteFunctionala;
}

int ComponentaElectronica::getContorComponente() {
    return contorComponente;
}

bool ComponentaElectronica::esteTensiuneInGamaStandard(double tensiune) {
    return tensiune >= 3.3 && tensiune <= 230.0;
}

std::ostream& operator<<(std::ostream& out, const ComponentaElectronica& comp) {
    comp.afiseaza(out);
    return out;
}
