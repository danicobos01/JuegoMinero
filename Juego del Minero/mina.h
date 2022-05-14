// Trabajo realizado por Daniel Cobos Peñas
// Grupo: 1ºB
// Correo: dcobos01@ucm.es


#ifndef mina_h
#define mina_h

#pragma once
#include <iostream>
#include <fstream>
using namespace std;

// Declaraciones de tipos y estructuras: 

const int MAX = 50;

typedef enum tCasilla {
	LIBRE, TIERRA, GEMA, PIEDRA, MURO, SALIDA, MINERO, TNT
};

typedef tCasilla tPlano[MAX][MAX];

typedef struct {
	tPlano plano_minas;
	int numFilas, numColumnas, filaMinero, columnaMinero, escala, metodoMov;
} tMina;

typedef char tPlanoCaracteres[3 * MAX][3 * MAX];

typedef int tPlanoColores[3 * MAX][3 * MAX];


// Prototipos:

// Función booleana para ver si el movimiento que se quiere realizar no se sale del mapa 
bool dentro_matriz(tMina& mina, int x, int y);

// Funcion que convierte un char en un tCasilla
tCasilla chartocasilla(char lectura);

// Para escribir los valores del tipo enumerado. El operador se sobrecarga para el tipo enumerado, no para la matriz.
ostream& operator<< (ostream& out, tCasilla const& e);

// Funcion de color 
void colorFondo(int color);

// Funcion que recibe una casilla y devuelve el numero del color correspondiente de dicha casilla
void casillaColor(tCasilla casilla, int& numColor);

// Lee el plano de la mina del fichero y lo guarda en memoria.
void cargar_mina(istream& fichero, tMina& mina);

// muestra por pantalla el plano en escala 1:1
void dibujar1_1(tMina& mina);

// Muestra por pantalla el plano de la mina en escala 1:3 llamando a la funcion dibuja3x3
void dibujar1_3(tMina& mina);

// Funcion que recibe la tCasilla y la mete en su matriz 3x3 correspondiente con los colores adecuados
void dibuja3x3(tCasilla casilla, tPlanoCaracteres caracteres, tPlanoColores colores, int i, int j);

#endif
