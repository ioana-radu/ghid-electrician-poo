#ifndef INTREBARE_QUIZ_H
#define INTREBARE_QUIZ_H

#include <iosfwd>
#include <string>
#include <vector>

class IntrebareQuiz {
private:
    std::string enunt = "Intrebare implicita";
    std::vector<std::string> variante = {"A", "B"};
    int indexRaspunsCorect = 0;
    int punctajIntrebare = 10;
    std::string indiciuTehnic = "Revizuieste notiunile de baza.";

    void valideaza() const;

public:
    // Constructori
    IntrebareQuiz();
    IntrebareQuiz(std::string enuntIntrebare, std::vector<std::string> varianteRaspuns,
                  int indexCorect, int punctaj, std::string indiciu);

    // Getteri
    [[nodiscard]] const std::string& getEnunt() const;
    [[nodiscard]] const std::vector<std::string>& getVariante() const;
    [[nodiscard]] int getIndexRaspunsCorect() const;
    [[nodiscard]] int getPunctaj() const;
    [[nodiscard]] const std::string& getIndiciu() const;

    // Metode specifice temei
    [[nodiscard]] bool verificaRaspuns(int indexAles) const;
    [[nodiscard]] int evalueazaRaspuns(int indexAles) const;

    // Operator de afișare
    friend std::ostream& operator<<(std::ostream& out, const IntrebareQuiz& intrebare);
};

#endif // INTREBARE_QUIZ_H
