#include <iostream>
#include <vector>
#include <memory>
#include <limits> // Required for numeric_limits
#include <stdexcept> // Required for runtime_error etc.
#include "../Conta.h"
#include "../Util.h"
#include "../date.h" // Include Date header

using namespace std;

// Helper function to read a valid date from user
Date lerDataUsuario() {
    int d, m, y;
    while (true) {
        cout << "Digite a data (DD MM AAAA): ";
        if (cin >> d >> m >> y) {
            try {
                Date data(d, m, y);
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpar buffer
                return data;
            } catch (const invalid_argument& e) {
                cerr << "Data inválida: " << e.what() << ". Tente novamente." << endl;
                cin.clear(); // Limpar flags de erro do cin
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpar buffer
            }
        } else {
            cerr << "Entrada inválida para data. Tente novamente." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void mostrarMenu() {
    cout << "\n--- Menu de Finanças Pessoais ---\n";
    cout << "1. Criar conta\n";
    cout << "2. Listar contas e detalhes\n";
    cout << "3. Adicionar despesa débito\n";
    cout << "4. Criar cartão de crédito\n";
    cout << "5. Adicionar despesa cartão\n";
    cout << "6. Criar poupança\n";
    cout << "7. Transferir entre contas\n";
    // --- Add new options later for editing/deleting/paying CC ---
    cout << "0. Sair\n";
    cout << "Escolha: ";
}

int main() {
    vector<unique_ptr<Conta>> contas;
    int opcao;
    string nome, descricao; // nomeArquivo initialized later
    double valor;
    int idx, idx2;

    // Initialize nomeArquivo. Ensure bin directory exists or handle path differently.
    string nomeArquivo = "bin/dados.txt"; // Simpler path for now, or use "bin/dados.txt" and ensure 'bin' exists
    // For "bin/dados.txt", you might need to create the directory "bin" manually first.

    // Attempt to load data at startup (optional)
    Util::carregarContas(contas, nomeArquivo);

    do {
        mostrarMenu();
        cin >> opcao;
        if (cin.fail()) {
            cerr << "Opção inválida. Por favor, insira um número." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcao = -1; // Invalid option to loop again
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpar o buffer do newline

        switch (opcao) {
            case 1:
                cout << "Nome da conta: ";
                getline(cin, nome);
                cout << "Saldo inicial: ";
                while (!(cin >> valor) || valor < 0) {
                    cout << "Saldo inválido. Digite um valor numérico não negativo: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                contas.push_back(make_unique<Conta>(nome, valor));
                cout << "Conta '" << nome << "' criada com sucesso." << endl;
                break;

            case 2:
                if (contas.empty()) {
                    cout << "Nenhuma conta cadastrada." << endl;
                    break;
                }
                cout << "\n--- Listagem de Contas ---\n";
                for (size_t i = 0; i < contas.size(); ++i) {
                    cout << i << ": " << contas[i]->getNome() << " - Saldo: R$" << contas[i]->getSaldo() << endl;
                    
                    cout << "  Despesas de débito:\n";
                    const auto& despesasDeb = contas[i]->getDespesasDebito();
                    if (despesasDeb.empty()) cout << "    Nenhuma despesa de débito.\n";
                    for (const auto& despesa : despesasDeb) {
                        cout << "    - " << despesa->getDescricao() << " (R$" << despesa->getValor() 
                             << " em " << despesa->getDate().toFormattedString() << ")\n";
                    }

                    cout << "  Cartões de crédito:\n";
                    const auto& cartoes = contas[i]->getCartoes();
                    if (cartoes.empty()) cout << "    Nenhum cartão de crédito.\n";
                    for (const auto& cartao : cartoes) {
                        cout << "    - Cartão: " << cartao->getNome() << " | Limite: R$" << cartao->getLimite()
                             << " | Fatura Atual: R$" << cartao->getFaturaAtual()
                             << " | Disponível: R$" << cartao->getSaldoDisponivel() << endl;
                        const auto& despesasCartao = cartao->getDespesas();
                        if (despesasCartao.empty()) cout << "      Nenhuma despesa no cartão.\n";
                        for (const auto& despesa : despesasCartao) {
                            cout << "      * " << despesa->getDescricao() << " (R$" << despesa->getValor()
                                 << " em " << despesa->getDate().toFormattedString() << ")\n";
                        }
                    }

                    cout << "  Poupanças:\n";
                    const auto& poupancas = contas[i]->getPoupancas();
                    if (poupancas.empty()) cout << "    Nenhuma poupança.\n";
                    for (const auto& poupanca : poupancas) {
                        cout << "    - Poupança: " << poupanca->getNome() << " - Saldo: R$" << poupanca->getSaldo() << endl;
                    }
                    cout << "------------------------" << endl;
                }
                break;

            case 3: // Adicionar despesa débito
                if (contas.empty()) { cout << "Crie uma conta primeiro.\n"; break; }
                cout << "Escolha a conta para adicionar despesa de débito (índice):\n";
                for (size_t i = 0; i < contas.size(); ++i) cout << i << ": " << contas[i]->getNome() << endl;
                cout << "Conta (índice): ";
                cin >> idx;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (idx >= 0 && idx < (int)contas.size()) {
                    cout << "Descrição da despesa: ";
                    getline(cin, descricao);
                    cout << "Valor da despesa: ";
                     while (!(cin >> valor) || valor <= 0) {
                        cout << "Valor inválido. Digite um valor numérico positivo: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    Date dataDespesa = lerDataUsuario();
                    contas[idx]->adicionarDespesaDebito(descricao, valor, dataDespesa);
                } else {
                    cout << "Índice de conta inválido.\n";
                }
                break;

            case 4: // Criar cartão de crédito
                if (contas.empty()) { cout << "Crie uma conta primeiro para associar o cartão.\n"; break; }
                cout << "Escolha a conta para adicionar o cartão (índice):\n";
                for (size_t i = 0; i < contas.size(); ++i) cout << i << ": " << contas[i]->getNome() << endl;
                cout << "Conta (índice): ";
                cin >> idx;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (idx >= 0 && idx < (int)contas.size()) {
                    cout << "Nome do cartão: ";
                    getline(cin, nome);
                    cout << "Limite do cartão: ";
                    double limiteCartao;
                    while (!(cin >> limiteCartao) || limiteCartao < 0) {
                        cout << "Limite inválido. Digite um valor numérico não negativo: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    contas[idx]->adicionarCartao(nome, limiteCartao);
                } else {
                    cout << "Índice de conta inválido.\n";
                }
                break;

            case 5: // Adicionar despesa cartão
                if (contas.empty()) { cout << "Crie uma conta e um cartão primeiro.\n"; break; }
                cout << "Escolha a conta do cartão (índice):\n";
                for (size_t i = 0; i < contas.size(); ++i) cout << i << ": " << contas[i]->getNome() << endl;
                cout << "Conta (índice): ";
                cin >> idx;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (idx >= 0 && idx < (int)contas.size()) {
                    if (contas[idx]->getCartoes().empty()) {
                        cout << "Esta conta não possui cartões de crédito.\n";
                        break;
                    }
                    cout << "Escolha o cartão para adicionar a despesa (índice):\n";
                    const auto& cartoesConta = contas[idx]->getCartoes(); // This is correct
                    for (size_t i = 0; i < cartoesConta.size(); ++i) {
                        cout << "  " << i << ": " << cartoesConta[i]->getNome() << endl;
                    }
                    cout << "Cartão (índice): ";
                    cin >> idx2;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Already done after lerDataUsuario

                    if (idx2 >= 0 && idx2 < (int)cartoesConta.size()) {
                        cout << "Descrição da despesa do cartão: ";
                        getline(cin, descricao); // Make sure cin.ignore was called before if needed
                        cout << "Valor da despesa do cartão: ";
                        while (!(cin >> valor) || valor <= 0) {
                            cout << "Valor inválido. Digite um valor numérico positivo: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        // cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Already done after lerDataUsuario
                        Date dataDespesaCartao = lerDataUsuario(); // This handles its own cin.ignore

                        // CORRECTED LINE:
                        // Use the -> operator directly on the unique_ptr from the const vector.
                        // This works because unique_ptr's -> gives non-const access to the managed object.
                        contas[idx]->getCartoes()[idx2]->adicionarDespesa(descricao, valor, dataDespesaCartao);

                    } else {
                        cout << "Índice de cartão inválido.\n";
                    }
                } else {
                    cout << "Índice de conta inválido.\n";
                }
                break;

            case 6: // Criar poupança
                if (contas.empty()) { cout << "Crie uma conta principal primeiro.\n"; break; }
                cout << "Escolha a conta principal para a poupança (índice):\n";
                for (size_t i = 0; i < contas.size(); ++i) cout << i << ": " << contas[i]->getNome() << endl;
                cout << "Conta (índice): ";
                cin >> idx;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (idx >= 0 && idx < (int)contas.size()) {
                    cout << "Nome da poupança: ";
                    getline(cin, nome);
                    cout << "Valor inicial para transferir para a poupança (0 se não quiser transferir agora): ";
                    while (!(cin >> valor) || valor < 0) {
                        cout << "Valor inválido. Digite um valor numérico não negativo: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    contas[idx]->criarPoupanca(nome, valor);
                } else {
                    cout << "Índice de conta inválido.\n";
                }
                break;

            case 7: // Transferir entre contas
                if (contas.size() < 2) { cout << "São necessárias pelo menos duas contas para transferir.\n"; break; }
                cout << "Contas disponíveis:\n";
                for (size_t i = 0; i < contas.size(); ++i) cout << i << ": " << contas[i]->getNome() << endl;
                cout << "Conta origem (índice): ";
                cin >> idx;
                cout << "Conta destino (índice): ";
                cin >> idx2;
                cout << "Valor a transferir: ";
                while (!(cin >> valor) || valor <= 0) {
                    cout << "Valor inválido. Digite um valor numérico positivo: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (idx >= 0 && idx2 >= 0 && idx < (int)contas.size() && idx2 < (int)contas.size() && idx != idx2) {
                    contas[idx]->transferir(*contas[idx2], valor);
                } else {
                    cout << "Índices de conta inválidos ou iguais.\n";
                }
                break;
            case 0:
                cout << "Encerrando programa...\n";
                break;

            default:
                cout << "Opção inválida. Tente novamente.\n";
        }

    } while (opcao != 0);
    Util::salvarContas(contas, nomeArquivo); // Save data before exiting
    return 0;
}
