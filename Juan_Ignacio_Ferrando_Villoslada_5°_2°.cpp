#include <iostream>
#include <cstdlib> // Incluimos libreria para random
#include <ctime> // Incluimos libreria para time()
#include <vector> // Incluimos libreria para vectores
#include <windows.h>  // Incluir la librería windows.h para cambiar colores en Windows

using namespace std;

const int TAMANIO_TABLERO = 15;
const int PASOS_LIEBRE = 2;
const int PASOS_TORTUGA = 1;
const int MAX_POZOS = 3;

struct Posicion {
    int x;
    int y;
};

// Función para cambiar el color del texto en la consola
void cambiarColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void mostrarMenu() {
    cout << "Bienvenido al Juego de la Tortuga y la Liebre!" << endl;
    cout << "Reglas del juego:" << endl;
    cout << "1. El tablero es de 15x15 y está lleno de 'X'." << endl;
    cout << "2. Hay pozos ('O') y comodines ('C') distribuidos aleatoriamente en el tablero." << endl;
    cout << "3. El equipo 1 (Tortuga) se representa con 'T' y el equipo 2 (Liebre) se representa con 'L'." << endl;
    cout << "4. La Tortuga avanza de 1 a 6 pasos por turno, y la Liebre avanza de 2 a 12 pasos." << endl;
    cout << "5. Si un equipo cae en un pozo tres veces, pierde el juego." << endl;
    cout << "6. Los comodines permiten a la Tortuga duplicar sus pasos y a la Liebre reducir sus pasos a la mitad en el siguiente turno." << endl;
    cout << "7. Gana el equipo que primero llegue a la posición (14, 14)." << endl;
    cout << "Presiona 's' para empezar el juego o cualquier otra tecla para salir." << endl;
}

vector<vector<char>> generarTablero() {
    vector<vector<char>> tablero(TAMANIO_TABLERO, vector<char>(TAMANIO_TABLERO, 'X'));

    // Generar pozos y comodines aleatorios
    for (int i = 0; i < TAMANIO_TABLERO; i++) {
        for (int j = 0; j < TAMANIO_TABLERO; j++) {
            int random = rand() % 10; // Probabilidad de 10% para pozo o comodín
            if (random == 0) {
                tablero[i][j] = 'O'; // Pozo
            } else if (random == 1) {
                tablero[i][j] = 'C'; // Comodín
            }
        }
    }

    return tablero;
}

void mostrarTablero(const vector<vector<char>>& tablero) {
    for (int i = 0; i < tablero.size(); i++) {
        for (int j = 0; j < tablero[i].size(); j++) {
            if (tablero[i][j] == 'T') {
                cambiarColor(2); // Cambiar a verde
                cout << tablero[i][j];
                cambiarColor(7); // Resetear a color original
            } else if (tablero[i][j] == 'L') {
                cambiarColor(4); // Cambiar a rojo
                cout << tablero[i][j];
                cambiarColor(7); // Resetear a color original
            } else {
                cout << tablero[i][j];
            }
            cout << " ";
        }
        cout << endl;
    }
}

