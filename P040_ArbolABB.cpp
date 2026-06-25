#include <iostream>
#include <fstream>
#include <limits>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <windows.h>
#include <clocale>

using namespace std;

int LeerEntero(const string &mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada inválida. Debe ingresar un número entero.\n";
    }
}

int LeerEnteroEnRango(const string &mensaje, int minimo, int maximo) {
    int valor;
    do {
        valor = LeerEntero(mensaje);
        if (valor < minimo || valor > maximo) {
            cout << "Opción fuera de rango. Debe ingresar un valor entre " << minimo << " y " << maximo << ".\n";
        }
    } while (valor < minimo || valor > maximo);
    return valor;
}

bool ConvertirLineaAEntero(const string &linea, int &valor) {
    stringstream ss(linea);
    char sobrante;
    if (!(ss >> valor)) return false;
    if (ss >> sobrante) return false;
    return true;
}

class ArbolABB {
public:
    class Nodo {
    public:
        Nodo(const int dat, Nodo *izq = nullptr, Nodo *der = nullptr)
            : dato(dat), izquierdo(izq), derecho(der) {}
        int dato;
        Nodo *izquierdo;
        Nodo *derecho;
    };

    Nodo *raiz;
    Nodo *actual;

    ArbolABB() : raiz(nullptr), actual(nullptr) {}
    ~ArbolABB() { Podar(raiz); }

    void Insertar(const int dat);
    int Borrar(const int dat);
    int Buscar(const int dat);
    bool Vacio(Nodo *r) const { return r == nullptr; }
    bool EsHoja(Nodo *r) const { return r != nullptr && r->izquierdo == nullptr && r->derecho == nullptr; }
    int NumeroNodos() const;
    int AlturaArbol() const;
    int Altura(const int dat);
    int &ValorActual() { return actual->dato; }
    void Raiz() { actual = raiz; }
    void InOrden(void (*func)(int &), Nodo *nodo = nullptr, bool r = true);
    void PreOrden(void (*func)(int &), Nodo *nodo = nullptr, bool r = true);
    void PostOrden(void (*func)(int &), Nodo *nodo = nullptr, bool r = true);
    void PorNiveles();
    void MostrarEstructura();
    void MostrarGirado();
    void Balancear();
    int Obtencion();
    int Otronodo();
    int Borrarnodo();
    int ArchivoGrabar();

private:
    void Podar(Nodo *&nodo);
    int ContarNodos(Nodo *nodo) const;
    int CalcularAltura(Nodo *nodo) const;
    bool BorrarRecursivo(Nodo *&nodo, const int dat);
    Nodo *BuscarMinimo(Nodo *nodo) const;
    void GuardarNodos(Nodo *nodo, vector<int> &nodos) const;
    Nodo *ConstruirArbolBalanceado(const vector<int> &nodos, int inicio, int fin);
    void MostrarEstructuraRec(Nodo *nodo, const string &prefijo, bool esUltimo, const string &etiqueta) const;
    void MostrarGiradoRec(Nodo *nodo, int espacio, int incremento) const;
} ArbolInt;

void ArbolABB::Podar(Nodo *&nodo) {
    if (nodo == nullptr) return;
    Podar(nodo->izquierdo);
    Podar(nodo->derecho);
    delete nodo;
    nodo = nullptr;
}

void ArbolABB::Insertar(const int dat) {
    Nodo *padre = nullptr;
    actual = raiz;
    while (actual != nullptr && dat != actual->dato) {
        padre = actual;
        if (dat > actual->dato) actual = actual->derecho;
        else actual = actual->izquierdo;
    }
    if (actual != nullptr) {
        cout << "El dato " << dat << " ya existe. No se insertó duplicado.\n";
        return;
    }
    Nodo *nuevo = new Nodo(dat);
    if (padre == nullptr) {
        raiz = nuevo;
    } else if (dat < padre->dato) {
        padre->izquierdo = nuevo;
    } else {
        padre->derecho = nuevo;
    }
}

