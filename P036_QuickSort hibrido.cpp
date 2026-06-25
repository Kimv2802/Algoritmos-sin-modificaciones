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
bool mensajelnsertionsort = false;
bool mensajeTim = false;

int leerEntero() {
    int valor;
    while (true) {
        if (cin >> valor) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Ingrese un número entero: ";
        }
    }
}

void insertionSort(int arr[], int left, int right, bool ascendente) {
    if (!mensajelnsertionsort) {
        cout << "Inicio ordenamiento con inserción en subarreglos de tamaño <= " << RUN << "\n";
        mensajelnsertionsort = true;
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

int medianaDeTres(int arr[], int low, int high, bool ascendente) {
    int mid = low + (high - low) / 2;
    if (ascendente) {
        if (arr[mid] < arr[low]) swap(arr[mid], arr[low]);
        if (arr[high] < arr[low]) swap(arr[high], arr[low]);
        if (arr[high] < arr[mid]) swap(arr[high], arr[mid]);
    } else {
        if (arr[mid] > arr[low]) swap(arr[mid], arr[low]);
        if (arr[high] > arr[low]) swap(arr[high], arr[low]);
        if (arr[high] > arr[mid]) swap(arr[high], arr[mid]);
    }
    swap(arr[mid], arr[high]);
    return arr[high];
}

int particion(int arr[], int low, int high, bool ascendente) {
    int pivot = medianaDeTres(arr, low, high, ascendente);
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if ((ascendente && arr[j] < pivot) || (!ascendente && arr[j] > pivot)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHybridRec(int arr[], int low, int high, bool ascendente) {
    while (low < high) {
        if (high - low + 1 <= RUN) {
            insertionSort(arr, low, high, ascendente);
            break;
        }
        int p = particion(arr, low, high, ascendente);
        
        if (p - 1 - low < high - (p + 1)) {
            quickSortHybridRec(arr, low, p - 1, ascendente);
            low = p + 1;
        } else {
            quickSortHybridRec(arr, p + 1, high, ascendente);
            high = p - 1;
        }
    }
}

void quickInsertionSort(int arr[], int n, bool ascendente) {
    if (!mensajeTim) {
        cout << "Inicio ordenamiento con Hybrid Quick-Insertion Sort (QuickSort + Insertion Sort, umbral = " << RUN << ")\n";
        mensajeTim = true;
    }
    auto inicio = high_resolution_clock::now();
    if (n > 1) {
        quickSortHybridRec(arr, 0, n - 1, ascendente);
    }
    auto fin = high_resolution_clock::now();
    duration<double> duracion = fin - inicio;
    cout << "Tiempo de ejecución de Hybrid Quick-Insertion Sort: " << duracion.count() << " segundos\n";
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
    
    archivo.clear();
    archivo.seekg(0);
    
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
    cout << "¿Cuántos elementos desea mostrar? ";
    int elementos = leerEntero();
    
    cout << "Contenido del arreglo: ";
    for (int i = 0; i < elementos && i < tam; i++) {
        cout << arreglo[i] << " ";
    }
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
        cout << "El arreglo no está ordenado";
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

void ordenarHybridQuickInsertion() {
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
        mensajeCambioOrden = true;
    }
    if (opcionsal == opcion && archivoOrdenado) {
        cout << "El arreglo ya ha sido ordenado en ese orden\n";
        return;
    }
    
    opcionsal = opcion;
    cout << "Iniciando ordenamiento con Hybrid Quick Insertion Sort de un arreglo de " << tam << " elementos\n";
    quickInsertionSort(arreglo, tam, asc);
    archivoOrdenado = true;
    cout << "Ordenamiento completado exitosamente\n";
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
        cout << "2. Ordenar con Hybrid Quick Insertion Sort\n";
        cout << "3. Guardar archivo\n";
        cout << "4. Mostrar arreglo\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        
        opcion = leerEntero();
        switch (opcion) {
            case 1: leerArchivo(); break;
            case 2: ordenarHybridQuickInsertion(); break;
            case 3: guardarArchivo(); break;
            case 4: mostrarArreglo(); break;
            case 5: continuar = !salir(); break;
            default: cout << "Opción inválida\n";
        }
    } while (continuar);
    
    return 0;
}
