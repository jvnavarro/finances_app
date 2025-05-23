#include "../Conta.h"
#include <iostream>

using namespace std;

Conta::Conta(string nome, double saldoInicial)
    : nome(nome), saldo(saldoInicial) {}
void Conta::depositar(double valor) {
    saldo += valor;
}
void Conta::sacar(double valor) {
    if (saldo >= valor) {
        saldo -= valor;
    } else {
        cout << "Saldo insuficiente." << endl;
    }
}
void Conta::carregarDespesaDebito(const string& descricao, double valor) {
    despesasDebito.push_back(make_unique<DespesaDebito>(descricao, valor));
    cout << "Despesa de débito carregada: " << descricao << " - R$" << valor << endl;
}
void Conta::adicionarDespesaDebito(const string& descricao, double valor) {
    if (saldo >= valor) {
        despesasDebito.push_back(make_unique<DespesaDebito>(descricao, valor));
        sacar(valor);
        cout << "Despesa de débito adicionada: " << descricao << " - R$" << valor << endl;
    } else {
        cout << "Saldo insuficiente para despesa de débito." << endl;
    }
}
void Conta::transferir(Conta& destino, double valor) {
    if (saldo >= valor) {
        sacar(valor);
        destino.depositar(valor);
        cout << "Transferência realizada de " << nome << " para " << destino.getNome() << endl;
    } else {
        cout << "Saldo insuficiente para transferir." << endl;
    }
}
void Conta::criarPoupanca(const string& nome, double valorInicial) {

        poupancas.push_back(make_unique<Conta>(nome, valorInicial));
        cout << "Poupança criada: " << nome << " com R$" << valorInicial << endl;
}
CartaoCredito* Conta::adicionarCartao(const string& nome) {
    cartoes.push_back(make_unique<CartaoCredito>(nome, this));
    return cartoes.back().get();
}
string Conta::getNome() const {
    return nome;
}
double Conta::getSaldo() const {
    return saldo;
}
const vector<unique_ptr<DespesaDebito>>& Conta::getDespesasDebito() const {
    return despesasDebito;
}
const vector<unique_ptr<CartaoCredito>>& Conta::getCartoes() const {
    return cartoes;
}
const vector<unique_ptr<Conta>>& Conta::getPoupancas() const {
    return poupancas;
}