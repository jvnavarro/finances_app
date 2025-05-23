#ifndef CARTAOCREDITO_H
#define CARTAOCREDITO_H

#include <string>
#include <vector>
#include <memory>
#include "Despesa.h"

using namespace std;
class Conta;  // declaração antecipada

class CartaoCredito {
private:
    string nome;
    double limite;
    double saldo;
    double fatura;
    
    Conta* contaAssociada;
    vector<unique_ptr<DespesaCartao>> despesas;

public:
    CartaoCredito(string nome, Conta* conta);
    void adicionarDespesa(const string& descricao, double valor);
    double totalFatura() const;
    void calcularFatura();
    void setLimite(double novoLimite);
    double getLimite() const;
    double getSaldo() const;
    double getFatura() const;
    string getNome() const;
    const vector<unique_ptr<DespesaCartao>>& getDespesas() const;
};

#endif