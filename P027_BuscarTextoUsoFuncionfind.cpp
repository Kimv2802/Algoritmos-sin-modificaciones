#include <iostream>
#include <string>
using namespace std;

int main() {
    setlocale(LC_ALL, "");
    string textoPrincipal, textoBuscado;

    cout << "Ingrese el texto donde desea buscar: ";
    getline(cin, textoPrincipal);
    cout << "Ingrese el texto que desea buscar: ";
    getline(cin, textoBuscado);

    // Uso de la función optimizada find()
    size_t posicion = textoPrincipal.find(textoBuscado);

    if (posicion != string::npos) {
        cout << "Texto encontrado en la posicion: " << posicion << endl;
    } else {
        cout << "Texto NO encontrado." << endl;
    }

    return 0;
}
