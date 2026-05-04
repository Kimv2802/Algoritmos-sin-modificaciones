#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    // Abre el archivo num10.txt
    ifstream archivo("num10.txt");
    string linea;

    if (!archivo) {
        cerr << "Error al abrir el archivo.\n";
        return 1;
    }

    // Lee el archivo línea por línea
    while (getline(archivo, linea)) {
        istringstream ss(linea);
        int valor;

        // Extrae todos los enteros que encuentre en la línea actual
        while (ss >> valor) {
            cout << "Leido: " << valor << endl;
        }
    }

    archivo.close();
    return 0;
}
