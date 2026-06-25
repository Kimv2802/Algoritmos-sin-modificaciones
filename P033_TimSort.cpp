#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>
#include <chrono>
using namespace std;
using namespace chrono;

const int RUN = 32;
int* arreglo = nullptr;
int tam = 0;
int opcionsal = 0;
bool archivoLeido = false;
bool archivoOrdenado = false;
bool archivoGrabado = false;
bool mensajeCambioOrden = false;
bool mensajeMerge = false;
bool mensajeInsertionsort = false;
bool mensajeTim = false;

int leerEntero() {
    int valor;
    while (true) {
        cin >> valor;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Ingrese un número entero: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
    }
}

void insertionSort(int arr[], int left, int right, bool ascendente) {
    if (!mensajeInsertionsort) {
        cout << "Inicio ordenamiento con inserción " << right - left << " de tamaño del subarreglo\n";
        mensajeInsertionsort = true;
    }
    for (int i = left + 1; i <= right; i++) {
        int temp = arr[i];
        int j = i - 1;
        while (j >= left && ((ascendente && arr[j] > temp) || (!ascendente && arr[j] < temp))) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

void merge(int arr[], int l, int m, int r, bool ascendente) {
    if (!mensajeMerge) {
        cout << "Inicio ordenamiento con Merge " << r - l << " tamaño de la división\n";
        mensajeMerge = true;
    }
    int len1 = m - l + 1, len2 = r - m;
    int* left = new int[len1];
    int* right = new int[len2];
    copy(arr + l, arr + m + 1, left);
    copy(arr + m + 1, arr + r + 1, right);
    
    int i = 0, j = 0, k = l;
    while (i < len1 && j < len2) {
        if ((ascendente && left[i] <= right[j]) || (!ascendente && left[i] >= right[j])) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }
    while (i < len1) arr[k++] = left[i++];
    while (j < len2) arr[k++] = right[j++];
    
    delete[] left;
    delete[] right;
}

void timSort(int arr[], int n, bool ascendente) {
    if (!mensajeTim) {
        cout << "Inicio ordenamiento con TimSort " << RUN << " de tamaño de run \n";
        mensajeTim = true;
    }
    auto inicio = high_resolution_clock::now();
    
    for (int i = 0; i < n; i += RUN) {
        insertionSort(arr, i, min(i + RUN - 1, n - 1), ascendente);
    }
    
    for (int size = RUN; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = min(left + 2 * size - 1, n - 1);
            if (mid < right) {
                merge(arr, left, mid, right, ascendente);
            }
        }
    }
    
    auto fin = high_resolution_clock::now();
    duration<double> duracion = fin - inicio;
    cout << "Tiempo de ejecución de TimSort: " << duracion.count() << " segundos\n";
}

void ordenarTimsort() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo\n";
        return;
    }
    if (archivoGrabado) {
        cout << "Archivo ya fue grabado\n";
        return;
    }
    cout << "Seleccione el orden:\n1. Ascendente\n2. Descendente\nOpción: ";
    int opcion = leerEntero();
    if (opcion <= 0 || opcion > 2) {
        cout << "Opción inválida\n";
        return;
    }
    bool asc = (opcion == 1);
    if (opcionsal != opcion && !mensajeCambioOrden) {
        cout << (asc ? "Cambiando a orden ascendente..." : "Cambiando a orden descendente...") << endl;
    }
    mensajeCambioOrden = true;
    if (opcionsal == opcion && archivoOrdenado) {
        cout << "El arreglo ya ha sido ordenado en ese orden\n";
        return;
    }
    opcionsal = opcion;
    cout << "Iniciando ordenamiento con Timsort de un arreglo de " << tam << " tamaño\n";
    timSort(arreglo, tam, asc);
    archivoOrdenado = true;
    cout << "Ordenamiento completado exitosamente\n";
}

void leerArchivo() {
    if (archivoLeido) {
        cout << "El archivo ya ha sido leído\n";
        return;
    }
    string nombre;
    cout << "Ingrese el nombre del archivo sin extensión: ";
    cin >> ws;
    getline(cin, nombre);
    nombre += ".txt";
    ifstream archivo(nombre);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo\n";
        return;
    }
    int valor;
    tam = 0;
    while (archivo >> valor) tam++;
    if (tam == 0) {
        cout << "El archivo está vacío\n";
        archivo.close();
        return;
    }
    archivo.clear(); archivo.seekg(0);
    delete[] arreglo;
    arreglo = new int[tam];
    for (int i = 0; i < tam; i++) archivo >> arreglo[i];
    archivo.close();
    archivoLeido = true;
    archivoOrdenado = false;
    archivoGrabado = false;
    cout << "Archivo leído correctamente. Total de valores: " << tam << endl;
}

void mostrarArreglo() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo\n";
        return;
    }
    if (archivoGrabado) {
        cout << "El archivo ya fue guardado";
        return;
    }
    cout << "¿Cuántos elementos desea mostrar? ";
    int elements = leerEntero();
    cout << "Contenido del arreglo: ";
    for (int i = 0; i < elements && i < tam; i++) cout << arreglo[i] << " ";
    cout << endl;
}

void guardarArchivo() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo\n";
        return;
    }
    if (archivoGrabado) {
        cout << "archivo ya grabado \n";
        return;
    }
    if (!archivoOrdenado) {
        cout << "El arreglo no esta ordenado";
        return;
    }
    string nombre;
    cout << "Ingrese el nombre del archivo para guardar (sin extensión): ";
    cin >> ws;
    getline(cin, nombre);
    nombre += ".txt";
    ofstream archivo(nombre);
    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo\n";
        return;
    }
    for (int i = 0; i < tam; i++) archivo << arreglo[i] << " ";
    archivo.close();
    archivoGrabado = true;
    cout << "Archivo guardado exitosamente como: " << nombre << endl;
}

bool salir() {
    if (!archivoLeido) {
        cout << "Debe leer un archivo antes de salir\n";
        return false;
    }
    if (!archivoGrabado) {
        cout << "Debe guardar el archivo antes de salir\n";
        return false;
    }
    delete[] arreglo;
    return true;
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "es_MX.UTF-8");
    int opcion;
    bool continuar = true;
    do {
        cout << "\nMenú:\n";
        cout << "1. Leer archivo\n";
        cout << "2. Ordenar con Timsort\n";
        cout << "3. Guardar archivo\n";
        cout << "4. Mostrar arreglo\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        opcion = leerEntero();
        switch (opcion) {
            case 1: leerArchivo(); break;
            case 2: ordenarTimsort(); break;
            case 3: guardarArchivo(); break;
            case 4: mostrarArreglo(); break;
            case 5: continuar = !salir(); break;
            default: cout << "Opción inválida\n";
        }
    } while (continuar);
    return 0;
}