bool moverEquipo(vector<vector<char>>& tablero, Posicion& pos, char equipo, int pasos, int& comodines, int& pozos) {
    // Marcar la posición actual como 'X'
    tablero[pos.x][pos.y] = 'X';
    
    // Mover el equipo
    pos.y += pasos;
    while (pos.y >= TAMANIO_TABLERO) {
        pos.y -= TAMANIO_TABLERO;
        pos.x++;
        if (pos.x >= TAMANIO_TABLERO) {
            pos.x = TAMANIO_TABLERO - 1;
            pos.y = TAMANIO_TABLERO - 1;
            break;
        }
    }

    // Comprobar la nueva posición
    if (tablero[pos.x][pos.y] == 'O') {
        pozos++;
        if (pozos >= MAX_POZOS) {
            return true; // El equipo ha perdido
        }
    } else if (tablero[pos.x][pos.y] == 'C') {
        comodines++;
    }

    // Actualizar la posición del equipo en el tablero
    tablero[pos.x][pos.y] = equipo;

    // Comprobar si el equipo ha llegado al final del tablero
    if (pos.x == TAMANIO_TABLERO - 1 && pos.y == TAMANIO_TABLERO - 1) {
        return true; // El equipo ha ganado
    }

    return false;
}

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    srand(time(NULL));

    char opcionMenu;
    mostrarMenu();
    cin >> opcionMenu;
    if (opcionMenu != 's' && opcionMenu != 'S') {
        return 0;
    }

    vector<vector<char>> tablero = generarTablero();

    // Posiciones iniciales de los equipos
    Posicion posEquipo1 = {0, 0};
    Posicion posEquipo2 = {0, 0};

    // Contadores de comodines y pozos
    int comodinesEquipo1 = 0;
    int pozosEquipo1 = 0;
    int comodinesEquipo2 = 0;
    int pozosEquipo2 = 0;

    // Colocar equipos en el tablero
    tablero[posEquipo1.x][posEquipo1.y] = 'T';
    tablero[posEquipo2.x][posEquipo2.y] = 'L';

    // Mostrar el tablero inicial
    mostrarTablero(tablero);

    // Variables para controlar el juego
    bool juegoTerminado = false;
    bool equipo1Perdio = false;
    bool equipo2Perdio = false;
    char opcion;

    while (!juegoTerminado) {
        cout << "Turno de equipo 1 (T): Presiona Enter para lanzar el dado..." << endl;
        cin.ignore(); // Esperar a que el usuario presione Enter
        int numRandomDado1 = rand() % 6 + 1;
        int pasosTortuga = numRandomDado1;
        if (tablero[posEquipo1.x][posEquipo1.y] == 'C') {
            pasosTortuga *= 2; // Doblar pasos en caso de comodín
        }
        juegoTerminado = moverEquipo(tablero, posEquipo1, 'T', pasosTortuga, comodinesEquipo1, pozosEquipo1);
        equipo1Perdio = pozosEquipo1 >= MAX_POZOS;

        limpiarPantalla();
        mostrarTablero(tablero);

        if (juegoTerminado || equipo1Perdio) break;

        cout << "Turno de equipo 2 (L): Presiona Enter para lanzar el dado..." << endl;
        cin.ignore(); // Esperar a que el usuario presione Enter
        int numRandomDado2 = rand() % 6 + 1;
        int pasosLiebre = numRandomDado2 * 2;
        if (tablero[posEquipo2.x][posEquipo2.y] == 'C') {
            pasosLiebre = numRandomDado2; // Reducir pasos en caso de comodín
        }
        juegoTerminado = moverEquipo(tablero, posEquipo2, 'L', pasosLiebre, comodinesEquipo2, pozosEquipo2);
        equipo2Perdio = pozosEquipo2 >= MAX_POZOS;

        limpiarPantalla();
        mostrarTablero(tablero);

        if (juegoTerminado || equipo2Perdio) break;

        cout << "¿Deseas continuar jugando? (s/n): ";
        cin >> opcion;
        if (opcion != 's' && opcion != 'S') {
            break;
        }

        limpiarPantalla();
    }

    if (posEquipo1.x == TAMANIO_TABLERO - 1 && posEquipo1.y == TAMANIO_TABLERO - 1) {
    	cambiarColor(2);
        cout << "¡Equipo 1 (T) ha ganado! Cayo en "<< pozosEquipo1 << " Pozos y uso " << comodinesEquipo1 << " Comodines" << endl;
        cambiarColor(7);
    } else if (posEquipo2.x == TAMANIO_TABLERO - 1 && posEquipo2.y == TAMANIO_TABLERO - 1) {
    	cambiarColor(2);
        cout << "¡Equipo 2 (L) ha ganado! Cayo en "<< pozosEquipo2 << " Pozos y uso " << comodinesEquipo2 << " Comodines" << endl;
        cambiarColor(7);
    } else if (equipo1Perdio) {
    	cambiarColor(4);
        cout << "¡Equipo 1 (T) ha perdido por caer en 3 pozos! uso "<< comodinesEquipo1 << " Comodines" << endl;
        cambiarColor(2); 
        cout << "¡Equipo 2 (L) ha ganado! cayo en " << pozosEquipo2 << " Pozos y uso " << comodinesEquipo2 << "Comodines" <<endl;
        cambiarColor(7);
    } else if (equipo2Perdio) {
    	cambiarColor(4);
        cout << "¡Equipo 2 (L) ha perdido por caer en 3 pozos! uso "<< comodinesEquipo2 << " Comodines" << endl;
        cambiarColor(2); 
        cout << "¡Equipo 1 (T) ha ganado! cayo en " << pozosEquipo1 << " Pozos y uso " << comodinesEquipo1 << " Comodines" <<endl;
        cambiarColor(7);
    } else {
        cout << "¡El juego ha terminado!" << endl;
    }

    cout << "¡Gracias por jugar!" << endl;

    return 0;
}
