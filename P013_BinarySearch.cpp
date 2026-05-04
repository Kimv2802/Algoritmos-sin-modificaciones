// Autores: Edna Martha Miranda Chávez
//          Sergio Fuenlabrada Velázquez
// Descripción: Implementación de búsqueda binaria sobre un arreglo dinámico ordenado

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <locale>

using namespace std;

// Variables globales
int* arreglo = nullptr;
int tam = 0;
bool archivoLeido = false;

// Verifica si el archivo contiene valores ordenados ascendentemente
bool archivoOrdenadoAscendentemente(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo para verificación\n";
        return false;
    }

    int actual, anterior;
    if (!(archivo >> anterior)) {
        cout << "El archivo está vacío o contiene datos inválidos\n";
        archivo.close();
        return false;
    }

    while (archivo >> actual) {
        if (actual < anterior) {
            archivo.close();
            return false;
        }
        anterior = actual;
    }

    archivo.close();
    return true;
}

// Leer archivo y almacenar en arreglo dinámico
void leerArchivo() {
    if (archivoLeido) {
        cout << "Ya se leyó un archivo\n";
        return;
    }

    string nombre;
    cout << "Ingrese el nombre del archivo sin extensión: ";
    cin >> ws;
    getline(cin, nombre);
    nombre += ".txt";

    if (!archivoOrdenadoAscendentemente(nombre)) {
        cout << "El archivo no está ordenado ascendentemente\n";
        return;
    }

    ifstream archivo(nombre);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo\n";
        return;
    }

    int valor;
    tam = 0;
    while (archivo >> valor) tam++;

    archivo.clear();
    archivo.seekg(0);

    delete[] arreglo;
    arreglo = new int[tam];

    for (int i = 0; i < tam; ++i) archivo >> arreglo[i];

    archivo.close();
    archivoLeido = true;
    cout << "Archivo leído correctamente \n Total de elementos: " << tam << "\n";
}

// Mostrar arreglo
void mostrarArreglo() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo\n";
        return;
    }

    cout << "Contenido del arreglo: ";
    for (int i = 0; i < tam; ++i) cout << arreglo[i] << " ";
    cout << endl;
}

// Búsqueda binaria
void busquedaBinaria() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo\n";
        return;
    }

    int objetivo;
    cout << "Ingrese el valor a buscar: ";
    cin >> objetivo;

    int inicio = 0, fin = tam - 1, medio, iteraciones = 0;

    while (inicio <= fin) {
        iteraciones++;
        medio = (inicio + fin) / 2;

        if (arreglo[medio] == objetivo) {
            cout << "Valor encontrado en el índice " << medio << " tras " << iteraciones << " iteraciones.\n";
            return;
        }

        if (arreglo[medio] < objetivo) inicio = medio + 1;
        else fin = medio - 1;
    }

    cout << "El valor no fue encontrado tras " << iteraciones << " iteraciones\n";
}

// Salir y liberar memoria
void salir() {
    delete[] arreglo;
    cout << "Proceso terminado\n";
}

// Menú principal
int main() {
    setlocale(LC_ALL, ""); // Permitir caracteres con acentos y ñ
    int opcion;

    do {
        cout << "\nMenú:\n";
        cout << "1. Leer archivo\n";
        cout << "2. Mostrar arreglo\n";
        cout << "3. Búsqueda binaria\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                leerArchivo();
                break;
            case 2:
                mostrarArreglo();
                break;
            case 3:
                busquedaBinaria();
                break;
            case 4:
                salir();
                break;
            default: 
                cout << "Opción inválida, intente de nuevo\n";
        }
    } while (opcion != 4);

    return 0;
}
