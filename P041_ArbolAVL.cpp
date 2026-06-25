#include <iostream>
#include <ctime>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <queue>
#include <vector>
#include <string>

using namespace std;

FILE *archivo1;
FILE *archivo2;
FILE *archivoSalidaOrdenado;

struct Registro {
    int num;
} entrada;

struct Registro1 {
    int num1;
    char coma1;
    double secs1;
    char coma2;
    int bus;
} entrada1[10000];

int salvan = 0;
clock_t t_ini, t_fin;
double secs;

class AVL;

class Nodo {
public:
    Nodo(const int dat, Nodo *pad = NULL, Nodo *izq = NULL, Nodo *der = NULL)
        : dato(dat), padre(pad), izquierdo(izq), derecho(der), FE(0) {}
    int dato;
    int FE;
    Nodo *izquierdo;
    Nodo *derecho;
    Nodo *padre;
    friend class AVL;
};

class AVL {
private:
    enum { IZQUIERDO, DERECHO };
    bool cambioFE;

public:
    Nodo *raiz;
    Nodo *actual;

    AVL() : raiz(NULL), actual(NULL), cambioFE(false) {}
    ~AVL() { Podar(raiz); }

    void Insertar(const int dat);
    int Borrar(const int dat);
    int Buscar(const int dat);
    bool Vacio(Nodo *r) { return r == NULL; }
    bool EsHoja(Nodo *r) { return !r->izquierdo && !r->derecho; }
    int NumeroNodos();
    int AlturaArbol();
    int Altura(const int dat);
    int &ValorActual() { return actual->dato; }
    void Raiz() { actual = raiz; }
    void InOrden(void (*func)(int&), Nodo *nodo = NULL, bool r = true);
    void PreOrden(void (*func)(int&), Nodo *nodo = NULL, bool r = true);
    void PostOrden(void (*func)(int&), Nodo *nodo = NULL, bool r = true);
    void PorNiveles();
    void MostrarEstructura();
    void MostrarGirado();
    int Obtencion();
    int Otronodo();
    int Borrarnodo();
    int ArchivoGrabar();

private:
    void Podar(Nodo *&nodo);
    int ContarNodos(Nodo *nodo);
    int CalcularAltura(Nodo *nodo);
    void Equilibrar(Nodo *nodo, int dir, bool nuevoNodo);
    void SBS(Nodo *nodo);
    void SBD(Nodo *nodo);
    void DBS(Nodo *nodo);
    void DBD(Nodo *nodo);
    void MostrarEstructuraRec(Nodo *nodo, const string &prefijo, bool esUltimo, const string &etiqueta);
    void MostrarGiradoRec(Nodo *nodo, int espacio, int incremento);
} ArbolInt;

void AVL::Podar(Nodo *&nodo) {
    if (nodo == NULL) return;
    Podar(nodo->izquierdo);
    Podar(nodo->derecho);
    delete nodo;
    nodo = NULL;
}

void AVL::Insertar(const int dat) {
    Nodo *padre = NULL;
    actual = raiz;
    while (actual != NULL && dat != actual->dato) {
        padre = actual;
        if (dat > actual->dato) actual = actual->derecho;
        else actual = actual->izquierdo;
    }
    if (actual != NULL) return;

    actual = new Nodo(dat, padre);
    if (padre == NULL) {
        raiz = actual;
    } else if (dat < padre->dato) {
        padre->izquierdo = actual;
        Equilibrar(padre, IZQUIERDO, true);
    } else {
        padre->derecho = actual;
        Equilibrar(padre, DERECHO, true);
    }
}

int AVL::Borrar(const int dat) {
    Nodo *padre = NULL;
    Nodo *nodo = raiz;
    while (nodo != NULL && dat != nodo->dato) {
        padre = nodo;
        if (dat > nodo->dato) nodo = nodo->derecho;
        else nodo = nodo->izquierdo;
    }
    if (nodo == NULL) return -1;

    Nodo *aux;
    if (nodo->izquierdo && nodo->derecho) {
        aux = nodo->derecho;
        while (aux->izquierdo) aux = aux->izquierdo;
        nodo->dato = aux->dato;
        nodo = aux;
        padre = nodo->padre;
    }

    int dir = (padre && padre->izquierdo == nodo) ? IZQUIERDO : DERECHO;
    aux = nodo->izquierdo ? nodo->izquierdo : nodo->derecho;

    if (aux) aux->padre = padre;

    if (!padre) raiz = aux;
    else if (padre->izquierdo == nodo) padre->izquierdo = aux;
    else padre->derecho = aux;

    delete nodo;

    if (padre) Equilibrar(padre, dir, false);
    return 1;
}

int AVL::Buscar(const int dat) {
    actual = raiz;
    while (actual != NULL) {
        if (dat == actual->dato) return 1;
        if (dat > actual->dato) actual = actual->derecho;
        else actual = actual->izquierdo;
    }
    return -1;
}

