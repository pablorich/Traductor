#ifndef SINTACTICO_H_INCLUDED
#define SINTACTICO_H_INCLUDED

#include "Lexico.h"

class SintacticoException : public exception {
    private:
        string msg;

    public:
        explicit SintacticoException(const char* message) : msg(message) { }

        explicit SintacticoException(const string& message) : msg(message) { }

        virtual ~SintacticoException() throw () { }

        virtual const char* what() const throw () {
            return msg.c_str();
            }
    };

class Sintactico {
    private:
        Lexico lexicParser;

    public:
        Sintactico() {}

        void checkSymbol(string symbol) {
            if(lexicParser.getCurrentSymbol() == symbol) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                }
            else {
                //error
                throwException("Simbolo no aceptado.");
                }
            }

        void checkType(string type) {
            if(lexicParser.getCurrentType() == type) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                }
            else {
                string errorStr;//error
                errorStr = "No aceptado sintacticamente. (checkType())\n";
                errorStr += "Tipo esperado: " + type;
                errorStr += ". Tipo obtenido: " + lexicParser.getCurrentType();
                throwException(errorStr);
                }
            }

        void S() {
            if(lexicParser.getCurrentType() == IDENTIFICADOR) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                checkType(ASIGNACION);
                ExpA();
                checkType(PUNTO_Y_COMA);
                S();
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_IF) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                IF();
                S();
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_WHILE) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                WHILE();
                S();
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_PRINT) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                PRINT();
                checkType(PUNTO_Y_COMA);
                S();
            }
        }

        void A() {
            if(lexicParser.getCurrentType() == IDENTIFICADOR) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                checkType(ASIGNACION);
                ExpA();
                checkType(PUNTO_Y_COMA);
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_IF) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                IF();
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_WHILE) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                WHILE();
            }
            else if(lexicParser.getCurrentType() == SENTENCIA_PRINT) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                PRINT();
                checkType(PUNTO_Y_COMA);
            }
        }

        void ExpA() {
            cout << "Expresion" << endl <<endl;
            if(lexicParser.getCurrentType() == IDENTIFICADOR || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == DIGITO) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                B();
            }
            else if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_BAJA_PRIORIDAD) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                ExpB();
            }
            else if(lexicParser.getCurrentType() == ABRE_PARENTESIS) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                ExpA();
                checkType(CIERRA_PARENTESIS);
                B();
            }
            else {
                checkType(CADENA);
            }
        }

        void ExpB() {
            if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                B();
            }
            else if(lexicParser.getCurrentType() == ABRE_PARENTESIS) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                ExpA();
                checkType(CIERRA_PARENTESIS);
                B();
            }
            else {
                string errorStr;//error
                errorStr = "Tipo no aceptado en la gramatica.\n";
                errorStr += ". Tipo obtenido: " + lexicParser.getCurrentType();
                throwException(errorStr);
            }
        }

        void BFollow() {
            if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                B();
            }
            else if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_BAJA_PRIORIDAD) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                BFollowB();
            }
            else if(lexicParser.getCurrentType() == ABRE_PARENTESIS) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                BFollow();
                checkType(CIERRA_PARENTESIS);
                B();
            }
            else {
                string errorStr;//error
                errorStr = "Tipo no aceptado en la gramatica.\n";
                errorStr += ". Tipo obtenido: " + lexicParser.getCurrentType();
                throwException(errorStr);
            }
        }

        void BFollowB() {
            if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                B();
            }
            else if(lexicParser.getCurrentType() == ABRE_PARENTESIS) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                BFollow();
                checkType(CIERRA_PARENTESIS);
                B();
            }
            else {
                string errorStr;//error
                errorStr = "Tipo no aceptado en la gramatica.\n";
                errorStr += ". Tipo obtenido: " + lexicParser.getCurrentType();
                throwException(errorStr);
            }
        }

        void B() {
            if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_BAJA_PRIORIDAD) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                BFollow();
            }
            else{
                C();
            }
        }

        void C() {
            if(lexicParser.getCurrentType() == OPERADOR_ARITMETICO_ALTA_PRIORIDAD) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                BFollow();
            }
        }

        void IF() {
            cout << "Sentencia IF" << endl <<endl;
            checkType(ABRE_PARENTESIS);
            cond();
            checkType(CIERRA_PARENTESIS);
            if(lexicParser.getCurrentType() == ABRE_LLAVE) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                S();
                checkType(CIERRA_LLAVE);
            }
            else
                A();
            opt();
        }

        void opt() {
            if(lexicParser.getCurrentType() == SENTENCIA_ELSE) {
                cout << "Bloque ELSE" << endl << endl;
                lexicParser.clean();
                lexicParser.getNextSymbol();
                if(lexicParser.getCurrentType() == ABRE_LLAVE) {
                    lexicParser.clean();
                    lexicParser.getNextSymbol();
                    S();
                    checkType(CIERRA_LLAVE);
                }
                else
                    A();
            }
        }

        void cond() {
            cout << "Condicion" << endl << endl;
            if(lexicParser.getCurrentType() == OPERADOR_NEGACION) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                condF();
            }
            else if(lexicParser.getCurrentType() == ABRE_PARENTESIS) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                cond();
                checkType(CIERRA_PARENTESIS);
                cond2();
            }
            else if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                B();
                cond2();
            }
