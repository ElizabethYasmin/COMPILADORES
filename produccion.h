#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
using namespace std;
class Production
{
private:
    Nodo leftSide;
    vector<Nodo> rightSide;
    float probability;

public:
    Production() = default;
    Production(Nodo leftSide, vector<Nodo> rightSide);
    Production(Nodo leftSide, vector<Nodo> rightSide, float probability);
    Nodo getLeftSide();
    vector<Nodo> getRightSide();
    void setLeftSide(Nodo LeftSide);
    void setRightSide(vector<Nodo> RightSide);
    vector<Production> readProduction(string production);
    void readLeftSide(string &production, int &index);
    vector<Nodo> readRightSide(string &production, int &index, int end);    
    vector<vector<Nodo>> readRightSideHelper(string &production, int &index);
    string toString();
    ~Production();
};

Production::Production(Nodo leftSide, vector<Nodo> rightSide)
{
    this->leftSide = leftSide;
    this->rightSide = rightSide;
}

Production::Production(Nodo leftSide, vector<Nodo> rightSide, float probability)
{
    this->leftSide = leftSide;
    this->rightSide = rightSide;
    this->probability = probability;
}

Nodo Production::getLeftSide()
{
    return leftSide;
}

vector<Nodo> Production::getRightSide()
{
    return rightSide;
}

void Production::setLeftSide(Nodo LeftSide)
{
    this->leftSide = LeftSide;
}

void Production::setRightSide(vector<Nodo> RightSide)
{
    this->rightSide = RightSide;
}

void Production::readLeftSide(string &production, int &index)
{
    while (index < production.size() && production[index] != ':')
        index++;
    Nodo Nodo;
    if (Nodo.readNodo(production, 0, index - 1)[0].getType() == NonTerminal)
    {
        leftSide = Nodo.readNodo(production, 0, index)[0];
    }
    else
        cout << "Produccion incorrecta\n";  
        
    index += 3; 
}

vector<Nodo> Production::readRightSide(string &production, int &index, int end)
{
    Nodo nodo;
    vector<Nodo> NodoList = nodo.readNodo(production, index, end);
    return NodoList;
}

vector<vector<Nodo>> Production::readRightSideHelper(string &production, int &index)
{
    vector<vector<Nodo>> RightSideList;
    int prevIndex = index;

    while (index < production.size())
    {
        if (production[index] == '|')
        {
            RightSideList.push_back(readRightSide(production, prevIndex, index - 1));
            prevIndex = index + 1;
        }
        index++;
    }
    RightSideList.push_back(readRightSide(production, prevIndex, index));

    return RightSideList;
}

vector<Production> Production::readProduction(string production)
{
    int index = 0;
    vector<Production> productions;

    readLeftSide(production, index);
    vector<vector<Nodo>> tempList = readRightSideHelper(production, index);

    Production prod;
    prod.setLeftSide(leftSide);

    for (int k = 0; k < tempList.size(); k++)
    {
        prod.setRightSide(tempList[k]);        
        productions.push_back(prod);
    }

    return productions;
}

string Production::toString()
{
    string temp = "";
    temp += "|";
    temp += leftSide.getValue();
    temp += "| -> |  ";
    for (int k = 0; k < rightSide.size(); k++)
    {
        if (rightSide[k].getType() == Terminal)
        {
            temp += "\"";
            temp += rightSide[k].getValue();
            temp += "\"";
        }
        else if (rightSide[k].getType() != NullType)
            temp += rightSide[k].getValue();
        temp += "  ";
    }
    temp += "|   ";
    return temp;
}

Production::~Production()
{
}
