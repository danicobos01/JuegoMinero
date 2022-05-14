// Trabajo realizado por Daniel Cobos Peñas
// Grupo: 1ºB
// Correo: dcobos01@ucm.es

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
#include "juego.h"
using namespace std;

typedef struct {
	int despV; // Desplazamiento vertical
	int despH; // Desplazamiento horizontal
}tDireccion;

tDireccion tdirs4[4] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

// Para la exploracion de casillas adyacentes
const int incF[8] = { 1,1,0,-1,-1,-1,0,1 };
const int incC[8] = { 0,1,1,1,0,-1,-1,-1 };

// Recibe la opcion de escala y llama a la funcion de dibujo correspondiente
void dibujar(tJuego& juego)
{
	// 1:1
	if (juego.mina.escala == 1)
	{
		dibujar1_1(juego.mina);
	}
	// 1:3
	else if (juego.mina.escala == 2)
	{
		dibujar1_3(juego.mina);
	}
	cout << endl << endl;
	cout << "Numero de gemas recogidas: " << juego.numGemas << endl;
	cout << "Numero de movimientos realizados: " << juego.numMov << endl;
	cout << "Unidades de dinamita usadas: " << juego.numDinamita << endl;

}

// Funcion que convierte en tipo tTecla el botón del teclado que haya tocado el usuario
tTecla leerTecla()
{
	tTecla tecla;
	cin.sync();
	int dir = _getch();
	// Siempre que la opcion no sea la "D" , el "Esc" o una flecha volvera a preguntar al usuario que toque una tecla valida
	while (dir != 27 && dir != 0xe0 && dir != 100)
	{
		cout << "Movimiento no valido, introduzca un movimiento valido";
		dir = _getch();
	}
	if (dir == 27)
	{
		tecla = SALIR;
	}
	else if (dir == 100)
	{
		tecla = DINAMITA;
	}
	else if (dir == 0xe0)
	{
		dir = _getch();
		if (dir == 72)
		{
			tecla = ARRIBA;
		}
		else if (dir == 80)
		{
			tecla = ABAJO;
		}
		else if (dir == 77)
		{
			tecla = DCHA;
		}
		else if (dir == 75)
		{
			tecla = IZDA;
		}
	}
	return tecla;
}

// Llamamos a la funcion cuando el minero empuja la piedra hacia un hueco vacío
void caidatrasEmpuje(tJuego& juego, tTecla& tecla)
{
	int i = 1;
	// Si la empuja hacia la derecha
	if (tecla == DCHA)
	{
		// Cuenta cuantos huecos hay libres
		while (juego.mina.plano_minas[juego.mina.filaMinero + i][juego.mina.columnaMinero + 1] == LIBRE)
		{
			juego.mina.plano_minas[juego.mina.filaMinero + i][juego.mina.columnaMinero + 1] = PIEDRA;
			juego.mina.plano_minas[juego.mina.filaMinero + (i - 1)][juego.mina.columnaMinero + 1] = LIBRE;
			i++;
		}
	}
	// Si la empuja hacia la izquierda
	else if (tecla == IZDA)
	{
		// Cuenta cuantos huecos hay libres
		while (juego.mina.plano_minas[juego.mina.filaMinero + i][juego.mina.columnaMinero - 1] == LIBRE)
		{
			juego.mina.plano_minas[juego.mina.filaMinero + i][juego.mina.columnaMinero - 1] = PIEDRA;
			juego.mina.plano_minas[juego.mina.filaMinero + (i - 1)][juego.mina.columnaMinero - 1] = LIBRE;
			i++;
		}
	}
}

// Dado un objeto cuenta cuantos hay por encima para llevar a cabo la cascada
int contadorObjetos(tJuego& juego, tTecla& tecla)
{
	int num = 0;
	int i = 1;
	while (juego.mina.plano_minas[(juego.mina.filaMinero - tdirs4[tecla].despV) - i][juego.mina.columnaMinero - tdirs4[tecla].despH] == PIEDRA || juego.mina.plano_minas[(juego.mina.filaMinero - tdirs4[tecla].despV) - i][juego.mina.columnaMinero - tdirs4[tecla].despH] == GEMA)
	{
		i++;
		num++;
	}
	return num;
}

