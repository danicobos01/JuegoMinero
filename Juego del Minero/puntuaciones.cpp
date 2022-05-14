// Trabajo realizado por Daniel Cobos Peñas
// Grupo: 1ºB
// Correo: dcobos01@ucm.es

/*
COMENTARIOS PARA EL CORRECTOR:
Usando \t he conseguido que me quede centrado para PersonaX, si el nombre es más largo o más corto quedará algo descompensado
Sin embargo funcionan bien las funciones
*/

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
#include <string>
#include "puntuaciones.h"
using namespace std;

// Funcion que da un valor a todos los datos posibles del array puntuacion
void inicializaMinasRecorridas(tPuntuacionJugador& puntuacion)
{
	for (int i = 0; i < NUM_TOTAL_MINAS; i++)
	{
		puntuacion.vMinasRecorridas[i].IdMina = -1;
		puntuacion.vMinasRecorridas[i].numMovimientos = -1;
		puntuacion.vMinasRecorridas[i].numGemas = -1;
		puntuacion.vMinasRecorridas[i].numDinamitas = -1;
		puntuacion.vMinasRecorridas[i].puntosMina = 0;
	}
}

// Funcion que lee del archivo puntuaciones y carga el marcador
bool cargar_Marcador(tPuntuaciones& marcador)
{
	bool ok = true;
	ifstream fichero;
	string nombreAux;
	int aux;
	char espacio;
	fichero.open("puntuaciones.txt");
	// Si no se abre el archivo
	if (!fichero.is_open())
	{
		ok = false;
	}
	else
	{
		getline(fichero, nombreAux);
		while (nombreAux != "000")
		{
			if (marcador.num_jugadores == marcador.capacidad)
			{
				aumentar_Capacidad(marcador);
			}
			marcador.array_clasificacion[marcador.num_jugadores].nombre = nombreAux;
			fichero >> marcador.array_clasificacion[marcador.num_jugadores].punt_total;
			fichero >> marcador.array_clasificacion[marcador.num_jugadores].minasRecorridas;
			inicializaMinasRecorridas(marcador.array_clasificacion[marcador.num_jugadores]);
			for (int i = 0; i < marcador.array_clasificacion[marcador.num_jugadores].minasRecorridas; i++)
			{
				fichero >> aux;
				aux = aux - 1;
				marcador.array_clasificacion[marcador.num_jugadores].vMinasRecorridas[aux].IdMina = aux + 1;
				fichero >> marcador.array_clasificacion[marcador.num_jugadores].vMinasRecorridas[aux].numMovimientos;
				fichero >> marcador.array_clasificacion[marcador.num_jugadores].vMinasRecorridas[aux].numGemas;
				fichero >> marcador.array_clasificacion[marcador.num_jugadores].vMinasRecorridas[aux].numDinamitas;
				fichero >> marcador.array_clasificacion[marcador.num_jugadores].vMinasRecorridas[aux].puntosMina;
			}
			marcador.num_jugadores++;
			fichero.get(espacio);
			fichero.get(espacio);
			getline(fichero, nombreAux);
		}
		ok = true;
	}
	return ok;
}

// Funcion que vuelca en el archivo puntuaciones la informacion de marcador
bool guardar_Marcador(tPuntuaciones& marcador)
{
	bool ok = true;
	ofstream fichero;
	fichero.open("puntuaciones.txt");
	if (!fichero.is_open())
	{
		ok = false;
	}
	else
	{
		for (int i = 0; i < marcador.num_jugadores; i++)
		{
			fichero << marcador.array_clasificacion[i].nombre << endl;
			fichero << marcador.array_clasificacion[i].punt_total << endl;
			fichero << marcador.array_clasificacion[i].minasRecorridas;
			if (marcador.array_clasificacion[i].minasRecorridas == 0)
			{
				cout << " ";
			}
			fichero << endl;
			for (int j = 0; j < NUM_TOTAL_MINAS; j++)
			{
				if (marcador.array_clasificacion[i].vMinasRecorridas[j].IdMina != -1)
				{
					fichero << marcador.array_clasificacion[i].vMinasRecorridas[j].IdMina << " ";
					fichero << marcador.array_clasificacion[i].vMinasRecorridas[j].numMovimientos << " ";
					fichero << marcador.array_clasificacion[i].vMinasRecorridas[j].numGemas << " ";
					fichero << marcador.array_clasificacion[i].vMinasRecorridas[j].numDinamitas << " ";
					fichero << marcador.array_clasificacion[i].vMinasRecorridas[j].puntosMina << " " << endl;
				}
			}
		}
		fichero << "000";
	}
	fichero.close();
	return ok;
}
	
