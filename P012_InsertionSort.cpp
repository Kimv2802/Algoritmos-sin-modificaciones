// Autores: Edna Martha Miranda Chávez
//          Sergio Fuenlabrada Velázquez
// Descripción: Ordenamiento por inserción con menú interactivo y gestión de archivos

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <locale>

using namespace std;

// Variables globales
int* arreglo = nullptr; // Arreglo dinámico para almacenar los enteros
int tam = 0; // Tamaño del arreglo
bool archivoLeido = false; // Indica si el archivo ha sido leído
bool archivoOrdenado = false; // Indica si el arreglo ha sido ordenado
bool archivoGrabado = false; // Indica si el archivo ha sido grabado

// Leer valores desde archivo
void leerArchivo() {
    if (archivoLeido) {
        cout << "El archivo ya ha sido leído.\n";
        return;
    }

    string nombre;
    cout << "Ingrese el nombre del archivo sin extensión: ";
    cin >> ws;
    getline(cin, nombre);
    nombre += ".txt"; // Se agrega extensión .txt

    ifstream archivo(nombre);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo.\n";
        return;
    }

    tam = 0;
    int valor;
    // Contar cuántos elementos tiene el archivo
    while (archivo >> valor) {
        tam++;
    }

    if (tam == 0) {
        cout << "El archivo está vacío.\n";
        archivo.close();
        return;
    }

    archivo.clear(); // Limpiar estado de lectura
    archivo.seekg(0); // Regresar al inicio del archivo

    delete[] arreglo; // Liberar memoria previa si la hubiera
    arreglo = new int[tam]; // Reservar nueva memoria

    for (int i = 0; i < tam; i++) {
        archivo >> arreglo[i];
    }

    archivo.close();
    archivoLeido = true;
    archivoOrdenado = false;
    archivoGrabado = false;
    cout << "Archivo leído correctamente. Total de valores: " << tam << endl;
}

// Mostrar contenido del arreglo
void mostrarArreglo() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo.\n";
        return;
    }

    if (archivoGrabado) {
        cout << "El archivo ya ha sido grabado.\n";
        return;
    }

    cout << "Contenido del arreglo: ";
    for (int i = 0; i < tam; i++) {
        cout << arreglo[i] << " ";
    }
    cout << endl;
}

// Ordenamiento por inserción
void ordenamientoInsercion() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo.\n";
        return;
    }
    if (archivoGrabado) {
        cout << "El archivo ya ha sido grabado.\n";
        return;
    }
    if (archivoOrdenado) {
        cout << "El arreglo ya ha sido ordenado.\n";
        return;
    }

    // Implementación clásica del ordenamiento por inserción
    for (int i = 1; i < tam; i++) {
        int k = arreglo[i]; // Elemento actual a insertar
        int j = i - 1;

        // Desplazar elementos mayores a la derecha
        while (j >= 0 && arreglo[j] > k) {
            arreglo[j + 1] = arreglo[j];
            j--;
        }
        arreglo[j + 1] = k; // Insertar en la posición correcta
    }

    archivoOrdenado = true;
    cout << "Ordenamiento por inserción completado exitosamente.\n";
}

// Guardar en archivo
void guardarArchivo() {
    if (!archivoLeido) {
        cout << "Primero debe leer un archivo.\n";
        return;
    }
    if (archivoGrabado) {
        cout << "El archivo ya ha sido grabado.\n";
        return;
    }

    string nombre;
    cout << "Ingrese el nombre del archivo para guardar (sin extensión): ";
    cin >> ws;
    getline(cin, nombre);
    nombre += ".txt";

    ofstream archivo(nombre);
    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo.\n";
        return;
    }

    for (int i = 0; i < tam; i++) {
        archivo << arreglo[i] << " "; // Guardar valores separados por espacio
    }

    archivo.close();
    archivoGrabado = true;
    cout << "Archivo grabado exitosamente como: " << nombre << endl;
}

// Salir del programa
bool salir() {
    if (!archivoLeido) {
        cout << "Debe leer un archivo antes de salir.\n";
        return false;
    }

    if (!archivoGrabado) {
        cout << "Debe grabar el archivo antes de salir.\n";
        return false;
    }

    delete[] arreglo; // Liberar memoria
    cout << "Proceso terminado.\n";
    return true;
}

// Menú principal
int main() {
    setlocale(LC_ALL, ""); // Permitir caracteres con acentos y ñ
    int opcion;
    bool continuar = true;

    do {
        cout << "\nMenú:\n";
        cout << "1. Leer archivo\n";
        cout << "2. Ordenar (inserción)\n";
        cout << "3. Grabar archivo\n";
        cout << "4. Mostrar arreglo\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                leerArchivo();
                break;
            case 2:
                ordenamientoInsercion();
                break;
            case 3:
                guardarArchivo();
                break;
            case 4:
                mostrarArreglo();
                break;
            case 5:
                continuar = !salir();
                break;
            default:
                cout << "Opción inválida. Intente de nuevo.\n";
        }
    } while (continuar);

    return 0;
}
