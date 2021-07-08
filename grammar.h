#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
using namespace std;
using namespace std;

namespace funcion
{

template<class Tipo>
bool contenido(Tipo t, vector<Tipo> container)
{
    for (int k = 0; k < container.size(); k++)
    {
        if (container[k] == t)
            return true;
    }
    return false;
}

template <class Tipo>
void printVector(vector<Tipo> container)
{
    for (int k = 0; k < container.size(); k++)
    {
        cout<< container[k]<< " ";
    }
    cout<< "\n";
}


template <class Tipo> // Copia los elementos de Origen que no estan en Destino
void join (vector<Tipo> &destino, vector<Tipo> origen )
{
    bool existe;
    if ( destino.size() == 0)
        destino = origen;
    
    for (int k = 0; k < origen.size(); k++)
    {
        existe = false;
        for (int p = 0; p < destino.size(); p++)
        {
            if (origen[k] == destino[p])
                existe = true;
        }
        if (!existe)
            destino.push_back(origen[k]);
    }
}


template <class Tipo>
void deleteFrom(Tipo elemento, vector<Tipo> &container){
    for (int k = 0; k < container.size(); k++)
    {
        if (container[k] == elemento)
            container.erase(container.begin()+k);
    }
}
}
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
        if(!funcion::contenido<Nodo>(producciones[k].getLeftSide(), No_Terminales))
            No_Terminales.push_back(producciones[k].getLeftSide());
        
        if (producciones[k].getLeftSide().getValue() == init)
            this->initial = init;

        auto temp = producciones[k].getRightSide();
        for (int p = 0; p < temp.size(); p++)
        {
            if ((temp[p].getType() == Terminal) && !funcion::contenido<Nodo>(temp[p], Terminales))
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
    if ( funcion::contenido(name, visited))
        return;
    string temp = "";
    if ( funcion::contenido<Nodo>(Nodo{name, Terminal}, Terminales) && !funcion::contenido<string>(name, firsts))
    {
        temp += "\"";
        temp += name;
        temp += "\"";
        firsts.push_back(temp);
    }
    else  //No terminal
    {
        vector<string> subResultado; // primeros
        string epsilon = "";
        for (int k = 0; k < producciones.size(); k++)
        {
            if (producciones[k].getLeftSide().getValue() == name) // Produccion
            {
                visited.push_back(producciones[k].getLeftSide().getValue());
                int index = 0;
                int longitud = producciones[k].getRightSide().size();
                do
                {
                    subResultado = getPrimeros2(producciones[k].getRightSide()[index++].getValue(), visited);
                    for (int t = 0; t < subResultado.size(); t++)
                    {
                        if (!funcion::contenido<string>(subResultado[t], firsts))
                            firsts.push_back(subResultado[t]);
                    }
                } 
                while (funcion::contenido<string>(epsilon, subResultado) && index < longitud);
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
    if (funcion::contenido<string>(name, visited))
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
    
    for (int k = 0; k < producciones.size(); k++)    // Recorrer por cada produccion
    {
        index = 0;
        prod_RS = producciones[k].getRightSide(); 
        while (index < prod_RS.size() && prod_RS[index].getValue()!= name)
            index++;    // ubicacion de siguientes

        if (index < prod_RS.size())
        {
            if (index == prod_RS.size() -1)
                funcion::join<string>(nextList, getSiguientes2(producciones[k].getLeftSide().getValue(), visited));
            else
            {
                index++;
                do
                {
                    temp = getPrimeros(prod_RS[index].getValue());
                    funcion::join<string>(nextList, temp);
                    
                    if (funcion::contenido<string>(epsilon, temp))
                        needRule3 = true;
                    else
                        needRule3 = false;
                    index++;
                }
                while ( index < prod_RS.size() && needRule3);
            }
            if (needRule3)
                funcion::join<string>(nextList, getSiguientes2(producciones[k].getLeftSide().getValue(), visited));
        }
    }
}


vector<string> Grammar::getSiguientes(string name)
{
    vector<string> visitedNodes;
    vector<string> nextList; 

    getSiguientesHelper(name, visitedNodes, nextList);
    funcion::deleteFrom<string>("\"\"", nextList);
    return nextList;
}

vector<string> Grammar::getSiguientes2(string name, vector<string> &visited)
{
    vector<string> nextList; 
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
        funcion::printVector<string>(temp);
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
        funcion::printVector<string>(temp);
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
