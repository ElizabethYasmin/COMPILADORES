#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
using namespace std;

enum Type
{
    Terminal,
    NonTerminal,
    NullType
};

class Nodo
{
private:
    string value;
    Type type;

public:
    Nodo();
    Nodo(string value, Type type);
    Type getType();
    string getValue();
    void setValues(string value, Type type);
    vector<Nodo> readNodo(string &text, int start, int end);
    string toString();
    bool operator ==(const Nodo &t) const;
    friend ostream &operator<<( ostream &output, Nodo &n );

    ~Nodo();
};
Nodo::Nodo()
{
    this->value = "--void--";
    this->type = NullType;
}

Nodo::Nodo(string value, Type type) 
{
    this->value = value;
    this->type = type;
}

Type Nodo::getType()
{
    return type;
}

string Nodo::getValue()
{
    return value;
}

void Nodo::setValues(string value, Type type)
{
    this->value = value;
    this->type = type;
}

vector<Nodo> Nodo::readNodo(string &text, int start, int end)
{
    bool opened = false;
    bool nonTerminal;
    string tempText = "";
    Nodo nodo;
    vector<Nodo> NodoList;
    for (int k = start; k <= end; k++)
    {
        if (text[k] != ' ')
        {
            if (text[k] == '<')
            {
                opened = true;
                nonTerminal = true;
            }
            else if (text[k] == '\"' && !opened)
            {
                opened = true;
                nonTerminal = false;
            }
            else if (opened && (text[k] == '>' || text[k] == '\"'))
            {
                opened = false;
                if (nonTerminal)
                    nodo.setValues(tempText, NonTerminal);
                else
                    nodo.setValues(tempText, Terminal);
                NodoList.push_back(nodo);
                tempText = "";
            }
            else if (opened)
                tempText += text[k];
        }
    }
    return NodoList;
}

string Nodo::toString()
{
    string temp = "";
    temp += "Valor: ";
    temp += this->value;
    temp += "\t";
    temp += "Tipo: ";
    switch (this->type)
    {
    case NonTerminal:
        temp += "NonTerminal";
        break;
    case Terminal:
        temp += "Terminal";
        break;
    case NullType:
        temp += "NullType";
        break;
    }
    temp += "\n";

    return temp;
}


bool Nodo::operator ==(const Nodo &t) const
{
    if ((this->type == t.type) && (this->value == t.value))
        return true;
    return false;
}

ostream &operator<<( ostream &output, Nodo &n )
{
    output<< "Valor: ";
    output<< n.getValue()<< "\t";
    output<< "Tipo: ";
    switch (n.getType())
    {
    case NonTerminal:
        output<< "NonTerminal";
        break;
    case Terminal:
        output<< "Terminal";
        break;
    case NullType:
        output<< "NullType";
        break;
    }
    output<< "\n";
}


Nodo::~Nodo()
{
}
