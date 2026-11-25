#include "MisiuneSera.h"

#include <algorithm>
#include <charconv>
#include <chrono>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <nlohmann_json.hpp>

#include "ActuatorSera.h"
#include "CircuitElectric.h"
#include "NivelTreasureHunt.h"
#include "PanouSolar.h"
#include "SenzorMonitorizare.h"

namespace {
using nlohmann::json;
using Ceas = std::chrono::steady_clock;

// Keep source documents in UTF-8 and render their text on ASCII-only terminals.
std::string textPentruConsola(std::string_view text) {
    constexpr std::pair<std::u8string_view, std::string_view> echivalente[] = {
        {u8"\u0102", "A"}, {u8"\u0103", "a"}, {u8"\u00C2", "A"}, {u8"\u00E2", "a"},
        {u8"\u00CE", "I"}, {u8"\u00EE", "i"}, {u8"\u0218", "S"}, {u8"\u0219", "s"},
        {u8"\u021A", "T"}, {u8"\u021B", "t"}, {u8"\u015E", "S"}, {u8"\u015F", "s"},
        {u8"\u0162", "T"}, {u8"\u0163", "t"}, {u8"\uFEFF", ""}, {u8"\u00A0", " "},
        {u8"\u2013", "-"}, {u8"\u2014", "-"}, {u8"\u2212", "-"},
        {u8"\u2018", "'"}, {u8"\u2019", "'"}, {u8"\u201C", "\""}, {u8"\u201D", "\""},
        {u8"\u00D7", "*"}, {u8"\u00B7", "*"}, {u8"\u00B2", "^2"},
        {u8"\u00B0C", "deg C"}, {u8"\u00B0", " deg"}, {u8"\u00B5", "u"}, {u8"\u03A9", "ohm"},
        {u8"\u0394", "Delta "}, {u8"\u03A6", "Phi"}, {u8"\u03B5", "epsilon"},
        {u8"\u03B7", "eta"}, {u8"\u03B8", "theta"}, {u8"\u03C0", " pi "}, {u8"\u03C1", "rho"},
        {u8"\u03C4", "tau"}, {u8"\u03C6", "phi"}, {u8"\u03C9", "omega "}, {u8"\u2113", "l"},
        {u8"\u2080", "_0"}, {u8"\u2081", "_1"}, {u8"\u2082", "_2"}, {u8"\u209B", "_s"},
        {u8"\u2192", "->"}, {u8"\u2248", "~="}, {u8"\u2265", ">="},
        {u8"\u221ALC", "sqrt(L*C)"}, {u8"\u221A2", "sqrt(2)"}, {u8"\u221A3", "sqrt(3)"},
        {u8"\u221A", "sqrt"},
        {u8"\u2500", "-"}, {u8"\u2502", "|"}, {u8"\u250C", "+"}, {u8"\u2510", "+"},
        {u8"\u2514", "+"}, {u8"\u2518", "+"}, {u8"\u251C", "+"}, {u8"\u2524", "+"}, {u8"\u25CF", "o"},
    };
    std::string rezultat;
    rezultat.reserve(text.size());
    while (!text.empty()) {
        const auto octet = static_cast<unsigned char>(text.front());
        if (octet < 128) {
            if (octet == '\t' || octet == '\n' || (octet >= ' ' && octet < 127)) {
                rezultat += text.front();
            }
            text.remove_prefix(1);
            continue;
        }
        const auto* echivalent = std::ranges::find_if(echivalente, [text](const auto& pereche) {
            return text.starts_with(std::string_view(
                reinterpret_cast<const char*>(pereche.first.data()), pereche.first.size()));
        });
        if (echivalent != std::end(echivalente)) {
            rezultat += echivalent->second;
            text.remove_prefix(echivalent->first.size());
        } else {
            rezultat += '?';
            text.remove_prefix(1);
            while (!text.empty() && (static_cast<unsigned char>(text.front()) & 0xC0) == 0x80) {
                text.remove_prefix(1);
            }
        }
    }
    return rezultat;
}

class ModulDeStudiu {
    NivelTreasureHunt etapa;
    std::vector<std::string> lectii;

public:
    ModulDeStudiu(const NivelTreasureHunt& nivel, std::vector<std::string> subiecte)
        : etapa(nivel), lectii(std::move(subiecte)) {}

