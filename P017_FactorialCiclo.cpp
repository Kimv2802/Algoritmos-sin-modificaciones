#include <iostream>
using namespace std;

int numero;
int resultado;
int i;

int main() {
    cout << "Valor para factorial ";
    cin >> numero;
    resultado = numero;
    for (i = 1; i < numero; i++) {
        resultado = resultado * i;
        cout << "numero es " << numero << "i es " << i << "resultado es " << resultado << endl;
    }
    cout << "El factorial de " << numero << "es: " << resultado << endl;
    // return 0;
}


