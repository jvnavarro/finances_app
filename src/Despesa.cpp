#include "../Despesa.h"
using namespace std;
Despesa::Despesa(string descricao, double valor)
    : descricao(descricao), valor(valor) {}

string Despesa::getDescricao() const {
    return descricao;
}

double Despesa::getValor() const {
    return valor;
}

// DespesaDebito
DespesaDebito::DespesaDebito(string descricao, double valor)
    : Despesa(descricao, valor) {}

string DespesaDebito::getTipo() const {
    return "DEBITO";
}

// DespesaCartao

DespesaCartao::DespesaCartao(string descricao, double valor)
    : Despesa(descricao, valor) {}

string DespesaCartao::getTipo() const {
    return "CARTAO";
}