// Cuenta cuantos huecos hay cuando cae una piedra
int contadorHuecos(tJuego& juego, tTecla& tecla)
{
	int num = 0;
	int i = 1;
	while (juego.mina.plano_minas[(juego.mina.filaMinero - tdirs4[tecla].despV) + i][juego.mina.columnaMinero - tdirs4[tecla].despH] == LIBRE)
	{
		i++;
		num++;
	}
	return num;
}

// Funcion que comprueba que el minero no este encerrado entre casillas que impiden cualquier tipo de movimiento
bool encerrado(tJuego& juego)
{
	bool aux = true;
	// Explorar adyacentes a una casilla
	for (int dir = 0; dir < 8; dir++)
	{
		if (juego.mina.plano_minas[juego.mina.filaMinero + incF[dir]][juego.mina.columnaMinero + incC[dir]] != TIERRA && juego.mina.plano_minas[juego.mina.filaMinero + incF[dir]][juego.mina.columnaMinero + incC[dir]] != LIBRE && juego.mina.plano_minas[juego.mina.filaMinero + incF[dir]][juego.mina.columnaMinero + incC[dir]] != GEMA && juego.mina.plano_minas[juego.mina.filaMinero + incF[dir]][juego.mina.columnaMinero + incC[dir]] != SALIDA)
		{
			aux = false;
		}
		else
		{
			aux = true;
			return aux;
		}
	}
	return aux;
}


// Funcion que deja libres toda las casillas adyacentes donde se haya colocado la dinamita. Si el minero es una de ellas detecta que ha muerto.
void explosion(tJuego& juego, int num)
{
	// Explorar adyacentes a una casilla
	for (int dir = 0; dir < 8; dir++)
	{
		if (dentro_matriz(juego.mina, (juego.mina.filaMinero + num) + incF[dir], juego.mina.columnaMinero + incC[dir]))
		{
			if (juego.mina.plano_minas[(juego.mina.filaMinero + num) + incF[dir]][juego.mina.columnaMinero + incC[dir]] == MINERO)
			{
				juego.estado = FRACASO;
			}
			if (juego.mina.plano_minas[(juego.mina.filaMinero + num) + incF[dir]][juego.mina.columnaMinero + incC[dir]] != SALIDA)
			{
				juego.mina.plano_minas[(juego.mina.filaMinero + num) + incF[dir]][juego.mina.columnaMinero + incC[dir]] = LIBRE;
			}
		}
	}
	juego.mina.plano_minas[(juego.mina.filaMinero + num)][juego.mina.columnaMinero] = LIBRE;
	juego.numDinamita++;
}

