#ifndef ARBOLSINTACTICO_H_INCLUDED
#define ARBOLSINTACTICO_H_INCLUDED

#include "Lexico.h"
#include "Arbol.h"

class ArbolSintacticoException : public exception {
    private:
        string msg;

    public:
        explicit ArbolSintacticoException(const char* message) : msg(message) { }

        explicit ArbolSintacticoException(const string& message) : msg(message) { }

        virtual ~ArbolSintacticoException() throw () { }

        virtual const char* what() const throw () {
            return msg.c_str();
            }
    };

class ArbolSintactico {
    private:
        Lexico lexicParser;
        ArbolS arbol;

    public:
        ArbolSintactico(){}

        void checkSymbol(string symbol) {
            if(lexicParser.getCurrentSymbol() == symbol) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                }
            else {
                //error
                throwException("Simbolo no aceptado en la gramatica.");
                }
            }

        void checkType(string type) {
            if(lexicParser.getCurrentType() == type) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                }
            else {
                string errorStr;//error
                errorStr = "Tipo no aceptado en la gramatica.\n";
                errorStr += "Tipo esperado: " + type;
                errorStr += ". Tipo obtenido: " + lexicParser.getCurrentType();
                throwException(errorStr);
                }
            }

        void S() {
            if(lexicParser.getCurrentType() == IDENTIFICADOR) {
                pID q = new Identificador(lexicParser.getCurrentSymbol());
                lexicParser.clean();
                lexicParser.getNextSymbol();
                checkType(ASIGNACION);
                pExpresion r = ExpA();
                checkType(PUNTO_Y_COMA);

                pSentencia dummy = new Asignacion(q, r);
                arbol.insertaNodo(dummy);
                S();
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_IF) {
                lexicParser.clean();
                lexicParser.getNextSymbol();

                pSentencia dummy = IF();
                arbol.insertaNodo(dummy);
                S();
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_WHILE) {
                lexicParser.clean();
                lexicParser.getNextSymbol();

                pSentencia dummy = WHILE();
                arbol.insertaNodo(dummy);
                S();
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_PRINT) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                pExpresion a = PRINT();
                checkType(PUNTO_Y_COMA);

                pSentencia dummy = new Print(a);
                arbol.insertaNodo(dummy);
                S();
            }
        }

        void SGen(pSentencia &a) {
                if(lexicParser.getCurrentType() == IDENTIFICADOR) {
                        pID q = new Identificador(lexicParser.getCurrentSymbol());
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        checkType(ASIGNACION);
                        pExpresion r = ExpA();
                        checkType(PUNTO_Y_COMA);

                        a = new Asignacion(q, r);
                        SGen(a->siguiente);
                }
                else if(lexicParser.getCurrentType() == SENTENCIA_IF) {
                        lexicParser.clean();
                        lexicParser.getNextSymbol();

                        a = IF();
                        SGen(a->siguiente);
                }
                else if(lexicParser.getCurrentType() == SENTENCIA_WHILE) {
                        lexicParser.clean();
                        lexicParser.getNextSymbol();

                        a = WHILE();
                        SGen(a->siguiente);
                }
                else if(lexicParser.getCurrentType() == SENTENCIA_PRINT) {
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        pExpresion b = PRINT();
                        checkType(PUNTO_Y_COMA);

                        a = new Print(b);
                        SGen(a->siguiente);
                }
        }

        pSentencia A() {
            if(lexicParser.getCurrentType() == IDENTIFICADOR) {
                pID q = new Identificador(lexicParser.getCurrentSymbol());
                lexicParser.clean();
                lexicParser.getNextSymbol();
                checkType(ASIGNACION);
                pExpresion r = ExpA();
                checkType(PUNTO_Y_COMA);

                pSentencia dummy = new Asignacion(q, r);
                return dummy;
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_IF) {
                lexicParser.clean();
                lexicParser.getNextSymbol();

                pSentencia dummy = IF();
                return dummy;
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_WHILE) {
                lexicParser.clean();
                lexicParser.getNextSymbol();

                pSentencia dummy = WHILE();
                return dummy;
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_PRINT) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                pExpresion a = PRINT();
                checkType(PUNTO_Y_COMA);

                pSentencia dummy = new Print(a);
                return dummy;
            }
            else
            {
                    pSentencia dummy = NULL;
                    return dummy;
            }
        }

        pExpresion SignedExp()
        {
                pExpresion b;
                if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR)
                {
                        if(lexicParser.getCurrentType()==DIGITO)
                                b = new Entero(lexicParser.getCurrentSymbol());
                        else if(lexicParser.getCurrentType()==REAL)
                                b = new Real(lexicParser.getCurrentSymbol());
                        else
                                b = new Identificador(lexicParser.getCurrentSymbol());
                        lexicParser.clean();
                        lexicParser.getNextSymbol();

                        if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_ALTA_PRIORIDAD)
                        {
                                string k = lexicParser.getCurrentSymbol();
                                lexicParser.clean();
                                lexicParser.getNextSymbol();
                                b = Alta(b,k);
                        }

                        return b;
                }
                else if(lexicParser.getCurrentType() == ABRE_PARENTESIS)
                {
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        b = ExpA();
                        checkType(CIERRA_PARENTESIS);
                        if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_ALTA_PRIORIDAD)
                        {
                                string k = lexicParser.getCurrentSymbol();
                                lexicParser.clean();
                                lexicParser.getNextSymbol();
                                b = Alta(b,k);
                        }
                        return b;
                }
                else
                {                                                                        //Aqui ni deberia entrar
                        b = new Expresion();
                        return b;
                }
        }

        pExpresion ExpA()
        {
                pExpresion a;
                string s;
                if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR ||
                   lexicParser.getCurrentType() == ABRE_PARENTESIS || lexicParser.getCurrentType() == OPERADOR_ARITMETICO_BAJA_PRIORIDAD) {

                        if(lexicParser.getCurrentType()==DIGITO)
                        {
                                a = new Entero(lexicParser.getCurrentSymbol());
                                lexicParser.clean();
                                lexicParser.getNextSymbol();
                        }
                        else if(lexicParser.getCurrentType()==REAL)
                        {
                                a = new Real(lexicParser.getCurrentSymbol());
                                lexicParser.clean();
                                lexicParser.getNextSymbol();
                        }
                        else if(lexicParser.getCurrentType() == IDENTIFICADOR)
                        {
                                a = new Identificador(lexicParser.getCurrentSymbol());
                                lexicParser.clean();
                                lexicParser.getNextSymbol();
                        }
                        else if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_BAJA_PRIORIDAD)
                        {
                                string k = lexicParser.getCurrentSymbol();
                                lexicParser.clean();
                                lexicParser.getNextSymbol();
                                pExpresion z = SignedExp();
                                a = new Signo(z,k);
                        }
                        else
                        {//ABRE_PARENTESIS
                                checkType(ABRE_PARENTESIS);
                                a = ExpA();
                                checkType(CIERRA_PARENTESIS);
                        }


                        while(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_ALTA_PRIORIDAD || lexicParser.getCurrentType() == OPERADOR_ARITMETICO_BAJA_PRIORIDAD)
                        {
                                if(lexicParser.getCurrentType()==OPERADOR_ARITMETICO_BAJA_PRIORIDAD)
                                {
                                        s = lexicParser.getCurrentSymbol();
                                        lexicParser.clean();
                                        lexicParser.getNextSymbol();
                                        a = Baja(a,s);
                                }
                                else if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_ALTA_PRIORIDAD)
                                {
                                        s = lexicParser.getCurrentSymbol();
                                        lexicParser.clean();
                                        lexicParser.getNextSymbol();
                                        a = Alta(a,s);
                                }
                        }

                        return a;

                }
                else if(lexicParser.getCurrentType()==CADENA)
                {
                        pExpresion c = new Cadena(lexicParser.getCurrentSymbol());
                        checkType(CADENA);
                        return c;
                }
                else
                {                                                                        //Aqui entra si hay error
                        pExpresion n = new Expresion();
                        return n;
                }
        }

        pExpresion Baja(pExpresion a, string s)
        {
                pExpresion b;
                if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR)
                {
                        if(lexicParser.getCurrentType()==DIGITO)
                                b = new Entero(lexicParser.getCurrentSymbol());
                        else if(lexicParser.getCurrentType()==REAL)
                                b = new Real(lexicParser.getCurrentSymbol());
                        else
                                b = new Identificador(lexicParser.getCurrentSymbol());
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_BAJA_PRIORIDAD ||
                           lexicParser.getCurrentType() == CIERRA_PARENTESIS || lexicParser.getCurrentType() == PUNTO_Y_COMA ||
                           lexicParser.getCurrentType() == OPERADOR_RELACIONAL || lexicParser.getCurrentType() == OPERADOR_LOGICO)
                        {
                                a = new Suma(a,b,s);
                                return a;
                        }
                        else
                        { // debe ser de alta prioridad
                                string k = lexicParser.getCurrentSymbol();
                                lexicParser.clean();
                                lexicParser.getNextSymbol();
                                b = Alta(b,k);
                                a = new Suma(a,b,s);
                                return a;
                        }
                }
                else if(lexicParser.getCurrentType() == ABRE_PARENTESIS)
                {
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        b = ExpA();
                        checkType(CIERRA_PARENTESIS);
                        if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_ALTA_PRIORIDAD)
                        {
                                string k = lexicParser.getCurrentSymbol();
                                lexicParser.clean();
                                lexicParser.getNextSymbol();
                                b = Alta(b,k);
                        }
                        a = new Suma(a,b,s);
                        return a;
                }
                else if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_BAJA_PRIORIDAD)
                {
                        string k = lexicParser.getCurrentSymbol();
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        pExpresion z = SignedExp();
                        z = new Signo(z,k);
                        a = new Suma(a,z,s);
                        return a;
                }
                else
                {                                                                        //Aqui ni deberia entrar
                        pExpresion n = new Expresion();
                        a = new Suma(a,n,s);
                        return a;
                }

        }

        pExpresion Alta(pExpresion a, string s)
        {
                pExpresion b;
                if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR)
                {
                        if(lexicParser.getCurrentType()==DIGITO)
                                b = new Entero(lexicParser.getCurrentSymbol());
                        else if(lexicParser.getCurrentType()==REAL)
                                b = new Real(lexicParser.getCurrentSymbol());
                        else
                                b = new Identificador(lexicParser.getCurrentSymbol());
                        lexicParser.clean();
                        lexicParser.getNextSymbol();

                        a = new Multiplicacion(a,b,s);
                        return a;
                }
                else if(lexicParser.getCurrentType() == ABRE_PARENTESIS)
                {
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        cout << "Alta" << endl;
                        pExpresion z = ExpA();
                        checkType(CIERRA_PARENTESIS);
                        a = new Multiplicacion(a,z,s);
                        return a;
                }
                else if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_BAJA_PRIORIDAD)
                {
                        string k = lexicParser.getCurrentSymbol();
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        pExpresion z = SignedExp();
                        z = new Signo(z,k);
                        a = new Multiplicacion(a,z,s);
                        return a;
                }
                else
                {                                                                        //Aqui ni deberia entrar porque ya paso por el sintactico
                        pExpresion n = new Expresion();
                        a = new Multiplicacion(a,n,s);
                        return a;
                }
        }

        pSentencia opt() {
            if(lexicParser.getCurrentType() == SENTENCIA_ELSE) {
                pSentencia a = NULL;
                lexicParser.clean();
                lexicParser.getNextSymbol();
                if(lexicParser.getCurrentType() == ABRE_LLAVE) {
                    lexicParser.clean();
                    lexicParser.getNextSymbol();
                    SGen(a);
                    checkType(CIERRA_LLAVE);
                }
                else
                    a = A();
                return a;
            }
            else{
                pSentencia n = NULL;
                return n;
            }
        }

        pCondicion cond()
        {
                pExpresion a,b;
                string s;
                a = ExpA();
//                if(lexicParser.getCurrentType() == OPERADOR_RELACIONAL)
//                {
                        s = lexicParser.getCurrentSymbol();
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
//                 }
                b = ExpA();
                pCondicion c = new Relacional(a,b,s);
                return c;
        }

        pSentencia IF() {
                pCondicion a;
                pSentencia b = NULL, c;
                checkType(ABRE_PARENTESIS);
                a = cond();
                checkType(CIERRA_PARENTESIS);
                if(lexicParser.getCurrentType() == ABRE_LLAVE) {
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        SGen(b);
                        checkType(CIERRA_LLAVE);
                }
                else
                        b = A();
                c = opt();
                pSentencia s = new SI(a,b,c);
                return s;
        }

        pSentencia WHILE() {
                pCondicion a;
                pSentencia b = NULL;
                checkType(ABRE_PARENTESIS);
                a = cond();
                checkType(CIERRA_PARENTESIS);
                if(lexicParser.getCurrentType() == ABRE_LLAVE) {
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        SGen(b);
                        checkType(CIERRA_LLAVE);
                }
                else
                        b = A();
                pSentencia s = new Mientras(a,b);
                return s;
        }

        pExpresion PRINT() {
            if(lexicParser.getCurrentType() == ABRE_PARENTESIS) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                pExpresion a = ExpA();
                checkType(CIERRA_PARENTESIS);
                return a;
            }
            else
            {
                pExpresion n = new Expresion();
                return n;
            }
        }

        /* ============================================================== */

        void throwException (const string& messageToThrow) {
            throw ArbolSintacticoException(messageToThrow);
            }

        /* ============================================================== */

        void analyze() {
            try
            {
                lexicParser.setOutput(false);
                lexicParser.getNextSymbol();
                cout << "Creando Arbol Sintactico" << endl << endl;
                S();
                if(lexicParser.getCurrentSymbol() != "$")
                {
                        string errorMsg;
                        errorMsg = "Termino mal arbol sintactico: ";
                        throwException(errorMsg);
                }


                cout << "Creando Arbol Semantico" << endl << endl;
                try
                {
                        arbol.analyze();
                }
                catch(ArbolSemanticoException e)
                {
                        throwException(e.what());
                }

            }
            catch(LexicoException e)
            {
                string errorMsg;
                errorMsg = "Error lexico en el arbol sintactico: ";
                errorMsg += e.what();
                throwException(errorMsg);
            }
//            arbol.imprime();
//            cout << "XML generado" << endl << endl;
        }

        /* ============================================================== */
    };

#endif // ARBOLSINTACTICO_H_INCLUDED

