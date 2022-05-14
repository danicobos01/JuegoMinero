// Trabajo realizado por Daniel Cobos Peñas
// Grupo: 1ºB
// Correo: dcobos01@ucm.es

#ifndef puntuaciones_h
#define puntuaciones_h

#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
using namespace std;

// Declaracion de tipos y estructuras

const int NUM_TOTAL_MINAS = 5;

typedef struct {
	int IdMina;
	int numMovimientos;
	int numGemas;
	int numDinamitas;
	int puntosMina;
} tDatosMina;

typedef struct {
	string nombre;
	int punt_total;
	int minasRecorridas;
	tDatosMina vMinasRecorridas[NUM_TOTAL_MINAS];
} tPuntuacionJugador;

typedef struct {
	int capacidad = 2;
	int num_jugadores = 0;
	tPuntuacionJugador* array_clasificacion;
} tPuntuaciones;

// Modulos:

// Funcion que lee del archivo puntuaciones y carga el marcador
bool cargar_Marcador(tPuntuaciones& marcador);

// Funcion que vuelca en el archivo puntuaciones la informacion de marcador
bool guardar_Marcador(tPuntuaciones& marcador);

// Funcion que muestra el menu de minas recorridas por un usuario
void mostrar_Minas_Usuario(const tPuntuaciones& marcador, int cont);

// Funcion que muestra todos los jugadores por orden alfabético
void mostrar_Alfabetico(const tPuntuaciones& marcador);

// Funcion que muestra la informacion de todos los usuarios
void mostrar_Datos_Usuario(const tPuntuaciones& marcador);

// Funcion que se encarga de inicializar el marcador
void inicializar_Marcador(tPuntuaciones& marcador, int& pos);

// Funcion que aumenta el tamaño del array dinamico
void aumentar_Capacidad(tPuntuaciones& marcador);

// Funcion que destruye el puntero para liberar memoria
void destruir(tPuntuaciones& marcador);

// Funcion que dado un nombre lo busca entre el resto de jugadores
bool buscar(const string& nombre, const tPuntuaciones& marcador, int& pos);

// Funcion que se encarga de insertar (por orden) el nuevo jugador
void insertar(tPuntuaciones& marcador, string const& nombre, int pos);

// Funcion que da un valor a todos los datos posibles del array puntuacion
void inicializaMinasRecorridas(tPuntuacionJugador& puntuacion);

// Calcula la puntuacion total
int puntTotal(int ancho, int alto, int gemas, int mov, int din);

// Devuelve la opcion elegida
int opElegida();

#endif