#include <iostream>
#include <fstream>

using namespace std;

int main() {
    // Abre el archivo en modo de entrada
    ifstream archivo("num10.txt", ios::in);
    char buffer[1024]; // Espacio en memoria para guardar el contenido

    if (!archivo) {
        cerr << "Error al abrir el archivo.\n";
        return 1;
    }

    // Lee hasta 1024 caracteres del archivo y los guarda en el buffer
    archivo.read(buffer, sizeof(buffer));

    // Muestra el contenido tal cual se leyó del archivo
    cout << "Contenido crudo:\n" << buffer << endl;

    archivo.close();
    return 0;
}
