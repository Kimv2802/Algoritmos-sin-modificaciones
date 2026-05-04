#include <iostream>
using namespace std;

// Función que recibe direcciones de memoria (apuntadores)
void intercambiar(int *a, int *b) {
    int temp = *a; // Guarda el valor contenido en la dirección de 'a'
    *a = *b;       // Asigna el valor de 'b' a la dirección de 'a'
    *b = temp;     // Asigna el valor temporal a la dirección de 'b'
}

int main() {
    int x = 5, y = 8;

    cout << "Antes de intercambiar: x = " << x << ", y = " << y << endl;
    
    // Es obligatorio pasar las direcciones usando '&'
    intercambiar(&x, &y); 

    cout << "Despues de intercambiar: x = " << x << ", y = " << y << endl;

    return 0;
}
