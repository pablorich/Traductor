#ifndef NODOS_H_INCLUDED
#define NODOS_H_INCLUDED

class NodeException : public exception {
    private:
        string msg;

    public:
        explicit NodeException(const char* message) : msg(message) { }

        explicit NodeException(const string& message) : msg(message) { }

        virtual ~NodeException() throw () { }

        virtual const char* what() const throw () {
            return msg.c_str();
            }
};


class Sentencia
{
public:
    Sentencia *siguiente = NULL;
    char tipo = 'e';

    Sentencia(){}

    virtual string imprime()
    {
        stringstream s;
        return s.str();
    }

    virtual string generaCodigo()
    {
        stringstream s;
        return s.str();
    }

    virtual char checkType()
    {
        return 'e';
    }

    virtual void semantico()
    {

    }

    virtual char getTipo()
    {
        return tipo;
    }

    virtual string getID()
    {
        return "\\";
    }

    virtual Sentencia* getBloque()
    {
        return NULL;
    }

    virtual Sentencia* getOtro()
    {
        return NULL;
    }

};
typedef Sentencia *pSentencia;

class Expresion
{
public:
    string simbolo;
    char tipo = 'e';

    Expresion *izq = NULL, *der = NULL;

    Expresion(){}

    virtual string imprime()
    {
        stringstream s;
        s << "<EXPRESION>" << endl;
        s << "</EXPRESION>" << endl;
        return s.str();
    }

    virtual string generaCodigo()
    {
        stringstream s;
        return s.str();
    }

    virtual string getSimbolo()
    {
        return simbolo;
    }

    virtual void semantico()
    {

    }

    virtual char getTipo()
    {
        return tipo;
    }

    virtual char dataType()
    {
        return 'E';
    }

};
typedef Expresion *pExpresion;

class Condicion
{
public:
    char tipo = 'e';
    string simbolo;

    Condicion(){}

    virtual string imprime()
    {
        stringstream s;
        s << "<EXPRESION>" << endl;
        s << "</EXPRESION>" << endl;
        return s.str();
    }

    virtual string generaCodigo()
    {
        stringstream s;
        return s.str();
    }

    virtual void semantico()
    {

    }

    virtual char getTipo()
    {
        return tipo;
    }

};
typedef Condicion *pCondicion;

//TIPOS DE DATOS

class Identificador : public Expresion
{
public:
    Identificador(string sim)
    {
        simbolo=sim;
    }

    string imprime()
    {
        stringstream s;
        s << "<ID>" << simbolo << "</ID>" << endl;
        return s.str();
    }

    string generaCodigo()
    {
        stringstream s;
        s << "push _"<< simbolo << ";" << endl;
        return s.str();
    }

    void semantico()
    {
        if(lista.buscaID(simbolo))
        {
            tipo = lista.tipoID(simbolo);
        }
        else
        {
            tipo = 'e';
        }
    }

    char getTipo()
    {
        return tipo;
    }

    char dataType()
    {
        return 'o';
    }

};
typedef Identificador *pID;

class Entero : public Expresion
{
public:
    Entero(string sim)
    {
        simbolo=sim;
    }

    string imprime()
    {
        stringstream s;
        s << "<ENTERO>" << simbolo << "</ENTERO>" << endl;
        return s.str();
    }

    string generaCodigo()
    {
        stringstream s;
        s << "push "<< simbolo << ";" << endl;
        return s.str();
    }

    void semantico()
    {
        tipo = 'i';
    }

    char getTipo()
    {
        return tipo;
    }

    char dataType()
    {
        return 'i';
    }

};
typedef Entero *pEntero;

class Real : public Expresion
{
public:
    Real(string sim)
    {
        simbolo=sim;
    }

    string imprime()
    {
        stringstream s;
        s << "<REAL>" << simbolo << "</REAL>" << endl;
        return s.str();
    }

    string generaCodigo()
    {
        stringstream s;
        s << "push "<< simbolo << ";" << endl;
        return s.str();
    }

    void semantico()
    {
        tipo = 'r';
    }