//            else { // BORRAR ESTE ELSE SI LA GRMATICA PERMITE CONDICION VACIA
//                std::string errorStr;//error
//                errorStr = "Tipo no aceptado en la gramatica.\n";
//                errorStr += ". Tipo obtenido: " + lexicParser.getCurrentType();
//                throwException(errorStr);
//            }
        }

        void condF() {
            if(lexicParser.getCurrentType() == OPERADOR_NEGACION) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                condF();
            }
            else if(lexicParser.getCurrentType() == ABRE_PARENTESIS) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                condF();
                checkType(CIERRA_PARENTESIS);
                cond2();
            }
            else if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                B();
                cond2();
            }
            else {
                string errorStr;//error
                errorStr = "Tipo no aceptado en la gramatica.\n";
                errorStr += ". Tipo obtenido: " + lexicParser.getCurrentType();
                throwException(errorStr);
            }
        }

        void condFollow() {
            if(lexicParser.getCurrentType() == ABRE_PARENTESIS) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                condFollow();
                checkType(CIERRA_PARENTESIS);
            }
            else if(lexicParser.getCurrentType() == DIGITO || lexicParser.getCurrentType() == REAL || lexicParser.getCurrentType() == IDENTIFICADOR) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                B();
            }
            else {
                string errorStr;//error
                errorStr = "Tipo no aceptado en la gramatica.\n";
                errorStr += ". Tipo obtenido: " + lexicParser.getCurrentType();
                throwException(errorStr);
            }
        }

        void cond2() {
            if(lexicParser.getCurrentType() == OPERADOR_RELACIONAL) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                condFollow();
                cond3();
            }
            else {
                cond3();
            }
        }

        void cond3() {
            if(lexicParser.getCurrentType() == OPERADOR_LOGICO) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                condF();
            }
        }

        void WHILE() {
                cout << "sentencia WHILE" << endl << endl;
                checkType(ABRE_PARENTESIS);
                cond();
                checkType(CIERRA_PARENTESIS);
                if(lexicParser.getCurrentType() == ABRE_LLAVE) {
                        lexicParser.clean();
                        lexicParser.getNextSymbol();
                        S();
                        checkType(CIERRA_LLAVE);
                }
                else
                        A();
        }

        void PRINT() {
            cout << "sentencia PRINT" << endl << endl;
            if(lexicParser.getCurrentType() == ABRE_PARENTESIS) {
                lexicParser.clean();
                lexicParser.getNextSymbol();
                ExpA();
                checkType(CIERRA_PARENTESIS);
            }
        }

        /* ============================================================== */

        void throwException (const string& messageToThrow) {
            throw SintacticoException(messageToThrow);
            }

        /* ============================================================== */

        void analyze() {
            try
            {
                lexicParser.getNextSymbol();
                S();
                if(lexicParser.getCurrentSymbol() == "$")
                {
                        cout << endl << "Analisis sintactico terminado" << endl << endl;
                        lexicParser.init();
                }
                else
                {
                        string errorMsg;
                        errorMsg = "Termino mal analisis sintactico: ";
                        throwException(errorMsg);
                }
            }
            catch(LexicoException e)
            {
                string errorMsg;
                errorMsg = "ERROR Lexico: ";
                errorMsg += e.what();
                throwException(errorMsg);
            }
        }

        /* ============================================================== */
    };

#endif // SINTACTICO_H_INCLUDED
