#include "IntrebareQuiz.h"

#include <ostream>
#include <stdexcept>
#include <utility>

void IntrebareQuiz::valideaza() const {
    if (enunt.empty()) {
        throw std::invalid_argument("Enuntul intrebarii nu poate fi vid!");
    }
    if (variante.size() < 2) {
        throw std::invalid_argument("Trebuie sa existe minim 2 variante de raspuns!");
    }
    if (indexRaspunsCorect < 0 || static_cast<std::size_t>(indexRaspunsCorect) >= variante.size()) {
        throw std::invalid_argument("Indexul raspunsului corect este invalid!");
    }
    if (punctajIntrebare <= 0) {
        throw std::invalid_argument("Punctajul intrebarii trebuie sa fie pozitiv!");
    }
}

IntrebareQuiz::IntrebareQuiz() = default;

IntrebareQuiz::IntrebareQuiz(std::string enuntIntrebare, std::vector<std::string> varianteRaspuns,
                             int indexCorect, int punctaj, std::string indiciu)
    : enunt(std::move(enuntIntrebare)),
      variante(std::move(varianteRaspuns)),
      indexRaspunsCorect(indexCorect),
      punctajIntrebare(punctaj),
      indiciuTehnic(std::move(indiciu)) {
    valideaza();
}

const std::string& IntrebareQuiz::getEnunt() const {
    return enunt;
}

const std::vector<std::string>& IntrebareQuiz::getVariante() const {
    return variante;
}

int IntrebareQuiz::getIndexRaspunsCorect() const {
    return indexRaspunsCorect;
}

int IntrebareQuiz::getPunctaj() const {
    return punctajIntrebare;
}

const std::string& IntrebareQuiz::getIndiciu() const {
    return indiciuTehnic;
}

bool IntrebareQuiz::verificaRaspuns(int indexAles) const {
    return indexAles == indexRaspunsCorect;
}

int IntrebareQuiz::evalueazaRaspuns(int indexAles) const {
    return verificaRaspuns(indexAles) ? punctajIntrebare : 0;
}

std::ostream& operator<<(std::ostream& out, const IntrebareQuiz& intrebare) {
    out << "Intrebare (" << intrebare.punctajIntrebare << " pct): " << intrebare.enunt << "\n";
    for (std::size_t i = 0; i < intrebare.variante.size(); ++i) {
        out << "   " << i + 1 << ") " << intrebare.variante[i] << "\n";
    }
    return out;
}
