#ifndef CONTA_H
#define CONTA_H

#include <string>
#include <vector>
#include <memory>
#include "Despesa.h"
#include "CartaoCredito.h"
using namespace std;


class Conta {
private:
    string nome;
    double saldo;
    vector<unique_ptr<DespesaDebito>> despesasDebito;
    vector<unique_ptr<CartaoCredito>> cartoes;
    vector<unique_ptr<Conta>> poupancas;

public:
    Conta(string nome, double saldoInicial);

    void depositar(double valor);
    void sacar(double valor);
    void carregarDespesaDebito(const string& descricao, double valor);
    void adicionarDespesaDebito(const string& descricao, double valor);
    void transferir(Conta& destino, double valor);
    void criarPoupanca(const string& nome, double valorInicial);

    CartaoCredito* adicionarCartao(const string& nome);

    string getNome() const;
    double getSaldo() const;
    const vector<unique_ptr<DespesaDebito>>& getDespesasDebito() const;
    const vector<unique_ptr<CartaoCredito>>& getCartoes() const;
    const vector<unique_ptr<Conta>>& getPoupancas() const;
};

#endif