// Funcion que lee el movimiento que se haya introducido
void leerMovimiento(tJuego& juego, tTecla& tecla)
{
	if (tecla == SALIR)
	{
		juego.estado = ABANDONO;
	}
	else if (tecla == DINAMITA)
	{
		int num = 0;
		// Cuenta cuantos huecos hay para poder poner la dinamita
		while (juego.mina.plano_minas[juego.mina.filaMinero + 1 + num][juego.mina.columnaMinero] == LIBRE)
		{
			num++;
		}

		// Si no hay hueco debajo el minero no podrá poner la dinamita
		if (num >= 1)
		{
			juego.mina.plano_minas[juego.mina.filaMinero + num][juego.mina.columnaMinero] = TNT;
			explosion(juego, num);
		}


	}
	else
	{
		bool movPosible = hacerMovimiento(juego, tecla);
		if (movPosible == true)
		{
			juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] = MINERO;
			// Tenemos que ver si encima del minero había piedras o no
			if (juego.mina.plano_minas[(juego.mina.filaMinero - tdirs4[tecla].despV) - 1][juego.mina.columnaMinero - tdirs4[tecla].despH] == PIEDRA || juego.mina.plano_minas[(juego.mina.filaMinero - tdirs4[tecla].despV) - 1][juego.mina.columnaMinero - tdirs4[tecla].despH] == GEMA)
			{
				// Contamos cuantos objetos hay por arriba y cuantos huecos por debajo
				int contHuecos = contadorHuecos(juego, tecla);
				int contEncima = contadorObjetos(juego, tecla);
				for (int j = 0; j <= contHuecos; j++)
				{
					int aux = 0;
					for (int i = 1; i <= contEncima; i++)
					{
						juego.mina.plano_minas[((juego.mina.filaMinero - tdirs4[tecla].despV) - aux) + j][juego.mina.columnaMinero - tdirs4[tecla].despH] = juego.mina.plano_minas[((juego.mina.filaMinero - tdirs4[tecla].despV) - i) + j][juego.mina.columnaMinero - tdirs4[tecla].despH];
						aux++;
					}
					juego.mina.plano_minas[((juego.mina.filaMinero - tdirs4[tecla].despV) - contEncima) + j][juego.mina.columnaMinero - tdirs4[tecla].despH] = LIBRE;
				}
				if (juego.mina.plano_minas[(juego.mina.filaMinero - tdirs4[tecla].despV) - (contEncima + contHuecos)][juego.mina.columnaMinero - tdirs4[tecla].despH] != SALIDA)
				{
					juego.mina.plano_minas[(juego.mina.filaMinero - tdirs4[tecla].despV) - (contEncima + contHuecos)][juego.mina.columnaMinero - tdirs4[tecla].despH] = LIBRE;
				}
			}
			// Si no habia nada encima se deja el hueco que ha dejado el minero libre
			else
			{
				juego.mina.plano_minas[juego.mina.filaMinero - tdirs4[tecla].despV][juego.mina.columnaMinero - tdirs4[tecla].despH] = LIBRE;
			}
		}
		// Si el movimiento no se ha podido realizar devuelve al minero a la posicion en la que estaba
		else if (movPosible == false)
		{
			juego.mina.filaMinero = juego.mina.filaMinero - tdirs4[tecla].despV;
			juego.mina.columnaMinero = juego.mina.columnaMinero - tdirs4[tecla].despH;
		}
		juego.numMov++;
	}
}

// Funcion que realiza el movimiento y si es posible devuelve true a la funcion leerMovimiento
bool hacerMovimiento(tJuego& juego, tTecla& mov)
{
	bool aux = false;
	// Tenemos que comprobar primero si puede realizar el movimiento:
	juego.mina.filaMinero = juego.mina.filaMinero + tdirs4[mov].despV;
	juego.mina.columnaMinero = juego.mina.columnaMinero + tdirs4[mov].despH;
	// Comprobamos que el movimiento se encuentra en los limites posibles del mapa
	if (dentro_matriz(juego.mina, juego.mina.filaMinero, juego.mina.columnaMinero) == true)
	{
		if (juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] != MURO)
		{
			if (juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] == PIEDRA)
			{
				// No se puede desplazar una piedra verticalmente
				if (mov == ARRIBA || mov == ABAJO)
				{
					aux = false;
				}
				else if (mov == IZDA)
				{
					if (juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero - 1] == LIBRE)
					{
						// Si la empujo al vacío
						if (juego.mina.plano_minas[juego.mina.filaMinero + 1][juego.mina.columnaMinero - 1] == LIBRE)
						{
							caidatrasEmpuje(juego, mov);
							juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] = MINERO;
						}
						// Si debajo hay algo
						else
						{
							juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero - 1] = PIEDRA;
							juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] = MINERO;
						}
						aux = true;
					}
					else
					{
						aux = false;
					}
				}
				else if (mov == DCHA)
				{
					if (juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero + 1] == LIBRE)
					{
						// Si la empujo al vacío
						if (juego.mina.plano_minas[juego.mina.filaMinero + 1][juego.mina.columnaMinero + 1] == LIBRE)
						{
							caidatrasEmpuje(juego, mov);
							juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] = MINERO;
						}
						// Si debajo hay algo
						else
						{
							juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero + 1] = PIEDRA;
							juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] = MINERO;
						}
						aux = true;
					}
					else
					{
						aux = false;
					}
				}
			}
			// Si es una gema, aumenta el contador de gemas
			else if (juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] == GEMA)
			{
				juego.numGemas++;
				juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] = MINERO;
				aux = true;
			}
			// Si es la salida, se concluye el nivel
			else if (juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] == SALIDA)
			{
				juego.estado = EXITO;
				aux = true;
			}
			else if (juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] == TIERRA || juego.mina.plano_minas[juego.mina.filaMinero][juego.mina.columnaMinero] == LIBRE)
			{
				aux = true;
			}
		}
		else
		{
			aux = false;
		}
	}
	else
	{
		aux = false;
	}

	return aux;
}


