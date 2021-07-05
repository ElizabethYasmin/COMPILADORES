#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
#include "estado.h"
#include "produccion.h"
#include "funcion.h"
#include "grammar.h"
#include "TAS.h"
#include "LL1.h"
using namespace std;
int main()
{
    LL1 ll1;
    ll1.setGrammar("sample.txt");
    ll1.tas.readFromText("TAS.txt", 6, 5); // Archivo TAS
    ll1.grammar.print("sample.txt");
    ll1.tas.print();
    string tempText ;
    cout<< "ANALIZAR->";
    cin.ignore();
    getline(cin, tempText);
    if (ll1.analyze(tempText))
        cout<< "ACCEPTE";
    else
        cout<< "NO ACEPTADO";

    return 0;
}
