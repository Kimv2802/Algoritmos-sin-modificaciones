// Autores: Edna Martha Miranda Chávez
//          Sergio Fuenlabrada Velázquez
// Descripción: Ordenamiento Radix Sort con menú y gestión de archivos

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <chrono>

using namespace std;

// Variables globales para control de estado
int tam = 0;
int* arreglo = nullptr;
bool archivoLeido = false;
bool archivoOrdenado = false;
bool archivoGrabado = false;

// --- Funciones del Algoritmo Radix Sort ---

// Encuentra el valor máximo para saber cuántos dígitos procesar
int obtenerMaximo(int arr[], int n) {
    int maximo = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maximo) maximo = arr[i];
    }
    return maximo;
}

// Counting Sort adaptado para un dígito específico (exp)
void countingSort(int arr[], int n, int exp) {
    int* salida = new int[n]; 
    int conteo[10] = {0};

    // Contar ocurrencias de cada dígito (0-9)
    for (int i = 0; i < n; i++)
        conteo[(arr[i] / exp) % 10]++;

    // Cambiar conteo[i] para que contenga la posición real en salida
    for (int i = 1; i < 10; i++)
        conteo[i] += conteo[i - 1];

    // Construir el arreglo de salida de forma estable
    for (int i = n - 1; i >= 0; i--) {
        int idx = (arr[i] / exp) % 10;
        salida[conteo[idx] - 1] = arr[i];
        conteo[idx]--;
    }

    // Copiar al arreglo original
    for (int i = 0; i < n; i++)
        arr[i] = salida[i];

    delete[] salida;
}

void radixSort(int arr[], int n) {
    int m = obtenerMaximo(arr, n);
    // Aplicar counting sort para cada posición decimal: 1, 10, 100...
    for (int exp = 1; m / exp > 0; exp *= 10)
        countingSort(arr, n, exp);
}

// --- Funciones del Menú ---

void leerArchivo() {
    if (archivoLeido) {
        cout << "Ya se ha leído un archivo.\n";
        return;
    }
    string nombre;
    cout << "Ingrese nombre del archivo (sin .txt): ";
    cin >> ws;
    getline(cin, nombre);
    nombre += ".txt";

    ifstream archivo(nombre);
    if (!archivo) {
        cout << "Error al abrir el archivo.\n";
        return;
    }

    int valor;
    tam = 0;
    while (archivo >> valor) tam++;

    archivo.clear();
    archivo.seekg(0);
    delete[] arreglo;
    arreglo = new int[tam];

    for (int i = 0; i < tam; i++) archivo >> arreglo[i];
    archivo.close();

    archivoLeido = true;
    archivoOrdenado = false;
    archivoGrabado = false;
    cout << "Leídos " << tam << " elementos.\n";
}

void ordenarRadix() {
    if (!archivoLeido) { cout << "Lea un archivo primero.\n"; return; }
    if (archivoOrdenado) { cout << "Ya está ordenado.\n"; return; }
    if (archivoGrabado) { cout << "Ya se grabó, no se puede reordenar.\n"; return; }

    auto inicio = chrono::high_resolution_clock::now();
    radixSort(arreglo, tam);
    auto fin = chrono::high_resolution_clock::now();
    
    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
    archivoOrdenado = true;
    cout << "Ordenado en " << duracion.count() << " ms.\n";
}

void guardarArchivo() {
    if (!archivoLeido || archivoGrabado) { cout << "Acción no permitida.\n"; return; }
    
    string nombre;
    cout << "Nombre para guardar: ";
    cin >> ws;
    getline(cin, nombre);
    nombre += ".txt";

    ofstream archivo(nombre);
    for (int i = 0; i < tam; i++) archivo << arreglo[i] << " ";
    archivo.close();

    archivoGrabado = true;
    cout << "Archivo guardado.\n";
}

void mostrarArreglo() {
    if (!archivoLeido || archivoGrabado) { cout << "No disponible.\n"; return; }
    for (int i = 0; i < tam; i++) cout << arreglo[i] << " ";
    cout << endl;
}

int main() {
    setlocale(LC_ALL, ""); 
    int opcion;
    do {
        cout << "\n1. Leer\n2. Radix Sort\n3. Grabar\n4. Mostrar\n5. Salir\nOpción: ";
        cin >> opcion;
        switch (opcion) {
            case 1: leerArchivo(); break;
            case 2: ordenarRadix(); break;
            case 3: guardarArchivo(); break;
            case 4: mostrarArreglo(); break;
            case 5: if (archivoLeido && archivoGrabado) { delete[] arreglo; return 0; }
                    else cout << "Debe leer y grabar antes de salir.\n";
        }
    } while (true);
}
