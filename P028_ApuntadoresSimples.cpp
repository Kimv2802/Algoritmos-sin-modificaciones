#include <iostream>
using namespace std;

int main() {
    int valor = 10;
    // Declaración e inicialización: p guarda la dirección de memoria de 'valor'
    int *p = &valor; 

    cout << "Valor inicial de variable 'valor': " << valor << endl;
    cout << "Direccion de memoria de 'valor': " << &valor << endl;
    cout << "Direccion almacenada en puntero p: " << p << endl;
    cout << "Valor al que apunta p (desreferenciando): " << *p << endl;

    // Modificar el valor a través del apuntador
    *p = 20; 
    cout << "Nuevo valor de 'valor' tras modificar via *p: " << valor << endl;

    return 0;
}
