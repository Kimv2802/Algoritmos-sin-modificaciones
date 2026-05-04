#include <iostream>
using namespace std;

int main() {
    int arreglo[5] = {1, 2, 3, 4, 5};
    int *p = arreglo; // 'p' apunta al primer elemento (arreglo[0])
    int suma = 0;

    cout << "Recorrido y modificacion con aritmetica de punteros:" << endl;

    for(int i = 0; i < 5; i++) {
        // *(p + i) es equivalente a arreglo[i]
        cout << "Valor original en posicion " << i << ": " << *(p + i) << endl;

        // Modificar el valor directamente en memoria (multiplicar por 2)
        *(p + i) = *(p + i) * 2;

        cout << "Valor modificado: " << *(p + i) << endl;
        suma += *(p + i);
    }

    cout << "Suma total de los elementos modificados: " << suma << endl;

    return 0;
}
