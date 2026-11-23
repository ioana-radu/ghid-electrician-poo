#undef NDEBUG
#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "ActuatorSera.h"
#include "AcumulatorLitiu.h"
#include "CircuitElectric.h"
#include "ComponentaElectronica.h"
#include "EroriCircuit.h"
#include "IntrebareQuiz.h"
#include "ModulTeoretic.h"
#include "NivelTreasureHunt.h"
#include "PanouSolar.h"
#include "SenzorMonitorizare.h"

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

        // Constructor de copiere si operator=
        ModulTeoretic copie(mParam);
        assert(copie.getDenumire() == mParam.getDenumire());
        const ModulTeoretic* ptrCopie = &copie;
        copie = *ptrCopie;
        assert(copie.getDenumire() == mParam.getDenumire());

        // Modificari
        mImplicit.setDenumire("Instalatii Domestice");
        mImplicit.setDificultate(4);
        mImplicit.setPuncte(80);
        mImplicit.setEsteDeblocat(true);
        assert(mImplicit.getDenumire() == "Instalatii Domestice");
        assert(mImplicit.getDificultate() == 4);

        // Operator ++
        const ModulTeoretic& ref = ++mImplicit;
        assert(&ref == &mImplicit);
        assert(mImplicit.getPuncte() == 90);

        // Calcul punctaj adaptiv
        assert(mParam.calculeazaPunctajFinal(40, 0) == 115);
        assert(mParam.calculeazaPunctajFinal(80, 50) == 0);

        // Afisare stream
        std::ostringstream oss;
        mParam.afiseazaDetaliiEtapa(oss);
        assert(oss.str().find("Circuite") != std::string::npos);
    }

    // =========================================================================
    // PARTEA 2: Testare Membri Statici ComponentaElectronica
    // =========================================================================
    {
        assert(ComponentaElectronica::esteTensiuneInGamaStandard(12.0));
        assert(ComponentaElectronica::esteTensiuneInGamaStandard(24.0));
        assert(ComponentaElectronica::esteTensiuneInGamaStandard(230.0));
        assert(!ComponentaElectronica::esteTensiuneInGamaStandard(1.0));
        assert(!ComponentaElectronica::esteTensiuneInGamaStandard(400.0));
    }

    // =========================================================================
    // PARTEA 3: Testare Derivata 1 - PanouSolar
    // =========================================================================
    {
        const int contorInainte = ComponentaElectronica::getContorComponente();

        PanouSolar panou("Panou Monocristalin 200W", 24.0, 8.5, true, 0.20, 1.5, 30.0);
        assert(ComponentaElectronica::getContorComponente() == contorInainte + 1);

        assert(panou.getDenumire() == "Panou Monocristalin 200W");
        assert(panou.getTensiuneNominala() == 24.0);
        assert(panou.getCurentMaxim() == 8.5);
        assert(panou.getEsteFunctionala());
        assert(panou.getEficientaFotovoltaica() == 0.20);
        assert(panou.getSuprafataMp() == 1.5);
        assert(panou.getUnghiOrientareGrade() == 30.0);
        assert(panou.getTipComponenta() == "Sursa Fotovoltaica");

        // Putere maxima teoretica: 1000 * 1.5 * 0.20 = 300 W
        assert(panou.calculeazaPutereMaximaTeoretica() == 300.0);

        // Simulare functionare la 800 W/m^2
        const double putereProdusa = panou.simuleazaFunctionare(800.0, 24.0);
        assert(putereProdusa > 0.0);

        // Testare marcare defecta si reparare din clasa de baza
        panou.marcheazaDefecta();
        assert(!panou.getEsteFunctionala());
        panou.reparaSauCalibreaza();
        assert(panou.getEsteFunctionala());

        // Optimizare unghi
        panou.optimizeazaUnghiOrientare(45.0);
        assert(panou.getUnghiOrientareGrade() == 45.0);

        // Clonare (Virtual Copy Constructor)
        std::unique_ptr<ComponentaElectronica> clona = panou.clone();
        assert(clona != nullptr);
        assert(clona->getDenumire() == panou.getDenumire());

        // Afisare NVI
        std::ostringstream oss;
        oss << panou;
        assert(oss.str().find("Sursa Fotovoltaica") != std::string::npos);
        assert(oss.str().find("Eficienta celule") != std::string::npos);
    }

    // =========================================================================
    // PARTEA 4: Testare Derivata 2 - SenzorMonitorizare
    // =========================================================================
    {
        SenzorMonitorizare senzor("Senzor Umiditate Sol", 5.0, 0.05, true, "Umiditate Sol", 40.0, 2.0);
        assert(senzor.getDenumire() == "Senzor Umiditate Sol");
        assert(senzor.getMarimeMasurata() == "Umiditate Sol");
        assert(senzor.getPragAlerta() == 40.0);
        assert(senzor.getValoareCalibrareOffset() == 2.0);
        assert(senzor.getTipComponenta() == "Senzor Monitorizare");

        // Simulare functionare si verificare alerta
        const double consum = senzor.simuleazaFunctionare(35.0, 5.0);
        assert(consum > 0.0);
        assert(!senzor.esteStareCritica(35.0)); // 35 + 2 = 37 < 40
        assert(senzor.esteStareCritica(39.0));  // 39 + 2 = 41 >= 40

        // Autocalibrare
        senzor.autocalibrare(5.0);
        assert(senzor.getValoareCalibrareOffset() == -5.0);

        // Clonare
        std::unique_ptr<ComponentaElectronica> clonaSenzor = senzor.clone();
        assert(clonaSenzor != nullptr);
        assert(clonaSenzor->getDenumire() == senzor.getDenumire());

        // Afisare NVI
        std::ostringstream oss;
        oss << senzor;
        assert(oss.str().find("Senzor Monitorizare") != std::string::npos);
        assert(oss.str().find("Prag de alerta") != std::string::npos);
    }

    // =========================================================================
    // PARTEA 5: Testare Derivata 3 - ActuatorSera
    // =========================================================================
    {
        ActuatorSera actuator("Pompa Submersibila", 24.0, 2.5, true, "Irigare", 60.0, 10);
        assert(actuator.getDenumire() == "Pompa Submersibila");
        assert(actuator.getRolSera() == "Irigare");
        assert(actuator.getPutereNominalaW() == 60.0);
        assert(actuator.getTotalCicluriComutare() == 10);
        assert(actuator.getTipComponenta() == "Actuator Executie");

        // Simulare functionare timp de 30 minute -> consum 30 Wh
        const double energieWh = actuator.simuleazaFunctionare(30.0, 24.0);
        assert(energieWh == 30.0);
        assert(actuator.getTotalCicluriComutare() == 11);

        // Testare fortata
        actuator.executaCicluTestareFortata();
        assert(actuator.getTotalCicluriComutare() == 12);

        actuator.reseteazaContorCicluri();
        assert(actuator.getTotalCicluriComutare() == 0);

        // Clonare
        std::unique_ptr<ComponentaElectronica> clonaActuator = actuator.clone();
        assert(clonaActuator != nullptr);
        assert(clonaActuator->getDenumire() == actuator.getDenumire());

        // Afisare NVI
        std::ostringstream oss;
        oss << actuator;
        assert(oss.str().find("Actuator Executie") != std::string::npos);
        assert(oss.str().find("Rol functional: Irigare") != std::string::npos);
    }

    // =========================================================================
    // PARTEA 6: Testare Derivata 4 - AcumulatorLitiu
    // =========================================================================
    {
        AcumulatorLitiu baterie("Banc LiFePO4", 24.0, 50.0, true, 100.0, 80.0, 10);
        assert(baterie.getDenumire() == "Banc LiFePO4");
        assert(baterie.getCapacitateAmperiOra() == 100.0);
        assert(baterie.getNivelIncarcareProcent() == 80.0);
        assert(baterie.getCicluriIncarcare() == 10);
        assert(baterie.getTipComponenta() == "Acumulator Energie");

        // Energie utila la 80% SoC: 100 Ah * 24 V * 0.8 = 1920 Wh
        const double energieDisponibila = baterie.simuleazaFunctionare(500.0, 24.0);
        assert(energieDisponibila == 1920.0);
        assert(baterie.getCicluriIncarcare() == 11);

        // Reincarcare maxima: 100 Ah * 24 V = 2400 Wh
        baterie.reincarcaLaCapacitateMaxima();
        assert(baterie.getNivelIncarcareProcent() == 100.0);
        assert(baterie.calculeazaEnergieTotalaWh() == 2400.0);

        // Clonare
        std::unique_ptr<ComponentaElectronica> clonaBaterie = baterie.clone();
        assert(clonaBaterie != nullptr);
        assert(clonaBaterie->getDenumire() == baterie.getDenumire());

        // Afisare NVI
        std::ostringstream oss;
        oss << baterie;
        assert(oss.str().find("Acumulator Energie") != std::string::npos);
        assert(oss.str().find("Capacitate stocare") != std::string::npos);
    }

    // =========================================================================
    // PARTEA 7: Testare CircuitElectric (Pointer de baza, Copy-and-Swap, Dynamic Cast)
    // =========================================================================
    {
        CircuitElectric circuit("Circuit Sera A1", 24.0);
        assert(circuit.getDenumireCircuit() == "Circuit Sera A1");
        assert(circuit.getTensiuneAlimentare() == 24.0);
        assert(circuit.getComponentaPrincipala() == nullptr);

        PanouSolar panou("Panou Sursa", 24.0, 10.0, true, 0.20, 2.0, 30.0);
        SenzorMonitorizare senzor("Senzor Temperatura", 24.0, 0.05, true, "Temperatura", 35.0, 0.0);
        ActuatorSera pompa("Pompa Picurare", 24.0, 2.0, true, "Irigare", 48.0, 5);

        circuit.conecteazaComponentaPrincipala(panou.clone());
        circuit.adaugaComponentaSecundara(senzor.clone());
        circuit.adaugaComponentaSecundara(pompa.clone());

        assert(circuit.getComponentaPrincipala() != nullptr);
        assert(circuit.getComponenteSecundare().size() == 2);

        // Simulare polimorfica prin pointerul de baza
        const double bilant = circuit.simuleazaSarcinaCircuit(500.0);
        assert(bilant > 0.0);

        // Downcast cu dynamic_cast
        circuit.optimizeazaSauCalibreazaEchipamente();

        // Copy constructor deep-copy
        CircuitElectric copieCircuit(circuit);
        assert(copieCircuit.getDenumireCircuit() == circuit.getDenumireCircuit());
        assert(copieCircuit.getComponentaPrincipala() != nullptr);
        assert(copieCircuit.getComponentaPrincipala() != circuit.getComponentaPrincipala()); // adrese diferite!
        assert(copieCircuit.getComponenteSecundare().size() == 2);

        // Copy-and-swap assignment
        CircuitElectric altCircuit("Circuit Nou", 12.0);
        altCircuit = copieCircuit;
        assert(altCircuit.getDenumireCircuit() == "Circuit Sera A1");
        assert(altCircuit.getComponentaPrincipala() != nullptr);
        // cppcheck-suppress knownConditionTrueFalse
        assert(altCircuit.getComponentaPrincipala() != copieCircuit.getComponentaPrincipala());

        // Afisare schema circuit NVI
        std::ostringstream oss;
        oss << circuit;
        assert(oss.str().find("SCHEMA CIRCUIT: Circuit Sera A1") != std::string::npos);
        assert(oss.str().find("Panou Sursa") != std::string::npos);
        assert(oss.str().find("Senzor Temperatura") != std::string::npos);
    }

    // =========================================================================
    // PARTEA 8: Testare Ierarhie Excepții
    // =========================================================================
    {
        // 1. TensiuneInvalidaException
        bool prins = false;
        try {
            PanouSolar p("Panou", 24.0, 5.0, true, 0.20, 1.0, 30.0);
            (void)p.simuleazaFunctionare(800.0, 100.0); // 100V pe panou de 24V -> TensiuneInvalidaException
        } catch (const TensiuneInvalidaException& ex) {
            prins = true;
            assert(std::string(ex.what()).find("Eroare Tensiune") != std::string::npos);
        }
        assert(prins && "A prins corect TensiuneInvalidaException");

        // 2. ComponentaDefectaException
        prins = false;
        try {
            ActuatorSera a("Pompa", 24.0, 2.0, false, "Irigare", 50.0); // defecta!
            (void)a.simuleazaFunctionare(10.0, 24.0);
        } catch (const ComponentaDefectaException& ex) {
            prins = true;
            assert(std::string(ex.what()).find("Eroare Componenta Hardware") != std::string::npos);
        }
        assert(prins && "A prins corect ComponentaDefectaException");

        // 3. ConfiguratieInvalidaException
        prins = false;
        try {
            PanouSolar p("Panou", 24.0, 5.0, true, 1.5, 1.0, 30.0); // eficienta 150% -> invalid
            (void)p;
        } catch (const ConfiguratieInvalidaException& ex) {
            prins = true;
            assert(std::string(ex.what()).find("Eroare Configurare Parametri") != std::string::npos);
        }
        assert(prins && "A prins corect ConfiguratieInvalidaException");
    }

    // =========================================================================
    // PARTEA 9: Testare IntrebareQuiz
    // =========================================================================
    {
        IntrebareQuiz q("Ce unitate de masura are rezistenta electrica?",
                        {"Amper", "Ohm", "Volt"}, 1, 25, "Formula U = R * I");
        assert(q.getEnunt() == "Ce unitate de masura are rezistenta electrica?");
        assert(q.getVariante().size() == 3);
        assert(q.getIndexRaspunsCorect() == 1);
        assert(q.getPunctaj() == 25);
        assert(q.getIndiciu() == "Formula U = R * I");
        assert(q.verificaRaspuns(1));
        assert(!q.verificaRaspuns(0));
        assert(q.evalueazaRaspuns(1) == 25);
        assert(q.evalueazaRaspuns(0) == 0);
    }

    // =========================================================================
    // PARTEA 10: Testare NivelTreasureHunt
    // =========================================================================
    {
        ModulTeoretic modul("Sigurante Automate", 2, 40, false);
        PanouSolar recompensa("Panou Recompensa", 24.0, 5.0, true, 0.20, 1.0, 30.0);
        NivelTreasureHunt nivel(2, "Atelierul de Protectii Electrice", modul, recompensa, 90);

        assert(nivel.getIdNivel() == 2);
        assert(nivel.getTitluNivel() == "Atelierul de Protectii Electrice");
        assert(nivel.getModulTeoretic().getDenumire() == "Sigurante Automate");
        assert(nivel.getComponentaRecompensa() != nullptr);
        assert(nivel.getComponentaRecompensa()->getDenumire() == "Panou Recompensa");
        assert(nivel.getScorMinimPromovare() == 90);

        // Adaugare intrebari
        nivel.adaugaIntrebare(IntrebareQuiz("Intrebare 1", {"A", "B"}, 0, 40, "Indiciu"));
        nivel.adaugaIntrebare(IntrebareQuiz("Intrebare 2", {"A", "B"}, 1, 50, "Indiciu"));
        assert(nivel.getIntrebari().size() == 2);

        assert(nivel.rezolvaQuiz({0, 1}) == 90);
        assert(nivel.rezolvaQuiz({0, 0}) == 40);

        // Rutare adaptiva
        assert(nivel.determinaUrmatorulNivelDificultate(160) == 3);
        assert(nivel.determinaUrmatorulNivelDificultate(90) == 2);
        assert(nivel.determinaUrmatorulNivelDificultate(40) == 1);

        // Finalizare nivel
        const bool promovat = nivel.finalizeazaNivel(90, 45, 0);
        assert(promovat);
        assert(nivel.getEsteCompletat());

        // Copy-and-swap pe NivelTreasureHunt
        NivelTreasureHunt copieNivel = nivel;
        assert(copieNivel.getComponentaRecompensa() != nullptr);
        // cppcheck-suppress knownConditionTrueFalse
        assert(copieNivel.getComponentaRecompensa() != nivel.getComponentaRecompensa());
        assert(copieNivel.getComponentaRecompensa()->getDenumire() == "Panou Recompensa");
    }

    return 0;
}
