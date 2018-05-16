#ifndef LEXICO_H_INCLUDED
#define LEXICO_H_INCLUDED

#include <fstream>

#define DIGITO "Digito"
#define IDENTIFICADOR "Id"
#define OPERADOR_ARITMETICO_ALTA_PRIORIDAD "OperadorAritmeticoAlta"
#define OPERADOR_ARITMETICO_BAJA_PRIORIDAD "OperadorAritmeticoBaja"
#define PUNTO_Y_COMA "PuntoYComa"
#define ASIGNACION "Asignacion"
#define REAL "Real"
#define OPERADOR_NEGACION "OperadorNegacion"
#define OPERADOR_LOGICO "OperadorLogico"
#define CADENA "Cadena"
#define OPERADOR_RELACIONAL "OperadorRelacional"
#define ABRE_PARENTESIS "AbreParentesis"
#define CIERRA_PARENTESIS "CierraParentesis"
#define ABRE_LLAVE "AbreLlave"
#define CIERRA_LLAVE "CierraLlave"
#define SENTENCIA_IF "SentenciaIf"
#define SENTENCIA_ELSE "SentenciaElse"
#define SENTENCIA_WHILE "SentenciaWhile"
#define SENTENCIA_PRINT "SentenciaPrint"

class LexicoException : public exception {
    private:
        string msg;

    public:
        explicit LexicoException(const char* message) : msg(message) { }

        explicit LexicoException(const string& message) : msg(message) { }

        virtual ~LexicoException() throw () { }

        virtual const char* what() const throw () {
            return msg.c_str();
            }
    };

class Lexico {
    private:
        bool endRead, endOfFileReached, output;
        int currentState; /// Estado actual
        string token; /// Simbolo actual (cadena)
        char currentChar; /// Caracter actual leido
        char separator = '\\';

        long currentPointerPos; /// Puntero del archivo

        /* ============================================================== */

        string divideString(const int& index) {
            int strLength, i;
            string newStr = "";

            strLength = token.length();

            if(index != 1 && index != 2) {
                return "ERROR; INDICE INCORRECTO (SOLO 1 o 2).";
                }

            for(i = 0; i < strLength; i++) {
                if(token[i] != separator) {
                    newStr += token[i];
                    }
                else {
                    break;
                    }
                }

            if(index == 2) {
                newStr = "";
                i++;
                while(i < strLength) {
                    newStr += token[i];
                    i++;
                    }
                }

            return newStr;
            }

        /* ============================================================== */

        void nextState(const int& newState) {
            token += currentChar;
            currentState = newState;
            }

        /* ============================================================== */

        void acceptance(const int& actualState) {
            endRead = true;

            currentPointerPos--;

            if(actualState == 1) {
                token += separator;
                token += DIGITO;
                }
            else if(actualState == 2) {
                token += separator;
                token += IDENTIFICADOR;
                }
            else if(actualState == 3) {
                token += separator;
                token += OPERADOR_ARITMETICO_ALTA_PRIORIDAD;
                }
            else if(actualState == 32) {
                token += separator;
                token += OPERADOR_ARITMETICO_BAJA_PRIORIDAD;
                }
            else if(actualState == 4) {
                token += separator;
                token += PUNTO_Y_COMA;
                }
            else if(actualState == 5) {
                token += separator;
                token += ASIGNACION;
                }
            else if(actualState == 8) {
                token += separator;
                token += REAL;
                }
            else if(actualState == 11) {
                token += separator;
                token += OPERADOR_NEGACION;
                }
            else if(actualState == 31) {
                token += separator;
                token += OPERADOR_LOGICO;
                }
            else if(actualState == 12) {
                token += separator;
                token += CADENA;
                }
            else if(actualState == 13 || actualState == 14) {
                token += separator;
                token += OPERADOR_RELACIONAL;
                }
            else if(actualState == 15) {
                token += separator;
                token += ABRE_PARENTESIS;
                }
            else if(actualState == 16) {
                token += separator;
                token += CIERRA_PARENTESIS;
                }
            else if(actualState == 17) {
                token += separator;
                token += ABRE_LLAVE;
                }
            else if(actualState == 18) {
                token += separator;
                token += CIERRA_LLAVE;
                }
            else if(actualState == 20) {
                token += separator;
                token += SENTENCIA_IF;
                }
            else if(actualState == 25) {
                token += separator;
                token += SENTENCIA_WHILE;
                }
            else if(actualState == 30) {
                token += separator;
                token += SENTENCIA_PRINT;
                }
            else if(actualState == 36){
                token += separator;
                token += SENTENCIA_ELSE;
                }
            }

        /* ================== ============================================ */

