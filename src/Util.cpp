#include "../Util.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace Util {

void salvarContas(const vector<unique_ptr<Conta>>& contas, const string& nomeArquivo) {
    ofstream arq(nomeArquivo);
    if (!arq) return;

    for (const auto& conta : contas) {
        arq << "CONTA " << conta->getNome() << " " << conta->getSaldo() << "\n";
        for (const auto& despesa : conta->getDespesasDebito()) {
            arq << "  DESPESA_DEBITO " << despesa->getDescricao() << " " << despesa->getValor() << "\n";
        }
        for (const auto& cartao : conta->getCartoes()) {
            arq << "  CARTAO " << cartao->getNome() << "\n";
            for (const auto& despesa : cartao->getDespesas()) {
                arq << "    DESPESA_CARTAO " << despesa->getDescricao() << " " << despesa->getValor() << "\n";
            }
        }
        for (const auto& poupanca : conta->getPoupancas()) {
            arq << "  POUPANCA " << poupanca->getNome() << " " << poupanca->getSaldo() << "\n";
        }
    }
}

void carregarContas(vector<unique_ptr<Conta>>& contas, const string& nomeArquivo) {
    ifstream arq(nomeArquivo);
    if (!arq) return;

    string linha;
    Conta* contaAtual = nullptr;
    CartaoCredito* cartaoAtual = nullptr;

    while (getline(arq, linha)) {
        istringstream iss(linha);
        string tipo;
        iss >> tipo;

        if (tipo == "CONTA") {
            string nome;
            double saldo;
            iss >> ws;
            getline(iss, nome, ' ');
            iss >> saldo;
            contas.push_back(make_unique<Conta>(nome, saldo));
            contaAtual = contas.back().get();
        } else if (tipo == "DESPESA_DEBITO" && contaAtual) {
            string desc;
            double valor;
            iss >> ws;
            getline(iss, desc, ' ');
            iss >> valor;
            contaAtual->carregarDespesaDebito(desc, valor);
        } else if (tipo == "CARTAO" && contaAtual) {
            string nome;
            iss >> ws;
            getline(iss, nome);
            cartaoAtual = contaAtual->adicionarCartao(nome);
            
        } else if (tipo == "DESPESA_CARTAO" && cartaoAtual) {
            string desc;
            double valor;
            iss >> ws;
            getline(iss, desc, ' ');
            iss >> valor;
            cartaoAtual->adicionarDespesa(desc, valor);
        } else if (tipo == "POUPANCA" && contaAtual) {
            string nome;
            double valor;
            iss >> ws;
            getline(iss, nome, ' ');
            iss >> valor;
            contaAtual->criarPoupanca(nome, valor);
        }
    }
}

}
