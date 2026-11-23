// Plain assert / static_assert test suite for all 4 classes in Ghidul Electricianului.
#undef NDEBUG
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ComponentaElectronica.h"
#include "IntrebareQuiz.h"
#include "ModulTeoretic.h"
#include "NivelTreasureHunt.h"

static_assert(ModulTeoretic::dificultateMin == 1, "Dificultatea minima trebuie sa fie 1");
static_assert(ModulTeoretic::dificultateMax == 5, "Dificultatea maxima trebuie sa fie 5");
static_assert(ModulTeoretic::puncteBonusPerIncrement == 10, "Bonusul per incrementare trebuie sa fie 10");

int main() {
    // =========================================================================
    // PARTEA 1: Testare ModulTeoretic
    // =========================================================================
    {
        ModulTeoretic mImplicit;
        assert(mImplicit.getDenumire() == "Necunoscut");
        assert(mImplicit.getDificultate() == 1);
        assert(mImplicit.getPuncte() == 0);
        assert(!mImplicit.getEsteDeblocat());
        assert(mImplicit.esteRecomandatPentruIncepatori());

        ModulTeoretic mParam("Circuite Serie-Paralel", 2, 50, true);
        assert(mParam.getDenumire() == "Circuite Serie-Paralel");
        assert(mParam.getDificultate() == 2);
        assert(mParam.getPuncte() == 50);
        assert(mParam.getEsteDeblocat());

        // Test validari si exceptii in constructor
        bool aAruncat = false;
        try {
            ModulTeoretic mInval("", 2, 50, true);
            (void)mInval;
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "Constructorul refuza denumire vida");

        aAruncat = false;
        try {
            ModulTeoretic mInval("Test", 9, 50, true);
            (void)mInval;
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "Constructorul refuza dificultate > 5");

        // Constructor de copiere si operator=
        ModulTeoretic copie(mParam);
        assert(copie.getDenumire() == mParam.getDenumire());
        const ModulTeoretic* ptrCopie = &copie;
        copie = *ptrCopie; // auto-atribuire testata in siguranta
        assert(copie.getDenumire() == mParam.getDenumire());

        // Setteri si validari
        mImplicit.setDenumire("Instalatii Domestice");
        mImplicit.setDificultate(4);
        mImplicit.setPuncte(80);
        mImplicit.setEsteDeblocat(true);
        assert(mImplicit.getDenumire() == "Instalatii Domestice");
        assert(mImplicit.getDificultate() == 4);
        assert(mImplicit.getPuncte() == 80);
        assert(mImplicit.getEsteDeblocat());

        // Operator ++
        const ModulTeoretic& ref = ++mImplicit;
        assert(&ref == &mImplicit);
        assert(mImplicit.getPuncte() == 90);

        // Calcul punctaj adaptiv
        assert(mParam.calculeazaPunctajFinal(40, 0) == 115); // 50 * 2 + 15 bonus viteza
        assert(mParam.calculeazaPunctajFinal(80, 50) == 0);  // penalizat sub 0 -> devine 0

        aAruncat = false;
        try {
            (void)mParam.calculeazaPunctajFinal(-5, 0);
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "calculeazaPunctajFinal refuza timp negativ");

        // Afisare pe stream
        std::ostringstream oss;
        mParam.afiseazaDetaliiEtapa(oss);
        assert(oss.str().find("Circuite") != std::string::npos);

        // Stream >>
        std::istringstream iss("Trifazat 3 70 1");
        assert(iss >> mParam);
        assert(mParam.getDenumire() == "Trifazat");
    }

    // =========================================================================
    // PARTEA 2: Testare ComponentaElectronica
    // =========================================================================
    {
        ComponentaElectronica compImplicita;
        assert(compImplicita.getDenumire() == "Componenta Generica");
        assert(compImplicita.getTip() == "Pasiv");
        assert(compImplicita.getValoareNominala() == 0.0);
        assert(compImplicita.getUnitateMasura() == "Ohm");
        assert(compImplicita.getTensiuneMaxima() == 12.0);
        assert(compImplicita.getEsteFunctionala());

        ComponentaElectronica panou("Panou Solar", "Sursa", 100.0, "W", 24.0, true);
        assert(panou.getDenumire() == "Panou Solar");
        assert(panou.getTip() == "Sursa");
        assert(panou.getValoareNominala() == 100.0);
        assert(panou.getUnitateMasura() == "W");
        assert(panou.getTensiuneMaxima() == 24.0);
        assert(panou.getEsteFunctionala());

        // Validari si exceptii constructor
        bool aAruncat = false;
        try {
            ComponentaElectronica invalida("", "Pasiv", 10.0, "Ohm", 12.0, true);
            (void)invalida;
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "Componenta refuza denumire vida");

        aAruncat = false;
        try {
            ComponentaElectronica invalida("Rezistor", "Pasiv", -1.0, "Ohm", 12.0, true);
            (void)invalida;
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "Componenta refuza valoare nominala negativa");

        aAruncat = false;
        try {
            ComponentaElectronica invalida("Rezistor", "Pasiv", 10.0, "Ohm", -5.0, true);
            (void)invalida;
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "Componenta refuza tensiune maxima <= 0");

        // Setteri si verificari
        panou.setDenumire("Panou Solar 120W");
        panou.setTip("SursaRegenerabila");
        panou.setValoareNominala(120.0);
        panou.setUnitateMasura("Watt");
        panou.setTensiuneMaxima(30.0);
        panou.setEsteFunctionala(false);

        assert(panou.getDenumire() == "Panou Solar 120W");
        assert(panou.getTip() == "SursaRegenerabila");
        assert(panou.getValoareNominala() == 120.0);
        assert(panou.getUnitateMasura() == "Watt");
        assert(panou.getTensiuneMaxima() == 30.0);
        assert(!panou.getEsteFunctionala());

        // Functionalitate reparare/calibrare
        panou.reparaSauCalibreaza();
        assert(panou.getEsteFunctionala());

        // Verificare siguranta functionare
        assert(panou.verificaSigurantaFunctionare(25.0) && "Tensiunea 25V <= 30V este sigura");
        assert(!panou.verificaSigurantaFunctionare(35.0) && "Tensiunea 35V > 30V este periculoasa");

        // Calcul curent maxim (Legea lui Ohm)
        const double curentMaxim = panou.calculeazaCurentMaxim(10.0);
        assert(curentMaxim == 3.0 && "30V / 10 Ohm = 3A");

        aAruncat = false;
        try {
            (void)panou.calculeazaCurentMaxim(0.0);
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "calculeazaCurentMaxim refuza rezistenta 0");

        // Operator <<
        std::ostringstream oss;
        oss << panou;
        assert(oss.str().find("Panou Solar") != std::string::npos);

        // Operator >>
        std::istringstream iss("Dioda Semiconductor 0.7 V 50.0 1");
        ComponentaElectronica dioda;
        assert(iss >> dioda);
        assert(dioda.getDenumire() == "Dioda");
        assert(dioda.getValoareNominala() == 0.7);
        assert(dioda.getTensiuneMaxima() == 50.0);
    }

    // =========================================================================
    // PARTEA 3: Testare IntrebareQuiz
    // =========================================================================
    {
        IntrebareQuiz qImplicita;
        assert(qImplicita.getEnunt() == "Intrebare implicita");
        assert(qImplicita.getVariante().size() == 2);
        assert(qImplicita.getIndexRaspunsCorect() == 0);
        assert(qImplicita.getPunctaj() == 10);
        assert(!qImplicita.getIndiciu().empty());

        IntrebareQuiz q("Ce unitate de masura are rezistenta electrica?",
                        {"Amper", "Ohm", "Volt"}, 1, 25, "Formula U = R * I");

        assert(q.getEnunt() == "Ce unitate de masura are rezistenta electrica?");
        assert(q.getVariante().size() == 3);
        assert(q.getIndexRaspunsCorect() == 1);
        assert(q.getPunctaj() == 25);
        assert(q.getIndiciu() == "Formula U = R * I");

        // Verificare raspuns
        assert(q.verificaRaspuns(1));
        assert(!q.verificaRaspuns(0));
        assert(q.evalueazaRaspuns(1) == 25);
        assert(q.evalueazaRaspuns(0) == 0);

        // Exceptii la parametri invalizi
        bool aAruncat = false;
        try {
            IntrebareQuiz qInval("", {"A", "B"}, 0, 10, "indiciu");
            (void)qInval;
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "IntrebareQuiz refuza enunt vid");

        aAruncat = false;
        try {
            IntrebareQuiz qInval("Enunt", {"DoarUna"}, 0, 10, "indiciu");
            (void)qInval;
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "IntrebareQuiz refuza mai putin de 2 variante");

        aAruncat = false;
        try {
            IntrebareQuiz qInval("Enunt", {"A", "B"}, 5, 10, "indiciu");
            (void)qInval;
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "IntrebareQuiz refuza index raspuns peste limita");

        // Operator <<
        std::ostringstream oss;
        oss << q;
        assert(oss.str().find("rezistenta electrica") != std::string::npos);
        assert(oss.str().find("Ohm") != std::string::npos);
    }

    // =========================================================================
    // PARTEA 4: Testare NivelTreasureHunt (Compunere de obiecte)
    // =========================================================================
    {
        NivelTreasureHunt nivelImplicit;
        assert(nivelImplicit.getIdNivel() == 1);
        assert(nivelImplicit.getTitluNivel() == "Nivel 1");
        assert(!nivelImplicit.getEsteCompletat());
        assert(nivelImplicit.getScorMinimPromovare() == 50);

        ModulTeoretic modul("Sigurante Automate", 2, 40, false);
        ComponentaElectronica sig("Siguranta 16A", "Protectie", 16.0, "A", 230.0, true);
        NivelTreasureHunt nivel(2, "Atelierul de Protectii Electrice", modul, sig, 90);

        assert(nivel.getIdNivel() == 2);
        assert(nivel.getTitluNivel() == "Atelierul de Protectii Electrice");
        assert(nivel.getModulTeoretic().getDenumire() == "Sigurante Automate");
        assert(nivel.getComponentaRecompensa().getDenumire() == "Siguranta 16A");
        assert(!nivel.getEsteCompletat());
        assert(nivel.getScorMinimPromovare() == 90);

        // Adaugare intrebari
        IntrebareQuiz q1("Ce face o siguranta automata la suprasarcina?",
                         {"Isi creste rezistenta", "Intrerupe circuitul", "Creste tensiunea"},
                         1, 40, "Protectie termica.");
        IntrebareQuiz q2("Ce curent nominal are siguranta studiata?",
                         {"16A", "32A", "6A"},
                         0, 30, "Verifica inscriptia de pe carcasa.");

        nivel.adaugaIntrebare(q1);
        nivel.adaugaIntrebare(q2);
        assert(nivel.getIntrebari().size() == 2);

        // Rezolvare quiz
        std::vector<int> raspunsuriCorecte = {1, 0};
        assert(nivel.rezolvaQuiz(raspunsuriCorecte) == 70);

        std::vector<int> raspunsuriPartiale = {1, 1}; // al doilea e gresit
        assert(nivel.rezolvaQuiz(raspunsuriPartiale) == 40);

        // Exceptie daca vectorul de raspunsuri nu are aceeasi dimensiune
        bool aAruncat = false;
        try {
            (void)nivel.rezolvaQuiz({1});
        } catch (const std::invalid_argument&) {
            aAruncat = true;
        }
        assert(aAruncat && "rezolvaQuiz arunca exceptie pe numar necorespunzator de raspunsuri");

        // Finalizare nivel cu scor insuficient (< 90 puncte)
        // Teorie: 40 * 2 = 80; cu 10 greseli -> 80 - 50 = 30; timp 120s -> 30 puncte teorie.
        // Quiz: 40 puncte => total 70 < 90.
        bool promovat = nivel.finalizeazaNivel(40, 120, 10);
        assert(!promovat && !nivel.getEsteCompletat());

        // Finalizare nivel cu scor excelent (>= 90 puncte)
        // Quiz: 70 puncte; Teorie: 40 * 2 + 15 (bonus viteza < 60s) = 95 => total 165.
        promovat = nivel.finalizeazaNivel(70, 45, 0);
        assert(promovat && nivel.getEsteCompletat());
        assert(nivel.getModulTeoretic().getEsteDeblocat());

        // Rutare adaptiva invatare
        assert(nivel.determinaUrmatorulNivelDificultate(165) == 3 && "Scor mare => avansare la dificultate 3");
        assert(nivel.determinaUrmatorulNivelDificultate(95) == 2 && "Scor normal => pastrare dificultate 2");
        assert(nivel.determinaUrmatorulNivelDificultate(50) == 1 && "Scor insuficient => consolidare la dificultate 1");

        // Testare operator << compus
        std::ostringstream oss;
        oss << nivel;
        assert(oss.str().find("Atelierul de Protectii Electrice") != std::string::npos);
        assert(oss.str().find("Sigurante Automate") != std::string::npos);
        assert(oss.str().find("Siguranta 16A") != std::string::npos);
    }

    return 0;
}
