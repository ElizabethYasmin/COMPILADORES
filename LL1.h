
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
using namespace std;
class LL1
{
public:
    Grammar grammar;
    TAS tas;
    void setTAS();
    void setGrammar(string fileName);
    queue<string> getQueueFromString(string cadena);
    bool analyze(string cadena);
    LL1() = default;
    LL1(Grammar g );
};


LL1::LL1(Grammar g)
{
    this->grammar = g;
}

void LL1::setTAS()
{
    tas.setFilas(grammar.getNonTerminals());
    tas.setColumnas(grammar.getTerminals());
    
    for ( int k = 0; k < this->tas.filas.size(); k++)
    {
        this->tas.primeros.push_back(grammar.getPrimeros(this->tas.filas[k]));
        this->tas.siguientes.push_back(grammar.getSiguientes(this->tas.filas[k]));
    }
    tas.setUpContent();
}


void LL1::setGrammar(string fileName)
{
    this->grammar.readGrammarFromTXT(fileName);
    setTAS();
}

queue<string> LL1::getQueueFromString(string cadena)
{
    queue<string> tempQueue; 
    string temp = "";
    for (int k = 0; k < cadena.size(); k++)
    {
        if (cadena[k] != ' ')
            temp += cadena[k];
        else
        {
            if (temp != "")
            {
                tempQueue.push(temp);
                temp = "";
            }
        }
    }
    if (temp != "")
    {
        tempQueue.push(temp);
    }
    return tempQueue;
}


bool LL1::analizar(string cadena)
{
    stack<string> pila;
    pila.push( "$" );
    pila.push( this->grammar.getInitial() );

    queue<string> entrada = getQueueFromString(cadena);
    entrada.push( "$" );

    while( ! entrada.empty() && !pila.empty() ) 
    {
        if ( entrada.front() == pila.top() ) 
        {
            entrada.pop();
            pila.pop();
        }
        else 
        {
            string temp = pila.top();
            pila.pop();
            Production prod = this->tas.getValue(temp, entrada.front());
            if (prod.getLeftSide().getType() == NullType)
            {
                cout<< "esta cadena\""<< entrada.front() <<"\" xxxx\n";
                return false;
            }
            
            vector<Nodo> tempNodos = prod.getRightSide();

            for (int k = tempNodos.size() -1; k >= 0; k--)
            {
                if (tempNodos[k].getValue() != "")
                    pila.push(tempNodos[k].getValue());
            }
        }
    }

    return (entrada.empty() && pila.empty());
}