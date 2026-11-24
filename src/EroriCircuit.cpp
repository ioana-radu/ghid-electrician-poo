#include "EroriCircuit.h"

#include <utility>

EroareSera::EroareSera(std::string mesaj)
    : mesajEroare(std::move(mesaj)) {
}

const char* EroareSera::what() const noexcept {
    return mesajEroare.c_str();
}

TensiuneInvalidaException::TensiuneInvalidaException(const std::string& mesaj)
    : EroareSera("[Eroare Tensiune Circuit] " + mesaj) {
}

ComponentaDefectaException::ComponentaDefectaException(const std::string& mesaj)
    : EroareSera("[Eroare Componenta Hardware] " + mesaj) {
}

ConfiguratieInvalidaException::ConfiguratieInvalidaException(const std::string& mesaj)
    : EroareSera("[Eroare Configurare Parametri] " + mesaj) {
}
