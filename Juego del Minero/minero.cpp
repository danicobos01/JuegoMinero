// Trabajo realizado por Daniel Cobos Peñas
// Grupo: 1ºB
// Correo: dcobos01@ucm.es


#include <iostream>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include "mina.h"
#include "juego.h"
#include "puntuaciones.h"
using namespace std;

int main()
{
	tJuego juego;
	tPuntuaciones marcador;
	int op;
	op = menuEscala(juego);
	if (op == 0)
	{
		return 0;
	}
	system("cls");
	op = menuMetodoMov(juego);
	if (op == 0)
	{
		return 0;
	}
	system("cls");
	int pos;
	// Inicializamos el marcador
	inicializar_Marcador(marcador, pos);
	int nivel = opElegida();
	while (nivel != 0)
	{
		system("cls");
		// Si carga la mina correctamente
		if (cargar_Juego(juego, nivel))
		{
			// Si el jugador ha acabado con EXITO ----> Se actualiza su marcador
			if (juego.estado != FRACASO)
			{
				// Vamos a actualizar la puntuacion;
				// Para evitar que sume 1 a minasrecorridas cuando ya ha recorrido previamente esa mina
				if (marcador.array_clasificacion[pos].vMinasRecorridas[nivel - 1].puntosMina < 1)
				{
					marcador.array_clasificacion[pos].minasRecorridas++;
				}
				marcador.array_clasificacion[pos].vMinasRecorridas[nivel - 1].IdMina = nivel;
				marcador.array_clasificacion[pos].vMinasRecorridas[nivel - 1].numMovimientos = juego.numMov;
				marcador.array_clasificacion[pos].vMinasRecorridas[nivel - 1].numGemas = juego.numGemas;
				marcador.array_clasificacion[pos].vMinasRecorridas[nivel - 1].numDinamitas = juego.numDinamita;
				marcador.array_clasificacion[pos].vMinasRecorridas[nivel - 1].puntosMina = puntTotal(juego.mina.numFilas, juego.mina.numColumnas, juego.numGemas, juego.numMov, juego.numDinamita);
				marcador.array_clasificacion[pos].punt_total = 0;
				for (int i = 0; i < NUM_TOTAL_MINAS; i++)
				{
					marcador.array_clasificacion[pos].punt_total = marcador.array_clasificacion[pos].vMinasRecorridas[i].puntosMina + marcador.array_clasificacion[pos].punt_total;
				}
				system("pause");
				system("cls");
				juego.estado = EXPLORANDO;
			}
			// Si no acaba con EXITO su marcador no se actualiza
			else
			{		
				system("cls");
				juego.estado = EXPLORANDO;
			}
		}
		else
		{
			system("cls");
			cout << "No se ha podido cargar el juego, compruebe sus archivos .txt";
			return 0;
		}
		mostrar_Datos_Usuario(marcador);
		// Dejamos a 0 los valores de la puntuacion
		juego.numGemas = 0;
		juego.numMov = 0;
		juego.numDinamita = 0;
		cout << "\n\t Que mina desea explorar ahora" << endl;
		cout << "\t Por favor introduzca un numero entre 1 y 5 para elegir una mina y un 0 para salir: ";
		nivel = opElegida();

	}
	guardar_Marcador(marcador);
	destruir(marcador);
	return 0;
}



// MAIN ANTIGUO / Practica primera (IGNORAR)
/*
/*
	tJuego juego;
	int nivel = 1;
	menuEscala(juego);
	system("cls");
	menuMetodoMov(juego);
	system("cls");
	while (nivel <= 4 && nivel > 0)
	{
		if (cargar_Juego(juego, nivel))
		{
			if (juego.estado != FRACASO)
			{
				int op = menuNivel();
				if (op == 0)
				{
					return 0;
				}
				else if (op == 1)
				{
					juego.estado = EXPLORANDO;
					nivel++;
				}
				system("cls");
			}
			else
			{
				int opcion = menuMuerte();
				if (opcion == 0)
				{
					return 0;
				}
				else if (opcion == 1)
				{
					juego.estado = EXPLORANDO;
				}
				system("cls");
			}
		}
		juego.numGemas = 0;
		juego.numMov = 0;
		juego.numDinamita = 0;

	}
	cout << "                          GAME OVER                                 ";
	return 0;
	*/