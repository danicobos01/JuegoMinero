// Trabajo realizado por Daniel Cobos Peñas
// Grupo: 1ºB
// Correo: dcobos01@ucm.es

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#include "mina.h"
using namespace std;

// bool para ver si el movimiento que se quiere realizar no se sale del mapa 
bool dentro_matriz(tMina& mina, int x, int y)
{
	if (x >= 0 && x < mina.numFilas && y >= 0 && y < mina.numColumnas)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Funcion que convierte un char en un tCasilla
tCasilla chartocasilla(char lectura)
{
	tCasilla aux = LIBRE;
	if (lectura == '-')
	{
		aux = LIBRE;
	}
	else if (lectura == 'G')
	{
		aux = GEMA;
	}
	else if (lectura == 'M')
	{
		aux = MURO;
	}
	else if (lectura == 'J')
	{
		aux = MINERO;
	}
	else if (lectura == 'P')
	{
		aux = PIEDRA;
	}
	else if (lectura == 'S')
	{
		aux = SALIDA;
	}
	else if (lectura == 'T')
	{
		aux = TIERRA;
	}
	return aux;
}

// Para escribir los valores del tipo enumerado. El operador se sobrecarga para el tipo enumerado, no para la matriz.
ostream& operator<< (ostream& out, tCasilla const& e)
{
	char aux = LIBRE;
	if (e == LIBRE)
	{
		aux = ' ';
	}
	else if (e == GEMA)
	{
		aux = 'G';
	}
	else if (e == MURO)
	{
		aux = 'X';
	}
	else if (e == MINERO)
	{
		aux = 'M';
	}
	else if (e == PIEDRA)
	{
		aux = '@';
	}
	else if (e == SALIDA)
	{
		aux = 'S';
	}
	else if (e == TIERRA)
	{
		aux = '.';
	}
	else if (e == TNT)
	{
		aux = 'D';
	}
	return cout << aux;
}

// Funcion de color 
void colorFondo(int color)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}

// Funcion que recibe una casilla y devuelve el numero del color correspondiente de dicha casilla
void casillaColor(tCasilla casilla, int& numColor)
{
	if (casilla == LIBRE || casilla == TIERRA || casilla == PIEDRA || casilla == MURO)
	{
		numColor = 4;
	}
	else if (casilla == MINERO)
	{
		numColor = 7;
	}
	else if (casilla == TNT)
	{
		numColor = 9;
	}
	else if (casilla == GEMA)
	{
		numColor = 2;
	}
	else if (casilla == SALIDA)
	{
		numColor = 1;
	}
}


// Lee el plano de la mina del fichero y lo guarda en memoria.
void cargar_mina(istream& fichero, tMina& mina)
{
	char aux;
	char saltoLinea;
	fichero >> mina.numFilas >> mina.numColumnas;
	for (int i = 0; i < mina.numFilas; i++)
	{
		fichero.get(saltoLinea);
		for (int j = 0; j < mina.numColumnas; j++)
		{
			fichero.get(aux);
			mina.plano_minas[i][j] = chartocasilla(aux);
			// Establece la posición del minero
			if (aux == 'J')
			{
				mina.filaMinero = i;
				mina.columnaMinero = j;
			}
		}
	}
}

// muestra por pantalla el plano en escala 1:1
void dibujar1_1(tMina& mina)
{
	tCasilla aux;
	int numColor = 15;
	for (int i = 0; i < mina.numFilas; i++)
	{
		for (int j = 0; j < mina.numColumnas; j++)
		{
			aux = mina.plano_minas[i][j];
			casillaColor(mina.plano_minas[i][j], numColor);
			colorFondo(numColor);
			cout << setw(2) << aux;
		}
		cout << endl;
	}
	// Devuelve al fondo el color negro
	colorFondo(0);
}

// Muestra por pantalla el plano de la mina en escala 1:3 llamando a la funcion dibuja3x3
void dibujar1_3(tMina& mina)
{
	tPlanoCaracteres caracteres;
	tPlanoColores colores;

	// Primero a cada elemento de la matriz le metemos en una matriz 3 x 3
	for (int i = 0; i < mina.numFilas; i++)
	{
		for (int j = 0; j < mina.numColumnas; j++)
		{
			dibuja3x3(mina.plano_minas[i][j], caracteres, colores, i, j);
		}
	}

	// Luego muestro la matriz que ha incrementado su tamaño en 3, tanto filas como columnas
	for (int k = 0; k < mina.numFilas * 3; k++)
	{
		for (int h = 0; h < mina.numColumnas * 3; h++)
		{
			colorFondo(colores[k][h]);
			cout << setw(2) << caracteres[k][h];
		}
		cout << endl;
	}
	// Dejo el fondo negro
	colorFondo(0);
}

// Funcion que recibe la tCasilla y la mete en su matriz 3x3 correspondiente con los colores adecuados
void dibuja3x3(tCasilla casilla, tPlanoCaracteres caracteres, tPlanoColores colores, int i, int j)
{
	for (int fila = 3 * i; fila < i * 3 + 3; fila++)
	{
		for (int col = 3 * j; col < j * 3 + 3; col++)
		{
			// Dependiendo la casilla que tenga tendrá un caracter asociado y un color u otro
			switch (casilla)
			{
			case PIEDRA:
				caracteres[fila][col] = '@';
				colores[fila][col] = 4;
				break;
			case LIBRE:
				caracteres[fila][col] = ' ';
				colores[fila][col] = 4;
				break;
			case MURO:
				caracteres[fila][col] = 'X';
				colores[fila][col] = 4;
				break;
			case TIERRA:
				caracteres[fila][col] = '.';
				colores[fila][col] = 4;
				break;
			case GEMA:
				caracteres[fila][col] = 'G';
				colores[fila][col] = 2;
				break;
			case SALIDA:
				caracteres[fila][col] = 'S';
				colores[fila][col] = 1;
				break;
			case MINERO:
				caracteres[fila][col] = 'M';
				colores[fila][col] = 7;
				break;
			case TNT:
				caracteres[fila][col] = 'D';
				colores[fila][col] = 9;
				break;

			}
		}
	}
}


