#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void configurarConsola();
void mostrarMenu(bool datosCargados, bool busquedaRealizada);
string leerCadenaNoVacia(const string& mensaje);
bool convertirEntero(const string& texto, int& valor);
int leerEntero(const string& mensaje, int minimo, int maximo);
bool leer(vector<int>& datos, string& nombreArchivo);
void mostrar(const vector<int>& datos);
void encontrar(const vector<int>& datos);

int main() {
    configurarConsola();
    vector<int> datos;
    string nombreArchivo;
    bool datosCargados = false;
    bool busquedaRealizada = false;
    int opcion;

    cout << "======================================\n";
    cout << "       PROGRAMA DE FUERZA BRUTA       \n";
    cout << "======================================\n";

    do {
        mostrarMenu(datosCargados, busquedaRealizada);
        opcion = leerEntero("Seleccione una opción (1-4): ", 1, 4);

        switch (opcion) {
            case 1:
                if (leer(datos, nombreArchivo)) {
                    datosCargados = true;
                    busquedaRealizada = false;
                }
                break;
            case 2:
                if (!datosCargados) {
                    cout << "\nError: Primero debe cargar los datos desde un archivo (Opción 1).\n";
                } else {
                    mostrar(datos);
                }
                break;
            case 3:
                if (!datosCargados) {
                    cout << "\nError: Primero debe cargar los datos desde un archivo (Opción 1).\n";
                } else {
                    encontrar(datos);
                    busquedaRealizada = true;
                }
                break;
            case 4:
                cout << "\nSaliendo del programa. ¡Hasta luego!\n";
                break;
        }
    } while (opcion != 4);

    return 0;
}

void configurarConsola() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    setlocale(LC_ALL, "es_MX.UTF-8");
}

void mostrarMenu(bool datosCargados, bool busquedaRealizada) {
    cout << "\n---------------- MENU GENERAL ----------------\n";
    cout << "1. Cargar datos desde archivo " << (datosCargados ? "[COMPLETADO]" : "[PENDIENTE]") << "\n";
    cout << "2. Mostrar datos cargados " << (datosCargados ? "" : "[BLOQUEADO]") << "\n";
    cout << "3. Buscar parejas por fuerza bruta " << (datosCargados ? (busquedaRealizada ? "[REALIZADO]" : "[PENDIENTE]") : "[BLOQUEADO]") << "\n";
    cout << "4. Salir\n";
    cout << "----------------------------------------------\n";
}

string leerCadenaNoVacia(const string& mensaje) {
    string entrada;
    while (true) {
        cout << mensaje;
        if (getline(cin >> ws, entrada)) {
            if (!entrada.empty()) {
                return entrada;
            }
        }
        cout << "Error: La entrada no puede estar vacía. Intente de nuevo.\n";
    }
}

bool convertirEntero(const string& texto, int& valor) {
    if (texto.empty()) return false;
    
    size_t inicio = 0;
    if (texto[0] == '-' || texto[0] == '+') {
        if (texto.size() == 1) return false;
        inicio = 1;
    }

    for (size_t i = inicio; i < texto.size(); i++) {
        if (!isdigit(texto[i])) return false;
    }

    try {
        long long temp = stoll(texto);
        if (temp < INT_MIN || temp > INT_MAX) return false;
        valor = static_cast<int>(temp);
        return true;
    } catch (...) {
        return false;
    }
}

int leerEntero(const string& mensaje, int minimo, int maximo) {
    string entrada;
    int valor;
    while (true) {
        entrada = leerCadenaNoVacia(mensaje);
        if (convertirEntero(entrada, valor)) {
            if (valor >= minimo && valor <= maximo) {
                return valor;
            }
            cout << "Error: El valor debe estar entre " << minimo << " y " << maximo << ".\n";
        } else {
            cout << "Error: Ingrese un número entero válido.\n";
        }
    }
}