void AVL::Equilibrar(Nodo *nodo, int dir, bool nuevoNodo) {
    bool salir = false;
    while (nodo && !salir) {
        if (nuevoNodo) {
            if (dir == IZQUIERDO) nodo->FE--;
            else nodo->FE++;
        } else {
            if (dir == IZQUIERDO) nodo->FE++;
            else nodo->FE--;
        }

        if (nodo->FE == 0) {
            if (nuevoNodo) salir = true;
        } else if (nodo->FE == -2) {
            if (nodo->izquierdo->FE <= 0) SBS(nodo);
            else DBS(nodo);
            if (nuevoNodo) salir = true;
            else nodo = nodo->padre;
        } else if (nodo->FE == 2) {
            if (nodo->derecho->FE >= 0) SBD(nodo);
            else DBD(nodo);
            if (nuevoNodo) salir = true;
            else nodo = nodo->padre;
        } else {
            if (!nuevoNodo) salir = true;
        }

        if (nodo) {
            padre = nodo->padre;
            if (padre) dir = (padre->izquierdo == nodo) ? IZQUIERDO : DERECHO;
            nodo = padre;
        }
    }
}

void AVL::SBS(Nodo *nodo) {
    Nodo *padre = nodo->padre;
    Nodo *P = nodo;
    Nodo *Q = P->izquierdo;
    Nodo *B = Q->derecho;

    if (padre) {
        if (padre->izquierdo == P) padre->izquierdo = Q;
        else padre->derecho = Q;
    } else {
        raiz = Q;
    }

    Q->padre = padre;
    P->padre = Q;
    if (B) B->padre = P;

    Q->derecho = P;
    P->izquierdo = B;

    if (Q->FE == 0) {
        P->FE = -1;
        Q->FE = 1;
    } else {
        P->FE = 0;
        Q->FE = 0;
    }
}

void AVL::SBD(Nodo *nodo) {
    Nodo *padre = nodo->padre;
    Nodo *P = nodo;
    Nodo *Q = P->derecho;
    Nodo *B = Q->izquierdo;

    if (padre) {
        if (padre->izquierdo == P) padre->izquierdo = Q;
        else padre->derecho = Q;
    } else {
        raiz = Q;
    }

    Q->padre = padre;
    P->padre = Q;
    if (B) B->padre = P;

    Q->izquierdo = P;
    P->derecho = B;

    if (Q->FE == 0) {
        P->FE = 1;
        Q->FE = -1;
    } else {
        P->FE = 0;
        Q->FE = 0;
    }
}

void AVL::DBS(Nodo *nodo) {
    Nodo *padre = nodo->padre;
    Nodo *P = nodo;
    Nodo *Q = P->izquierdo;
    Nodo *R = Q->derecho;
    Nodo *B = R->izquierdo;
    Nodo *C = R->derecho;

    if (padre) {
        if (padre->izquierdo == P) padre->izquierdo = R;
        else padre->derecho = R;
    } else {
        raiz = R;
    }

    R->padre = padre;
    P->padre = R;
    Q->padre = R;
    if (B) B->padre = Q;
    if (C) C->padre = P;

    R->izquierdo = Q;
    R->derecho = P;
    Q->derecho = B;
    P->izquierdo = C;

    if (R->FE == -1) {
        P->FE = 1;
        Q->FE = 0;
    } else if (R->FE == 1) {
        P->FE = 0;
        Q->FE = -1;
    } else {
        P->FE = 0;
        Q->FE = 0;
    }
    R->FE = 0;
}

void AVL::DBD(Nodo *nodo) {
    Nodo *padre = nodo->padre;
    Nodo *P = nodo;
    Nodo *Q = P->derecho;
    Nodo *R = Q->izquierdo;
    Nodo *B = R->derecho;
    Nodo *C = R->izquierdo;

    if (padre) {
        if (padre->izquierdo == P) padre->izquierdo = R;
        else padre->derecho = R;
    } else {
        raiz = R;
    }

    R->padre = padre;
    P->padre = R;
    Q->padre = R;
    if (B) B->padre = Q;
    if (C) C->padre = P;

    R->derecho = Q;
    R->izquierdo = P;
    Q->izquierdo = B;
    P->derecho = C;

    if (R->FE == 1) {
        P->FE = -1;
        Q->FE = 0;
    } else if (R->FE == -1) {
        P->FE = 0;
        Q->FE = 1;
    } else {
        P->FE = 0;
        Q->FE = 0;
    }
    R->FE = 0;
}

void AVL::InOrden(void (*func)(int&), Nodo *nodo, bool r) {
    if (r) nodo = raiz;
    if (nodo == NULL) return;
    InOrden(func, nodo->izquierdo, false);
    func(nodo->dato);
    InOrden(func, nodo->derecho, false);
}

void AVL::PreOrden(void (*func)(int&), Nodo *nodo, bool r) {
    if (r) nodo = raiz