// Funcion que se encarga de insertar (por orden) el nuevo jugador
void insertar(tPuntuaciones& marcador, string const& nombre, int pos)
{
	marcador.num_jugadores++;
	if (marcador.num_jugadores >= marcador.capacidad)
	{
		aumentar_Capacidad(marcador);
	}

	for (int i = marcador.num_jugadores; i > pos; i--)
	{
		marcador.array_clasificacion[i] = marcador.array_clasificacion[i - 1];
	}
	inicializaMinasRecorridas(marcador.array_clasificacion[pos]);
	marcador.array_clasificacion[pos].nombre = nombre;
	marcador.array_clasificacion[pos].punt_total = 0;
	marcador.array_clasificacion[pos].minasRecorridas = 0;

}

// Funcion que dado un nombre lo busca entre el resto de jugadores
bool buscar(const string& nombre, const tPuntuaciones& marcador, int& pos)
{
	bool encontrado = false;
	int ini = 0;
	int fin = marcador.num_jugadores - 1;
	int mitad; 
	while (!encontrado && (ini <= fin))
	{
		mitad = (ini + fin) / 2;
		// Si el nombre es el mismo que el de posicion
		if (nombre == marcador.array_clasificacion[mitad].nombre)
		{
			pos = mitad;
			encontrado = true;
		}
		// Si el nombre (alfabeticamente) es anterior al de la posicion
		else if (nombre < marcador.array_clasificacion[mitad].nombre)
		{
			fin = mitad - 1;
			if (ini > fin)
			{
				pos = ini;
			}
		}
		// Si el nombre (alfabeticamente) es posterior al de la posicion
		else
		{
			ini = mitad + 1;
			if (fin < ini)
			{
				pos = ini;
			}
		}
	}
	return encontrado;
}

// Funcion que se encarga de inicializar el marcador
void inicializar_Marcador(tPuntuaciones& marcador, int& pos)
{
	string nombre;
	// int pos;
	marcador.capacidad = 2;
	marcador.array_clasificacion = new tPuntuacionJugador[marcador.capacidad];
	if (!cargar_Marcador(marcador))
	{
		cout << "No se ha podido cargar el archivo";
	}
	else
	{
		cout << "\n\t" <<  "Introduzca tu nombre de jugador/a " ;
		cin >> nombre;
		// Si no encuentra el nombre
		if (!buscar(nombre, marcador, pos))
		{
			insertar(marcador, nombre, pos);
			mostrar_Alfabetico(marcador);
		}
		else
		{
			mostrar_Minas_Usuario(marcador, pos);
		}
	}
}

// Funcion que muestra el menu de minas recorridas por un usuario
void mostrar_Minas_Usuario(const tPuntuaciones& marcador, int cont)
{
	int primeraVez = 0;
	cout << " \n\t Ya esta registrado/a" << endl;
	cout << "\n\t Mira las minas que ya ha recorrido  " << endl << endl;
	cout << marcador.array_clasificacion[cont].nombre << "\t\tMovimientos" << "\tGemas" << "\tDinamitas" << "\tPuntos" << "\t\tPuntos en total" << endl;
	for (int i = 0; i < NUM_TOTAL_MINAS; i++)
	{
		if (marcador.array_clasificacion[cont].vMinasRecorridas[i].IdMina > 0)
		{
			cout << "Mina " << marcador.array_clasificacion[cont].vMinasRecorridas[i].IdMina;
			cout << "\t\t\t" << marcador.array_clasificacion[cont].vMinasRecorridas[i].numMovimientos;
			cout << "\t\t" << marcador.array_clasificacion[cont].vMinasRecorridas[i].numGemas;
			cout << "\t" << marcador.array_clasificacion[cont].vMinasRecorridas[i].numDinamitas;
			cout << "\t\t" << marcador.array_clasificacion[cont].vMinasRecorridas[i].puntosMina;
			primeraVez++;
			if (primeraVez == 1)
			{
				cout << "\t\t" << marcador.array_clasificacion[cont].punt_total;
			}
		}
		cout << endl;
	}
	cout << marcador.array_clasificacion[cont].nombre << ", que mina desea explorar ahora" << endl;
	cout << "Por favor introduzca un numero entre 1 y 5 para elegir una mina y un 0 para salir: ";
}