    char getTipo()
    {
        return tipo;
    }

    char dataType()
    {
        return 'i';
    }

};
typedef Real *pReal;

class Cadena : public Expresion
{
public:
    Cadena(string sim)
    {
        simbolo=sim;
    }

    string imprime()
    {
        stringstream s;
        s << "<CADENA>" << simbolo << "</CADENA>" << endl;
        return s.str();
    }

    string generaCodigo()//Use getSimbolo() instead
    {
        stringstream s;
        //s << "push "<< simbolo << ";" << endl;
        return s.str();
    }

    void semantico()
    {
        tipo = 'c';
    }

    char getTipo()
    {
        return tipo;
    }

    char dataType()
    {
        return 'i';
    }

};
typedef Cadena *pCadena;

//FIN TIPOS DE DATOS
//TIPOS DE OPERACIONES ARITMETICAS

class Suma : public Expresion
{
public:
    Suma(Expresion *i, Expresion *d, string sim)
    {
        izq = i;
        der = d;
        simbolo = sim;
    }

    string imprime()
    {
        stringstream s;
        s << "<SUMA value=\""<< simbolo <<"\">" << endl;
        s << izq->imprime();
        s << der->imprime();
        s << "</SUMA>" << endl;
        return s.str();
    }

    string generaCodigo()
    {
        stringstream s;
        if(tipo == 'i')
        {
            if(simbolo == "+")
            {
                s << izq->generaCodigo();
                s << der->generaCodigo();
                s << "pop ebx;" << endl;
                s << "pop eax;" << endl;
                s << "add eax, ebx;" << endl;
                s << "push eax;" << endl;
            }
            else if(simbolo == "-")
            {
                s << izq->generaCodigo();
                s << der->generaCodigo();
                s << "pop ebx;" << endl;
                s << "pop eax;" << endl;
                s << "sub eax, ebx;" << endl;
                s << "push eax;" << endl;
            }
            else
                s << ";Operación desconocida" << endl;
        }
        else
            s << ";Solo acepta enteros" << endl;

        return s.str();
    }

    void semantico()
    {
        izq->semantico();
        der->semantico();

        if(izq->getTipo() == 'i' && der->getTipo() == 'i')
        {
            tipo = 'i';
        }
        else if(izq->getTipo() == 'r' && der->getTipo() == 'r')
        {
            tipo = 'r';
        }
        else
        {
            tipo = 'e';
        }
    }

    char getTipo()
    {
        return tipo;
    }

    char dataType()
    {
        return 'o';
    }

};
typedef Suma *pSuma;

class Signo : public Expresion
{
public:
    Signo(pExpresion d, string sim)
    {
        der = d;
        simbolo = sim;
    }

    string imprime()
    {
        stringstream s;
        s << "<SIGNO value=\""<< simbolo <<"\">" << endl;
        s << der->imprime();
        s << "</SIGNO>" << endl;
        return s.str();
    }

    string generaCodigo()
    {
        stringstream s;
        if(simbolo == "+")
        {
            s << der->generaCodigo();
        }
        else if(simbolo == "-")
        {
            if(der->dataType() == 'i')
                s << "push " << simbolo << der->getSimbolo() << ";" << endl;
            else if(der->dataType() == 'o')
            {
                s << der->generaCodigo();
                s << "pop eax;" << endl;
                s << "mov ebx, -1;" << endl;
                s << "imul ebx" << endl;
                s << "push eax;" << endl;
            }
        }
        else
            s << ";Operación desconocida" << endl;

        return s.str();
    }

    void semantico()
    {
        der->semantico();

        tipo = der->getTipo();
    }

    char getTipo()
    {
        return tipo;
    }

    char dataType()
    {
        return 'o';
    }

};
typedef Signo *pSigno;

class Multiplicacion : public Expresion
{
public:
    Multiplicacion(Expresion *i, Expresion *d, string sim)
    {
        izq = i;
        der = d;
        simbolo = sim;
    }