    NivelTreasureHunt& nivel() { return etapa; }
    const NivelTreasureHunt& nivel() const { return etapa; }
    const std::vector<std::string>& subiecte() const { return lectii; }
};

json citesteJson(const std::filesystem::path& cale) {
    std::ifstream fisier(cale);
    if (!fisier) {
        throw std::runtime_error("Fisier indisponibil: " + cale.string());
    }
    try {
        return json::parse(fisier);
    } catch (const json::exception& eroare) {
        throw std::runtime_error(cale.string() + ": " + eroare.what());
    }
}

std::unique_ptr<ComponentaElectronica> citesteRecompensa(const json& date) {
    const auto tip = date.at("tip").get<std::string>();
    const auto denumire = textPentruConsola(date.at("denumire").get<std::string>());
    const auto tensiune = date.at("tensiune").get<double>();
    const auto curent = date.at("curent").get<double>();
    const auto functionala = date.at("functionala").get<bool>();
    if (tip == "panou") {
        return std::make_unique<PanouSolar>(denumire, tensiune, curent, functionala,
            date.at("eficienta").get<double>(), date.at("suprafata").get<double>(),
            date.at("unghi").get<double>());
    }
    if (tip == "senzor") {
        return std::make_unique<SenzorMonitorizare>(denumire, tensiune, curent, functionala,
            textPentruConsola(date.at("marime").get<std::string>()), date.at("prag").get<double>(),
            date.at("offset").get<double>());
    }
    if (tip == "actuator") {
        return std::make_unique<ActuatorSera>(denumire, tensiune, curent, functionala,
            textPentruConsola(date.at("rol").get<std::string>()), date.at("putere").get<double>());
    }
    throw std::runtime_error("Tip de recompensa necunoscut: " + tip);
}

std::vector<ModulDeStudiu> citesteModule(const json& lectii, const json& quizuri, const json& dictionar) {
    if (!lectii.is_array() || lectii.empty()) {
        throw std::runtime_error("Manualul trebuie sa contina cel putin o lectie.");
    }
    std::vector<ModulDeStudiu> manual;
    std::set<std::string> lectiiIncluse;
    std::set<int> idModule;
    manual.reserve(lectii.size());
    for (const auto& date : lectii) {
        const auto id = date.at("id").get<int>();
        if (!idModule.insert(id).second) {
            throw std::runtime_error("ID de modul duplicat: " + std::to_string(id));
        }
        const auto titlu = textPentruConsola(date.at("titlu").get<std::string>());
        const auto dificultate = date.at("dificultate").get<int>();
        const auto prag = date.at("prag").get<int>();
        const auto recompensa = citesteRecompensa(date.at("recompensa"));
        ModulTeoretic modul(titlu, dificultate, 0, true);
        NivelTreasureHunt nivel(id, titlu, modul, *recompensa, prag);

        const auto& intrebari = quizuri.at(std::to_string(id));
        if (!intrebari.is_array() || intrebari.empty()) {
            throw std::runtime_error("Quizul este gol la lectia: " + titlu);
        }
        int punctajMaxim = 0;
        std::vector<std::string> subiecte;
        for (const auto& intrebare : intrebari) {
            const auto cod = intrebare.at("lectie").get<std::string>();
            if (!dictionar.contains(cod) || !lectiiIncluse.insert(cod).second) {
                throw std::runtime_error("Lectie necunoscuta sau repetata in quiz: " + cod);
            }
            subiecte.push_back(cod);
            const auto puncte = intrebare.at("puncte").get<int>();
            auto variante = intrebare.at("variante").get<std::vector<std::string>>();
            for (auto& varianta : variante) {
                varianta = textPentruConsola(varianta);
            }
            nivel.adaugaIntrebare(IntrebareQuiz(
                textPentruConsola(intrebare.at("enunt").get<std::string>()), std::move(variante),
                intrebare.at("corect").get<int>(), puncte,
                textPentruConsola(intrebare.at("indiciu").get<std::string>())));
            if (puncte > std::numeric_limits<int>::max() - punctajMaxim) {
                throw std::runtime_error("Punctajul quizului depaseste intervalul acceptat.");
            }
            punctajMaxim += puncte;
        }
        if (prag > punctajMaxim) {
            throw std::runtime_error("Pragul depaseste punctajul maxim la lectia: " + titlu);
        }
        manual.emplace_back(nivel, std::move(subiecte));
    }
    if (lectiiIncluse.size() != dictionar.size()) {
        throw std::runtime_error("Fiecare lectie din dictionar trebuie sa aiba o intrebare.");
    }
    return manual;
}

std::string citesteLectie(const std::filesystem::path& director, const json& intrare) {
    const auto cale = director / intrare.at("fisier").get<std::string>();
    std::ifstream fisier(cale, std::ios::binary);
    if (!fisier) {
        throw std::runtime_error("Lectie indisponibila: " + cale.string());
    }
    std::string continut;
    std::string linie;
    while (std::getline(fisier, linie)) {
        if (!linie.empty() && linie.back() == '\r') {
            linie.pop_back();
        }
        continut += linie + '\n';
    }
    if (continut.empty() || fisier.bad()) {
        throw std::runtime_error("Lectia nu a putut fi citita: " + cale.string());
    }
    return continut;
}

void afiseazaDefinitie(std::string_view cod, const json& dictionar,
                      const std::filesystem::path& director, std::ostream& out) {
    const auto intrare = dictionar.find(std::string(cod));
    if (intrare == dictionar.end()) {
        out << "Cod de lectie necunoscut. Foloseste trei cifre, de exemplu d 088.\n";
        return;
    }
    std::istringstream continut(textPentruConsola(citesteLectie(director, *intrare)));
    std::string linie;
    bool inDefinitie = false;
    out << textPentruConsola(intrare->at("termen").get<std::string>()) << '\n';
    while (std::getline(continut, linie)) {
        if (linie.starts_with("## ")) {
            if (inDefinitie) {
                return;
            }
            inDefinitie = linie == "## Definitie";
        } else if (inDefinitie) {
            out << linie << '\n';
        }
    }
}

void afiseazaLectie(std::string_view text, std::ostream& out) {
    std::istringstream continut(textPentruConsola(text));
    std::string linie;
    bool inSchema = false;
    bool randGol = false;
    while (std::getline(continut, linie)) {
        if (linie.starts_with("~~~") || linie.starts_with("```")) {
            inSchema = !inSchema;
            continue;
        }
        if (!inSchema) {
            if (linie.ends_with("](#probleme-rezolvate)") || linie.ends_with("](../CUPRINS.md)")) {
                continue;
            }
            if (linie.starts_with("# ") || linie.starts_with("## ")) {
                linie.erase(0, linie.find(' ') + 1);
            } else if (linie.starts_with("Categoria din capturi - ")) {
                linie.replace(0, std::string_view("Categoria din capturi - ").size(), "Categorie: ");
            } else if (linie.starts_with("- [") && linie.ends_with(')')) {
                const auto separator = linie.find("](");
                if (separator != std::string::npos) {
                    linie.replace(separator, 2, " - ");
                    linie.erase(2, 1);
                    linie.pop_back();
                }
            }
        }
        if (linie.empty() && randGol) {
            continue;
        }
        out << linie << '\n';
        randGol = linie.empty();
    }
}

bool parcurgeLectia(const std::string& cod, const json& dictionar,
                    const std::filesystem::path& director, std::istream& in, std::ostream& out) {
    out << '\n';
    afiseazaLectie(citesteLectie(director, dictionar.at(cod)), out);
    std::string comanda;
    out << "Enter pentru a continua sau d <cod> pentru dictionar: " << std::flush;
    while (std::getline(in, comanda)) {
        if (comanda.empty()) {
            return true;
        }
        if (comanda.starts_with("d ")) {
            afiseazaDefinitie(std::string_view(comanda).substr(2), dictionar, director, out);
        } else {
            out << "Comanda necunoscuta.\n";
        }
        out << "> " << std::flush;
    }
    return false;
}

std::optional<int> citesteOptiune(std::istream& in, std::ostream& out, std::size_t maxim) {
    std::string linie;
    while (std::getline(in, linie)) {
        const auto text = std::string_view(linie);
        const auto inceput = text.find_first_not_of(" \t\r");
        if (inceput != std::string_view::npos) {
            const auto optiune = text.substr(inceput, text.find_last_not_of(" \t\r") - inceput + 1);
            int valoare = 0;
            const auto [sfarsit, eroare] = std::from_chars(
                optiune.data(), optiune.data() + optiune.size(), valoare);
            if (eroare == std::errc{} && sfarsit == optiune.data() + optiune.size() &&
                valoare >= 1 && static_cast<std::size_t>(valoare) <= maxim) {
                return valoare - 1;
            }
        }
        out << "Introdu un numar intre 1 si " << maxim << ": " << std::flush;
    }
    return std::nullopt;
}

std::size_t urmatorulModul(const std::vector<ModulDeStudiu>& manual, std::size_t curenta, bool accelerat) {
    if (accelerat) {
        const auto dificultate = manual[curenta].nivel().getModulTeoretic().getDificultate();
        for (std::size_t i = curenta + 1; i < manual.size(); ++i) {
            const auto& nivel = manual[i].nivel();
            if (!nivel.getEsteCompletat() && nivel.getModulTeoretic().getDificultate() > dificultate + 1) {
                return i;
            }
        }
    }
    const auto pozitie = std::ranges::find_if(manual, [](const ModulDeStudiu& modul) {
        return !modul.nivel().getEsteCompletat();
    });
    return static_cast<std::size_t>(pozitie - manual.begin());
}

bool monteazaRecompensa(const NivelTreasureHunt& nivel, std::size_t pozitie, CircuitElectric& circuit,
                        std::istream& in, std::ostream& out) {
    auto componenta = nivel.getComponentaRecompensa()->clone();
    out << "Recompensa castigata: " << componenta->getDenumire() << ".\n";
    if (!componenta->getEsteFunctionala()) {
        out << "Componenta virtuala este defecta.\n"
            << "1) Remediaza defectul in simulator\n2) Incearca montarea directa\n> " << std::flush;
        auto alegere = citesteOptiune(in, out, 2);
        while (alegere && *alegere != 0) {
            out << "Montaj blocat. Remediaza componenta inainte de montare.\n> " << std::flush;
            alegere = citesteOptiune(in, out, 2);
        }
        if (!alegere) {
            return false;
        }
        componenta->reparaSauCalibreaza();
        out << "Componenta a fost reparata.\n";
    }
    if (pozitie == 0) {
        circuit.conecteazaComponentaPrincipala(std::move(componenta));
    } else {
        circuit.adaugaComponentaSecundara(std::move(componenta));
    }
    out << "Componenta montata in circuitul serei.\n";
    return true;
}
}

