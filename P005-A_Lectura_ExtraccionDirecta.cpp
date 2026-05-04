#include <iostream>
#include <fstream>
using namespace std;

int main() {
    // Intenta abrir el archivo num10.txt
    ifstream archivo("num10.txt");
    int valor;

    // Validación de apertura
    if (!archivo) {
        cerr << "Error al abrir el archivo.\n";
        return 1;
    }

    // Lee el archivo mientras la extracción sea exitosa
    while (archivo >> valor) {
        cout << "Leido: " << valor << endl;
    }

    archivo.close(); // Siempre cerrar el archivo al finalizar[cite: 11]
    return 0;
}
