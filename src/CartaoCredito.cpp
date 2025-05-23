#include "../CartaoCredito.h"
#include "../Conta.h"
#include <iostream>

using namespace std;
CartaoCredito::CartaoCredito(string nome, Conta* conta)
    : nome(nome), contaAssociada(conta) {}

void CartaoCredito::calcularFatura() {
    fatura = 0.0;
    for (const auto& despesa : despesas) {
        fatura += despesa->getValor();
    }
}
void CartaoCredito::adicionarDespesa(const string& descricao, double valor) {
    despesas.push_back(make_unique<DespesaCartao>(descricao, valor));
    cout << "Despesa adicionada ao cartão " << nome << ": " << descricao << " - R$" << valor << endl;
}

double CartaoCredito::totalFatura() const {
    double total = 0.0;
    for (const auto& despesa : despesas) {
        total += despesa->getValor();
    }
    return total;
}
void CartaoCredito::setLimite(double novoLimite) {
    limite = novoLimite;
}
string CartaoCredito::getNome() const {
    return nome;
}

const vector<unique_ptr<DespesaCartao>>& CartaoCredito::getDespesas() const {
    return despesas;
}

double CartaoCredito::getLimite() const {
    return limite;
}
double CartaoCredito::getSaldo() const {
    return saldo;
}
double CartaoCredito::getFatura() const {
    return fatura;
}