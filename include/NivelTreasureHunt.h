#ifndef NIVEL_TREASURE_HUNT_H
#define NIVEL_TREASURE_HUNT_H

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "ComponentaElectronica.h"
#include "IntrebareQuiz.h"
#include "ModulTeoretic.h"

class NivelTreasureHunt {
private:
    int idNivel = 1;
    std::string titluNivel = "Nivel 1";
    ModulTeoretic modulTeoretic;
    std::vector<IntrebareQuiz> intrebari;
    std::unique_ptr<ComponentaElectronica> componentaRecompensa;
    bool nivelCompletat = false;
    int scorMinimPromovare = 50;

    void valideaza() const;

public:
    // Constructori
    NivelTreasureHunt();
    NivelTreasureHunt(int id, std::string titlu, ModulTeoretic modul,
                      const ComponentaElectronica& recompensa, int scorMinim);

    // Regula celor 3/5: Copy-and-Swap pentru gestionarea corecta a pointerului de baza
    NivelTreasureHunt(const NivelTreasureHunt& altul);
    friend void swap(NivelTreasureHunt& first, NivelTreasureHunt& second) noexcept;
    NivelTreasureHunt& operator=(NivelTreasureHunt altul);
    NivelTreasureHunt(NivelTreasureHunt&& altul) noexcept = default;
    NivelTreasureHunt& operator=(NivelTreasureHunt&& altul) noexcept = default;
    ~NivelTreasureHunt();

    // Getteri
    [[nodiscard]] int getIdNivel() const;
    [[nodiscard]] const std::string& getTitluNivel() const;
    [[nodiscard]] const ModulTeoretic& getModulTeoretic() const;
    [[nodiscard]] const ComponentaElectronica* getComponentaRecompensa() const;
    [[nodiscard]] const std::vector<IntrebareQuiz>& getIntrebari() const;
    [[nodiscard]] bool getEsteCompletat() const;
    [[nodiscard]] int getScorMinimPromovare() const;

    // Metode specifice temei
    void adaugaIntrebare(IntrebareQuiz intrebareNoua);
    [[nodiscard]] int rezolvaQuiz(const std::vector<int>& raspunsuriDate) const;
    [[nodiscard]] bool finalizeazaNivel(int scorQuiz, int timpSecunde, int greseliQuiz);
    [[nodiscard]] int determinaUrmatorulNivelDificultate(int scorTotal) const;

    // Operator compus de afisare
    friend std::ostream& operator<<(std::ostream& out, const NivelTreasureHunt& nivel);
};

#endif // NIVEL_TREASURE_HUNT_H
