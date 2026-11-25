#ifndef MISIUNE_SERA_H
#define MISIUNE_SERA_H

#include <filesystem>
#include <iosfwd>

std::filesystem::path gasesteDirectorDate(const std::filesystem::path& executabil);
int ruleazaMisiuneSera(const std::filesystem::path& directorDate, std::istream& in,
                      std::ostream& out, std::ostream& erori);

#endif
