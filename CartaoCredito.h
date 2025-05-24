#ifndef CARTAOCREDITO_H
#define CARTAOCREDITO_H

#include <string>
#include <vector>
#include <memory>
#include "Despesa.h" // Contém DespesaCartao que usa Date
#include "date.h"    // Para o parâmetro Date em adicionarDespesa

class Conta;  // Declaração antecipada

class CartaoCredito {
private:
    std::string nome;
    double limite;
    // double saldo; // Removido, usaremos saldo_disponivel calculado
    // double fatura; // Removido, usaremos fatura_atual
    double fatura_atual; // Saldo da fatura atual do cartão

    Conta* contaAssociada;
    std::vector<std::unique_ptr<DespesaCartao>> despesas;

    void recalcularFatura(); // Método privado para atualizar fatura_atual

public:
    // Construtor atualizado para incluir o limite
    CartaoCredito(std::string nome, Conta* conta, double limiteCartao);

    // Método atualizado para incluir a data da despesa
    void adicionarDespesa(const std::string& descricao, double valor, const Date& data);

    void setLimite(double novoLimite);
    double getLimite() const;
    double getSaldoDisponivel() const; // Saldo = limite - fatura_atual
    double getFaturaAtual() const;     // Retorna a fatura_atual calculada
    std::string getNome() const;
    const std::vector<std::unique_ptr<DespesaCartao>>& getDespesas() const;

    // Métodos que foram removidos ou substituídos:
    // double totalFatura() const; // Substituído por getFaturaAtual()
    // void calcularFatura();    // Lógica agora em recalcularFatura() e chamada internamente
    // double getSaldo() const;    // Substituído por getSaldoDisponivel()
    // double getFatura() const;   // Renomeado para getFaturaAtual()
};

#endif