int ArbolABB::Buscar(const int dat) {
    actual = raiz;
    while (actual != nullptr) {
        if (dat == actual->dato) return 1;
        if (dat > actual->dato) actual = actual->derecho;
        else actual = actual->izquierdo;
    }
    return -1;
}

ArbolABB::Nodo *ArbolABB::BuscarMinimo(Nodo *nodo) const {
    if (nodo == nullptr) return nullptr;
    while (nodo->izquierdo != nullptr) {
        nodo = nodo->izquierdo;
    }
    return nodo;
}

bool ArbolABB::BorrarRecursivo(Nodo *&nodo, const int dat) {
    if (nodo == nullptr) return false;
    if (dat < nodo->dato) {
        return BorrarRecursivo(nodo->izquierdo, dat);
    }
    if (dat > nodo->dato) {
        return BorrarRecursivo(nodo->derecho, dat);
    }
    
    if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
        delete nodo;
        nodo = nullptr;
        return true;
    }
    if (nodo->izquierdo == nullptr) {
        Nodo *aux = nodo;
        nodo = nodo->derecho;
        delete aux;
        return true;
    }
    if (nodo->derecho == nullptr) {
        Nodo *aux = nodo;
        nodo = nodo->izquierdo;
        delete aux;
        return true;
    }
    
    Nodo *sucesor = BuscarMinimo(nodo->derecho);
    nodo->dato = sucesor->dato;
    return BorrarRecursivo(nodo->derecho, sucesor->dato);
}

int ArbolABB::Borrar(const int dat) {
    bool borrado = BorrarRecursivo(raiz, dat);
    actual = raiz;
    return borrado ? 0 : 1;
}

void ArbolABB::InOrden(void (*func)(int &), Nodo *nodo, bool r) {
    if (r) nodo = raiz;
    if (nodo == nullptr) return;
    InOrden(func, nodo->izquierdo, false);
    func(nodo->dato);
    InOrden(func, nodo->derecho, false);
}

void ArbolABB::PreOrden(void (*func)(int &), Nodo *nodo, bool r) {
    if (r) nodo = raiz;
    if (nodo == nullptr) return;
    func(nodo->dato);
    PreOrden(func, nodo->izquierdo, false);
    PreOrden(func, nodo->derecho, false);
}

void ArbolABB::PostOrden(void (*func)(int &), Nodo *nodo, bool r) {
    if (r) nodo = raiz;
    if (nodo == nullptr) return;
    PostOrden(func, nodo->izquierdo, false);
    PostOrden(func, nodo->derecho, false);
    func(nodo->dato);
}

int ArbolABB::ContarNodos(Nodo *nodo) const {
    if (nodo == nullptr) return 0;
    return 1 + ContarNodos(nodo->izquierdo) + ContarNodos(nodo->derecho);
}

int ArbolABB::NumeroNodos() const {
    return ContarNodos(raiz);
}

int ArbolABB::CalcularAltura(Nodo *nodo) const {
    if (nodo == nullptr) return -1;
    return 1 + max(CalcularAltura(nodo->izquierdo), CalcularAltura(nodo->derecho));
}

int ArbolABB::AlturaArbol() const {
    return CalcularAltura(raiz);
}

int ArbolABB::Altura(const int dat) {
    int altura = 0;
    actual = raiz;
    while (actual != nullptr) {
        if (dat == actual->dato) return altura;
        altura++;
        if (dat > actual->dato) actual = actual->derecho;
        else actual = actual->izquierdo;
    }
    return -1;
}

