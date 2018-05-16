#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

class ActualID
{
public:
    ActualID *siguiente = NULL;
    string simbolo;
    char tipo;

    ActualID(string sim, char t)
    {
        simbolo = sim;
        tipo = t;
    }

    void setTipo(char t)
    {
        tipo = t;
    }

    char getTipo()
    {
        return tipo;
    }

    string getID()
    {
        return simbolo;
    }
};
typedef ActualID *pActualID;


class Lista
{
private:
    pActualID root;
    pActualID fin;
public:

    Lista()
    {
        root = NULL;
        fin = NULL;
    }

    bool vacio()
    {
        return (root == NULL);
    }

    void insertaNodo(string nuevo, char t)
    {
        if(vacio())
        {
            root = new ActualID(nuevo,t);
            fin = root;
        }
        else
        {
            if(!buscaID(nuevo))
            {
                fin->siguiente = new ActualID(nuevo, t);
                fin = fin->siguiente;
            }
            else
            {
                updateType(nuevo, t);
            }
        }
    }

    bool buscaID(string busca)
    {
        pActualID temp = root;
        while(temp != NULL)
        {
            if(temp->getID() == busca)
            {
                return true;
            }
            temp = temp->siguiente;
        }
        return false;
    }

    char tipoID(string busca)
    {
        pActualID temp = root;
        while(temp != NULL)
        {
            if(temp->getID() == busca)
            {
                return temp->getTipo();
            }
            temp = temp->siguiente;
        }
        return 'e';
    }

    void updateType(string busca, char t)
    {
        pActualID temp = root;
        while(temp != NULL)
        {
            if(temp->getID() == busca)
            {
                temp->setTipo(t);
                break;
            }
            temp = temp->siguiente;
        }
    }

    void imprime()
    {
        pActualID temp = root;
        while(temp != NULL)
        {
            cout << temp->getID() << "\t" << temp->getTipo() << endl;
            temp = temp->siguiente;
        }
    }


};

#endif // LISTA_H_INCLUDED

