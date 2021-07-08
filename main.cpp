#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
#include "estado.h"
#include "produccion.h"
#include "grammar.h"
#include "TAS.h"
#include "LL1.h"
using namespace std;
int main()
{
    LL1 ll1;
    ll1.setGrammar("grammar.txt");
    ll1.tas.readFromText("modelo.txt", 6, 5);//para el ejemplo de la pagina 
    ll1.grammar.print("grammar.txt");
    ll1.tas.print();
    string c ;
    cout<< "ANALIZAR->";
    cin.ignore();
    getline(cin, c);
    if (ll1.analizar(c)==true)
        cout<< "NO ACEPTADO";
    else
        cout<< "ACEPTADO";

    return 0;
}
