#include "NivelTreasureHunt.h"

#include <iostream>
#include <utility>

#include "EroriCircuit.h"
#include "PanouSolar.h"

void NivelTreasureHunt::valideaza() const {
    if (idNivel <= 0) {
        throw ConfiguratieInvalidaException("ID-ul nivelului trebuie sa fie strict pozitiv!");
    }
    if (titluNivel.empty()) {
        throw ConfiguratieInvalidaException("Titlul etapei nu poate fi vid!");
    }
    if (scorMinimPromovare <= 0) {
        throw ConfiguratieInvalidaException("Scorul minim de promovare trebuie sa fie strict pozitiv!");
    }
    if (!componentaRecompensa) {
        throw ConfiguratieInvalidaException("Nivelul trebuie sa aiba o componenta electronica drept recompensa!");
    }
}

NivelTreasureHunt::NivelTreasureHunt()
    : idNivel(1),
      titluNivel("Nivel 1: Introducere in Circuitul Serei"),
      modulTeoretic("Teorie Fundamentala Circuite", 1, 30, true),
      intrebari(),
      componentaRecompensa(std::make_unique<PanouSolar>("Panou Solar Implicit", 12.0, 2.5, true, 0.18, 0.8, 30.0)),
      nivelCompletat(false),
      scorMinimPromovare(50) {
    valideaza();
}

NivelTreasureHunt::NivelTreasureHunt(int id, std::string titlu, ModulTeoretic modul,
                                     const ComponentaElectronica& recompensa, int scorMinim)
    : idNivel(id),
      titluNivel(std::move(titlu)),
      modulTeoretic(std::move(modul)),
      intrebari(),
      componentaRecompensa(recompensa.clone()),
      nivelCompletat(false),
      scorMinimPromovare(scorMinim) {
    valideaza();
}

NivelTreasureHunt::NivelTreasureHunt(const NivelTreasureHunt& altul)
    : idNivel(altul.idNivel),
      titluNivel(altul.titluNivel),
      modulTeoretic(altul.modulTeoretic),
      intrebari(altul.intrebari),
      componentaRecompensa(altul.componentaRecompensa ? altul.componentaRecompensa->clone() : nullptr),
      nivelCompletat(altul.nivelCompletat),
      scorMinimPromovare(altul.scorMinimPromovare) {
}

void swap(NivelTreasureHunt& first, NivelTreasureHunt& second) noexcept {
    using std::swap;
    swap(first.idNivel, second.idNivel);
    swap(first.titluNivel, second.titluNivel);
    swap(first.modulTeoretic, second.modulTeoretic);
    swap(first.intrebari, second.intrebari);
    swap(first.componentaRecompensa, second.componentaRecompensa);
    swap(first.nivelCompletat, second.nivelCompletat);
    swap(first.scorMinimPromovare, second.scorMinimPromovare);
}

NivelTreasureHunt& NivelTreasureHunt::operator=(NivelTreasureHunt altul) {
    swap(*this, altul);
    return *this;
}

NivelTreasureHunt::~NivelTreasureHunt() = default;

int NivelTreasureHunt::getIdNivel() const {
    return idNivel;
}

const std::string& NivelTreasureHunt::getTitluNivel() const {
    return titluNivel;
}

const ModulTeoretic& NivelTreasureHunt::getModulTeoretic() const {
    return modulTeoretic;
}

const ComponentaElectronica* NivelTreasureHunt::getComponentaRecompensa() const {
    return componentaRecompensa.get();
}

const std::vector<IntrebareQuiz>& NivelTreasureHunt::getIntrebari() const {
    return intrebari;
}

bool NivelTreasureHunt::getEsteCompletat() const {
    return nivelCompletat;
}

int NivelTreasureHunt::getScorMinimPromovare() const {
    return scorMinimPromovare;
}

void NivelTreasureHunt::adaugaIntrebare(IntrebareQuiz intrebareNoua) {
    intrebari.push_back(std::move(intrebareNoua));
}

int NivelTreasureHunt::rezolvaQuiz(const std::vector<int>& raspunsuriDate) const {
    if (raspunsuriDate.size() != intrebari.size()) {
        throw ConfiguratieInvalidaException("Numarul de raspunsuri oferite nu corespunde cu numarul de intrebari!");
    }

    int scorObtinut = 0;
    for (std::size_t i = 0; i < intrebari.size(); ++i) {
        scorObtinut += intrebari[i].evalueazaRaspuns(raspunsuriDate[i]);
    }
    return scorObtinut;
}

bool NivelTreasureHunt::finalizeazaNivel(int scorQuiz, int timpSecunde, int greseliQuiz) {
    if (scorQuiz < 0 || timpSecunde < 0 || greseliQuiz < 0) {
        throw ConfiguratieInvalidaException("Scorul, timpul si numarul greselilor nu pot fi negative!");
    }
    if (nivelCompletat) {
        return true;
    }

    if (!intrebari.empty() && scorQuiz >= scorMinimPromovare) {
        nivelCompletat = true;
        modulTeoretic.deblocheazaModul();
        return true;
    }

    return false;
}

int NivelTreasureHunt::determinaUrmatorulNivelDificultate(int scorTotal) const {
    const int pragAvansat = scorMinimPromovare + 50;
    const int dificultateCurenta = modulTeoretic.getDificultate();

    if (scorTotal >= pragAvansat) {
        if (dificultateCurenta < ModulTeoretic::dificultateMax) {
            return dificultateCurenta + 1;
        }
        return ModulTeoretic::dificultateMax;
    }

    if (scorTotal >= scorMinimPromovare) {
        return dificultateCurenta;
    }

    if (dificultateCurenta > ModulTeoretic::dificultateMin) {
        return dificultateCurenta - 1;
    }
    return ModulTeoretic::dificultateMin;
}

std::ostream& operator<<(std::ostream& out, const NivelTreasureHunt& nivel) {
    out << "========================================================\n"
        << "  ETAPA TREASURE HUNT #" << nivel.idNivel << ": " << nivel.titluNivel << "\n"
        << "  Status: " << (nivel.nivelCompletat ? "[COMPLETAT - DEBLOCAT]" : "[IN DESFASURARE]") << "\n"
        << "  Scor minim necesar: " << nivel.scorMinimPromovare << " puncte\n"
        << "--------------------------------------------------------\n"
        << "  MODUL DE STUDIU ASOCIAT:\n"
        << "  " << nivel.modulTeoretic << "\n"
        << "--------------------------------------------------------\n"
        << "  COMPONENTA PREMIU:\n";
    if (nivel.componentaRecompensa) {
        nivel.componentaRecompensa->afiseaza(out);
    } else {
        out << "  [Fara componenta premiu]\n";
    }
    out << "--------------------------------------------------------\n"
        << "  Numar provocari si intrebari active: " << nivel.intrebari.size() << "\n"
        << "========================================================";
    return out;
}