        string nextSymbol() {
            while(!endRead) {
                currentChar = readNextChar();
                //std::cout << "letra actual: " << currentChar << std::endl;

                if(currentChar == '$') {
                    if(currentState == 0) {
                        token = currentChar;
                        token += "\\FINAL";
                        break;
                    }
                    else {
                        error();
                    }

                    }

                switch (currentState) {
                    /* ============================================ */
                    case 0:
                        if(currentChar == ' ' || (currentChar >= 0 && currentChar <= 31)) { //Ignoro espacios
                            //nextState(0);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo numeros
                            nextState(1);
                            }
                        else if(currentChar == 'i') {
                            nextState(19);//Checa si es if
                            }
                        else if(currentChar == 'e') {
                            nextState(33);//Checa si es else
                            }
                        else if(currentChar == 'w') {
                            nextState(21);//Checa si es while
                            }
                        else if(currentChar == 'p') {
                            nextState(26);//Checa si es print
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122) || currentChar == '_') {
                            nextState(2);//Si es una letra alfabetica
                            }
                        else if(currentChar == '*' || currentChar == '/' || currentChar == '%') {
                            nextState(3);//Aritmetico Alta Prioridad
                            }
                        else if(currentChar == '+' || currentChar == '-' ) {
                            nextState(32);//Aritmetico Baja Prioridad
                            }
                        else if(currentChar == ';') {
                            nextState(4);//Delimitador
                            }
                        else if(currentChar == '=') {
                            nextState(5);//asignacion o relacional de igualdad
                            }
                        else if(currentChar == '"') {
                            nextState(6);//Cadena de doble comilla
                            }
                        else if(currentChar == '\'') {
                            nextState(7);//Cadena de comilla simple
                            }
                        else if(currentChar == '&') {
                            nextState(9); //Logico && (Estado 8 es para numero real)
                            }
                        else if(currentChar == '|') {
                            nextState(10); //Logico ||
                            }
                        else if(currentChar == '!') {
                            nextState(11); //Logico ! o Relacional !=
                            }
                        else if(currentChar == '<' || currentChar == '>' ) {
                            nextState(13); //Relacionales
                            }
                        else if(currentChar == '(' ) {
                            nextState(15); //
                            }
                        else if(currentChar == ')' ) {
                            nextState(16); //
                            }
                        else if(currentChar == '{' ) {
                            nextState(17); //
                            }
                        else if(currentChar == '}' ) {
                            nextState(18); //
                            }
                        else {
                            error();
                            }

                        break;
                    /* ============================================ */
                    case 1: //Cayó entero
                        if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(1);
                            }
                        else if(currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/' || currentChar == '%' || currentChar == '=' || currentChar == ';' || currentChar == '<' || currentChar == '>' || currentChar == ')' || currentChar == ' ') {
                            acceptance(currentState);
                            }
                        else if(currentChar == '.') {
                            nextState(8); //Numero Real
                            }
                        else {
                            error();
                            }