void ArbolABB::PorNiveles() {
    if (raiz == nullptr) {
        cout << "El árbol está vacío.\n";
        return;
    }
    int opcion;
    cout << "\n¿Desde dónde desea iniciar el recorrido?\n";
    cout << "1. Desde la raíz\n";
    cout << "2. Desde un nodo específico\n";
    opcion = LeerEnteroEnRango("Seleccione una opción: ", 1, 2);
    Nodo *nodoInicial = raiz;
    if (opcion == 2) {
        int valor = LeerEntero("Ingrese el valor del nodo desde donde desea iniciar: ");
        if (Buscar(valor) == 1) {
            nodoInicial = actual;
        } else {
            cout << "Nodo no encontrado. El recorrido iniciará desde la raíz.\n";
        }
    }
    queue<Nodo *> cola;
    cola.push(nodoInicial);
    int nivel = 0;
    cout << "\n>> Recorrido del árbol por niveles <<\n";
    while (!cola.empty()) {
        int tamNivel = static_cast<int>(cola.size());
        cout << "Nivel " << nivel << ": ";
        for (int i = 0; i < tamNivel; ++i) {
            Nodo *nodo = cola.front();
            cola.pop();
            cout << "[" << nodo->dato << "] ";
            if (nodo->izquierdo != nullptr) cola.push(nodo->izquierdo);
            if (nodo->derecho != nullptr) cola.push(nodo->derecho);
        }
        cout << endl;
        nivel++;
    }
}

void ArbolABB::MostrarEstructuraRec(Nodo *nodo, const string &prefijo, bool esUltimo, const string &etiqueta) const {
    if (nodo == nullptr) return;
    cout << prefijo;
    cout << (esUltimo ? "+-- " : "+-- ") << etiqueta << ": ";
    cout << nodo->dato << endl;
    bool tieneIzq = nodo->izquierdo != nullptr;
    bool tieneDer = nodo->derecho != nullptr;
    if (tieneIzq || tieneDer) {
        string nuevoPrefijo = prefijo + (esUltimo ? "    " : "¦   ");
        if (tieneIzq) {
            MostrarEstructuraRec(nodo->izquierdo, nuevoPrefijo, !tieneDer, "Izq");
        } else {
            cout << nuevoPrefijo << "+-- Izq: NULL\n";
        }
        if (tieneDer) {
            MostrarEstructuraRec(nodo->derecho, nuevoPrefijo, true, "Der");
        } else {
            cout << nuevoPrefijo << "+-- Der: NULL\n";
        }
    }
}

void ArbolABB::MostrarEstructura() {
    if (raiz == nullptr) {
        cout << "El árbol está vacío.\n";
        return;
    }
    cout << "\n>> Representación jerárquica del árbol <<\n";
    cout << "Raíz: " << raiz->dato << endl;
    bool tieneIzq = raiz->izquierdo != nullptr;
    bool tieneDer = raiz->derecho != nullptr;
    if (tieneIzq) {
        MostrarEstructuraRec(raiz->izquierdo, "", !tieneDer, "Izq");
    } else if (tieneDer) {
        cout << "+-- Izq: NULL\n";
    }
    if (tieneDer) {
        MostrarEstructuraRec(raiz->derecho, "", true, "Der");
    } else if (tieneIzq) {
        cout << "+-- Der: NULL\n";
    }
}

void ArbolABB::MostrarGiradoRec(Nodo *nodo, int espacio, int incremento) const {
    if (nodo == nullptr) return;
    espacio += incremento;
    MostrarGiradoRec(nodo->derecho, espacio, incremento);
    cout << endl;
    for (int i = incremento; i < espacio; i++) {
        cout << " ";
    }
    cout << nodo->dato << "\n";
    MostrarGiradoRec(nodo->izquierdo, espacio, incremento);
}

void ArbolABB::MostrarGirado() {
    if (raiz == nullptr) {
        cout << "El árbol está vacío.\n";
        return;
    }
    cout << "\n>> Árbol girado 90 grados <<\n";
    cout << "Lectura visual: derecha arriba, izquierda abajo.\n";
    MostrarGiradoRec(raiz, 0, 6);
    cout << endl;
}

void ArbolABB::GuardarNodos(Nodo *nodo, vector<int> &nodos) const {
    if (nodo == nullptr) return;
    GuardarNodos(nodo->izquierdo, nodos);
    nodos.push_back(nodo->dato);
    GuardarNodos(nodo->derecho, nodos);
}

ArbolABB::Nodo *ArbolABB::ConstruirArbolBalanceado(const vector<int> &nodos, int inicio, int fin) {
    if (inicio > fin) return nullptr;
    int medio = inicio + (fin - inicio) / 2;
    Nodo *nodo = new Nodo(nodos[medio]);
    nodo->izquierdo = ConstruirArbolBalanceado(nodos, inicio, medio - 1);
    nodo->derecho = ConstruirArbolBalanceado(nodos, medio + 1, fin);
    return nodo;
}

