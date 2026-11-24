#ifndef ERORI_CIRCUIT_H
#define ERORI_CIRCUIT_H

#include <exception>
#include <string>

// Clasa de baza pentru toate exceptiile din proiectul Ghidul Electricianului
class EroareSera : public std::exception {
protected:
    std::string mesajEroare;

public:
    explicit EroareSera(std::string mesaj);
    [[nodiscard]] const char* what() const noexcept override;
};

// Categoria 1: Erori electrice legate de tensiuni invalide sau periculoase
class TensiuneInvalidaException : public EroareSera {
public:
    explicit TensiuneInvalidaException(const std::string& mesaj);
};

// Categoria 2: Erori hardware legate de componente defecte sau arse
class ComponentaDefectaException : public EroareSera {
public:
    explicit ComponentaDefectaException(const std::string& mesaj);
};

// Categoria 3: Erori logice legate de configurari fizice sau parametri absurzi
class ConfiguratieInvalidaException : public EroareSera {
public:
    explicit ConfiguratieInvalidaException(const std::string& mesaj);
};

#endif // ERORI_CIRCUIT_H