                        break;
                    /* ============================================ */
                    case 2: //Cayo letra
                        if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(currentState);
                            }

                        break;
                    /* ============================================ */
                    case 3: //Operador Aritmetico Alta Prioridad
                        acceptance(currentState);
                        break;
                    /* ============================================ */
                    case 4: //Delimitador
                        acceptance(currentState);
                        break;
                    /* ============================================ */
                    case 5: //Asignacion
                        if(currentChar == '=') {
                            nextState(14);
                            }
                        else {
                            acceptance(currentState);
                            }

                        break;
                    /* ============================================ */
                    case 6: //Cadena de comilla doble
                        if(currentChar == '"') {
                            nextState(12); //Aceptacion de cadena
                            }
                        else {
                            nextState(6); //Consume todo tipo de caracter
                            }
                        break;
                    /* ============================================ */
                    case 7: //Cadena de comilla simple
                        if(currentChar == '\'') {
                            nextState(12); //Aceptacion de cadena
                            }
                        else {
                            nextState(7); //Consume todo tipo de caracter
                            }
                        break;
                    /* ============================================ */
                    case 8: //Numero real
                        if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(8);
                            }
                        else if(currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/' || currentChar == '%' || currentChar == '=' || currentChar == ';' || currentChar == '<' || currentChar == '>' || currentChar == ')' || currentChar == ' ') {
                            acceptance(currentState);
                            }
                        else {
                            error();
                            }
                        break;
                    /* ============================================ */
                    case 9: //Logico &&
                        if(currentChar == '&') {
                            nextState(31);
                            }
                        else {
                            error();
                            }
                        break;
                    /* ============================================ */
                    case 10: //Logico ||
                        if(currentChar == '|') {
                            nextState(31);
                            }
                        else {
                            error();
                            }
                        break;
                    /* ============================================ */
                    case 11: //Logico !
                        if(currentChar == '=') {
                            nextState(14);
                            }
                        else {
                            acceptance(currentState);
                            }
                        break;
                    /* ============================================ */
                    case 12: //Cadena
                        acceptance(currentState);
                        break;
                    /* ============================================ */
                    case 13: //Relacional de un caracter
                        if(currentChar == '=') {
                            nextState(14);
                            }
                        else {
                            acceptance(currentState);
                            }
                        break;
                    /* ============================================ */
                    case 14: //Relacional de dos caracteres
                        acceptance(currentState);
                        break;
                    /* ============================================ */

                    case 15: //Simbolo (
                        acceptance(currentState);
                        break;
                    /* ============================================ */
                    case 16: //Simbolo )
                        acceptance(currentState);
                        break;
                    /* ============================================ */
                    case 17: //Simbolo {
                        acceptance(currentState);
                        break;
                    /* ============================================ */
                    case 18: //Simbolo }
                        acceptance(currentState);
                        break;
                    /* ============================================ */
                    /* ============================================ */
                    case 19: //i de sentencia if
                        if(currentChar == 'f') {
                            nextState(20);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 20: //if de sentencia if
                        if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(currentState); //if aceptado
                            }
                        break;
                    /* ============================================ */
                    /* ============================================ */
                    case 21: //w de sentencia while
                        if(currentChar == 'h') {
                            nextState(22);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 22: //wh de sentencia while
                        if(currentChar == 'i') {
                            nextState(23);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 23: //whi de sentencia while
                        if(currentChar == 'l') {
                            nextState(24);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 24: //whil de sentencia while
                        if(currentChar == 'e') {
                            nextState(25);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 25: //while de sentencia while
                        if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(currentState);
                            }
                        break;
                    /* ============================================ */
                    /* ============================================ */
                    case 26: //p de sentencia print
                        if(currentChar == 'r') {
                            nextState(27);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 27: //pr de sentencia print
                        if(currentChar == 'i') {
                            nextState(28);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 28: //pri de sentencia print
                        if(currentChar == 'n') {
                            nextState(29);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 29: //prin de sentencia print
                        if(currentChar == 't') {
                            nextState(30);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 30: //print de sentencia print
                        if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(currentState);
                            }
                        break;
                    /* ============================================ */
                    /* ============================================ */
                    case 31: //Logico de dos caracteres
                        acceptance(currentState);
                        break;
                    /* ============================================ */
                    /* ============================================ */
                    case 32: //Aritmetico Baja Prioridad
                        acceptance(currentState);
                        break;
                    /* ============================================ */
                    case 33:
                        if(currentChar == 'l') {
                            nextState(34);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 34:
                        if(currentChar == 's') {
                            nextState(35);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 35:
                        if(currentChar == 'e') {
                            nextState(36);
                            }
                        else if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(2);
                            }
                        break;
                    /* ============================================ */
                    case 36:
                        if(isdigit(currentChar) != 0) { //Consumo enteros
                            nextState(2);
                            }
                        else if((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) { //Si es una letra alfabetica
                            nextState(2);
                            }
                        else if(currentChar == '_') {
                            nextState(2);
                            }
                        else {
                            acceptance(currentState);
                            }
                        break;
                    /* ============================================ */

                    default:
                        break;
                    }
                }

            return token;
            }

        /* ============================================================== */

        char readNextChar () {
            ifstream fileToRead("entrada.txt", ios::in);
            char auxChar = ' ';

            //fileToRead.open("entrada.txt");

            if(endOfFileReached) {
                fileToRead.clear();
                fileToRead.close();
                return '$';

                }

            if(fileToRead.is_open()) {
                fileToRead.seekg(currentPointerPos++, std::ios::cur);
                fileToRead.get(auxChar);

                if(fileToRead.eof()) {
                    fileToRead.clear();
                    endOfFileReached = true;
                    }
                }
            else {
                if(fileToRead.eof()) {
                        fileToRead.clear();
                    fileToRead.close();
                    throwException("ERROR: No se pudo abrir el archivo a leer.");
                }
            }

            fileToRead.close();

            return auxChar;
        }

        void error() {
            string errorMessage = "ERROR LEXICO: Gramatica no reconocida: ";
            errorMessage += currentChar;
            throwException(errorMessage);
            }

        void throwException (const string& messageToThrow) {
            throw LexicoException(messageToThrow);
            }

        /* ============================================================== */

    public:
        Lexico() {
            endOfFileReached = false;
            endRead = false;
            currentState = 0;
            token = "";
            currentPointerPos = 0;
            output = true;
        }

        void init()
        {
            endOfFileReached = false;
            endRead = false;
            currentState = 0;
            token = "";
            currentPointerPos = 0;
            output = true;
        }

        /* ============================================================== */

        void clean()
        {
            token = "";
            endRead = false;
            currentState = 0;
        }

        long getPOS()
        {
                return currentPointerPos;
        }

        string getCurrentSymbol()
        {
            return divideString(1);
        }

        string getCurrentType()
        {
            return divideString(2);
        }

        bool isEndOfFileReached()
        {
            return endOfFileReached;
        }

        void getNextSymbol()
        {
            nextSymbol();
            if(output)
                cout << "Simbolo Actual: " << getCurrentSymbol() << "\t" << getCurrentType() << endl;
        }

        void setOutput(bool a)
        {
            output = a;
        }
    };

#endif // LEXICO_H_INCLUDED
