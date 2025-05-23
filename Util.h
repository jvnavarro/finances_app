#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include "Conta.h"

namespace Util {
    void salvarContas(const std::vector<std::unique_ptr<Conta>>& contas, const std::string& nomeArquivo);
    void carregarContas(std::vector<std::unique_ptr<Conta>>& contas, const std::string& nomeArquivo);
}

#endif

