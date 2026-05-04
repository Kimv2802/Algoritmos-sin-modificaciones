#include <iostream>
#include <string>
using namespace std;

int main() {
    setlocale(LC_ALL, ""); 
    string textoPrincipal, textoBuscado;
    bool encontrado = false;

    cout << "Ingrese el texto principal: ";
    getline(cin, textoPrincipal);
    cout << "Ingrese el texto a buscar: ";
    getline(cin, textoBuscado);

    int longitudTexto = textoPrincipal.length();
    int longitudBuscado = textoBuscado.length();

    if (longitudBuscado > longitudTexto) {
        cout << "El texto buscado es mas largo. Busqueda imposible." << endl;
        return 0;
    }

    // Recorrido con índices numéricos
    for (int i = 0; i <= longitudTexto - longitudBuscado; i++) {
        int j = 0;
        while (j < longitudBuscado && textoPrincipal[i + j] == textoBuscado[j]) {
            j++;
        }
        if (j == longitudBuscado) {
            cout << "Texto encontrado en la posicion: " << i << endl;
            encontrado = true;
            break; 
        }
    }

    if (!encontrado) {
        cout << "Texto NO encontrado en el texto principal." << endl;
    }

    return 0;
}
