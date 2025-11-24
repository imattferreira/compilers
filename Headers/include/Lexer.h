#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <string>
#include <vector>

using namespace std;

class Lexer
{
public:
    Lexer(const string& codigo);
    vector<Token> Analisar();

private:
    vector<Token> tokens;
    int i, estado_atual;
    string lexema, codigo;

    void inicializar();
    void q0();
    void q1();
    void q2();
    void q3();
    void q4();
    void q5();
    void q6();
    void q7();
    void q8();
    void q9();
    void q10();

    bool isEspaco(char c);
    bool isDigito(char c);
    bool isOperadorAritmetico(char c);
    bool isLetra(char c);
    bool isUnderline(char c);
    bool isOperadorRelacional(char c);
    bool isOperadorLogico(char c);
    bool isPontuacao(char c);
};

#endif
