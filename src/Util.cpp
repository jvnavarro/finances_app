#include "../Util.h"
#include "../date.h" // For Date class and its I/O
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> // For std::quoted

using namespace std;

namespace Util
{

    // Helper to read a quoted string or a single word if not quoted
    std::string readPossiblyQuotedString(std::istream &is)
    {
        std::string res;
        is >> ws; // Consume leading whitespace
        char next_char = is.peek();
        if (next_char == '"')
        {
            is >> std::quoted(res);
        }
        else
        {
            is >> res; // Read as a single word if not quoted (for backward compatibility or simple names)
                       // Or enforce quoting by throwing an error if quote not found.
                       // For now, let's try to be flexible but saving will always use quotes.
        }
        return res;
    }

    void salvarContas(const vector<unique_ptr<Conta>> &contas, const string &nomeArquivo)
    {
        ofstream arq(nomeArquivo);
        if (!arq)
        {
            cerr << "Erro ao abrir arquivo para salvar: " << nomeArquivo << endl;
            return;
        }

        for (const auto &conta : contas)
        {
            // Use std::quoted for names that might contain spaces
            arq << "CONTA " << std::quoted(conta->getNome()) << " " << conta->getSaldo() << "\n";
            for (const auto &despesa : conta->getDespesasDebito())
            {
                arq << "  DESPESA_DEBITO " << std::quoted(despesa->getDescricao()) << " "
                    << despesa->getValor() << " " << despesa->getDate().toFormattedString() << "\n"; // Added Date
            }
            for (const auto &cartao : conta->getCartoes())
            {
                arq << "  CARTAO " << std::quoted(cartao->getNome()) << " " << cartao->getLimite() << "\n"; // Added Limite
                for (const auto &despesa : cartao->getDespesas())
                {
                    arq << "    DESPESA_CARTAO " << std::quoted(despesa->getDescricao()) << " "
                        << despesa->getValor() << " " << despesa->getDate().toFormattedString() << "\n"; // Added Date
                }
            }
            for (const auto &poupanca : conta->getPoupancas())
            {
                // Assuming poupanca (which is a Conta) also needs quoted name
                arq << "  POUPANCA " << std::quoted(poupanca->getNome()) << " " << poupanca->getSaldo() << "\n";
            }
        }
        cout << "Contas salvas em " << nomeArquivo << endl;
    }

