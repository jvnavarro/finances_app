#include <iostream>
#include <vector>
#include <memory>
#include "../Conta.h"
#include "../Util.h"

using namespace std;

void mostrarMenu() {
    cout << "\n--- Menu de Finanças Pessoais ---\n";
    cout << "1. Criar conta\n";
    cout << "2. Listar contas\n";
    cout << "3. Adicionar despesa débito\n";
    cout << "4. Criar cartão de crédito\n";
    cout << "5. Adicionar despesa cartão\n";
    cout << "6. Criar poupança\n";
    cout << "7. Transferir entre contas\n";
    cout << "8. Salvar dados\n";
    cout << "9. Carregar dados\n";
    cout << "0. Sair\n";
    cout << "Escolha: ";
}

int main() {
    vector<unique_ptr<Conta>> contas;
    int opcao;
    string nome, descricao, nomeArquivo = "bin/dados.txt";
    double valor;
    int idx, idx2;

    do {
        mostrarMenu();
        cin >> opcao;
        cin.ignore();

        switch (opcao) {
            case 1:
                cout << "Nome da conta: ";
                getline(cin, nome);
                cout << "Saldo inicial: ";
                cin >> valor;
                contas.push_back(make_unique<Conta>(nome, valor));
                break;

            case 2:
                for (size_t i = 0; i < contas.size(); ++i) {
                    cout << i << ": " << contas[i]->getNome() << " - R$" << contas[i]->getSaldo() << endl;
                    cout << "  Despesas de débito:\n";
                    const auto& despesas = contas[i]->getDespesasDebito();
                    for (const auto& despesa : despesas) {
                        cout << "    " << despesa->getDescricao() << " - R$" << despesa->getValor() << endl;
                    }
                    cout << "  Cartões de crédito:\n";
                    const auto& cartoes = contas[i]->getCartoes();
                    for (const auto& cartao : cartoes) {
                        cout << "    " << cartao->getNome() << " - R$" << cartao->getLimite() << endl;
                        const auto& despesasCartao = cartao->getDespesas();
                        for (const auto& despesa : despesasCartao) {
                            cout << "      " << despesa->getDescricao() << " - R$" << despesa->getValor() << endl;
                        }
                    }
                    cout << "  Poupanças:\n";
                    const auto& poupancas = contas[i]->getPoupancas();
                    for (const auto& poupanca : poupancas) {
                        cout << "    " << poupanca->getNome() << " - R$" << poupanca->getSaldo() << endl;
                    }

                }
                break;

            case 3:
                for (size_t i = 0; i < contas.size(); ++i) {
                    cout << i << ": " << contas[i]->getNome() << " - R$" << contas[i]->getSaldo() << endl;
                }
                cout << "Conta (índice): ";
                cin >> idx;
                cin.ignore();
                if (idx >= 0 && idx < (int)contas.size()) {
                    cout << "Descrição: ";
                    getline(cin, descricao);
                    cout << "Valor: ";
                    cin >> valor;
                    contas[idx]->adicionarDespesaDebito(descricao, valor);
                }
                break;

            case 4:
                for (size_t i = 0; i < contas.size(); ++i) {
                    cout << i << ": " << contas[i]->getNome() << " - R$" << contas[i]->getSaldo() << endl;
                }
                cout << "Conta (índice): ";
                cin >> idx;
                cin.ignore();
                if (idx >= 0 && idx < (int)contas.size()) {
                    cout << "Nome do cartão: ";
                    getline(cin, nome);
                    contas[idx]->adicionarCartao(nome);
                }
                break;

            case 5:
                for (size_t i = 0; i < contas.size(); ++i) {
                    cout << i << ": " << contas[i]->getNome() << " - R$" << contas[i]->getSaldo() << endl;
                }
                cout << "Conta (índice): ";
                cin >> idx;
                cin.ignore();
                if (idx >= 0 && idx < (int)contas.size()) {
                    const auto& cartoes = contas[idx]->getCartoes();
                    for (size_t i = 0; i < cartoes.size(); ++i) {
                        cout << "  " << i << ": " << cartoes[i]->getNome() << endl;
                    }
                    cout << "Cartão (índice): ";
                    cin >> idx2;
                    cin.ignore();
                    if (idx2 >= 0 && idx2 < (int)cartoes.size()) {
                        cout << "Descrição: ";
                        getline(cin, descricao);
                        cout << "Valor: ";
                        cin >> valor;
                        cartoes[idx2]->adicionarDespesa(descricao, valor);
                    }
                }
                break;

            case 6:
                for (size_t i = 0; i < contas.size(); ++i) {
                    cout << i << ": " << contas[i]->getNome() << " - R$" << contas[i]->getSaldo() << endl;
                }
                cout << "Conta (índice): ";
                cin >> idx;
                cin.ignore();
                if (idx >= 0 && idx < (int)contas.size()) {
                    cout << "Nome da poupança: ";
                    getline(cin, nome);
                    cout << "Valor inicial: ";
                    cin >> valor;
                    contas[idx]->criarPoupanca(nome, valor);
                }
                break;

            case 7:
                for (size_t i = 0; i < contas.size(); ++i) {
                    cout << i << ": " << contas[i]->getNome() << " - R$" << contas[i]->getSaldo() << endl;
                }
                cout << "Conta origem (índice): ";
                cin >> idx;
                cout << "Conta destino (índice): ";
                cin >> idx2;
                cout << "Valor: ";
                cin >> valor;
                if (idx >= 0 && idx2 >= 0 && idx < (int)contas.size() && idx2 < (int)contas.size()) {
                    contas[idx]->transferir(*contas[idx2], valor);
                }
                break;

            case 8:
                Util::salvarContas(contas, nomeArquivo);
                cout << "Dados salvos.\n";
                break;

            case 9:
                contas.clear();
                Util::carregarContas(contas, nomeArquivo);
                cout << "Dados carregados.\n";
                break;

            case 0:
                cout << "Encerrando programa...\n";
                break;

            default:
                cout << "Opção inválida.\n";
        }

    } while (opcao != 0);

    return 0;
}