bool cargar_Juego(tJuego& juego, int nivel)
{
	ifstream fichero;
	bool is_valido;
	if (nivel == 1)
	{
		fichero.open("1.txt");
	}
	else if (nivel == 2)
	{
		fichero.open("2.txt");
	}
	else if (nivel == 3)
	{
		fichero.open("3.txt");
	}
	else if (nivel == 4)
	{
		fichero.open("4.txt");
	}
	else if (nivel == 5)
	{
		fichero.open("5.txt");
	}
	// Comprueba si se ha abierto correctamente
	if (fichero.is_open())
	{
		// Carga la mina desde el fichero
		cargar_mina(fichero, juego.mina);
		tTecla aux;
		dibujar(juego);
		aux = leerTecla();
		while (aux != SALIR && juego.estado != EXITO && juego.estado != FRACASO)
		{
			system("cls");
			// Primero comprueba si está encerrado o no
			if (encerrado(juego) == false)
			{
				juego.estado = FRACASO;
				dibujar(juego);
			}
			else
			{
				leerMovimiento(juego, aux);
				dibujar(juego);
			}
			// Luego mira si ha muerto o si se ha quedado encerrado (FRACASO)
			if (juego.estado == FRACASO)
			{
				cout << "Ha muerto. Pulse esc para continuar para continuar" << endl;
			}
			aux = leerTecla();
		}
		is_valido = true;
	}
	else
	{
		is_valido = false;
	}
	fichero.close();
	return is_valido;
}

// Programa principal :

// Funcion que pregunta el usuario a que escala quiere jugar
int menuEscala(tJuego& juego)
{
	int op;
	cout << "1. Jugar partida a escala 1:1" << endl;
	cout << "2. Jugar partida a escala 1.3" << endl;
	cout << "0. Salir" << endl;
	cout << "Opcion: ";
	cin >> juego.mina.escala;
	while (juego.mina.escala < 0 || juego.mina.escala >= 3)
	{
		cout << endl << "Por favor, introduzca un valor valido: ";
		cin >> juego.mina.escala;
	}
	op = juego.mina.escala;
	return op;
	cout << endl;
}

// Funcion que pregunta al usuario como prefiere realizar los movimientos
int menuMetodoMov(tJuego& juego)
{
	int op;
	string nombreFichero;
	cout << "1. Introducir movimientos por teclado" << endl;
	cout << "2. Introducir movimientos por fichero" << endl;
	cout << "0. Salir" << endl;
	cout << "Opcion: ";
	cin >> juego.mina.metodoMov;
	while (juego.mina.metodoMov < 0 || juego.mina.metodoMov >= 3)
	{
		cout << endl << "Por favor, introduzca un valor valido: ";
		cin >> juego.mina.metodoMov;
	}
	if (juego.mina.metodoMov == 2)
	{
		cout << "Introduzca el nombre del fichero: ";
		cin >> nombreFichero;
	}
	op = juego.mina.metodoMov;
	return op;
}

// Menu que pregunta una vez has completado algun nivel
int menuNivel()
{
	int num;
	cout << "1. Jugar siguiente nivel" << endl;
	cout << "0. Salir" << endl;
	cout << "Opcion: ";
	cin >> num;
	while (num != 0 && num != 1)
	{
		cout << endl << "Por favor, introduzca un valor valido: ";
		cin >> num;
	}
	return num;
}

// Menu que aparece una vez el minero ha muerto o se ha quedado atrapado
int menuMuerte()
{
	int num;
	cout << "Ha perdido, que desea hacer: " << endl;
	cout << "1. Reintentar nivel" << endl;
	cout << "0. Salir" << endl;
	cout << "Opcion: ";
	cin >> num;
	while (num != 0 && num != 1)
	{
		cout << endl << "Por favor, introduzca un valor valido: ";
		cin >> num;
	}
	return num;
}