    string imprime()
    {
        stringstream s;
        s << "<MULT value=\""<< simbolo <<"\">" << endl;
        s << izq->imprime();
        s << der->imprime();
        s << "</MULT>" << endl;
        return s.str();
    }

    string generaCodigo()
    {
        stringstream s;
        if(tipo == 'i')
        {
            if(simbolo == "*")
            {
                s << izq->generaCodigo();
                s << der->generaCodigo();
                s << "pop ebx;" << endl;
                s << "pop eax;" << endl;
                s << "mov edx, 0;" << endl;
                s << "imul ebx;" << endl;
                s << "push eax;" << endl;
            }
            else if(simbolo == "/")
            {
                s << izq->generaCodigo();
                s << der->generaCodigo();
                s << "pop ebx;" << endl;
                s << "pop eax;" << endl;
                s << "mov edx, 0;" << endl;

                s << "cmp ebx,0;" << endl;
                s << "jge DIV" << contadorDIV << endl;
                s << "mov edx, -1;" << endl;                        //Si un operando es negativo se debe poner -1 aqui, si los 2 son positivos se pone 0
                s << "DIV" << contadorDIV << ": " << endl;
                contadorDIV++;

                s << "cmp eax,0;" << endl;
                s << "jge DIV" << contadorDIV << endl;
                s << "mov edx, -1;" << endl;                        //Si un operando es negativo se debe poner -1 aqui, si los 2 son positivos se pone 0
                s << "DIV" << contadorDIV << ": " << endl;
                contadorDIV++;

                s << "idiv ebx;" << endl;
                s << "push eax;" << endl;
            }
            else if(simbolo == "%")
            {
                s << izq->generaCodigo();
                s << der->generaCodigo();
                s << "pop ebx;" << endl;
                s << "pop eax;" << endl;
                s << "mov edx, 0;" << endl;

                s << "cmp ebx,0;" << endl;
                s << "jge DIV" << contadorDIV << endl;
                s << "mov edx, -1;" << endl;                        //Si un operando es negativo se debe poner -1 aqui, si los 2 son positivos se pone 0
                s << "DIV" << contadorDIV << ": " << endl;
                contadorDIV++;

                s << "cmp eax,0;" << endl;
                s << "jge DIV" << contadorDIV << endl;
                s << "mov edx, -1;" << endl;                        //Si un operando es negativo se debe poner -1 aqui, si los 2 son positivos se pone 0
                s << "DIV" << contadorDIV << ": " << endl;
                contadorDIV++;

                s << "idiv ebx;" << endl;

                s << "cmp edx,0;" << endl;
                s << "jge DIV" << contadorDIV << endl;
                s << "add edx, ebx;" << endl;                        //Si edx es negativo, sumele el modulo
                s << "DIV" << contadorDIV << ": " << endl;
                contadorDIV++;

                s << "push edx;" << endl;
            }
            else
                s << ";Operación desconocida" << endl;
        }
        else
            s << ";Solo acepta enteros" << endl;

        return s.str();
    }

    void semantico()
    {
        izq->semantico();
        der->semantico();

        if(izq->getTipo() == 'i' && der->getTipo() == 'i')
        {
            tipo = 'i';
        }
        else if(izq->getTipo() == 'r' && der->getTipo() == 'r')
        {
            tipo = 'r';
        }
        else
        {
            tipo = 'e';
        }
    }

    char getTipo()
    {
        return tipo;
    }

    char dataType()
    {
        return 'o';
    }

};
typedef Multiplicacion *pMultiplicacion;


//FIN OPERACIONES ARITMETICAS
//OPERACION RELACIONAL

class Relacional : public Condicion
{
    Expresion *izq = NULL, *der = NULL;
public:
    Relacional(Expresion *i, Expresion *d, string sim)
    {
        izq = i;
        der = d;
        simbolo = sim;
    }

    string getSimbolo()
    {
        if (simbolo == ">")
            return "&gt;";
        else if(simbolo == "<")
            return "&lt;";
        else if(simbolo == "==")
            return simbolo;
        else if(simbolo == "!=")
            return simbolo;
        else if(simbolo == "<=")
            return "&lt;=";
        else if(simbolo == ">=")
            return "&gt;=";
        else
            return "ERROR";
    }

