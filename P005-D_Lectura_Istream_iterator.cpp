#include <iostream>
#include <fstream>
#include <iterator> // Requerido para istream_iterator
#include <vector>   // Requerido para el contenedor vector

using namespace std;

int main() {
    ifstream archivo("num10.txt");

    if (!archivo) {
        cerr << "Error al abrir el archivo.\n";
        return 1;
    }

    // Crea un iterador que apunta al inicio del archivo y otro para el final
    istream_iterator<int> inicio(archivo), fin;

    // Inicializa el vector 'numeros' con el contenido del archivo[cite: 8]
    vector<int> numeros(inicio, fin);

    // Recorre el vector para mostrar los datos cargados[cite: 8]
    for (int n : numeros) {
        cout << "Leido: " << n << endl;
    }

    archivo.close();
    return 0;
}
