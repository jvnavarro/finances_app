#include "../CartaoCredito.h" // Certifique-se que o caminho está correto
#include "../Conta.h"         // Para o membro contaAssociada
#include "../date.h"          // Para o tipo Date
#include <iostream>          // Para std::cout
#include <numeric>           // Para std::accumulate (se necessário, mas recalculateFatura faz loop)

// Construtor atualizado
CartaoCredito::CartaoCredito(std::string nome, Conta* conta, double limiteCartao)
    : nome(nome), contaAssociada(conta), limite(limiteCartao > 0 ? limiteCartao : 0.0), fatura_atual(0.0) {
    // saldo_disponivel será calculado como limite - fatura_atual
    // Garante que o limite não seja negativo
}

// Método interno para recalcular a fatura_atual
void CartaoCredito::recalcularFatura() {
    fatura_atual = 0.0;
    for (const auto& despesa : despesas) {
        fatura_atual += despesa->getValor();
    }
}

// Método atualizado para adicionar despesa com data
void CartaoCredito::adicionarDespesa(const std::string& descricao, double valor, const Date& data) {
    if (valor <= 0) {
        std::cout << "Valor da despesa deve ser positivo." << std::endl;
        return;
    }
    if (getSaldoDisponivel() >= valor) {
        // A classe DespesaCartao agora também precisa de um construtor que aceite Date
        despesas.push_back(std::make_unique<DespesaCartao>(descricao, valor, data));
        recalcularFatura(); // Atualiza a fatura após adicionar a despesa
        std::cout << "Despesa adicionada ao cartao " << nome << ": " << descricao
                  << " - R$" << valor << " em " << data.toFormattedString() << std::endl;
    } else {
        std::cout << "Limite disponivel insuficiente no cartao " << nome
                  << " para a despesa de R$" << valor << ". Saldo disponivel: R$" << getSaldoDisponivel() << std::endl;
    }
}

void CartaoCredito::setLimite(double novoLimite) {
    if (novoLimite >= 0) {
        limite = novoLimite;
        std::cout << "Limite do cartao " << nome << " alterado para R$" << limite << std::endl;
        // A fatura não muda, mas o saldo disponível sim.
    } else {
        std::cout << "Limite deve ser um valor nao negativo." << std::endl;
    }
}

double CartaoCredito::getLimite() const {
    return limite;
}

double CartaoCredito::getSaldoDisponivel() const {
    return limite - fatura_atual;
}

double CartaoCredito::getFaturaAtual() const {
    // A fatura_atual é mantida atualizada por recalcularFatura()
    return fatura_atual;
}

std::string CartaoCredito::getNome() const {
    return nome;
}

const std::vector<std::unique_ptr<DespesaCartao>>& CartaoCredito::getDespesas() const {
    return despesas;
}

// Os métodos antigos como calcularFatura(), totalFatura(), getSaldo(), getFatura()
// foram removidos ou substituídos pelas novas implementações e nomes.
// Se você os tinha no seu .cpp, eles devem ser removidos.