    string getJumpCondition()
    {
        if (simbolo == ">")
            return "jng ";
        else if(simbolo == "<")
            return "jnl ";
        else if(simbolo == "==")
            return "jne ";
        else if(simbolo == "!=")
            return "je ";
        else if(simbolo == "<=")
            return "jnle ";
        else if(simbolo == ">=")
            return "jnge ";
        else
            return "jmp ";
    }

    string imprime()
    {
        stringstream s;
        s << "<EXPRESION value=\""<< getSimbolo() <<"\">" << endl;
        s << izq->imprime();
        s << der->imprime();
        s << "</EXPRESION>" << endl;
        return s.str();
    }

    string generaCodigo()
    {
        stringstream s;
        s << izq->generaCodigo();
        s << der->generaCodigo();
        s << "pop ebx;" << endl;
        s << "pop eax;" << endl;
        s << "cmp eax, ebx;" << endl;//Deja banderas de comparación
        s << getJumpCondition();
        return s.str();
    }


    void semantico()
    {
        izq->semantico();
        der->semantico();

        if(izq->getTipo() == 'i' && der->getTipo() == 'i')
        {
            tipo = 'i';
        }
        else if(izq->getTipo() == 'r' && der->getTipo() == 'r')
        {
            tipo = 'r';
        }
        else
        {
            tipo = 'e';
        }
    }

    char getTipo()
    {
        return tipo;
    }

};
typedef Relacional *pRelacional;

class Logico : public Condicion
{
    Condicion *izq = NULL, *der = NULL;
public:
    Logico(Condicion*i, Condicion*d, string sim)
    {
        izq = i;
        der = d;
        simbolo = sim;
    }

};
typedef Logico *pLogico;

class LogicoNOT : public Condicion
{
    Condicion *sig = NULL;
public:
    LogicoNOT(Condicion*s)
    {
        sig = s;
    }

};
typedef LogicoNOT *pLogicoNOT;

//SENTENCIAS

class Asignacion : public Sentencia
{
    Expresion *der = NULL;
    Identificador *izq = NULL;
public:

    Asignacion(Identificador *i, Expresion *d)
    {
        izq = i;
        der = d;
    }

    Asignacion(){}

    char checkType()
    {
        return 'a';
    }

    string imprime()
    {
        stringstream s;
        s << "<ASIGNACION>" << endl;
        s << izq->imprime();
        s << der->imprime();
        s << "</ASIGNACION>" << endl;
        return s.str();
    }

    string generaCodigo()
    {
        stringstream s;
        if(tipo == 'i')//Asignación para enteros
        {
            s << der->generaCodigo();
            s << "pop _" << izq->getSimbolo() << ";" << endl;
        }
        else //Empezar por aqui si se quieren integrar numeros reales o cadenas
            s << ";Solo asigna enteros" << endl;

        return s.str();
    }

    void semantico()
    {
        der->semantico();
        tipo = der->getTipo();
    }

    char getTipo()
    {
        return tipo;
    }

    string getID()
    {
        return izq->simbolo;
    }
};
typedef Asignacion *pAsignacion;

class Print : public Sentencia
{
    Expresion *contenido = NULL;
public:

    Print(Expresion *c)
    {
        contenido = c;
    }

    Print(){}

    char checkType()
    {
        return 'p';
    }

    string imprime()
    {
        stringstream s;
        s << "<IMPRIME>" << endl;
        s << "<EXPRESION>" << endl;
        s << contenido->imprime();
        s << "</EXPRESION>" << endl;
        s << "</IMPRIME>" << endl;
        return s.str();
    }

    string generaCodigo()
    {
        stringstream s;
        if(tipo == 'i')
        {
            s << contenido->generaCodigo();
            s << "pop eax" << endl;
            s << "print str$(eax),10;" << endl;
        }
        else if(tipo == 'c')
        {
            s << "print chr$(" << contenido->getSimbolo() << "),10;" << endl;
        }

        return s.str();
    }