    void carregarContas(vector<unique_ptr<Conta>> &contas, const string &nomeArquivo)
    {
        ifstream arq(nomeArquivo);
        if (!arq)
        {
            cerr << "Erro ao abrir arquivo para carregar: " << nomeArquivo << " (Pode não existir ainda)." << endl;
            return;
        }

        contas.clear(); // Clear existing data before loading

        string linha;
        Conta *contaAtual = nullptr;
        CartaoCredito *cartaoAtual = nullptr;

        while (getline(arq, linha))
        {
            if (linha.empty() || linha.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
            { // Skip empty or whitespace-only lines
                continue;
            }
            istringstream iss(linha);
            string tipo;
            iss >> tipo;

            if (tipo == "CONTA")
            {
                string nome;
                double saldo;
                // iss >> ws; // std::quoted handles leading whitespace
                // getline(iss, nome, ' '); // Old way, problematic
                iss >> std::quoted(nome); // Read quoted name
                iss >> saldo;
                if (iss.fail())
                {
                    cerr << "Erro ao ler dados da CONTA. Linha: " << linha << endl;
                    continue;
                }
                contas.push_back(make_unique<Conta>(nome, saldo));
                contaAtual = contas.back().get();
                cartaoAtual = nullptr; // Reset cartaoAtual when a new conta is processed
            }
            else if (tipo == "DESPESA_DEBITO" && contaAtual)
            {
                string desc;
                double valor;
                string dataStr;
                Date dataDespesa;
                // iss >> ws;
                // getline(iss, desc, ' '); // Old way
                iss >> std::quoted(desc); // Read quoted description
                iss >> valor >> dataStr;

                if (iss.fail())
                {
                    cerr << "Erro ao ler dados da DESPESA_DEBITO (valor/dataStr). Linha: " << linha << endl;
                    continue;
                }

                istringstream dateStream(dataStr);
                dateStream >> dataDespesa; // Use Date's operator>>
                if (dateStream.fail() || !dataDespesa.isValid())
                {
                    cerr << "Data inválida para DESPESA_DEBITO: " << dataStr << ". Usando data padrão (hoje)." << endl;
                    dataDespesa = Date(); // Default to today if parsing fails or is invalid
                }
                contaAtual->carregarDespesaDebito(desc, valor, dataDespesa);
            }
            else if (tipo == "CARTAO" && contaAtual)
            {
                string nomeCartao;
                double limiteCartao = 0.0; // Default limit if not found, or make it mandatory
                // iss >> ws;
                // getline(iss, nome); // Old way
                iss >> std::quoted(nomeCartao); // Read quoted name
                iss >> limiteCartao;            // Read limit
                if (iss.fail())
                { // If limit reading fails (e.g., old format file)
                    cerr << "Aviso: Limite do cartão '" << nomeCartao << "' não encontrado ou inválido. Usando 0.0. Linha: " << linha << endl;
                    iss.clear(); // Clear error flags
                    // Try to re-parse name if limit was part of it due to old format
                    // This part can be tricky for backward compatibility.
                    // For now, assume new format or default limit.
                    limiteCartao = 0.0; // Reset if parsing failed.
                }
                cartaoAtual = contaAtual->adicionarCartao(nomeCartao, limiteCartao);
            }
            else if (tipo == "DESPESA_CARTAO" && cartaoAtual)
            {
                string desc;
                double valor;
                string dataStr;
                Date dataDespesa;
                // iss >> ws;
                // getline(iss, desc, ' '); // Old way
                iss >> std::quoted(desc); // Read quoted description
                iss >> valor >> dataStr;

                if (iss.fail())
                {
                    cerr << "Erro ao ler dados da DESPESA_CARTAO (valor/dataStr). Linha: " << linha << endl;
                    continue;
                }

                istringstream dateStream(dataStr);
                dateStream >> dataDespesa; // Use Date's operator>>
                if (dateStream.fail() || !dataDespesa.isValid())
                {
                    cerr << "Data inválida para DESPESA_CARTAO: " << dataStr << ". Usando data padrão (hoje)." << endl;
                    dataDespesa = Date(); // Default to today
                }
                cartaoAtual->adicionarDespesa(desc, valor, dataDespesa);
            }
            else if (tipo == "POUPANCA" && contaAtual)
            {
                string nomePoupanca;
                double valorInicial;
                iss >> std::quoted(nomePoupanca); // Read quoted name
                iss >> valorInicial;
                if (iss.fail())
                {
                    cerr << "Erro ao ler dados da POUPANCA. Linha: " << linha << endl;
                    continue;
                }
                // CORRECTED LINE: Use the dedicated loading method
                contaAtual->criarPoupancaParaCarregamento(nomePoupanca, valorInicial); // Or change criarPoupanca to not debit if loading
                // For simplicity, let's assume criarPoupanca handles this, or we need a carregarPoupanca
                // The current `criarPoupanca` debits from the main account. This is not desired during loading.
                // We'll need a separate mechanism or modify criarPoupanca.
                // Quick fix for loading: directly add to poupancas vector.
                // This requires a non-const getter or making Util a friend class.
                // Let's add a temporary public method in Conta for loading poupancas if needed,
                // or adjust criarPoupanca for loading context.
                // For now, the line below is conceptual.
                // contaAtual->carregarPoupanca(nomePoupanca, valorInicial); // Ideal
                // If no such method: (This is a hack, shows design consideration)
                //    std::vector<std::unique_ptr<Conta>>* poupancasVec = const_cast<std::vector<std::unique_ptr<Conta>>*>(&contaAtual->getPoupancas());
                //    poupancasVec->push_back(make_unique<Conta>(nomePoupanca, valorInicial));
                // A better way would be a dedicated loading method in Conta:
                // void Conta::carregarPoupanca(const std::string& nome, double saldo) {
                //     poupancas.push_back(std::make_unique<Conta>(nome, saldo));
                // }
                // For now, I'll assume you'll add such a method to Conta.cpp:
                // In Conta.h: void carregarPoupanca(const std::string& nome, double saldoInicial);
                // In Conta.cpp: void Conta::carregarPoupanca(const std::string& nome, double saldoInicial) {
                //                    poupancas.push_back(std::make_unique<Conta>(nome, saldoInicial));
                //                }
                // Then here:
                //contaAtual->criarPoupancaParaCarregamento(nomePoupanca, valorInicial); // Assume this new method exists in Conta
            }
            else
            {
                if (!linha.empty() && tipo.rfind("//", 0) != 0 && !tipo.empty())
                { // not a comment and not empty
                    cerr << "Tipo de linha desconhecido ou contexto inválido no arquivo de dados: '" << tipo << "'" << endl;
                }
            }
        }
        if (contas.empty() && arq.eof() && !arq.bad() && arq.tellg() > 0)
        {
            // File was not empty but no accounts were loaded, might indicate format issues.
            cout << "Arquivo de dados lido, mas nenhuma conta carregada. Verifique o formato do arquivo." << endl;
        }
        else if (!contas.empty())
        {
            cout << "Dados carregados de " << nomeArquivo << endl;
        }
    }

} // namespace Util