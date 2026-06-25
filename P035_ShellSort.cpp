#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <locale>
#include <chrono>

using namespace std;
using namespace chrono;

// Variables globales
vector<int> datos;
bool archivoLeido = false;
bool archivoOrdenado = false;
bool archivoGrabado = false;
bool ordenAscendente = true;
int comparaciones = 0;
int intercambios = 0;
int opcionSel = 0;

int leerEntero(string mensaje = "Ingrese un número: ") {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. ";
        }
    }
}

void shellSort(vector<int>& arr, bool ascendente) {
    comparaciones = 0;
    intercambios = 0;
    int n = arr.size();
    auto inicio = high_resolution_clock::now();

    for (int gap = n / 2; gap > 0; gap /= 2) {
        cout << "Tamaño del intervalo (gap): " << gap << endl;
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && ((ascendente && (++comparaciones && arr[j - gap] > temp)) || 
                                     (!ascendente && (++comparaciones && arr[j - gap] < temp))); j -= gap) {
                arr[j] = arr[j - gap];
                intercambios++;
            }
            arr[j] = temp;
        }
    }

    auto fin = high_resolution_clock::now();
    duration<double> duracion = fin - inicio;
    archivoOrdenado = true;
    ordenAscendente = ascendente;

    cout << "Shell Sort completado\n";
    cout << "Comparaciones: " << comparaciones << endl;
    cout << "Intercambios: " << intercambios << endl;
    cout << "Tiempo de ejecución: " << duracion.count() << " segundos" << endl;
}

void leerArchivo() {
    if (archivoLeido) {
        cout << "Ya se ha leído un archivo anteriormente\n";
        return;
    }
    string nombre;
    cout << "Nombre del archivo sin extensión: ";
    getline(cin >> ws, nombre);
    nombre += ".txt";

    ifstream archivo(nombre);
    if (!archivo) {
        cout << "No se pudo abrir el archivo\n";
        return;
    }

    int valor;
    while (archivo >> valor) {
        datos.push_back(valor);
    }
    archivo.close();

    if (datos.empty()) {
        cout << "El archivo está vacío\n";
        return;
    }

    archivoLeido = true;
    cout << "Archivo leído correctamente con " << datos.size() << " elementos\n";
}

void mostrarArreglo() {
    if (!archivoLeido) {
        cout << "Debe leer primero un archivo\n";
        return;
    }
    int n = leerEntero("¿Cuántos elementos desea mostrar?: ");
    n = min(n, static_cast<int>(datos.size()));
    
    for (int i = 0; i < n; i++) {
        cout << datos[i] << " ";
    }
    cout << endl;
}

void ordenar() {
    if (!archivoLeido) {
        cout << "Debe leer primero un archivo\n";
        return;
    }
    if (archivoGrabado) {
        cout << "El archivo ya fue guardado\n";
        return;
    }

    cout << "Seleccione el orden:\n1. Ascendente\n2. Descendente\nOpción: ";
    int opcion = leerEntero("");
    
    if (opcion < 1 || opcion > 2) {
        cout << "Opción inválida\n";
        return;
    }

    if (archivoOrdenado && opcion == opcionSel) {
        cout << "Ya ha sido ordenado en la misma opción\n";
        return;
    }

    opcionSel = opcion;
    bool asc = (opcion == 1);
    shellSort(datos, asc);
}

void grabarArchivo() {
    if (!archivoLeido) {
        cout << "Debe leer primero un archivo\n";
        return;
    }
    if (archivoGrabado) {
        cout << "El archivo ya fue guardado\n";
        return;
    }
    if (!archivoOrdenado) {
        cout << "Debe ordenar antes de grabar\n";
        return;
    }

    string nombre;
    cout << "Nombre del archivo de salida (sin extensión): ";
    getline(cin >> ws, nombre);
    nombre += ".txt";

    ofstream archivo(nombre);
    for (int val : datos) {
        archivo << val << " ";
    }
    archivo.close();

    archivoGrabado = true;
    cout << "Archivo grabado como: " << nombre << endl;
}

bool salir() {
    if (!archivoLeido) {
        cout << "Debe leer primero un archivo\n";
        return false;
    }
    if (!archivoGrabado) {
        cout << "Debe grabar antes de salir\n";
        return false;
    }
    return true;
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "es_MX.UTF-8");
    int opcion;
    bool continuar = true;

    while (continuar) {
        cout << "\nMenú:\n";
        cout << "1. Leer archivo\n";
        cout << "2. Ordenar (Shell Sort)\n";
        cout << "3. Mostrar datos\n";
        cout << "4. Grabar archivo\n";
        cout << "5. Salir\n";
        opcion = leerEntero("Seleccione una opción: ");

        switch (opcion) {
            case 1: leerArchivo(); break;
            case 2: ordenar(); break;
            case 3: mostrarArreglo(); break;
            case 4: grabarArchivo(); break;
            case 5: continuar = !salir(); break;
            default: cout << "Opción inválida\n";
        }
    }
    return 0;
}
