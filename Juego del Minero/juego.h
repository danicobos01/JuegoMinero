// Trabajo realizado por Daniel Cobos Peñas
// Grupo: 1ºB
// Correo: dcobos01@ucm.es

#ifndef juego_h
#define juego_h

#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
#include "mina.h"
using namespace std;

// Declaracion de tipos y estructuras:

typedef enum tTecla {
	ARRIBA, ABAJO, IZDA, DCHA, SALIR, DINAMITA
};

typedef enum tEstado {
	EXPLORANDO, EXITO, ABANDONO, FRACASO
};

typedef struct {
	tMina mina;
	int numGemas = 0;
	int numMov = 0;
	int numDinamita = 0;
	tEstado estado;
} tJuego;





// Prototipos:

// Recibe la opcion de escala y llama a la funcion de dibujo correspondiente
void dibujar(tJuego& juego);

// Funcion que convierte en tipo tTecla el botón del teclado que haya tocado el usuario
tTecla leerTecla();

// Llamamos a la funcion cuando el minero empuja la piedra hacia un hueco vacío
void caidatrasEmpuje(tJuego& juego, tTecla& tecla);

// Dado un objeto cuenta cuantos hay por encima para llevar a cabo la cascada
int contadorObjetos(tJuego& juego, tTecla& tecla);

// Cuenta cuantos huecos hay cuando cae una piedra
int contadorHuecos(tJuego& juego, tTecla& tecla);

// Funcion que comprueba que el minero no este encerrado entre casillas que impiden cualquier tipo de movimiento
bool encerrado(tJuego& juego);

// Funcion que deja libres toda las casillas adyacentes donde se haya colocado la dinamita. Si el minero es una de ellas detecta que ha muerto.
void explosion(tJuego& juego, int num);

// Funcion que lee el movimiento que se haya introducido
void leerMovimiento(tJuego& juego, tTecla& tecla);

// Funcion que realiza el movimiento y si es posible devuelve true a la funcion leerMovimiento
bool hacerMovimiento(tJuego& juego, tTecla& mov);

// Funcion que se encarga de abrir los ficheros y cargar la mina y realizar los movimientos pertinentes
bool cargar_Juego(tJuego& juego, int nivel);

// Funcion que pregunta el usuario a que escala quiere jugar
int menuEscala(tJuego& juego);

// Funcion que pregunta al usuario como prefiere realizar los movimientos
int menuMetodoMov(tJuego& juego);

// Menu que pregunta una vez has completado algun nivel
int menuNivel();

// Menu que aparece una vez el minero ha muerto o se ha quedado atrapado
int menuMuerte();


#endif