    void semantico()
    {
        contenido->semantico();
        tipo = contenido->getTipo();
    }

    char getTipo()
    {
        return tipo;
    }

};
typedef Print *pPrint;

class SI : public Sentencia
{
    Condicion *cond = NULL;
    Sentencia *bloque = NULL, *otro = NULL;
public:

    SI(Condicion *c, Sentencia *b, Sentencia *o)
    {
        cond = c;
        bloque = b;
        otro = o;
    }

    SI(){}

    char checkType()
    {
        return 'i';
    }

    string imprime()
    {
        pSentencia temp;
        stringstream s;

        s << "<SI>" << endl;
        s << cond->imprime();
        s << "<BLOQUE>" << endl;
        temp = bloque;
        while(temp != NULL)
        {
            s << temp->imprime();
            temp = temp->siguiente;
        }
        s << "</BLOQUE>" << endl;
        s << "<OTRO>" << endl;
        temp = otro;
        while(temp != NULL)
        {
            s << temp->imprime();
            temp = temp->siguiente;
        }
        s << "</OTRO>" << endl;
        s << "</SI>" << endl;

        return s.str();
    }

    void semantico()
    {
        cond->semantico();
        tipo = cond->getTipo();
    }

    string generaCodigo()
    {
        pSentencia temp;
        stringstream s;
        if(tipo == 'i')
        {
            s << cond->generaCodigo() << "SI" << contadorIF << endl;

            temp = bloque;
            while(temp != NULL)
            {
                s << temp->generaCodigo();
                temp = temp->siguiente;
            }

            s << "jmp FINSI" << contadorIF << endl;
            s << "SI" << contadorIF << ": " << endl;

            temp = otro;
            while(temp != NULL)
            {
                s << temp->generaCodigo();
                temp = temp->siguiente;
            }

            s << "FINSI" << contadorIF << ": " << endl;

            contadorIF++;
        }
        else //Empezar por aqui si se quieren integrar numeros reales o cadenas a la condición
            s << ";Solo compara enteros" << endl;

        return s.str();
    }

    pSentencia getBloque()
    {
        return bloque;
    }

    pSentencia getOtro()
    {
        return otro;
    }

    char getTipo()
    {
        return tipo;
    }

};
typedef SI *pSI;

class Mientras : public Sentencia
{
    Condicion *cond = NULL;
    Sentencia *bloque = NULL;
public:

    Mientras(Condicion *c, Sentencia *b)
    {
        cond = c;
        bloque = b;
    }

    Mientras(){}

    char checkType()
    {
        return 'w';
    }

    string imprime()
    {
        pSentencia temp;
        stringstream s;

        s << "<MIENTRAS>" << endl;
        s << cond->imprime();
        s << "<BLOQUE>" << endl;
        temp = bloque;
        while(temp != NULL)
        {
            s << temp->imprime();
            temp = temp->siguiente;
        }
        s << "</BLOQUE>" << endl;
        s << "</MIENTRAS>" << endl;

        return s.str();
    }

    string generaCodigo()
    {
        pSentencia temp;
        stringstream s;
        if(tipo == 'i')
        {
            s << "MIENTRAS" << contadorWHILE << ": " << endl;
            s << cond->generaCodigo() << "FINMIENTRAS" << contadorWHILE << endl;

            temp = bloque;
            while(temp != NULL)
            {
                s << temp->generaCodigo();
                temp = temp->siguiente;
            }

            s << "jmp MIENTRAS" << contadorWHILE << endl;
            s << "FINMIENTRAS" << contadorWHILE << ": " << endl;

            contadorWHILE++;
        }
        else //Empezar por aqui si se quieren integrar numeros reales o cadenas a la condición
            s << ";Solo compara enteros" << endl;

        return s.str();
    }

    void semantico()
    {
        cond->semantico();
        tipo = cond->getTipo();
    }

    pSentencia getBloque()
    {
        return bloque;
    }

    char getTipo()
    {
        return tipo;
    }

};
typedef Mientras *pMientras;


#endif // NODOS_H_INCLUDED
