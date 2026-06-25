#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <locale>
#include <cmath>
#include <chrono>

using namespace std;
using namespace chrono;

// Variables globales
vector<int> datos;
bool archivoLeido = false;
bool archivoOrdenado = false;
bool archivoGrabado = false;
bool ascendentePrevio = true;
int comparaciones = 0;
int inserciones = 0;
int cubetasUsadas = 0;
int opcionsel = 0;

int leerEntero(string mensaje = "Ingrese un número: ") {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
    }
}

void binSort(vector<int>& arr, bool ascendente) {
    if (arr.empty()) return;

    auto inicio = high_resolution_clock::now();
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int rango = maxVal - minVal + 1;
    int numCubetas = sqrt(arr.size()) + 1;
    double factor = static_cast<double>(numCubetas - 1) / rango;
    vector<vector<int>> cubetas(numCubetas);

    inserciones = 0;
    for (int val : arr) {
        int indice = static_cast<int>((val - minVal) * factor);
        if (indice < 0) indice = 0;
        if (indice >= numCubetas) indice = numCubetas - 1;
        cubetas[indice].push_back(val);
        inserciones++;
    }

    comparaciones = 0;
    arr.clear();

    if (ascendente) {
        for (auto& cubeta : cubetas) {
            sort(cubeta.begin(), cubeta.end(), [](int a, int b) {
                comparaciones++;
                return a < b;
            });
            arr.insert(arr.end(), cubeta.begin(), cubeta.end());
        }
    } else {
        for (auto it = cubetas.rbegin(); it != cubetas.rend(); ++it) {
            sort(it->begin(), it->end(), [](int a, int b) {
                comparaciones++;
                return a > b;
            });
            arr.insert(arr.end(), it->begin(), it->end());
        }
    }

    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);

    archivoOrdenado = true;
    cubetasUsadas = numCubetas;

    cout << "BinSort completado correctamente\n";
    cout << "Comparaciones: " << comparaciones << "\nInserciones: " << inserciones << "\nCubetas usadas: " << cubetasUsadas << "\n";
    cout << "Tiempo de ejecución: " << duracion.count() << " milisegundos\n";
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
    cout << "Seleccione el orden:\n1. Ascendente\n2. Descendente\n";
    int opcion = leerEntero("Opción: ");
    if (opcion != 1 && opcion != 2) {
        cout << "Opción inválida\n";
        return;
    }
    if (archivoOrdenado && opcion == opcionsel) {
        cout << "Ya ha sido ordenado\n";
        return;
    }
    
    opcionsel = opcion;
    bool asc = (opcion == 1);
    
    if (archivoOrdenado && asc == ascendentePrevio) {
        cout << "El arreglo ya fue ordenado en ese orden\n";
        return;
    }
    
    ascendentePrevio = asc;
    binSort(datos, asc);
}

void grabarArchivo() {
    if (!archivoOrdenado) {
        cout << "Debe ordenar antes de grabar\n";
        return;
    }
    if (archivoGrabado) {
        cout << "Ya se grabo el archivo\n";
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
        cout << "Hay que leer primero el archivo\n";
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
        cout << "2. Ordenar (BinSort ascendente o descendente)\n";
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
