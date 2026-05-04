// Autores: Edna Martha Miranda Chávez / Sergio Fuenlabrada Velázquez
// Descripción: Programa en C++ para ordenar un archivo de enteros usando el método IntroSort

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <locale>
#include <chrono>
#include <limits>

using namespace std;

// Variables globales
int* arreglo = nullptr;
int tam = 0;
int opcionsal = 0;
bool archivoLeido = false;
bool archivoOrdenado = false;
bool archivoGrabado = false;

// Flags de activación de algoritmos para mostrar mensajes una sola vez
bool mensajeHeapsort = false;
bool mensajeInsertionsort = false;

int leerEntero() {
    int valor;
    while (true) {
        cin >> valor;
        if (cin.fail()) {
            cin.clear(); // Limpia el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta la entrada inválida
            cout << "Entrada inválida. Por favor, ingrese un número entero: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return valor;
        }
    }
}

void insertionSort(int* arr, int izquierda, int derecha, bool ascendente) {
    if (!mensajeInsertionsort) {
        cout << "Se activa Insertion Sort para subarreglo pequeño (" << derecha - izquierda + 1 << " elementos)\n";
        mensajeInsertionsort = true;
    }
    for (int i = izquierda + 1; i <= derecha; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= izquierda && ((ascendente && arr[j] > key) || (!ascendente && arr[j] < key))) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void heapify(int* arr, int n, int i, bool ascendente) {
    int extremo = i;
    int izquierda = 2 * i + 1;
    int derecha = 2 * i + 2;

    if (ascendente) {
        if (izquierda < n && arr[izquierda] > arr[extremo]) extremo = izquierda;
        if (derecha < n && arr[derecha] > arr[extremo]) extremo = derecha;
    } else {
        if (izquierda < n && arr[izquierda] < arr[extremo]) extremo = izquierda;
        if (derecha < n && arr[derecha] < arr[extremo]) extremo = derecha;
    }

    if (extremo != i) {
        swap(arr[i], arr[extremo]);
        heapify(arr, n, extremo, ascendente);
    }
}

void heapSort(int* arr, int n, bool ascendente) {
    if (!mensajeHeapsort) {
        cout << "Se activa Heapsort debido a profundidad excesiva en QuickSort\n";
        mensajeHeapsort = true;
    }
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, ascendente);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, ascendente);
    }
}

void introsortUtil(int* arr, int izquierda, int derecha, int profundidadMax, bool ascendente) {
    int n = derecha - izquierda + 1;
    if (n <= 16) {
        insertionSort(arr, izquierda, derecha, ascendente);
        return;
    }
    if (profundidadMax == 0) {
        heapSort(arr + izquierda, n, ascendente);
        return;
    }

    int pivote = arr[izquierda + (derecha - izquierda) / 2];
    int i = izquierda, j = derecha;

    while (i <= j) {
        if (ascendente) {
            while (arr[i] < pivote) i++;
            while (arr[j] > pivote) j--;
        } else {
            while (arr[i] > pivote) i++;
            while (arr[j] < pivote) j--;
        }
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if (izquierda < j) introsortUtil(arr, izquierda, j, profundidadMax - 1, ascendente);
    if (i < derecha) introsortUtil(arr, i, derecha, profundidadMax - 1, ascendente);
}

void introsort(int* arr, int n, bool ascendente) {
    mensajeHeapsort = false;
    mensajeInsertionsort = false;
    int profundidadMax = 2 * log(n);
    cout << "Comienza Introsort con profundidad máxima permitida: " << profundidadMax << "\n";
    introsortUtil(arr, 0, n - 1, profundidadMax, ascendente);
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
    cout << "Cuantos elementos quiere mostrar: ";
    int elementos = leerEntero();
    if (elementos > tam) elementos = tam;
    cout << "Contenido del arreglo: ";
    for (int i = 0; i < elementos; i++) cout << arreglo[i] << " ";
    cout << endl;
}

void ordenarIntrosort() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo\n";
        return;
    }
    cout << "Seleccione el orden:\n1. Ascendente\n2. Descendente\nOpción: ";
    int opcion = leerEntero();
    if (opcion <= 0 || opcion > 2) {
        cout << "Opción invalida\n";
        return;
    }
    if (opcionsal == opcion && archivoOrdenado) {
        cout << "El arreglo ya ha sido ordenado, en el mismo orden\n";
        return;
    }

    opcionsal = opcion;
    bool asc = (opcion == 1);
    auto inicio = chrono::high_resolution_clock::now();
    introsort(arreglo, tam, asc);
    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);

    archivoOrdenado = true;
    cout << "Tiempo de ejecución: " << duracion.count() << " milisegundos\n";
    cout << "Ordenamiento completado exitosamente\n";
}

void guardarArchivo() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo\n";
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
    cout << "Archivo grabado exitosamente como: " << nombre << endl;
}

bool salir() {
    if (!archivoLeido) return true; 
    if (!archivoGrabado) {
        cout << "Debe grabar un archivo antes de salir\n";
        return false;
    }
    delete[] arreglo;
    return true;
}

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, " ");
    int opcion;
    bool continuar = true;

    do {
        cout << "\nMenú:\n";
        cout << "1. Leer archivo\n";
        cout << "2. Ordenar con Introsort\n";
        cout << "3. Grabar archivo\n";
        cout << "4. Mostrar arreglo\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        opcion = leerEntero();

        switch (opcion) {
            case 1: leerArchivo(); break;
            case 2: ordenarIntrosort(); break;
            case 3: guardarArchivo(); break;
            case 4: mostrarArreglo(); break;
            case 5: continuar = !salir(); break;
            default: cout << "Opción inválida\n";
        }
    } while (continuar);

    return 0;
}