void ArbolABB::Balancear() {
    vector<int> nodos;
    GuardarNodos(raiz, nodos);
    if (nodos.empty()) {
        cout << "No se puede balancear: el árbol está vacío.\n";
        return;
    }
    Podar(raiz);
    raiz = ConstruirArbolBalanceado(nodos, 0, static_cast<int>(nodos.size()) - 1);
    actual = raiz;
    cout << "Árbol balanceado correctamente mediante reconstrucción.\n";
}

int ArbolABB::Obtencion() {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo sin extensión: ";
    cin >> nombreArchivo;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    nombreArchivo += ".txt";
    ifstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        cout << "Error: no se puede abrir el archivo " << nombreArchivo << endl;
        return 1;
    }
    cout << "Se abrió el archivo correctamente: " << nombreArchivo << endl;
    string linea;
    int numero;
    int insertados = 0;
    int invalidos = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        if (ConvertirLineaAEntero(linea, numero)) {
            Insertar(numero);
            cout << "Nodo procesado: " << numero << endl;
            insertados++;
        } else {
            cout << "Dato inválido omitido: " << linea << endl;
            invalidos++;
        }
    }
    archivo.close();
    cout << "Lectura finalizada. Datos procesados: " << insertados << ". Datos inválidos omitidos: " << invalidos << ".\n";
    return 0;
}

int ArbolABB::Borrarnodo() {
    int si;
    do {
        int nodo2 = LeerEntero("Indica nodo a borrar: ");
        int enc = Borrar(nodo2);
        if (enc == 0) {
            cout << "El nodo " << nodo2 << " fue borrado.\n";
        } else {
            cout << "El nodo " << nodo2 << " no existe.\n";
        }
        si = LeerEnteroEnRango("¿Desea borrar otro? (si=1, no=0): ", 0, 1);
    } while (si == 1);
    return 0;
}

int ArbolABB::Otronodo() {
    int numnod;
    do {
        numnod = LeerEntero("¿Cuántos nodos deseas agregar? ");
        if (numnod < 0) cout << "La cantidad de nodos no puede ser negativa.\n";
    } while (numnod < 0);
    for (int k = 1; k <= numnod; k++) {
        int nodo1 = LeerEntero("Teclea número de nodo: ");
        Insertar(nodo1);
    }
    return 0;
}

vector<int> nodosTemporales;

void GuardarNodosTemporales(int &dato) {
    nodosTemporales.push_back(dato);
}

int ArbolABB::ArchivoGrabar() {
    if (raiz == nullptr) {
        cout << "No se puede guardar: el árbol está vacío.\n";
        return 1;
    }
    nodosTemporales.clear();
    cout << "\nSeleccione el tipo de recorrido para guardar el árbol:\n";
    cout << "1. Inorden\n";
    cout << "2. Preorden\n";
    cout << "3. Postorden\n";
    int recorrido = LeerEnteroEnRango("Opción: ", 1, 3);
    switch (recorrido) {
        case 1: InOrden(GuardarNodosTemporales); break;
        case 2: PreOrden(GuardarNodosTemporales); break;
        case 3: PostOrden(GuardarNodosTemporales); break;
    }
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo sin extensión para guardar: ";
    cin >> nombreArchivo;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    nombreArchivo += ".txt";
    ofstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
        return 1;
    }
    for (int dato : nodosTemporales) {
        archivo << dato << endl;
    }
    archivo.close();
    cout << "Archivo generado correctamente: " << nombreArchivo << endl;
    return 0;
}

void Mostrar(int &d) {
    cout << d << " ";
}

