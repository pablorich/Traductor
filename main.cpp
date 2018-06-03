#include <string>
#include <iostream>
#include <cctype>
#include <exception>
#include <windows.h>
#include <sstream>

using namespace std;

#include "util.h"
#include "lista.h"
Lista lista;
int contadorIF = 0;
int contadorWHILE = 0;
int contadorDIV = 0;

#include "Sintactico.h"
#include "ArbolSintactico.h"

using namespace std;

void salida(bool);
void construyeArbol();

int main() {
    try
    {
        Sintactico sintacticAnalyzer;
        sintacticAnalyzer.analyze();
        construyeArbol();
    }
    catch(SintacticoException e)
    {
        cout << e.what();
        salida(false);
    }

    return 0;
}

void salida(bool V) {
        ofstream writeFile;

        remove("salida.txt");
        writeFile.open("salida.txt");

        if(writeFile.is_open()) {
                if(V)
                    writeFile << "1";
                else
                    writeFile << "0";
        }

        writeFile.close();
}

void construyeArbol()
{
    try
    {
        ArbolSintactico arbolSint;
        arbolSint.analyze();
        //salida(true);
    }
    catch(ArbolSintacticoException e)
    {
        throw SintacticoException(e.what());
    }

}