bool leer(vector<int>& datos, string& nombreArchivo) {
    if (!datos.empty()) {
        cout << "\nYa se han cargado datos previamente (" << datos.size() << " elementos).\n";
        cout << "Archivo de origen: " << nombreArchivo << "\n";
        cout << "¿Desea sobrescribir los datos actuales?\n";
        cout << "1. Sí, cargar un nuevo archivo\n";
        cout << "2. No, mantener los datos actuales\n";
        int opc = leerEntero("Seleccione una opción (1-2): ", 1, 2);
        if (opc == 2) return false;
    }

    string nombre = leerCadenaNoVacia("\nIngrese el nombre del archivo (sin extensión): ");
    nombre += ".txt";

    ifstream archivo(nombre);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo \"" << nombre << "\". Verifique que exista.\n";
        return false;
    }

    vector<int> tempDatos;
    string linea;
    int numero;
    long long lineasProcesadas = 0;
    long long elementosValidos = 0;

    while (getline(archivo, linea)) {
        lineasProcesadas++;
        stringstream ss(linea);
        string token;
        while (ss >> token) {
            if (convertirEntero(token, numero)) {
                tempDatos.push_back(numero);
                elementosValidos++;
            } else {
                cout << "Aviso: Se omitió el token inválido \"" << token << "\" en la línea " << lineasProcesadas << ".\n";
            }
        }
    }

    archivo.close();

    if (tempDatos.empty()) {
        cout << "Error: El archivo \"" << nombre << "\" no contiene ningún número entero válido.\n";
        return false;
    }

    datos = move(tempDatos);
    nombreArchivo = nombre;

    cout << "\n¡Carga exitosa!\n";
    cout << "Archivo: " << nombreArchivo << "\n";
    cout << "Líneas leídas: " << lineasProcesadas << "\n";
    cout << "Elementos válidos cargados: " << elementosValidos << "\n";
    return true;
}

void mostrar(const vector<int>& datos) {
    cout << "\n---- DATOS CARGADOS ----\n";
    cout << "Total de elementos: " << datos.size() << "\n\n";
    
    int cantidad = leerEntero("¿Cuántos elementos desea visualizar?: ", 1, INT_MAX);
    if (cantidad > static_cast<int>(datos.size())) {
        cout << "Aviso: Se solicitó mostrar " << cantidad << " elementos, pero solo hay " << datos.size() << " disponibles.\n";
        cantidad = static_cast<int>(datos.size());
    }

    cout << "\nMostrando los primeros " << cantidad << " valores:\n";
    for (int i = 0; i < cantidad; i++) {
        cout << "[" << i << "] = " << datos[i] << "\n";
    }

    if (cantidad < static_cast<int>(datos.size())) {
        cout << "... Se omitieron " << datos.size() - cantidad << " valores del archivo.\n";
    }
}

void encontrar(const vector<int>& datos) {
    cout << "\n---- BÚSQUEDA POR FUERZA BRUTA ----\n";
    int objetivo = leerEntero("Ingrese el valor objetivo de la suma: ", INT_MIN, INT_MAX);

    long long comparaciones = 0;
    long long coincidencias = 0;

    cout << "\nParejas encontradas:\n";
    for (size_t i = 0; i < datos.size() - 1; i++) {
        for (size_t j = i + 1; j < datos.size(); j++) {
            comparaciones++;
            long long suma = static_cast<long long>(datos[i]) + datos[j];
            if (suma == objetivo) {
                coincidencias++;
                cout << coincidencias << ". posiciones [" << i << "] y [" << j << "]: " 
                     << datos[i] << " + " << datos[j] << " = " << objetivo << "\n";
            }
        }
    }

    if (coincidencias == 0) {
        cout << "No se encontró ninguna pareja cuya suma sea igual a " << objetivo << ".\n";
    }

    cout << "\nEstadísticas de la búsqueda:\n";
    cout << "- Total de comparaciones realizadas: " << comparaciones << "\n";
    cout << "- Total de parejas que coinciden: " << coincidencias << "\n";
}
