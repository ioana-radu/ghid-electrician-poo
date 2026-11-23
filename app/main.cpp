#include <iostream>
#include <memory>
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

int main() {
    std::cout << "========================================================\n"
              << "   GHIDUL ELECTRICIANULUI - MISIUNEA SERA INTELIGENTA   \n"
              << "                 ETAPA 2: ARHITECTURA POO               \n"
              << "========================================================\n\n";

    // -------------------------------------------------------------------------
    // 1. Membri statici: Verificare contor componente create si utilitar static
    // -------------------------------------------------------------------------
    std::cout << "[1] Verificare membri statici:\n";
    std::cout << "  - Componente electronice active la start: "
              << ComponentaElectronica::getContorComponente() << "\n";
    std::cout << "  - Tensiunea de 24V este in gama standard de operare: "
              << (ComponentaElectronica::esteTensiuneInGamaStandard(24.0) ? "DA" : "NU")
              << "\n\n";

    // -------------------------------------------------------------------------
    // 2. Creare obiecte polimorfice derivate din ComponentaElectronica
    //    (Apel constructori baza din constructori derivate)
    // -------------------------------------------------------------------------
    std::cout << "[2] Initializare componente polimorfice (Ierarhie de mostenire):\n";

    PanouSolar panouPrincipal("Panou Fotovoltaic Monocristalin", 24.0, 10.0, true, 0.22, 1.8, 30.0);
    SenzorMonitorizare senzorSol("Senzor Umiditate Industrial 24V", 24.0, 0.05, true, "Umiditate Sol", 40.0, 0.0);
    ActuatorSera pompaIrigare("Pompa Submersibila 24V", 24.0, 3.5, true, "Irigare Sera", 60.0, 0);
    AcumulatorLitiu baterieBanc("Banc Acumulatori LiFePO4", 24.0, 40.0, true, 100.0, 85.0, 12);

    std::cout << "  - Numar total componente create in memorie: "
              << ComponentaElectronica::getContorComponente() << "\n\n";

    // -------------------------------------------------------------------------
    // 3. Testare afisare polimorfica prin NVI (Non-Virtual Interface)
    // -------------------------------------------------------------------------
    std::cout << "[3] Afisare componente prin interfata Non-Virtuala (operator<<):\n";
    std::cout << panouPrincipal << "\n";
    std::cout << senzorSol << "\n";
    std::cout << pompaIrigare << "\n";
    std::cout << baterieBanc << "\n";

    // -------------------------------------------------------------------------
    // 4. Clasa cu atribut pointer la clasa de baza: CircuitElectric
    //    (Gestionare prin smart pointers: std::unique_ptr)
    // -------------------------------------------------------------------------
    std::cout << "[4] Asamblare circuit comanda sera (Clasa cu pointer de baza):\n";
    CircuitElectric circuitSera("Sistem Central de Alimentare si Automatizare", 24.0);

    // Conectam panoul fotovoltaic ca sursa principala (folosim constructorul virtual clone)
    circuitSera.conecteazaComponentaPrincipala(panouPrincipal.clone());

    // Adaugam senzori, actuatoare si stocare ca si componente secundare polimorfice
    circuitSera.adaugaComponentaSecundara(senzorSol.clone());
    circuitSera.adaugaComponentaSecundara(pompaIrigare.clone());
    circuitSera.adaugaComponentaSecundara(baterieBanc.clone());

    // Afisare schema circuit apeland interfata NVI a componentelor prin pointeri de baza
    std::cout << circuitSera << "\n";

    // -------------------------------------------------------------------------
    // 5. Apel de functii virtuale specifice temei prin pointerul de baza
    // -------------------------------------------------------------------------
    std::cout << "[5] Simulare polimorfica circuit (Apel simuleazaFunctionare):\n";
    const double radiatieSolaraWmp = 850.0; // W/m^2 intr-o zi insorita
    const double bilantW = circuitSera.simuleazaSarcinaCircuit(radiatieSolaraWmp);
    std::cout << "  - Putere generata/consumata in circuit la radiatie "
              << radiatieSolaraWmp << " W/m^2: " << bilantW << " W.\n\n";

    // -------------------------------------------------------------------------
    // 6. Testare downcast cu sens folosind dynamic_cast
    // -------------------------------------------------------------------------
    std::cout << "[6] Optimizare echipamente prin downcast (dynamic_cast):\n";
    circuitSera.optimizeazaSauCalibreazaEchipamente();
    std::cout << "  - Echipamentele au fost optimizate specific tipului lor concret.\n\n";

    // -------------------------------------------------------------------------
    // 7. Testare Copy-and-Swap si constructor de copiere deep-copy (clone)
    // -------------------------------------------------------------------------
    std::cout << "[7] Testare Copy Constructor si Copy-and-Swap pe clasa CircuitElectric:\n";
    CircuitElectric copieCircuit = circuitSera; // Copy constructor
    CircuitElectric altCircuit("Circuit Rezerva", 12.0);
    altCircuit = copieCircuit; // Copy assignment operator prin Copy-and-Swap
    std::cout << "  - Circuitul '" << altCircuit.getDenumireCircuit()
              << "' a fost copiat prin deep-copy si copy-and-swap cu succes.\n\n";

    // -------------------------------------------------------------------------
    // 8. Integrare NivelTreasureHunt cu noua arhitectura
    // -------------------------------------------------------------------------
    std::cout << "[8] Rulare Nivel Treasure Hunt educativ:\n";
    ModulTeoretic modul("Alimentare Fotovoltaica si Monitorizare Automata", 2, 60, true);
    NivelTreasureHunt nivel1(1, "Atelierul 2: Independenta Energetica a Serei",
                            modul, panouPrincipal, 80);

    IntrebareQuiz intrebare1(
        "Ce rol are dioda de blocare intr-un circuit cu panou fotovoltaic?",
        {"Amplifica tensiunea", "Previne descarcarea bateriei noaptea in panou", "Raceste panoul"},
        1, 40,
        "Curentul nu trebuie sa circule in sens invers prin celule."
    );
    IntrebareQuiz intrebare2(
        "Daca umiditatea solului scade sub 30%, ce componenta trebuie declansata?",
        {"Pompa de irigare", "Senzorul de lumina", "Panoul fotovoltaic"},
        0, 40,
        "Ucenicul electrician cupleaza releul pompei pentru udare."
    );

    nivel1.adaugaIntrebare(intrebare1);
    nivel1.adaugaIntrebare(intrebare2);

    std::cout << nivel1 << "\n\n";

    const std::vector<int> raspunsuriJucator = {1, 0};
    const int scorQuiz = nivel1.rezolvaQuiz(raspunsuriJucator);
    std::cout << "  - Scor obtinut la quiz-ul tehnic: " << scorQuiz << " puncte.\n";

    const bool promovat = nivel1.finalizeazaNivel(scorQuiz, 45, 0);
    std::cout << "  - Rezultat etapa: " << (promovat ? "PROMOVAT!" : "REPICAT!") << "\n\n";

    // -------------------------------------------------------------------------
    // 9. Ierarhie proprie de exceptii (try - catch pentru fiecare categorie)
    // -------------------------------------------------------------------------
    std::cout << "[9] Testare mecanism de prindere al exceptiilor proprii:\n";

    // Test Categoria 1: TensiuneInvalidaException
    try {
        std::cout << "  -> Testare aplicare tensiune periculoasa (80V pe componenta de 24V):\n";
        PanouSolar panouTest("Panou Test", 24.0, 5.0, true, 0.20, 1.0, 30.0);
        (void)panouTest.simuleazaFunctionare(800.0, 80.0); // 80V depaseste mult tensiunea de 24V
    } catch (const TensiuneInvalidaException& ex) {
        std::cout << "     [Prins cu succes] " << ex.what() << "\n";
    }

    // Test Categoria 2: ComponentaDefectaException
    try {
        std::cout << "  -> Testare simulare pe o componenta defecta / arsa:\n";
        ActuatorSera pompaArsa("Pompa Arsa", 24.0, 3.0, false, "Irigare", 50.0);
        (void)pompaArsa.simuleazaFunctionare(10.0, 24.0);
    } catch (const ComponentaDefectaException& ex) {
        std::cout << "     [Prins cu succes] " << ex.what() << "\n";
    }

    // Test Categoria 3: ConfiguratieInvalidaException
    try {
        std::cout << "  -> Testare instantiere componenta cu eficienta absurda (150%):\n";
        PanouSolar panouAbsurd("Panou Perpetuum Mobile", 24.0, 5.0, true, 1.5, 1.0, 30.0);
        (void)panouAbsurd;
    } catch (const ConfiguratieInvalidaException& ex) {
        std::cout << "     [Prins cu succes] " << ex.what() << "\n";
    }

    std::cout << "\n========================================================\n"
              << "   TOATE MODULELE SI VERIFICARILE AU FOST FINALIZATE!   \n"
              << "========================================================\n";

    return 0;
}
