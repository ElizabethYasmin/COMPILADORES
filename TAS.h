
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
using namespace std;

namespace funtion{
template <class Tipo>
void print(vector<Tipo> container)
{
    for (int k = 0; k < container.size(); k++)
    {
        cout<< container[k];
    }
    cout<< "\n";
}}


class TAS
{
public:
    vector<string> NoTerminales;   
    vector<string> Terminales;  
    vector<vector<Production>> content;
    vector<vector<string>> siguientes;
    vector<vector<string>> primeros;

    void setNoTerminales(vector<Nodo> noTerminales);
    void setTerminales(vector<Nodo> terminales);
    void setPrimeros(vector<string> firsts);
    void setSiguientes(vector<string> nexts);

    Production getValue(string f, string c);
    void setValue(Production prod, string f, string c);
    void setValue(Production prod, int f, int c);
    void setUpContent();

    void readFromText(string fileName, int n, int m);

    void print();

    TAS() = default;
    ~TAS();
};



void TAS::setNoTerminales(vector<Nodo> noTerminales)
{
    for (int k = 0; k < noTerminales.size(); k++)
    {
        this->NoTerminales.push_back(noTerminales[k].getValue());
    }
}
void TAS::setTerminales(vector<Nodo> terminales)
{
    for (int k = 0; k < terminales.size(); k++)
    {
        if (terminales[k].getValue() != "")
            this->Terminales.push_back(terminales[k].getValue());
    }
    this->Terminales.push_back("$");
}


Production TAS::getValue(string f, string c)
{
    vector<Nodo> tempVector;
    Nodo tempNodo{"", NullType };
    tempVector.push_back(tempNodo);
    Production prod{tempNodo, tempVector};

    for (int k = 0; k < NoTerminales.size(); k++)
    {
        if (NoTerminales[k] == f)
        {
            for (int p = 0; p < Terminales.size(); p++)
            {
                if (Terminales[p] == c)
                    return content[k][p];
            }
        }
    }
    return prod;
}

void TAS::setValue(Production prod, string f, string c)
{
    for (int k = 0; k < NoTerminales.size(); k++)
    {
        if (NoTerminales[k] == f)
        {
            for (int p = 0; p < Terminales.size(); p++)
            {
                if (Terminales[p] == c)
                    content[k][p] = prod;
            }
        }
    }
}

void TAS::setValue(Production prod, int f, int c)
{
    content[f][c] = prod;
}



void TAS::setUpContent()
{
    vector<Production> tempVector;
    Nodo tempNodo{"vacio", NullType};
    vector<Nodo> tempRS;
    tempRS.push_back(tempNodo);
    Production tempProd{tempNodo, tempRS};
   
    
    for (int k = 0; k < this->Terminales.size(); k++)
    {
        
        tempVector.push_back(tempProd);
    }
    for (int k = 0; k < this->NoTerminales.size(); k++)
    {
        content.push_back(tempVector);
    }
}

void TAS::print()
{
   
    cout<< "Primeros-> \n";
    for (int k = 0; k < this->primeros.size(); k++)
    {
        cout<< this->NoTerminales[k]<<" -> ";
        funtion::print<string> (this->primeros[k]);
    }
    cout<< "Siguiente-> \n";
    for (int k = 0; k < this->primeros.size(); k++)
    {
        cout<< this->NoTerminales[k]<<" -> ";
        funtion::print<string> (this->siguientes[k]);
    }

    cout<<"------------------------------------TAS----------------------------------------"<<endl;
    for (int k = 0; k < content.size(); k++)
    {
        for (int p = 0; p < content[0].size(); p++)
        {
            
            cout<< content[k][p].toString();
        }
        cout<<endl;
    }
    cout<<"------------------------------------------------------------------"<<endl;

}



void TAS::readFromText(string fileName, int n, int m)
{
    ifstream file(fileName);
    string tempText;

    Production prod;
    int f = 0, c = 0; 
    while (getline(file, tempText))
    {
        if ( tempText != "[]" )
        {
            vector<Production> tempList = prod.readProduction(tempText);
            if (tempList.size() == 1)
                setValue(tempList[0], f, c);
        }
        c++;
        if (c >= n)
        {
            c = 0;
            f++;
        }
    }
}


TAS::~TAS()
{
}
