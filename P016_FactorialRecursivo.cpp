#include <iostream>
using namespace std;

int numero;
int resultado;

int factorial(int n) {
    if (n == 1) { // controla llegar a caso base
        return 1;
    } else {
        return n * factorial(n - 1); // se llama a si misma -caso recursivo
    }
}

int main() {
    cout << "Valor para factorial ";
    cin >> numero;
    resultado = factorial(numero); // Primera llamada caso base
    cout << "El factorial de " << numero << " es: " << resultado << endl;
    return 0;
}
