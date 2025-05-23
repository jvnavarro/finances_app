#ifndef DATE_H
#define DATE_H

#include <string>
#include <stdexcept>
#include <iostream>

class Date {
private:
    int day;
    int month;
    int year;

    // Método auxiliar privado
    bool isLeapYear() const;

public:
    // Construtores
    Date(); // Data atual
    Date(int d, int m, int y);
    
    // Getters
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    
    // Setters com validação
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);
    
    // Operações
    bool isValid() const;
    std::string toString() const;
    std::string toFormattedString() const; // DD/MM/AAAA
    int daysBetween(const Date& other) const;
    Date addDays(int days) const;
    
    // Operadores
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;
    
    // Amigos para E/S
    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    friend std::istream& operator>>(std::istream& is, Date& date);
    
    // Constantes úteis
    static const int DAYS_IN_MONTH[12];
    static const std::string MONTH_NAMES[12];
};

#endif // DATE_H
