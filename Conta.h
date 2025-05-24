#ifndef CONTA_H
#define CONTA_H

#include <string>
#include <vector>
#include <memory>
#include "Despesa.h"       // Needs to know Despesa for DespesaDebito
#include "CartaoCredito.h" // Needs to know CartaoCredito
#include "date.h"          // For Date type
using namespace std;
// Forward declaration of CartaoCredito might be removed if CartaoCredito.h is fully included
// class CartaoCredito;

class Conta
{
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

    // Modified to include Date
    void carregarDespesaDebito(const string &descricao, double valor, const Date &data);
    void adicionarDespesaDebito(const string &descricao, double valor, const Date &data);

    void transferir(Conta &destino, double valor);
    void criarPoupanca(const string &nome, double valorInicial);

    // Modified to include limit for the new card
    CartaoCredito *adicionarCartao(const string &nome, double limiteCartao);

    string getNome() const;
    double getSaldo() const;
    const vector<unique_ptr<DespesaDebito>> &getDespesasDebito() const;
    const vector<unique_ptr<CartaoCredito>> &getCartoes() const;
    const vector<unique_ptr<Conta>> &getPoupancas() const;

    // ... other public methods in Conta.h
public:
    void criarPoupancaParaCarregamento(const std::string &nome, double valorInicial);
};

#endif