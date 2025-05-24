#include "../Conta.h"
#include "../date.h" // For Date type
#include <iostream>

Conta::Conta(string nome, double saldoInicial)
    : nome(nome), saldo(saldoInicial) {}

void Conta::depositar(double valor) {
    if (valor > 0) {
        saldo += valor;
        cout << "Depósito de R$" << valor << " realizado na conta " << nome << ". Novo saldo: R$" << saldo << endl;
    } else {
        cout << "Valor do depósito deve ser positivo." << endl;
    }
}

void Conta::sacar(double valor) {
    if (valor <= 0) {
        cout << "Valor do saque deve ser positivo." << endl;
        return;
    }
    if (saldo >= valor) {
        saldo -= valor;
        cout << "Saque de R$" << valor << " realizado na conta " << nome << ". Novo saldo: R$" << saldo << endl;
    } else {
        cout << "Saldo insuficiente para saque na conta " << nome << "." << endl;
    }
}

// Modified to include Date
void Conta::carregarDespesaDebito(const string& descricao, double valor, const Date& data) {
    despesasDebito.push_back(make_unique<DespesaDebito>(descricao, valor, data));
    // Saldo is not affected here as it's for loading existing data where saldo is already set
    cout << "Despesa de débito carregada: " << descricao << " - R$" << valor 
              << " em " << data.toFormattedString() << endl;
}

// Modified to include Date
void Conta::adicionarDespesaDebito(const string& descricao, double valor, const Date& data) {
    if (valor <= 0) {
        cout << "Valor da despesa deve ser positivo." << endl;
        return;
    }
    if (saldo >= valor) {
        // Note: The DespesaDebito itself doesn't alter the account's saldo directly.
        // The act of adding the expense implies the money is spent from the account.
        despesasDebito.push_back(make_unique<DespesaDebito>(descricao, valor, data));
        // Call sacar to actually reduce the account balance
        this->sacar(valor); // This will also print saque message. 
                            // You might want to avoid double messaging or make sacar quieter if called internally.
        cout << "Despesa de débito adicionada à conta " << nome << ": " << descricao 
                  << " - R$" << valor << " em " << data.toFormattedString() << endl;
    } else {
        cout << "Saldo insuficiente para adicionar despesa de débito na conta " << nome << "." << endl;
    }
}

void Conta::transferir(Conta& destino, double valor) {
    if (valor <= 0) {
        cout << "Valor da transferência deve ser positivo." << endl;
        return;
    }
    if (this == &destino) {
        cout << "Não é possível transferir para a mesma conta." << endl;
        return;
    }
    if (saldo >= valor) {
        this->sacar(valor); // this->saldo -= valor;
        destino.depositar(valor); // destino.saldo += valor;
        cout << "Transferência de R$" << valor << " realizada de " << nome 
                  << " para " << destino.getNome() << endl;
    } else {
        cout << "Saldo insuficiente para transferir da conta " << nome << "." << endl;
    }
}

void Conta::criarPoupanca(const string& nomePoupanca, double valorInicial) {
    if (valorInicial < 0) {
        cout << "Valor inicial da poupança não pode ser negativo." << endl;
        return;
    }
    if (saldo >= valorInicial) { // Check if main account has funds to move to savings
        poupancas.push_back(make_unique<Conta>(nomePoupanca, valorInicial));
        this->sacar(valorInicial); // Remove from main account balance
        cout << "Poupança '" << nomePoupanca << "' criada com R$" << valorInicial 
                  << " transferidos da conta " << nome << "." << endl;
    } else {
        cout << "Saldo insuficiente na conta " << nome 
                  << " para criar poupança com valor inicial de R$" << valorInicial << endl;
    }
}

// Modified to include limit for the new card
CartaoCredito* Conta::adicionarCartao(const string& nomeCartao, double limiteCartao) {
    if (limiteCartao < 0) {
        cout << "Limite do cartão não pode ser negativo." << endl;
        return nullptr; // Indicate failure
    }
    cartoes.push_back(make_unique<CartaoCredito>(nomeCartao, this, limiteCartao));
    cout << "Cartão de crédito '" << nomeCartao << "' adicionado à conta " << nome 
              << " com limite de R$" << limiteCartao << endl;
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

// ... in Conta.cpp
void Conta::criarPoupancaParaCarregamento(const std::string& nomePoupanca, double valorInicial) {
    poupancas.push_back(std::make_unique<Conta>(nomePoupanca, valorInicial));
    // Optional: A quieter message for loading
    // std::cout << "Poupança (carregada): " << nomePoupanca << " com R$" << valorInicial << std::endl;
}