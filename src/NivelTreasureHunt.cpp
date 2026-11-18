#include "NivelTreasureHunt.h"

#include <algorithm>
#include <ostream>
#include <stdexcept>
#include <utility>

void NivelTreasureHunt::valideaza() const {
    if (idNivel <= 0) {
        throw std::invalid_argument("ID-ul nivelului trebuie sa fie strict pozitiv!");
    }
    if (titluNivel.empty()) {
        throw std::invalid_argument("Titlul nivelului nu poate fi vid!");
    }
    if (scorMinimPromovare <= 0) {
        throw std::invalid_argument("Scorul minim de promovare trebuie sa fie pozitiv!");
    }
}

NivelTreasureHunt::NivelTreasureHunt() = default;

NivelTreasureHunt::NivelTreasureHunt(int id, std::string titlu, ModulTeoretic modul,
                                     ComponentaElectronica recompensa, int scorMinim)
    : idNivel(id),
      titluNivel(std::move(titlu)),
      modulTeoretic(std::move(modul)),
      intrebari(),
      componentaRecompensa(std::move(recompensa)),
      nivelCompletat(false),
      scorMinimPromovare(scorMinim) {
    valideaza();
}

int NivelTreasureHunt::getIdNivel() const {
    return idNivel;
}

const std::string& NivelTreasureHunt::getTitluNivel() const {
    return titluNivel;
}

const ModulTeoretic& NivelTreasureHunt::getModulTeoretic() const {
    return modulTeoretic;
}

const ComponentaElectronica& NivelTreasureHunt::getComponentaRecompensa() const {
    return componentaRecompensa;
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
        throw std::invalid_argument("Numarul de raspunsuri nu corespunde cu numarul de intrebari!");
    }
    int scorTotal = 0;
    for (std::size_t i = 0; i < intrebari.size(); ++i) {
        scorTotal += intrebari[i].evalueazaRaspuns(raspunsuriDate[i]);
    }
    return scorTotal;
}

bool NivelTreasureHunt::finalizeazaNivel(int scorQuiz, int timpSecunde, int greseliQuiz) {
    const int punctajTeorie = modulTeoretic.calculeazaPunctajFinal(timpSecunde, greseliQuiz);
    const int punctajTotal = scorQuiz + punctajTeorie;

    if (punctajTotal >= scorMinimPromovare) {
        modulTeoretic.deblocheazaModul();
        nivelCompletat = true;
        return true;
    }
    return false;
}

int NivelTreasureHunt::determinaUrmatorulNivelDificultate(int scorTotal) const {
    if (scorTotal >= 120) {
        return std::min(ModulTeoretic::dificultateMax, modulTeoretic.getDificultate() + 1);
    }
    if (scorTotal >= scorMinimPromovare) {
        return modulTeoretic.getDificultate();
    }
    return std::max(ModulTeoretic::dificultateMin, modulTeoretic.getDificultate() - 1);
}

std::ostream& operator<<(std::ostream& out, const NivelTreasureHunt& nivel) {
    out << "========================================\n"
        << "Etapa Treasure Hunt [" << nivel.idNivel << "]: " << nivel.titluNivel << "\n"
        << "Status: " << (nivel.nivelCompletat ? "COMPLETAT" : "IN DESFASURARE") << "\n"
        << "Scor minim necesar: " << nivel.scorMinimPromovare << " puncte\n"
        << "--- Modul de studiu inclus ---\n"
        << nivel.modulTeoretic << "\n"
        << "--- Recompensa la finalizare ---\n"
        << nivel.componentaRecompensa << "\n"
        << "--- Numar provocari/intrebari: " << nivel.intrebari.size() << " ---\n"
        << "========================================\n";
    return out;
}
