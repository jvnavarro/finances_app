#ifndef DESPESA_H
#define DESPESA_H
using namespace std;
#include <string>
#include "date.h" // Added include

class Despesa {
protected:
    string descricao;
    double valor;
    Date data; // Added Date member

public:
    // Modified constructor to include Date
    Despesa(string descricao, double valor, const Date& data);
    virtual ~Despesa() = default;

    string getDescricao() const;
    double getValor() const;
    Date getDate() const; // Added Date getter
    virtual string getTipo() const = 0;
};

class DespesaDebito : public Despesa {
public:
    // Modified constructor
    DespesaDebito(string descricao, double valor, const Date& data);
    string getTipo() const override;
};

class DespesaCartao : public Despesa {
public:
    // Modified constructor
    DespesaCartao(string descricao, double valor, const Date& data);
    string getTipo() const override;
};

#endif