#ifndef MODUL_TEORETIC_H
#define MODUL_TEORETIC_H

#include <iosfwd>
#include <string>

class ModulTeoretic {
public:
    static constexpr int dificultateMin = 1;
    static constexpr int dificultateMax = 5;
    static constexpr int puncteBonusPerIncrement = 10;

private:
    std::string denumire = "Necunoscut";
    int dificultate = 1;
    int puncteRecompensa = 0;
    bool esteDeblocat = false;

    void valideaza() const;

public:
    // 1. Constructori și Destructor (Regula celor 3)
    ModulTeoretic();
    ModulTeoretic(std::string denumireModul, int dificultateModul, int puncte, bool deblocat);
    ModulTeoretic(const ModulTeoretic& altul);
    ~ModulTeoretic();

    // 2. Operator de atribuire (=)
    ModulTeoretic& operator=(const ModulTeoretic& altul);

    // 3. Getteri și Setteri
    [[nodiscard]] const std::string& getDenumire() const;
    void setDenumire(std::string denumireNoua);

    [[nodiscard]] int getDificultate() const;
    void setDificultate(int dificultateNoua);

    [[nodiscard]] int getPuncte() const;
    void setPuncte(int puncteNoi);

    [[nodiscard]] bool getEsteDeblocat() const;
    void setEsteDeblocat(bool deblocatNou);

    // 4. Operatori de afișare și citire (<< și >>)
    friend std::ostream& operator<<(std::ostream& out, const ModulTeoretic& modul);
    friend std::istream& operator>>(std::istream& in, ModulTeoretic& modul);

    // 5. Operator de pre-incrementare (++m)
    ModulTeoretic& operator++();

    // 6. Metode specifice temei Ghidul Electricianului
    void deblocheazaModul();
    [[nodiscard]] int calculeazaPunctajFinal(int timpSecunde, int incercariGresite) const;
    [[nodiscard]] bool esteRecomandatPentruIncepatori() const;
    void afiseazaDetaliiEtapa() const;
    void afiseazaDetaliiEtapa(std::ostream& out) const;
};

#endif // MODUL_TEORETIC_H
