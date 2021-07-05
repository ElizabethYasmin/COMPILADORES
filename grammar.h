#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
using namespace std;

class Grammar
{
private:
    string initial;
    vector<Production> producciones;
    vector<Nodo> No_Terminales;
    vector<Nodo> Terminales;

public:
    Grammar() = default;
    Grammar(vector<Production> producciones);
    void readGrammarFromTXT(string fileName);
    void setElements(string init);
    
    string getInitial();
    vector<Production> getProductions();
    vector<Nodo> getNonTerminals();
    vector<Nodo> getTerminals();

    vector<Production> getProductions(string nt);
    
    void getPrimerosHelper(string name, vector<string> &firsts, vector<string> &visited);
    vector<string> getPrimeros2(string name, vector<string> &visited);
    vector<string> getPrimeros(string name);
    
    vector<string> getSiguientes(string name);
    vector<string> getSiguientes2(string name, vector<string> &visited);
    void getSiguientesHelper(string name, vector<string> &visited, vector<string> &nextList);

    void printPrimeros();
    void printSiguientes();

    void print(string archivo);
    
    ~Grammar();
};

Grammar::Grammar(vector<Production> producciones)
{
    this->producciones = producciones;
}

void Grammar::readGrammarFromTXT(string fileName)
{
    ifstream file(fileName);
    string tempText;
    Production prod;
    while (getline(file, tempText))
    {
        vector<Production> tempList = prod.readProduction(tempText);
        for (int k = 0; k < tempList.size(); k++)
        {
            producciones.push_back(tempList[k]);
        }
    }
    string init;
    init="X";
    setElements(init);
}

void Grammar::setElements(string init)
{
    for (int k = 0; k < producciones.size(); k++)
    {
        if(!extra::contains<Nodo>(producciones[k].getLeftSide(), No_Terminales))
            No_Terminales.push_back(producciones[k].getLeftSide());
        
        if (producciones[k].getLeftSide().getValue() == init)
            this->initial = init;

        auto temp = producciones[k].getRightSide();
        for (int p = 0; p < temp.size(); p++)
        {
            if ((temp[p].getType() == Terminal) && !extra::contains<Nodo>(temp[p], Terminales))
                Terminales.push_back(temp[p]);
        }
    }
}


string Grammar::getInitial()
{
    return this->initial;
}

vector<Nodo> Grammar::getNonTerminals()
{
    return No_Terminales;
}

vector<Nodo> Grammar::getTerminals()
{
    return Terminales;
}



vector<Production> Grammar::getProductions()
{
    return producciones;
}

vector<Production> Grammar::getProductions(string nt)
{
    vector<Production> NTproductions;
    for (int k = 0; k < this->producciones.size(); k++)
    {
        for (int p = 0; p < this->producciones[k].getRightSide().size(); p++)
        {
            if (this->producciones[k].getRightSide()[p].getValue() == nt)
                NTproductions.push_back(producciones[k]);
        }
    }
    return NTproductions;
}



void Grammar::getPrimerosHelper(string name, vector<string> &firsts, vector<string> &visited)
{
    if ( extra::contains(name, visited))
        return;
    string temp = "";
    if ( extra::contains<Nodo>(Nodo{name, Terminal}, Terminales) && !extra::contains<string>(name, firsts))
    {
        temp += "\"";
        temp += name;
        temp += "\"";
        firsts.push_back(temp);
    }
    else  // X is not  Terminal
    {
        vector<string> subResultado; // primeros de cada Y_i
        string epsilon = "";
        for (int k = 0; k < producciones.size(); k++)
        {
            if (producciones[k].getLeftSide().getValue() == name) // determinada produccion
            {
                visited.push_back(producciones[k].getLeftSide().getValue());
                int index = 0;
                int longitud = producciones[k].getRightSide().size();
                do
                {
                    subResultado = getPrimeros2(producciones[k].getRightSide()[index++].getValue(), visited); // Y_0
                    for (int t = 0; t < subResultado.size(); t++)
                    {
                        if (!extra::contains<string>(subResultado[t], firsts))
                            firsts.push_back(subResultado[t]);
                    }
                } 
                while (extra::contains<string>(epsilon, subResultado) && index < longitud);
            }
        }        
    }
}

vector<string> Grammar::getPrimeros2(string name, vector<string> &visited)
{
    vector<string> firstList;
    getPrimerosHelper(name, firstList, visited);
    return firstList;
}

vector<string> Grammar::getPrimeros(string name)
{
    vector<string> firstList;
    vector<string> visistedNodesList;
    getPrimerosHelper(name, firstList, visistedNodesList);
    return firstList;
}



void Grammar::getSiguientesHelper(string name, vector<string> &visited, vector<string> &nextList)
{
    if (extra::contains<string>(name, visited))
        return;
    else
        visited.push_back(name);

    vector<Nodo> prod_RS;
    int index;
    string epsilon = "\"\"";
    vector<string> temp;
    bool needRule3 = false;

    if (name == this->initial)
        nextList.push_back("$");
    
    for (int k = 0; k < producciones.size(); k++)    // Iterar por cada produccion
    {
        index = 0;
        prod_RS = producciones[k].getRightSide(); // prod.RS
        while (index < prod_RS.size() && prod_RS[index].getValue()!= name)
            index++;    // ubicacion del elemento del cual se obtiene sus SIGUIENTES

        if (index < prod_RS.size())
        {
            if (index == prod_RS.size() -1)
                extra::join<string>(nextList, getSiguientes2(producciones[k].getLeftSide().getValue(), visited));
            else
            {
                index++;
                do
                {
                    temp = getPrimeros(prod_RS[index].getValue());
                    extra::join<string>(nextList, temp);
                    
                    if (extra::contains<string>(epsilon, temp))
                        needRule3 = true;
                    else
                        needRule3 = false;
                    index++;
                }
                while ( index < prod_RS.size() && needRule3);
            }
            if (needRule3)
                extra::join<string>(nextList, getSiguientes2(producciones[k].getLeftSide().getValue(), visited));
        }
    }
}


vector<string> Grammar::getSiguientes(string name)
{
    vector<string> visitedNodes;
    vector<string> nextList; 

    getSiguientesHelper(name, visitedNodes, nextList);
    extra::deleteFrom<string>("\"\"", nextList);
    return nextList;
}

vector<string> Grammar::getSiguientes2(string name, vector<string> &visited)
{
    vector<string> nextList; // listas de siguientes
    getSiguientesHelper(name, visited, nextList);

    return nextList;
}

void Grammar::printPrimeros()
{
    vector<string>  temp;
    for (int k = 0; k < this->No_Terminales.size(); k++)
    {
        cout<< "Nodo:"<<No_Terminales[k].getValue()<<"\n Primeros:\t";
        temp = getPrimeros(this->No_Terminales[k].getValue());
        extra::printVector<string>(temp);
        cout<< "\n";
    }
    cout<< "\n";
}

void Grammar::printSiguientes()
{
    vector<string>  temp;
    for (int k = 0; k < this->No_Terminales.size(); k++)
    {
        cout<< "Nodo:"<<No_Terminales[k].getValue()<<"\n Siguientes:\t";
        temp = getSiguientes(this->No_Terminales[k].getValue());
        extra::printVector<string>(temp);
        cout<< "\n";
    }
    cout<< "\n";
}


void Grammar::print(string archivo)
{
    cout<<"GRAMMAR"<<endl;
    ifstream file(archivo);
    string tempText;
    Production prod;
    while ( getline(file, tempText)){
        cout<<tempText<<endl;
    }
}

Grammar::~Grammar()
{
}