void subMenuRecorrido() {
    int subopcion = 0;
    do {
        cout << "\n>> Submenú Mostrar árbol <<" << endl;
        cout << "1. Mostrar Inorden" << endl;
        cout << "2. Mostrar Postorden" << endl;
        cout << "3. Mostrar Preorden" << endl;
        cout << "4. Mostrar por niveles" << endl;
        cout << "5. Mostrar representación jerárquica" << endl;
        cout << "6. Mostrar árbol girado 90 grados" << endl;
        cout << "7. Regresar al menú principal" << endl;
        subopcion = LeerEnteroEnRango("Seleccione una opción: ", 1, 7);
        switch (subopcion) {
            case 1:
                cout << "InOrden: ";
                ArbolInt.InOrden(Mostrar);
                cout << endl;
                break;
            case 2:
                cout << "PostOrden: ";
                ArbolInt.PostOrden(Mostrar);
                cout << endl;
                break;
            case 3:
                cout << "PreOrden: ";
                ArbolInt.PreOrden(Mostrar);
                cout << endl;
                break;
            case 4:
                ArbolInt.PorNiveles();
                break;
            case 5:
                ArbolInt.MostrarEstructura();
                break;
            case 6:
                ArbolInt.MostrarGirado();
                break;
            case 7:
                cout << "Regresando al menú principal..." << endl;
                break;
        }
    } while (subopcion != 7);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

    int opc = 0;
    int nn;
    int an;
    int b0 = 0;
    int b1 = -1;
    bool archivoGuardado = false;

    do {
        cout << "\n Árbol Binario de Búsqueda" << endl;
        cout << " Menú" << endl << endl;
        cout << "1. Creación del árbol desde archivo" << endl;
        cout << "2. Insertar nodo" << endl;
        cout << "3. Borrar nodo" << endl;
        cout << "4. Buscar nodo" << endl;
        cout << "5. Mostrar recorridos y representación del árbol" << endl;
        cout << "6. Altura del árbol" << endl;
        cout << "7. Altura de un nodo determinado" << endl;
        cout << "8. Número de nodos" << endl;
        cout << "9. Mostrar árbol por niveles" << endl;
        cout << "10. Balancear árbol" << endl;
        cout << "11. Guardar árbol" << endl;
        cout << "0. Salir" << endl;
        opc = LeerEnteroEnRango("Seleccione una opción: ", 0, 11);

        if (opc != 0 && opc != 1 && b0 == 0) {
            cout << "Error: árbol no creado. Use primero la opción 1.\n";
            continue;
        }

        if (archivoGuardado && opc != 0) {
            cout << "El árbol ya fue guardado. Solo puede salir del programa con la opción 0.\n";
            continue;
        }

        switch (opc) {
            case 1:
                ArbolInt.Obtencion();
                b0 = 1;
                break;
            case 2:
                ArbolInt.Otronodo();
                break;
            case 3:
                ArbolInt.Borrarnodo();
                break;
            case 4:
                nn = LeerEntero("Indica el nodo a buscar: ");
                b1 = ArbolInt.Buscar(nn);
                if (b1 == 1) {
                    cout << "Encontré el nodo " << nn << endl;
                } else {
                    cout << "No encontré el nodo " << nn << endl;
                }
                break;
            case 5:
                subMenuRecorrido();
                break;
            case 6:
                an = ArbolInt.AlturaArbol();
                if (an == -1) {
                    cout << "El árbol está vacío.\n";
                } else {
                    cout << "Altura del árbol: " << an << endl;
                }
                break;
            case 7:
                nn = LeerEntero("Indica el nodo a buscar: ");
                an = ArbolInt.Altura(nn);
                if (an != -1) {
                    cout << "Altura del nodo " << nn << " es " << an << endl;
                } else {
                    cout << "El nodo " << nn << " no existe" << endl;
                }
                break;
            case 8:
                cout << "Número de nodos: " << ArbolInt.NumeroNodos() << endl;
                break;
            case 9:
                ArbolInt.PorNiveles();
                break;
            case 10:
                ArbolInt.Balancear();
                break;
            case 11:
                if (ArbolInt.ArchivoGrabar() == 0) {
                    archivoGuardado = true;
                }
                break;
            case 0:
                cout << "Termina proceso" << endl;
                break;
        }
    } while (opc != 0);

    return 0;
}
