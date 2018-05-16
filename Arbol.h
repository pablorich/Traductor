#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include "Nodos.h"

class ArbolSemanticoException : public exception {
    private:
        string msg;

    public:
        explicit ArbolSemanticoException(const char* message) : msg(message) { }

        explicit ArbolSemanticoException(const string& message) : msg(message) { }

        virtual ~ArbolSemanticoException() throw () { }

        virtual const char* what() const throw () {
            return msg.c_str();
            }
    };

class ArbolS
{
private:
    pSentencia root;
    pSentencia fin;

public:

    ArbolS()
    {
        //root->siguiente = NULL;
        root = NULL;
        //fin->siguiente = NULL;
        fin = NULL;
    }

    bool vacio()
    {
        return (root == NULL);
    }

    void insertaNodo(pSentencia nuevo)
    {
        nuevo->siguiente = NULL;
        if(vacio())
        {
            root = nuevo;
            fin = nuevo;
        }
        else
        {
            fin->siguiente = nuevo;
            fin = nuevo;
        }
    }

    void imprime()
    {
        //limpiarPantalla();
        ofstream writeFile;

        remove("salida.txt");
        writeFile.open("salida.txt");

        if(writeFile.is_open()) {
            writeFile << "<PROGRAMA>" << endl;
            pSentencia temp = root;
            while(temp != NULL)
            {
                writeFile << temp->imprime();
                temp = temp->siguiente;
            }
            writeFile<< "</PROGRAMA>" << endl;
        }

        writeFile.close();

    }

    void throwException (const string& messageToThrow) {
            throw ArbolSemanticoException(messageToThrow);
            }

    void analyze()
    {
        try
        {
            cout << "Esto es un analisis" << endl << endl;
            pSentencia temp = root;

            recorre(temp);

            //lista.imprime();

            generaCodigo();
        }
        catch(NodeException e)
        {
            throwException(e.what());
        }

    }

    void recorre(pSentencia temp)
    {
        char t;
        while(temp != NULL)
        {
            t = temp->checkType();
            if(t == 'e')
            {
                string errorMessage = "Tipo de Sentencia invalida: \n";
                errorMessage += temp->imprime();
                throw NodeException(errorMessage);
            }
            else if(t == 'a')
            {
                //Obtiene el tipo de dato de la expresion
                temp->semantico();
                if(temp->getTipo() == 'e')
                {
                    string errorMessage = "Asignacion Invalida (Semanticamente): \n";
                    errorMessage += temp->imprime();
                    throw NodeException(errorMessage);
                }
                //inserta ID de la asignacion a la lista de ID's
                lista.insertaNodo(temp->getID(), temp->getTipo());
            }
            else if(t == 'p')
            {
                //Obtiene tipo de dato de la expresion
                temp->semantico();
                if(temp->getTipo() == 'e')
                {
                    string errorMessage = "Print Invalido (Semanticamente): \n";
                    errorMessage += temp->imprime();
                    throw NodeException(errorMessage);
                }
            }
            else if(t == 'i')
            {
                //Obtiene el tipo de dato de la condicion
                temp->semantico();
                if(temp->getTipo() == 'e')
                {
                    string errorMessage = "Condicion Invalida en IF (Semanticamente): \n";
                    errorMessage += temp->imprime();
                    throw NodeException(errorMessage);
                }
                //recorre las sentencias dentro del bloque
                recorre(temp->getBloque());
                recorre(temp->getOtro());
            }
            else if(t == 'w')
            {
                //Obtiene el tipo de dato de la condicion
                temp->semantico();
                if(temp->getTipo() == 'e')
                {
                    string errorMessage = "Condicion Invalida en WHILE (Semanticamente): \n";
                    errorMessage += temp->imprime();
                    throw NodeException(errorMessage);
                }
                //recorre las sentencias dentro del bloque
                recorre(temp->getBloque());
            }

            temp = temp->siguiente;
        }
    }

    void generaCodigo()
    {
        ofstream writeFile;

        remove("salida.txt");
        writeFile.open("salida.txt");

        if(writeFile.is_open()) {
            writeFile << ".386" << endl ;
            writeFile << ".model flat, stdcall" << endl;
            writeFile << "option casemap:none" << endl;
            writeFile << "\tinclude \\masm32\\macros\\macros.asm" << endl;
            writeFile << "\tinclude \\masm32\\include\\masm32.inc" << endl;
            writeFile << "\tinclude \\masm32\\include\\kernel32.inc" << endl;
            writeFile << "\tincludelib \\masm32\\lib\\masm32.lib" << endl;
            writeFile << "\tincludelib \\masm32\\lib\\kernel32.lib" << endl << endl;


            writeFile << ".data" << endl;
            //
            writeFile << ".data?" << endl;

            writeFile << ".code" << endl;
            writeFile << "inicio:" << endl << endl;

            pSentencia temp = root;
            while(temp != NULL)
            {
                writeFile << temp->generaCodigo();
                temp = temp->siguiente;
            }

            writeFile << "exit" << endl << endl;
            writeFile << "end inicio" << endl;
        }

        writeFile.close();
    }

};

#endif // ARBOL_H_INCLUDED
