#include <iostream>
#include <stdexcept>
#include <vector>

#include "ComponentaElectronica.h"
#include "IntrebareQuiz.h"
#include "ModulTeoretic.h"
#include "NivelTreasureHunt.h"

int main() {
    std::cout << "========================================================\n"
              << "   GHIDUL ELECTRICIANULUI - MISIUNEA SERA INTELIGENTA   \n"
              << "========================================================\n\n";

    // 1. Initializare modul de studiu pentru Etapa 1
    ModulTeoretic modulSolar("Alimentare Solara si Legea lui Ohm", 2, 50, true);
    modulSolar.afiseazaDetaliiEtapa();
    std::cout << "Stare modul teoretic: " << modulSolar << "\n";

    if (modulSolar.esteRecomandatPentruIncepatori()) {
        std::cout << "-> Modulul este accesibil pentru ucenici (dificultate: "
                  << modulSolar.getDificultate() << ").\n";
    }

    // 2. Componenta pusa in joc pentru circuitul final
    ComponentaElectronica panouSolar("Panou Fotovoltaic Monocristalin", "Sursa",
                                     50.0, "W", 18.0, true);
    std::cout << "\nRecompensa deblocabila in acest nivel:\n" << panouSolar << "\n";

    // Testare calcul curent maxim si siguranta
    const double rezistentaSarcina = 3.6;
    const double curentEstimativ = panouSolar.calculeazaCurentMaxim(rezistentaSarcina);
    std::cout << "Curent maxim debitat pe o sarcina de " << rezistentaSarcina
              << " Ohm: " << curentEstimativ << " A.\n";

    // 3. Creare Etapa / Nivel de Treasure Hunt (Compunere de obiecte)
    NivelTreasureHunt nivel1(1, "Atelierul 1: Captarea Energiei Solare",
                            modulSolar, panouSolar, 80);

    // 4. Adaugare intrebari si provocari in nivel
    IntrebareQuiz q1(
        "Ce marime se masoara in Amperi conform Legii lui Ohm?",
        {"Tensiunea electrica", "Intensitatea curentului electric", "Rezistenta electrica"},
        1, 30,
        "Indiciu circuit: Ai nevoie de conductoare groase pentru curenti mari."
    );

    IntrebareQuiz q2(
        "Daca tensiunea este 12V si rezistenta este 4 Ohm, ce curent trece prin circuit?",
        {"3 A", "48 A", "0.33 A"},
        0, 40,
        "Indiciu circuit: Formula fundamentala este I = U / R."
    );

    nivel1.adaugaIntrebare(q1);
    nivel1.adaugaIntrebare(q2);

    // 5. Afisare nivel compus (folosind operator<< compus)
    std::cout << "\n" << nivel1 << "\n";

    // 6. Simulare rezolvare quiz de catre jucator
    std::cout << "Jucatorul parcurge testul tehnic din nivelul 1...\n";
    std::vector<int> raspunsuriJucator = {1, 0}; // Ambele raspunsuri corecte
    const int scorQuiz = nivel1.rezolvaQuiz(raspunsuriJucator);
    std::cout << "Scor obtinut la quiz: " << scorQuiz << " puncte din 70 posibile.\n";

    // 7. Finalizare nivel si logica adaptiva
    const int timpRezolvareSecunde = 48; // rapid (< 60s -> bonus viteza)
    const int greseli = 0;
    const bool promovat = nivel1.finalizeazaNivel(scorQuiz, timpRezolvareSecunde, greseli);

    std::cout << "\nRezultat finalizare etapa: " << (promovat ? "PROMOVAT!" : "REPICAT!") << "\n";
    const int scorTotalEtapa = scorQuiz + nivel1.getModulTeoretic().calculeazaPunctajFinal(timpRezolvareSecunde, greseli);
    std::cout << "Punctaj total acumulat: " << scorTotalEtapa << " puncte.\n";

    const int urmatoareaDificultate = nivel1.determinaUrmatorulNivelDificultate(scorTotalEtapa);
    std::cout << "Algoritmul adaptiv recomanda pentru Nivelul 2 dificultatea: "
              << urmatoareaDificultate << " / 5.\n\n";

    // 8. Utilizare setteri si verificare siguranta pe componente
    ComponentaElectronica sigurantaAutomata;
    sigurantaAutomata.setDenumire("Siguranta Fuzibila Rapida");
    sigurantaAutomata.setTip("Protectie");
    sigurantaAutomata.setValoareNominala(10.0);
    sigurantaAutomata.setUnitateMasura("A");
    sigurantaAutomata.setTensiuneMaxima(24.0);
    sigurantaAutomata.setEsteFunctionala(false);

    std::cout << "Componenta defecta gasita in inventar:\n" << sigurantaAutomata << "\n";
    std::cout << "Se calibreaza si se repara siguranta...\n";
    sigurantaAutomata.reparaSauCalibreaza();
    std::cout << "Siguranta verificata la 20V: "
              << (sigurantaAutomata.verificaSigurantaFunctionare(20.0) ? "SIGURA" : "PERICOL DE ARDERE")
              << "\n\n";

    // 9. Testare mecanism de prindere a excepțiilor
    try {
        std::cout << "Incercam definirea unei componente cu tensiune maxima invalida (-5V)...\n";
        ComponentaElectronica compInvalida("Condensator", "Pasiv", 100, "uF", -5.0, true);
        (void)compInvalida;
    } catch (const std::invalid_argument& e) {
        std::cout << "Exceptie prinsa cu succes: " << e.what() << "\n\n";
    }

    // 10. Incarcare automata de date din fisierul tastatura.txt
    std::cout << "Incarcare date de configurare din fluxul de intrare (tastatura.txt):\n";
    ModulTeoretic modulIncarcat;
    ComponentaElectronica compIncarcata;

    if (std::cin >> modulIncarcat) {
        std::cout << "Modul citit: " << modulIncarcat.getDenumire()
                  << " (Puncte: " << modulIncarcat.getPuncte() << ")\n";
    }
    if (std::cin >> compIncarcata) {
        std::cout << "Componenta citita: " << compIncarcata.getDenumire()
                  << " (" << compIncarcata.getValoareNominala() << " "
                  << compIncarcata.getUnitateMasura() << ")\n";
    }

    std::cout << "\nToate etapele si verificarile au fost finalizate cu succes!\n";
    return 0;
}
