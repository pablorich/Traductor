#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void limpiarPantalla()
{
    system(CLEAR);
}

void limpiarBufferTeclado()
{
    cin.clear();
    while (cin.get() != '\n');
}


void pausar()
{
    cout << endl << "Presiona enter para continuar... ";
    limpiarBufferTeclado();
    //cin.ignore();
}

void espera(int time)
{
    Sleep(1000 * time);//15 segundos
}

#endif // UTIL_H_INCLUDED