// Funcion que muestra todos los jugadores por orden alfabético
void mostrar_Alfabetico(const tPuntuaciones& marcador)
{
	cout << "\n\t Eres nuevo/a" << endl;
	cout << "\n\t Mira las puntuaciones del resto de jugadores" << endl;
	cout << "\n\t ---------------- LISTA DE JUGADORES ------------------" << endl;
	for (int i = 0; i < marcador.num_jugadores; i++)
	{
		if (marcador.array_clasificacion[i].punt_total > 0)
		{
			cout << "\t\t\t" << marcador.array_clasificacion[i].nombre << "  " << marcador.array_clasificacion[i].punt_total << endl;
		}
	}
	cout << endl << "\t";
	system("pause");
	cout << "\n\t Que mina desea explorar ahora" << endl;
	cout << "\t Por favor introduzca un numero entre 1 y 5 para elegir una mina y un 0 para salir: ";
}

// Funcion que muestra la informacion de todos los usuarios
void mostrar_Datos_Usuario(const tPuntuaciones& marcador)
{
	cout << "\n\n\t\t ------------- JUGADORES ORDENADOS POR NOMBRE -------------" << endl << endl;
	for (int i = 0; i < marcador.num_jugadores; i++)
	{
		int aux = 0;
		cout << "\t" << marcador.array_clasificacion[i].nombre << "\t Movimientos" << "\t Gemas" << "\t Dinamitas" << "\t Puntos" << "\t Puntos en total" << endl;
		for (int j = 0; j < NUM_TOTAL_MINAS; j++)
		{
			if (marcador.array_clasificacion[i].vMinasRecorridas[j].IdMina > 0)
			{
				aux++;
				cout << "\t Mina " << marcador.array_clasificacion[i].vMinasRecorridas[j].IdMina;
				cout << "\t\t " << marcador.array_clasificacion[i].vMinasRecorridas[j].numMovimientos;
				cout << "\t\t " << marcador.array_clasificacion[i].vMinasRecorridas[j].numGemas;
				cout << "\t " << marcador.array_clasificacion[i].vMinasRecorridas[j].numDinamitas;
				cout << "\t\t " << marcador.array_clasificacion[i].vMinasRecorridas[j].puntosMina;
				if (aux == 1)
				{
					cout << "\t " << marcador.array_clasificacion[i].punt_total;
				}
				cout << endl;
			}
		}
		cout << endl;
	}
}

// Funcion que aumenta el tamaño del array dinamico
void aumentar_Capacidad(tPuntuaciones& marcador)
{
	tPuntuacionJugador* aux = nullptr;
	aux = new tPuntuacionJugador[2 * marcador.capacidad];
	for (int i = 0; i < marcador.num_jugadores; i++)
	{
		aux[i] = marcador.array_clasificacion[i];
	}
	delete[] marcador.array_clasificacion;
	marcador.array_clasificacion = aux;
	marcador.capacidad *= 2;
	aux = nullptr;
}

// Funcion que destruye el puntero para liberar memoria
void destruir(tPuntuaciones& marcador)
{
	delete[] marcador.array_clasificacion;
}

// Calcula la puntuacion total
int puntTotal(int ancho, int alto, int gemas, int mov, int din)
{
	int A = 10;
	int B = 2;
	int solucion;
	solucion = ancho * alto + A * gemas - mov - B * din;
	return solucion;
}

// Devuelve la opcion elegida
int opElegida()
{
	int op;
	cin >> op;
	while (!(op >= 0 && op < 6))
	{
		cout << "\nPorfavor introduzca un valor valido: ";
		cin >> op;
	}
	return op;
}