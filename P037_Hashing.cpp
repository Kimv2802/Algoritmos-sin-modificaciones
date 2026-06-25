#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>
#include <list>

using namespace std;

struct Estudiante {
    string boleta;
    string nombre;
};

class TablaHash {
private:
    static const int tamanoTabla = 100;
    vector<list<Estudiante>> tabla;

    int funcionHash(const string& boleta) {
        int suma = 0;
        for (char c : boleta) {
            suma += c;
        }
        return suma % tamanoTabla;
    }

public:
    TablaHash() : tabla(tamanoTabla) {}

    void agregarEstudiante(const string& boleta, const string& nombre) {
        int indice = funcionHash(boleta);

        for (const auto& estudiante : tabla[indice]) {
            if (estudiante.boleta == boleta) {
                cout << "Error: El estudiante con boleta " << boleta << " ya existe." << endl;
                return;
            }
        }

        Estudiante nuevoEstudiante = {boleta, nombre};
        tabla[indice].push_back(nuevoEstudiante);
    }

    void mostrarTabla() {
        for (int i = 0; i < tamanoTabla; i++) {
            cout << "Índice " << i << ": ";
            for (const auto& estudiante : tabla[i]) {
                cout << "[" << estudiante.boleta << " - " << estudiante.nombre << "] -> ";
            }
            cout << "NULL" << endl;
        }
    }

    void buscarEstudiante(const string& boleta) {
        int indice = funcionHash(boleta);
        for (const auto& estudiante : tabla[indice]) {
            if (estudiante.boleta == boleta) {
                cout << "Estudiante encontrado: " << estudiante.nombre << " (Índice: " << indice << ")" << endl;
                return;
            }
        }
        cout << "Estudiante con boleta " << boleta << " no encontrado." << endl;
    }

    void leerArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
            return;
        }

        string linea;
        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string boleta, nombre;
            if (getline(ss, boleta, ',') && getline(ss, nombre)) {
                agregarEstudiante(boleta, nombre);
            } else {
                cout << "Error al procesar la línea: " << linea << endl;
            }
        }
        archivo.close();
        cout << "Datos leídos correctamente desde el archivo." << endl;
    }
};

int main() {
    setlocale(LC_ALL, "");
    TablaHash tabla;
    int opcion;

    do {
        cout << "\nMenú:" << endl;
        cout << "1. Leer archivo" << endl;
        cout << "2. Mostrar tabla" << endl;
        cout << "3. Buscar estudiante" << endl;
        cout << "4. Finalizar" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombreArchivo;
                cout << "Ingrese el nombre del archivo (sin extensión): ";
                cin >> nombreArchivo;
                nombreArchivo += ".txt";
                tabla.leerArchivo(nombreArchivo);
                break;
            }
            case 2:
                tabla.mostrarTabla();
                break;
            case 3: {
                string boletaBusqueda;
                cout << "Ingrese la boleta del estudiante a buscar: ";
                cin >> boletaBusqueda;
                tabla.buscarEstudiante(boletaBusqueda);
                break;
            }
            case 4:
                cout << "Programa finalizado." << endl;
                break;
            default:
                cout << "Opción inválida." << endl;
        }
    } while (opcion != 4);

    return 0;
}

