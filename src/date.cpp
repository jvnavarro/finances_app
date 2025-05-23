#include "../date.h"
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

// Constantes estáticas
const int Date::DAYS_IN_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const string Date::MONTH_NAMES[12] = {"Janeiro", "Fevereiro", "Março", "Abril", 
                                     "Maio", "Junho", "Julho", "Agosto", 
                                     "Setembro", "Outubro", "Novembro", "Dezembro"};

// Construtor padrão (data atual)
Date::Date() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    day = now->tm_mday;
    month = now->tm_mon + 1;
    year = now->tm_year + 1900;
}

// Construtor parametrizado
Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    if (!isValid()) {
        throw invalid_argument("Data inválida no construtor");
    }
}

bool Date::isLeapYear() const {
    return (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
}

bool Date::isValid() const {
    if (year < 1) return false;
    if (month < 1 || month > 12) return false;
    
    int maxDays = DAYS_IN_MONTH[month - 1];
    if (month == 2 && isLeapYear()) {
        maxDays = 29;
    }
    
    return day >= 1 && day <= maxDays;
}

void Date::setDay(int d) {
    int oldDay = day;
    day = d;
    if (!isValid()) {
        day = oldDay;
        throw invalid_argument("Dia inválido");
    }
}

void Date::setMonth(int m) {
    int oldMonth = month;
    month = m;
    if (!isValid()) {
        month = oldMonth;
        throw invalid_argument("Mês inválido");
    }
}

void Date::setYear(int y) {
    int oldYear = year;
    year = y;
    if (!isValid()) {
        year = oldYear;
        throw invalid_argument("Ano inválido");
    }
}

string Date::toString() const {
    return to_string(day) + " de " + MONTH_NAMES[month - 1] + " de " + to_string(year);
}

string Date::toFormattedString() const {
    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d", day, month, year);
    return string(buffer);
}

int Date::daysBetween(const Date& other) const {
    // Implementação simplificada (para versão completa, considerar algoritmo preciso)
    int daysThis = year * 365 + month * 30 + day;
    int daysOther = other.year * 365 + other.month * 30 + other.day;
    return abs(daysThis - daysOther);
}

Date Date::addDays(int days) const {
    // Implementação básica - versão completa requeriria cálculo de dias por mês/ano
    Date newDate(*this);
    newDate.day += days;
    
    // Ajustar para meses/anos
    while (newDate.day > DAYS_IN_MONTH[newDate.month - 1]) {
        newDate.day -= DAYS_IN_MONTH[newDate.month - 1];
        newDate.month++;
        if (newDate.month > 12) {
            newDate.month = 1;
            newDate.year++;
        }
    }
    
    return newDate;
}

// Operadores de comparação
bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator>(const Date& other) const {
    return other < *this;
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<=(const Date& other) const {
    return !(*this > other);
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

// Operadores de E/S
ostream& operator<<(ostream& os, const Date& date) {
    os << date.toFormattedString();
    return os;
}

istream& operator>>(istream& is, Date& date) {
    char sep;
    int d, m, y;
    is >> d >> sep >> m >> sep >> y;
    
    date = Date(d, m, y); // Usa o construtor que já valida
    return is;
}
