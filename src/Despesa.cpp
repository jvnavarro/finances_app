#include "../Despesa.h" // Assuming Despesa.h is in the parent directory
#include "../date.h"   // Assuming date.h is in the parent directory
using namespace std;
Despesa::Despesa(std::string descricao, double valor, const Date& data)
    : descricao(descricao), valor(valor), data(data) {}

std::string Despesa::getDescricao() const {
    return descricao;
}

double Despesa::getValor() const {
    return valor;
}

Date Despesa::getDate() const {
    return data;
}

// --- DespesaDebito ---
DespesaDebito::DespesaDebito(std::string descricao, double valor, const Date& data)
    : Despesa(descricao, valor, data) {}

std::string DespesaDebito::getTipo() const {
    return "Débito";
}

// --- DespesaCartao ---
DespesaCartao::DespesaCartao(std::string descricao, double valor, const Date& data)
    : Despesa(descricao, valor, data) {}

std::string DespesaCartao::getTipo() const {
    return "Cartão";
}