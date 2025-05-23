#ifndef DESPESA_H
#define DESPESA_H
using namespace std;
#include <string>

class Despesa {
protected:
    string descricao;
    double valor;
public:
    Despesa(string descricao, double valor);
    virtual ~Despesa() = default;

    string getDescricao() const;
    double getValor() const;
    virtual string getTipo() const = 0;
};

class DespesaDebito : public Despesa {
public:
    DespesaDebito(string descricao, double valor);
    string getTipo() const override;
};

class DespesaCartao : public Despesa {
public:
    DespesaCartao(string descricao, double valor);
    string getTipo() const override;
};

#endif