std::filesystem::path gasesteDirectorDate(const std::filesystem::path& executabil) {
    const std::filesystem::path local = "assets";
    std::error_code eroare;
    if (std::filesystem::exists(local / "manual.json", eroare)) {
        return local;
    }
    return executabil.parent_path() / local;
}

int ruleazaMisiuneSera(const std::filesystem::path& directorDate, std::istream& in,
                      std::ostream& out, std::ostream& erori) {
    try {
        const auto date = citesteJson(directorDate / "manual.json");
        const auto dictionar = citesteJson(directorDate / "dictionar.json");
        const auto quizuri = citesteJson(directorDate / "quizuri.json");
        const auto directorTeorie = (directorDate / ".." / "Teorie_electricitate").lexically_normal();
        auto manual = citesteModule(date.at("module"), quizuri, dictionar);
        CircuitElectric circuit(textPentruConsola(date.at("sera").get<std::string>()),
                                date.at("tensiune").get<double>());
        const auto timpAccelerat = std::chrono::seconds(date.at("secunde_pe_intrebare").get<int>());
        if (timpAccelerat.count() <= 0) {
            throw std::runtime_error("Timpul pentru traseul accelerat trebuie sa fie pozitiv.");
        }

        out << "Misiunea Sera Inteligenta\nNumele ucenicului: " << std::flush;
        std::string ucenic;
        if (!std::getline(in, ucenic)) {
            return 0;
        }
        ucenic = textPentruConsola(ucenic);
        if (ucenic.find_first_not_of(" \t\r") == std::string::npos) {
            ucenic = "Ucenic";
        }

        std::size_t curenta = 0;
        while (curenta < manual.size()) {
            auto& modul = manual[curenta];
            auto& nivel = modul.nivel();
            out << "\n=== " << nivel.getTitluNivel() << " ===\n";
            auto lectiiDeParcurs = modul.subiecte();
            while (true) {
                for (const auto& cod : lectiiDeParcurs) {
                    if (!parcurgeLectia(cod, dictionar, directorTeorie, in, out)) {
                        return 0;
                    }
                }
                out << "\nApasa Enter pentru a porni quizul cronometrat." << std::endl;
                std::string confirmare;
                if (!std::getline(in, confirmare)) {
                    return 0;
                }
                lectiiDeParcurs.clear();

                std::vector<int> raspunsuri;
                const auto start = Ceas::now();
                for (const auto& intrebare : nivel.getIntrebari()) {
                    const auto& cod = modul.subiecte()[raspunsuri.size()];
                    out << intrebare << "> " << std::flush;
                    const auto raspuns = citesteOptiune(in, out, intrebare.getVariante().size());
                    if (!raspuns) {
                        return 0;
                    }
                    raspunsuri.push_back(*raspuns);
                    if (!intrebare.verificaRaspuns(*raspuns)) {
                        lectiiDeParcurs.push_back(cod);
                    }
                }
                const auto durata = Ceas::now() - start;
                const int scor = nivel.rezolvaQuiz(raspunsuri);
                const auto secunde = std::chrono::duration_cast<std::chrono::seconds>(durata).count();
                out << "Scor: " << scor << " puncte. Timp: " << secunde << " s.\n";
                for (std::size_t i = 0; i < raspunsuri.size(); ++i) {
                    const auto& intrebare = nivel.getIntrebari()[i];
                    if (!intrebare.verificaRaspuns(raspunsuri[i])) {
                        out << "Lectia " << modul.subiecte()[i] << ": " << intrebare.getIndiciu() << '\n';
                    }
                }
                const auto timpRaportat = static_cast<int>(std::min<std::chrono::seconds::rep>(
                    secunde, std::numeric_limits<int>::max()));
                if (!nivel.finalizeazaNivel(scor, timpRaportat, static_cast<int>(lectiiDeParcurs.size()))) {
                    out << "Pragul este " << nivel.getScorMinimPromovare()
                        << " puncte. Recapituleaza lectiile la care ai gresit.\n";
                    continue;
                }
                if (!monteazaRecompensa(nivel, curenta, circuit, in, out)) {
                    return 0;
                }
                const auto timpLimita = timpAccelerat * static_cast<std::chrono::seconds::rep>(raspunsuri.size());
                const auto urmator = urmatorulModul(manual, curenta, lectiiDeParcurs.empty() && durata < timpLimita);
                if (urmator < manual.size() && urmator > curenta + 1) {
                    out << "Traseu accelerat: treci la un modul mai dificil.\n";
                }
                curenta = urmator;
                break;
            }
        }
        out << '\n' << ucenic << ", ai reabilitat sera inteligenta in simulator!\n" << circuit;
        return 0;
    } catch (const std::exception& eroare) {
        erori << "Misiunea nu poate continua: " << textPentruConsola(eroare.what()) << '\n';
        return 1;
    }
}
