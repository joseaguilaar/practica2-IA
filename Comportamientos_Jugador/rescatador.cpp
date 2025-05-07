#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"

//POSIBLES MEJORAS:
//sobre el cono de vision, elegir la casilla que lleva a un camino menos visitado(fijandome en las casillas mas
// alante del cono de vision)

#include <iostream>
#include <algorithm>
#include <list>
#include <queue>

using namespace std;

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0(sensores);
		break;
	case 1:
		accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		accion = ComportamientoRescatadorNivel_3 (sensores);
		break;
	case 4:
		// accion = ComportamientoRescatadorNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoRescatador::interact(Action accion, int valor)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//--------------------------------NIVELES 0,1-----------------------------------
//
////////////////////////////////////////////////////////////////////////////////


/************************************************************************/
// Devuelve la casilla mas interesante de entre las 8 que tiene alrededor, en orden
// de las que cueste menos a mas giros llegar. Y en orden de importancia de casillas:
// objetivo, zapatillas(si no las tiene) y camino
int VeoCasillaInteresanteR_Nivel0( bool zap, vector<int> v_l, vector<unsigned char> s_l )
{	
	//buscamos objetivo
	for( int i=0; i<8; i++ ){
		if( s_l[i] == 'X' ){
			return i;
		}
	}
	//buscamos zapatillas
	for( int i=0; i<8 && !zap ; i++ ){
		if( s_l[i] == 'D' ){
			return i;
		}
	}

	vector<pair<pair<char,int>, int>> prioridad({ {{s_l[1],1},v_l[1]}, {{s_l[0],0},v_l[0]}, {{s_l[2],2},v_l[2]}, 
												  {{s_l[7],7},v_l[7]}, {{s_l[3],3},v_l[3]}, {{s_l[6],6},v_l[6]}, 
												  {{s_l[4],4},v_l[4]}, {{s_l[5],5},v_l[5]}  
												});

	//ordenamos en funcion del valor entero, pero manteniendose en la misma posicion
	//los enteros que sean iguales (por defecto: {1,0,2,7,3,6,4,5})
	stable_sort(prioridad.begin(), prioridad.end(), 
	[](const std::pair<pair<char,int>, int>& a, const std::pair<pair<char,int>, int>& b) { return a.second < b.second; });

	if ( prioridad[0].first.first =='C' ) return prioridad[0].first.second;
	else if ( prioridad[1].first.first=='C' ) return prioridad[1].first.second;
	else if ( prioridad[2].first.first=='C' ) return prioridad[2].first.second;
	else if ( prioridad[3].first.first=='C' ) return prioridad[3].first.second;
	else if ( prioridad[4].first.first=='C' ) return prioridad[4].first.second;
	else if ( prioridad[5].first.first=='C' ) return prioridad[5].first.second;
	else if ( prioridad[6].first.first=='C' ) return prioridad[6].first.second;
	else if ( prioridad[7].first.first=='C' ) return prioridad[7].first.second;
	else return -1;
}

/************************************************************************/
//en superficie local(s_l) se almacenara el tipo de casilla o 'P' si no es viable
void ViableR (bool zap, vector<int> &v_l, vector<unsigned char> &s_l, vector<unsigned char> &c_l, int cota_orig, vector<unsigned char> &e_l)
{
	for( int i=0; i<8; i++ ){

		if(v_l[i] == numeric_limits<int>::max()) 
			s_l[i] = 'P';
		else if( ! ((abs(c_l[i] - cota_orig)<=1 || (zap && abs(c_l[i] - cota_orig)<=2))  && e_l[i]!='a') )
			s_l[i] = 'P'; //modificamos a precipicio
	}
}

/************************************************************************/
// Actualiza las matrices MapaResultado, MapaCotas y Visitadas
void SituarSensorEnMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, vector<vector<unsigned char>> &e, vector<vector<int>> &v, 
						 Sensores sensores)
{
	m[sensores.posF][sensores.posC] = sensores.superficie[0];
	a[sensores.posF][sensores.posC] = sensores.cota[0];
	e[sensores.posF][sensores.posC] = sensores.agentes[0];
	v[sensores.posF][sensores.posC] = v[sensores.posF][sensores.posC]+1;
	
	int pos = 1;
	switch (sensores.rumbo)
	{
	case norte:

		for( int i=1; i<=3; i++ ){
			for( int j=-i; j<=i; j++ ){
				m[sensores.posF - i][sensores.posC + j] = sensores.superficie[pos];
				a[sensores.posF - i][sensores.posC + j] = sensores.cota[pos];
				e[sensores.posF - i][sensores.posC + j] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF - i][sensores.posC + j];
				pos++;
			}
		}
		break;

	case noreste:

		for( int i=1; i<=3; i++ ){
			
			for( int j=0; j<=i; j++ ){
				m[sensores.posF - i][sensores.posC + j] = sensores.superficie[pos];
				a[sensores.posF - i][sensores.posC + j] = sensores.cota[pos];
				e[sensores.posF - i][sensores.posC + j] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF - i][sensores.posC + j];
				pos++;
			}

			for( int k = i-1; k>=0; k-- ){
				m[sensores.posF - k][sensores.posC + i] = sensores.superficie[pos];
				a[sensores.posF - k][sensores.posC + i] = sensores.cota[pos];
				e[sensores.posF - k][sensores.posC + i] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF - k][sensores.posC + i];
				pos++;
			}
		}
		break;

	case este:

		for( int i=1; i<=3; i++ ){
			for( int j=-i; j<=i; j++ ){
				m[sensores.posF + j][sensores.posC + i] = sensores.superficie[pos];
				a[sensores.posF + j][sensores.posC + i] = sensores.cota[pos];
				e[sensores.posF + j][sensores.posC + i] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF + j][sensores.posC + i];
				pos++;
			}
		}
		break;

	case sureste:

		for (int i = 1; i <= 3; ++i) {

			for (int j = 0; j <= i; ++j) {
				m[sensores.posF + j][sensores.posC + i] = sensores.superficie[pos];
				a[sensores.posF + j][sensores.posC + i] = sensores.cota[pos];
				e[sensores.posF + j][sensores.posC + i] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF + j][sensores.posC + i];
 				pos++;
			}

			for (int k = i - 1; k >= 0; --k) {
				m[sensores.posF + i][sensores.posC + k] = sensores.superficie[pos];
				a[sensores.posF + i][sensores.posC + k] = sensores.cota[pos];
				e[sensores.posF + i][sensores.posC + k] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF + i][sensores.posC + k];
				pos++;
			}
		}
		break;
	
	case sur:

		for (int i = 1; i <= 3; ++i) {
			for (int j = -i; j <= i; ++j) {
				m[sensores.posF + i][sensores.posC - j] = sensores.superficie[pos];
				a[sensores.posF + i][sensores.posC - j] = sensores.cota[pos];
				e[sensores.posF + i][sensores.posC - j] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF + i][sensores.posC - j];
				pos++;
			}
		}
		break;
	
	case suroeste:

		for (int i = 1; i <= 3; ++i) {

			for (int j = 0; j <= i; ++j) {
				m[sensores.posF + i][sensores.posC - j] = sensores.superficie[pos];
				a[sensores.posF + i][sensores.posC - j] = sensores.cota[pos];
				e[sensores.posF + i][sensores.posC - j] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF + i][sensores.posC - j];
				pos++;
			}

			for (int k = i - 1; k >= 0; --k) {
				m[sensores.posF + k][sensores.posC - i]    = sensores.superficie[pos];
				a[sensores.posF + k][sensores.posC - i]    = sensores.cota[pos];
				e[sensores.posF + k][sensores.posC - i]    = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF + k][sensores.posC - i];
				pos++;
			}
		}
		break;
	
	case oeste:

		for (int i = 1; i <= 3; ++i) {
			for (int j = -i; j <= i; ++j) {
				m[sensores.posF - j][sensores.posC - i] = sensores.superficie[pos];
				a[sensores.posF - j][sensores.posC - i] = sensores.cota[pos];
				e[sensores.posF - j][sensores.posC - i] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF - j][sensores.posC - i];
				pos++;
			}
		}
		break;
	
	case noroeste:

		for (int i = 1; i <= 3; ++i) {

			for (int j = 0; j <= i; ++j) {
				m[sensores.posF - j][sensores.posC - i] = sensores.superficie[pos];
				a[sensores.posF - j][sensores.posC - i] = sensores.cota[pos];
				e[sensores.posF - j][sensores.posC - i] = sensores.agentes[pos];
				//v_l[pos] = v[sensores.posF - j][sensores.posC - i];
				pos++;
			}
			for (int k = i - 1; k >= 0; --k) {
				m[sensores.posF - i][sensores.posC - k] = sensores.superficie[pos];
				a[sensores.posF - i][sensores.posC - k] = sensores.cota[pos];
				e[sensores.posF - i][sensores.posC - k] = sensores.agentes[pos];
				
				//v_l[pos] = v[sensores.posF - i][sensores.posC - k];
				pos++;
			}
		}
		break;
	}
}

/************************************************************************/
//inicializamos cada casilla de visitada_local:
// NOTA: si la casilla no esta descubierta en mapaResultado la inicializamos a infinito
void InicializarVisitadaLocal( vector<vector<unsigned char>> &m, vector<vector<int>> &v, vector<int> &v_l, Sensores sensores)
{	
	int f = sensores.posF, c = sensores.posC;
	switch (sensores.rumbo) {
	  case norte:
		v_l[0] = (m[f-1][c-1] == '?') ? numeric_limits<int>::max() : v[f-1][c-1]; // NW
		v_l[1] = (m[f-1][c]   == '?') ? numeric_limits<int>::max() : v[f-1][c];   // N
		v_l[2] = (m[f-1][c+1] == '?') ? numeric_limits<int>::max() : v[f-1][c+1]; // NE
		v_l[3] = (m[f][c+1]   == '?') ? numeric_limits<int>::max() : v[f][c+1];   // E
		v_l[4] = (m[f+1][c+1] == '?') ? numeric_limits<int>::max() : v[f+1][c+1]; // SE
		v_l[5] = (m[f+1][c]   == '?') ? numeric_limits<int>::max() : v[f+1][c];   // S
		v_l[6] = (m[f+1][c-1] == '?') ? numeric_limits<int>::max() : v[f+1][c-1]; // SW
		v_l[7] = (m[f][c-1]   == '?') ? numeric_limits<int>::max() : v[f][c-1];   // W

		break;
	
	  case noreste:
		v_l[0] = (m[f-1][c]   == '?') ? numeric_limits<int>::max() : v[f-1][c];   // N
		v_l[1] = (m[f-1][c+1] == '?') ? numeric_limits<int>::max() : v[f-1][c+1]; // NE
		v_l[2] = (m[f][c+1]   == '?') ? numeric_limits<int>::max() : v[f][c+1];   // E
		v_l[3] = (m[f+1][c+1] == '?') ? numeric_limits<int>::max() : v[f+1][c+1]; // SE
		v_l[4] = (m[f+1][c]   == '?') ? numeric_limits<int>::max() : v[f+1][c];   // S
		v_l[5] = (m[f+1][c-1] == '?') ? numeric_limits<int>::max() : v[f+1][c-1]; // SW
		v_l[6] = (m[f][c-1]   == '?') ? numeric_limits<int>::max() : v[f][c-1];   // W
		v_l[7] = (m[f-1][c-1] == '?') ? numeric_limits<int>::max() : v[f-1][c-1]; // NW
		break;
	
	  case este:
		v_l[0] = (m[f-1][c+1] == '?') ? numeric_limits<int>::max() : v[f-1][c+1]; // NE
		v_l[1] = (m[f][c+1]   == '?') ? numeric_limits<int>::max() : v[f][c+1];   // E
		v_l[2] = (m[f+1][c+1] == '?') ? numeric_limits<int>::max() : v[f+1][c+1]; // SE
		v_l[3] = (m[f+1][c]   == '?') ? numeric_limits<int>::max() : v[f+1][c];   // S
		v_l[4] = (m[f+1][c-1] == '?') ? numeric_limits<int>::max() : v[f+1][c-1]; // SW
		v_l[5] = (m[f][c-1]   == '?') ? numeric_limits<int>::max() : v[f][c-1];   // W
		v_l[6] = (m[f-1][c-1] == '?') ? numeric_limits<int>::max() : v[f-1][c-1]; // NW
		v_l[7] = (m[f-1][c]   == '?') ? numeric_limits<int>::max() : v[f-1][c];   // N
		break;
	
	  case sureste:
		v_l[0] = (m[f][c+1]   == '?') ? numeric_limits<int>::max() : v[f][c+1];   // E
		v_l[1] = (m[f+1][c+1] == '?') ? numeric_limits<int>::max() : v[f+1][c+1]; // SE
		v_l[2] = (m[f+1][c]   == '?') ? numeric_limits<int>::max() : v[f+1][c];   // S
		v_l[3] = (m[f+1][c-1] == '?') ? numeric_limits<int>::max() : v[f+1][c-1]; // SW
		v_l[4] = (m[f][c-1]   == '?') ? numeric_limits<int>::max() : v[f][c-1];   // W
		v_l[5] = (m[f-1][c-1] == '?') ? numeric_limits<int>::max() : v[f-1][c-1]; // NW
		v_l[6] = (m[f-1][c]   == '?') ? numeric_limits<int>::max() : v[f-1][c];   // N
		v_l[7] = (m[f-1][c+1] == '?') ? numeric_limits<int>::max() : v[f-1][c+1]; // NE
		break;
	
	  case sur:
		v_l[0] = (m[f+1][c+1] == '?') ? numeric_limits<int>::max() : v[f+1][c+1]; // SE
		v_l[1] = (m[f+1][c]   == '?') ? numeric_limits<int>::max() : v[f+1][c];   // S
		v_l[2] = (m[f+1][c-1] == '?') ? numeric_limits<int>::max() : v[f+1][c-1]; // SW
		v_l[3] = (m[f][c-1]   == '?') ? numeric_limits<int>::max() : v[f][c-1];   // W
		v_l[4] = (m[f-1][c-1] == '?') ? numeric_limits<int>::max() : v[f-1][c-1]; // NW
		v_l[5] = (m[f-1][c]   == '?') ? numeric_limits<int>::max() : v[f-1][c];   // N
		v_l[6] = (m[f-1][c+1] == '?') ? numeric_limits<int>::max() : v[f-1][c+1]; // NE
		v_l[7] = (m[f][c+1]   == '?') ? numeric_limits<int>::max() : v[f][c+1];   // E
		break;
	
	  case suroeste:
		v_l[0] = (m[f+1][c]   == '?') ? numeric_limits<int>::max() : v[f+1][c];   // S
		v_l[1] = (m[f+1][c-1] == '?') ? numeric_limits<int>::max() : v[f+1][c-1]; // SW
		v_l[2] = (m[f][c-1]   == '?') ? numeric_limits<int>::max() : v[f][c-1];   // W
		v_l[3] = (m[f-1][c-1] == '?') ? numeric_limits<int>::max() : v[f-1][c-1]; // NW
		v_l[4] = (m[f-1][c]   == '?') ? numeric_limits<int>::max() : v[f-1][c];   // N
		v_l[5] = (m[f-1][c+1] == '?') ? numeric_limits<int>::max() : v[f-1][c+1]; // NE
		v_l[6] = (m[f][c+1]   == '?') ? numeric_limits<int>::max() : v[f][c+1];   // E
		v_l[7] = (m[f+1][c+1] == '?') ? numeric_limits<int>::max() : v[f+1][c+1]; // SE
		break;
	
	  case oeste:
		v_l[0] = (m[f+1][c-1] == '?') ? numeric_limits<int>::max() : v[f+1][c-1]; // SW
		v_l[1] = (m[f][c-1]   == '?') ? numeric_limits<int>::max() : v[f][c-1];   // W
		v_l[2] = (m[f-1][c-1] == '?') ? numeric_limits<int>::max() : v[f-1][c-1]; // NW
		v_l[3] = (m[f-1][c]   == '?') ? numeric_limits<int>::max() : v[f-1][c];   // N
		v_l[4] = (m[f-1][c+1] == '?') ? numeric_limits<int>::max() : v[f-1][c+1]; // NE
		v_l[5] = (m[f][c+1]   == '?') ? numeric_limits<int>::max() : v[f][c+1];   // E
		v_l[6] = (m[f+1][c+1] == '?') ? numeric_limits<int>::max() : v[f+1][c+1]; // SE
		v_l[7] = (m[f+1][c]   == '?') ? numeric_limits<int>::max() : v[f+1][c];   // S
		break;
	
	  case noroeste:
		v_l[0] = (m[f][c-1]   == '?') ? numeric_limits<int>::max() : v[f][c-1];   // W
		v_l[1] = (m[f-1][c-1] == '?') ? numeric_limits<int>::max() : v[f-1][c-1]; // NW
		v_l[2] = (m[f-1][c]   == '?') ? numeric_limits<int>::max() : v[f-1][c];   // N
		v_l[3] = (m[f-1][c+1] == '?') ? numeric_limits<int>::max() : v[f-1][c+1]; // NE
		v_l[4] = (m[f][c+1]   == '?') ? numeric_limits<int>::max() : v[f][c+1];   // E
		v_l[5] = (m[f+1][c+1] == '?') ? numeric_limits<int>::max() : v[f+1][c+1]; // SE
		v_l[6] = (m[f+1][c]   == '?') ? numeric_limits<int>::max() : v[f+1][c];   // S
		v_l[7] = (m[f+1][c-1] == '?') ? numeric_limits<int>::max() : v[f+1][c-1]; // SW
		break;
	}
}

/************************************************************************/
//inicializamos cada casilla de superficie_local:
void InicializarSuperficieLocal( vector<vector<unsigned char>> &m, vector<unsigned char> &s_l, Sensores sensores)
{
	int f = sensores.posF, c = sensores.posC;
	switch (sensores.rumbo) {
		case norte:
		  s_l[0] = m[f-1][c-1]; // NW
		  s_l[1] = m[f-1][c];   // N
		  s_l[2] = m[f-1][c+1]; // NE
		  s_l[3] = m[f][c+1];   // E
		  s_l[4] = m[f+1][c+1]; // SE
		  s_l[5] = m[f+1][c];   // S
		  s_l[6] = m[f+1][c-1]; // SW
		  s_l[7] = m[f][c-1];   // W
		  break;
	  
		case noreste:
		  s_l[0] = m[f-1][c];   // N
		  s_l[1] = m[f-1][c+1]; // NE
		  s_l[2] = m[f][c+1];   // E
		  s_l[3] = m[f+1][c+1]; // SE
		  s_l[4] = m[f+1][c];   // S
		  s_l[5] = m[f+1][c-1]; // SW
		  s_l[6] = m[f][c-1];   // W
		  s_l[7] = m[f-1][c-1]; // NW
		  break;
	  
		case este:
		  s_l[0] = m[f-1][c+1]; // NE
		  s_l[1] = m[f][c+1];   // E
		  s_l[2] = m[f+1][c+1]; // SE
		  s_l[3] = m[f+1][c];   // S
		  s_l[4] = m[f+1][c-1]; // SW
		  s_l[5] = m[f][c-1];   // W
		  s_l[6] = m[f-1][c-1]; // NW
		  s_l[7] = m[f-1][c];   // N
		  break;
	  
		case sureste:
		  s_l[0] = m[f][c+1];   // E
		  s_l[1] = m[f+1][c+1]; // SE
		  s_l[2] = m[f+1][c];   // S
		  s_l[3] = m[f+1][c-1]; // SW
		  s_l[4] = m[f][c-1];   // W
		  s_l[5] = m[f-1][c-1]; // NW
		  s_l[6] = m[f-1][c];   // N
		  s_l[7] = m[f-1][c+1]; // NE
		  break;
	  
		case sur:
		  s_l[0] = m[f+1][c+1]; // SE
		  s_l[1] = m[f+1][c];   // S
		  s_l[2] = m[f+1][c-1]; // SW
		  s_l[3] = m[f][c-1];   // W
		  s_l[4] = m[f-1][c-1]; // NW
		  s_l[5] = m[f-1][c];   // N
		  s_l[6] = m[f-1][c+1]; // NE
		  s_l[7] = m[f][c+1];   // E
		  break;
	  
		case suroeste:
		  s_l[0] = m[f+1][c];   // S
		  s_l[1] = m[f+1][c-1]; // SW
		  s_l[2] = m[f][c-1];   // W
		  s_l[3] = m[f-1][c-1]; // NW
		  s_l[4] = m[f-1][c];   // N
		  s_l[5] = m[f-1][c+1]; // NE
		  s_l[6] = m[f][c+1];   // E
		  s_l[7] = m[f+1][c+1]; // SE
		  break;
	  
		case oeste:
		  s_l[0] = m[f+1][c-1]; // SW
		  s_l[1] = m[f][c-1];   // W
		  s_l[2] = m[f-1][c-1]; // NW
		  s_l[3] = m[f-1][c];   // N
		  s_l[4] = m[f-1][c+1]; // NE
		  s_l[5] = m[f][c+1];   // E
		  s_l[6] = m[f+1][c+1]; // SE
		  s_l[7] = m[f+1][c];   // S
		  break;
	  
		case noroeste:
		  s_l[0] = m[f][c-1];   // W
		  s_l[1] = m[f-1][c-1]; // NW
		  s_l[2] = m[f-1][c];   // N
		  s_l[3] = m[f-1][c+1]; // NE
		  s_l[4] = m[f][c+1];   // E
		  s_l[5] = m[f+1][c+1]; // SE
		  s_l[6] = m[f+1][c];   // S
		  s_l[7] = m[f+1][c-1]; // SW
		  break;
	  }

}

/************************************************************************/
//inicializamos cada casilla de visitada_local:
void InicializarCotasLocal( vector<vector<unsigned char>> &a, vector<unsigned char> &c_l, Sensores sensores)
{
	int f = sensores.posF, c = sensores.posC;
	switch (sensores.rumbo) {
		case norte:
		  c_l[0] = a[f-1][c-1]; // NW
		  c_l[1] = a[f-1][c];   // N
		  c_l[2] = a[f-1][c+1]; // NE
		  c_l[3] = a[f][c+1];   // E
		  c_l[4] = a[f+1][c+1]; // SE
		  c_l[5] = a[f+1][c];   // S
		  c_l[6] = a[f+1][c-1]; // SW
		  c_l[7] = a[f][c-1];   // W
		  break;
	  
		case noreste:
		  c_l[0] = a[f-1][c];   // N
		  c_l[1] = a[f-1][c+1]; // NE
		  c_l[2] = a[f][c+1];   // E
		  c_l[3] = a[f+1][c+1]; // SE
		  c_l[4] = a[f+1][c];   // S
		  c_l[5] = a[f+1][c-1]; // SW
		  c_l[6] = a[f][c-1];   // W
		  c_l[7] = a[f-1][c-1]; // NW
		  break;
	  
		case este:
		  c_l[0] = a[f-1][c+1]; // NE
		  c_l[1] = a[f][c+1];   // E
		  c_l[2] = a[f+1][c+1]; // SE
		  c_l[3] = a[f+1][c];   // S
		  c_l[4] = a[f+1][c-1]; // SW
		  c_l[5] = a[f][c-1];   // W
		  c_l[6] = a[f-1][c-1]; // NW
		  c_l[7] = a[f-1][c];   // N
		  break;
	  
		case sureste:
		  c_l[0] = a[f][c+1];   // E
		  c_l[1] = a[f+1][c+1]; // SE
		  c_l[2] = a[f+1][c];   // S
		  c_l[3] = a[f+1][c-1]; // SW
		  c_l[4] = a[f][c-1];   // W
		  c_l[5] = a[f-1][c-1]; // NW
		  c_l[6] = a[f-1][c];   // N
		  c_l[7] = a[f-1][c+1]; // NE
		  break;
	  
		case sur:
		  c_l[0] = a[f+1][c+1]; // SE
		  c_l[1] = a[f+1][c];   // S
		  c_l[2] = a[f+1][c-1]; // SW
		  c_l[3] = a[f][c-1];   // W
		  c_l[4] = a[f-1][c-1]; // NW
		  c_l[5] = a[f-1][c];   // N
		  c_l[6] = a[f-1][c+1]; // NE
		  c_l[7] = a[f][c+1];   // E
		  break;
	  
		case suroeste:
		  c_l[0] = a[f+1][c];   // S
		  c_l[1] = a[f+1][c-1]; // SW
		  c_l[2] = a[f][c-1];   // W
		  c_l[3] = a[f-1][c-1]; // NW
		  c_l[4] = a[f-1][c];   // N
		  c_l[5] = a[f-1][c+1]; // NE
		  c_l[6] = a[f][c+1];   // E
		  c_l[7] = a[f+1][c+1]; // SE
		  break;
	  
		case oeste:
		  c_l[0] = a[f+1][c-1]; // SW
		  c_l[1] = a[f][c-1];   // W
		  c_l[2] = a[f-1][c-1]; // NW
		  c_l[3] = a[f-1][c];   // N
		  c_l[4] = a[f-1][c+1]; // NE
		  c_l[5] = a[f][c+1];   // E
		  c_l[6] = a[f+1][c+1]; // SE
		  c_l[7] = a[f+1][c];   // S
		  break;
	  
		case noroeste:
		  c_l[0] = a[f][c-1];   // W
		  c_l[1] = a[f-1][c-1]; // NW
		  c_l[2] = a[f-1][c];   // N
		  c_l[3] = a[f-1][c+1]; // NE
		  c_l[4] = a[f][c+1];   // E
		  c_l[5] = a[f+1][c+1]; // SE
		  c_l[6] = a[f+1][c];   // S
		  c_l[7] = a[f+1][c-1]; // SW
		  break;
	  }

}

/************************************************************************/
//inicializamos cada casilla de superficie_local:
void InicializarEntidadesLocal( vector<vector<unsigned char>> &e, vector<unsigned char> &e_l, Sensores sensores)
{
	int f = sensores.posF, c = sensores.posC;
	switch (sensores.rumbo) {
		case norte:
		  e_l[0] = e[f-1][c-1]; // NW
		  e_l[1] = e[f-1][c];   // N
		  e_l[2] = e[f-1][c+1]; // NE
		  e_l[3] = e[f][c+1];   // E
		  e_l[4] = e[f+1][c+1]; // SE
		  e_l[5] = e[f+1][c];   // S
		  e_l[6] = e[f+1][c-1]; // SW
		  e_l[7] = e[f][c-1];   // W
		  break;
	  
		case noreste:
		  e_l[0] = e[f-1][c];   // N
		  e_l[1] = e[f-1][c+1]; // NE
		  e_l[2] = e[f][c+1];   // E
		  e_l[3] = e[f+1][c+1]; // SE
		  e_l[4] = e[f+1][c];   // S
		  e_l[5] = e[f+1][c-1]; // SW
		  e_l[6] = e[f][c-1];   // W
		  e_l[7] = e[f-1][c-1]; // NW
		  break;
	  
		case este:
		  e_l[0] = e[f-1][c+1]; // NE
		  e_l[1] = e[f][c+1];   // E
		  e_l[2] = e[f+1][c+1]; // SE
		  e_l[3] = e[f+1][c];   // S
		  e_l[4] = e[f+1][c-1]; // SW
		  e_l[5] = e[f][c-1];   // W
		  e_l[6] = e[f-1][c-1]; // NW
		  e_l[7] = e[f-1][c];   // N
		  break;
	  
		case sureste:
		  e_l[0] = e[f][c+1];   // E
		  e_l[1] = e[f+1][c+1]; // SE
		  e_l[2] = e[f+1][c];   // S
		  e_l[3] = e[f+1][c-1]; // SW
		  e_l[4] = e[f][c-1];   // W
		  e_l[5] = e[f-1][c-1]; // NW
		  e_l[6] = e[f-1][c];   // N
		  e_l[7] = e[f-1][c+1]; // NE
		  break;
	  
		case sur:
		  e_l[0] = e[f+1][c+1]; // SE
		  e_l[1] = e[f+1][c];   // S
		  e_l[2] = e[f+1][c-1]; // SW
		  e_l[3] = e[f][c-1];   // W
		  e_l[4] = e[f-1][c-1]; // NW
		  e_l[5] = e[f-1][c];   // N
		  e_l[6] = e[f-1][c+1]; // NE
		  e_l[7] = e[f][c+1];   // E
		  break;
	  
		case suroeste:
		  e_l[0] = e[f+1][c];   // S
		  e_l[1] = e[f+1][c-1]; // SW
		  e_l[2] = e[f][c-1];   // W
		  e_l[3] = e[f-1][c-1]; // NW
		  e_l[4] = e[f-1][c];   // N
		  e_l[5] = e[f-1][c+1]; // NE
		  e_l[6] = e[f][c+1];   // E
		  e_l[7] = e[f+1][c+1]; // SE
		  break;
	  
		case oeste:
		  e_l[0] = e[f+1][c-1]; // SW
		  e_l[1] = e[f][c-1];   // W
		  e_l[2] = e[f-1][c-1]; // NW
		  e_l[3] = e[f-1][c];   // N
		  e_l[4] = e[f-1][c+1]; // NE
		  e_l[5] = e[f][c+1];   // E
		  e_l[6] = e[f+1][c+1]; // SE
		  e_l[7] = e[f+1][c];   // S
		  break;
	  
		case noroeste:
		  e_l[0] = e[f][c-1];   // W
		  e_l[1] = e[f-1][c-1]; // NW
		  e_l[2] = e[f-1][c];   // N
		  e_l[3] = e[f-1][c+1]; // NE
		  e_l[4] = e[f][c+1];   // E
		  e_l[5] = e[f+1][c+1]; // SE
		  e_l[6] = e[f+1][c];   // S
		  e_l[7] = e[f+1][c-1]; // SW
		  break;
	  }

}


/************************************************************************/
Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	Action accion;
	
	//Fase 1, actualizar variables de estadoREstadoR
	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaR( mapaResultado, mapaCotas, mapaEntidades, mapaVisitadas, sensores );

	InicializarVisitadaLocal( mapaResultado, mapaVisitadas, visitada_local, sensores );
	InicializarSuperficieLocal( mapaResultado, superficie_local, sensores );
	InicializarCotasLocal( mapaCotas, cotas_local, sensores );
	InicializarEntidadesLocal( mapaEntidades, entidades_local, sensores );
	
	//Fase 2, determinar siguiente accion a realizar
	if( sensores.superficie[0] == 'X' ) {
		accion = IDLE;
	}
	else if( giroSR != 0 ){ 
		accion = TURN_SR;
		giroSR--;
	}
	else if( giro135izq != 0){
		accion = TURN_L;
		giro135izq--;
		giroSR = 1;
	}
	else if( giro180 != 0 ){
		accion = TURN_L;
		giro180--;
	}
	else{

		ViableR( tiene_zapatillas, visitada_local, superficie_local, cotas_local, sensores.cota[0], entidades_local );

		//pos -> [0,7]
		int pos = VeoCasillaInteresanteR_Nivel0( tiene_zapatillas, visitada_local, superficie_local );

		switch(pos)
		{	
			case 1:
				accion = WALK;      //WALK
				break;
			case 0:			
				giroSR = 1;
				accion = TURN_L;    //TURN_SL
				break;
			case 2:		
				accion = TURN_SR;   //TURN_SR
				break;
			case 7:                 //TURN_L
				accion = TURN_L;
				break;
			case 3:					//TURN_R
				giroSR = 1;
				accion = TURN_SR;
				break;
			case 6: 			    //2*TURN_L + TURN_SR
				accion = TURN_L;
				giro135izq = 1;
				break;
			case 4:                 //3*TURN_SR
				giroSR = 2;
				accion = TURN_SR;
				break;
			case 5:					//2*TURN_L
				giro180 = 1;
				accion = TURN_L;
				break;
			case -1:                //si no hay ninguna casilla interesante gira hacia la izquierda para seguir buscando
				accion = TURN_L;
				break;
		}
	}

	last_action = accion;
	return (accion);
}


/************************************************************************/
// Devuelve la casilla mas interesante de entre las 3 que tiene delante, en orden
int VeoCasillaInteresanteR_Nivel1( bool zap, vector<int> v_l, vector<unsigned char> s_l, Sensores sensores)
{	
	int permite_agua = false;

	for( int i=0; i<16 && !zap; i++ ){
		if( sensores.superficie[i] == 'D'){
			permite_agua = true;
			break;
		}
	}

	int rodeados = 0;
	//buscamos zapatillas
	for( int i=0; i<8 && !zap ; i++ ){
		if( s_l[i] == 'D' ){
			return i;
		}
	}
	for( int i=0; i<8 ; i++ ){
		if( s_l[i] != 'C' && s_l[i] != 'S' && s_l[i] != 'X' && s_l[i] != 'D' ){
			rodeados ++;
		}
	}

	//cout <<rodeados<<"|";
	if( rodeados == 8 ){
		permite_agua = true;
	}

	

	//mejor resultado:
	vector<pair<pair<char,int>, int>> prioridad({ {{s_l[1],1},v_l[1]-1}, {{s_l[2],2},v_l[2]+1}, {{s_l[0],0},v_l[0]+1}, 
												  {{s_l[7],7},v_l[7]+2}, {{s_l[3],3},v_l[3]+2}, {{s_l[6],6},v_l[6]+3}, 
												  {{s_l[4],4},v_l[4]+3}, {{s_l[5],5},v_l[5]+5}  
												});


	//ordenamos en funcion del valor entero, pero manteniendose en la misma posicion
	//los enteros que sean iguales (por defecto: {1,0,2,7,3,6,4,5})
	stable_sort(prioridad.begin(), prioridad.end(), 
	[](const std::pair<pair<char,int>, int>& a, const std::pair<pair<char,int>, int>& b) { return a.second < b.second; });

	if ( prioridad[0].first.first =='C'|| prioridad[0].first.first=='S' || prioridad[0].first.first=='X' || prioridad[0].first.first=='D' || (permite_agua && prioridad[0].first.first=='A') ) return prioridad[0].first.second;
	else if (prioridad[1].first.first=='C' || prioridad[1].first.first=='S' || prioridad[1].first.first=='X' || prioridad[1].first.first=='D' || (permite_agua && prioridad[1].first.first=='A') ) return prioridad[1].first.second;
	else if (prioridad[2].first.first=='C' || prioridad[2].first.first=='S' || prioridad[2].first.first=='X' || prioridad[2].first.first=='D' || (permite_agua && prioridad[2].first.first=='A') ) return prioridad[2].first.second;
	else if (prioridad[3].first.first=='C' || prioridad[3].first.first=='S' || prioridad[3].first.first=='X' || prioridad[3].first.first=='D' || (permite_agua && prioridad[3].first.first=='A') ) return prioridad[3].first.second;
	else if (prioridad[4].first.first=='C' || prioridad[4].first.first=='S' || prioridad[4].first.first=='X' || prioridad[4].first.first=='D' || (permite_agua && prioridad[4].first.first=='A') ) return prioridad[4].first.second;
	else if (prioridad[5].first.first=='C' || prioridad[5].first.first=='S' || prioridad[5].first.first=='X' || prioridad[5].first.first=='D' || (permite_agua && prioridad[5].first.first=='A') ) return prioridad[5].first.second;
	else if (prioridad[6].first.first=='C' || prioridad[6].first.first=='S' || prioridad[6].first.first=='X' || prioridad[6].first.first=='D' || (permite_agua && prioridad[6].first.first=='A') ) return prioridad[6].first.second;
	else if (prioridad[7].first.first=='C' || prioridad[7].first.first=='S' || prioridad[7].first.first=='X' || prioridad[7].first.first=='D' || (permite_agua && prioridad[7].first.first=='A') ) return prioridad[7].first.second;
	else return -1;	
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	Action accion;
	
	//Fase 1, actualizar variables de estadoREstadoR
	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaR( mapaResultado, mapaCotas, mapaEntidades, mapaVisitadas, sensores );

	InicializarVisitadaLocal( mapaResultado, mapaVisitadas, visitada_local, sensores );
	InicializarSuperficieLocal( mapaResultado, superficie_local, sensores );
	InicializarCotasLocal( mapaCotas, cotas_local, sensores );
	InicializarEntidadesLocal( mapaEntidades, entidades_local, sensores );
	
	//Fase 2, determinar siguiente accion a realizar
	if(sensores.energia <= 3){
		accion = IDLE;
	}
	else if( giroSR != 0 ){ 
		accion = TURN_SR;
		giroSR--;
	}
	else if( giro135izq != 0){
		accion = TURN_L;
		giro135izq--;
		giroSR = 1;
	}
	else if( giro180 != 0 ){
		accion = TURN_L;
		giro180--;
	}
	else{

		ViableR( tiene_zapatillas, visitada_local, superficie_local, cotas_local, sensores.cota[0], entidades_local );

		//pos -> [0,7]
		int pos = VeoCasillaInteresanteR_Nivel1( tiene_zapatillas, visitada_local, superficie_local, sensores );

		switch(pos)
		{	
			case 1:
				accion = WALK;      //WALK
				break;
			case 0:			
				giroSR = 1;
				accion = TURN_L;    //TURN_SL
				break;
			case 2:		
				accion = TURN_SR;   //TURN_SR
				break;
			case 7:                 //TURN_L
				accion = TURN_L;
				break;
			case 3:					//TURN_R
				giroSR = 1;
				accion = TURN_SR;
				break;
			case 6: 			    //2*TURN_L + TURN_SR
				accion = TURN_L;
				giro135izq = 1;
				break;
			case 4:                 //3*TURN_SR
				giroSR = 2;
				accion = TURN_SR;
				break;
			case 5:					//2*TURN_L
				giro180 = 1;
				accion = TURN_L;
				break;
			case -1:                //si no hay ninguna casilla interesante gira hacia la izquierda para seguir buscando
				accion = TURN_L;
				break;
		}
	}

	last_action = accion;
	return (accion);
}



////////////////////////////////////////////////////////////////////////////////
//
//-------------------------------NIVELES 2,3,4----------------------------------
//
////////////////////////////////////////////////////////////////////////////////

/************************************************************************/
void AnularMatrizR(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

/************************************************************************/
void ComportamientoRescatador::VisualizaPlan(const EstadoR &st,
                                             const list<Action> &plan)
{
    AnularMatrizR(mapaConPlan);
    EstadoR cst = st;

    auto it = plan.begin();
    while (it != plan.end())
    {
        switch (*it)
        {
            case RUN:
                switch (cst.brujula)
                {
                    case 0:  cst.f--;            break;
                    case 1:  cst.f--;  cst.c++;  break;
                    case 2:             cst.c++; break;
                    case 3:  cst.f++;  cst.c++;  break;
                    case 4:  cst.f++;            break;
                    case 5:  cst.f++;  cst.c--;  break;
                    case 6:             cst.c--; break;
                    case 7:  cst.f--;  cst.c--;  break;
                }
                mapaConPlan[cst.f][cst.c] = 3;
            
            case WALK:
                switch (cst.brujula)
                {
                    case 0:  cst.f--;            break;
                    case 1:  cst.f--;  cst.c++;  break;
                    case 2:             cst.c++; break;
                    case 3:  cst.f++;  cst.c++;  break;
                    case 4:  cst.f++;            break;
                    case 5:  cst.f++;  cst.c--;  break;
                    case 6:             cst.c--; break;
                    case 7:  cst.f--;  cst.c--;  break;
                }
                mapaConPlan[cst.f][cst.c] = 1;
                break;

            case TURN_SR:
                cst.brujula = (cst.brujula + 1) % 8;
                break;

            case TURN_L:
                cst.brujula = (cst.brujula + 6) % 8;
                break;
        }
        ++it;
    }
}

/************************************************************************/
void ComportamientoRescatador::PintaPlan(const list<Action> &plan, bool zap)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == WALK)
		{
			cout << "W ";
		}
		else if (*it == RUN)
		{
			cout << "R ";
		}
		else if (*it == TURN_SR)
		{
			cout << "r ";
		}
		else if (*it == TURN_L)
		{
			cout << "L ";
		}
		else if (*it == CALL_ON)
		{
			cout << "C ";
		}
		else if (*it == CALL_OFF)
		{
			cout << "c ";
		}
		else if (*it == IDLE)
		{
			cout << "I ";
		}
		else
		{
			cout << "-_ ";
		}
		it++;
	}
	cout << "( longitud " << plan.size();
	if (zap) cout << "[Z]";
	cout << ")\n";
}


/************************************************************************/
EstadoR NextCasillaR(const EstadoR &st, const Action &a){

	EstadoR siguiente = st;

	switch (st.brujula)
	{
	case norte:
		siguiente.f = st.f - 1;
		break;
	case noreste:
		siguiente.f = st.f - 1;
		siguiente.c = st.c + 1;
		break;
	case este:
		siguiente.c = st.c + 1;
		break;
	case sureste:
		siguiente.f = st.f + 1;
		siguiente.c = st.c + 1;
		break;
	case sur:
		siguiente.f = st.f + 1;
		break;
	case suroeste:
		siguiente.f = st.f + 1;
		siguiente.c = st.c - 1;
		break;
	case oeste:
		siguiente.c = st.c - 1;
		break;
	case noroeste:
		siguiente.f = st.f - 1;
		siguiente.c = st.c - 1;
		break;
	}
	return siguiente;
}


/************************************************************************/
bool CasillaAccesibleR(const Action &a, const EstadoR &st, const vector<vector<unsigned char>> &terreno,
					   const vector<vector<unsigned char>> &altura, bool intermedia_RUN)
{
	EstadoR next = NextCasillaR(st, a);
	bool check1 = false, check2 = false;

	check1 = terreno[next.f][next.c] != 'P' && terreno[next.f][next.c] != 'M' && terreno[next.f][next.c] != 'B';

	if( !intermedia_RUN ){

		check2 = (abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1) || 
				((abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 2) && st.zapatillas) ;
	}
	else{ //en casilla inter de RUN no puedes subir
		//check2 = (altura[next.f][next.c] - altura[st.f][st.c] <= 0);
		check2 = true;
	}

	return (check1 && check2);
}

/************************************************************************/
bool CasillaAccesibleRunR( const EstadoR &st, const vector<vector<unsigned char>> &terreno,
					   const vector<vector<unsigned char>> &altura)
{	
	if( CasillaAccesibleR(WALK, st, terreno, altura, true) ){
		EstadoR inter = NextCasillaR(st, WALK);

		if( terreno[inter.f][inter.c] == 'D' )
				inter.zapatillas = true;
				

		EstadoR next = NextCasillaR(inter, WALK);
		bool check1 = false, check2 = false;

		check1 = terreno[next.f][next.c] != 'P' && terreno[next.f][next.c] != 'M' && terreno[next.f][next.c] != 'B';
		check2 = (abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1) || 
				((abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 2) && st.zapatillas) ;

		return (check1 && check2);
	}
	return (false);
}

/************************************************************************/
EstadoR applyR(Action accion, const EstadoR &st, const vector<vector<unsigned char>> &terreno,
			   const vector<vector<unsigned char>> &altura)
{
	EstadoR next = st;

	switch(accion){
	case TURN_SR:
		next.brujula = (next.brujula+1)%8;
		break;

	case TURN_L:
		next.brujula = (next.brujula+6)%8;
		break;

	case WALK:				
	
		if (CasillaAccesibleR(accion,st,terreno,altura, false)){
			next = NextCasillaR(st, accion);
		}
		break;

	case RUN:
		
		if( CasillaAccesibleRunR(st, terreno, altura)){
				EstadoR intermedio = NextCasillaR(st, WALK);

				if( terreno[intermedio.f][intermedio.c] == 'D' )
					intermedio.zapatillas = true;

				next = NextCasillaR(intermedio, WALK);
		}
		break;
	}
	return (next);

}

/************************************************************************/
int costeEnergiaR( EstadoR &inicio, EstadoR &fin, Action &a, const vector<vector<unsigned char>> &terreno,
				  const vector<vector<unsigned char>> &altura)
{
	int coste = 0;
	unsigned char terr = terreno[inicio.f][inicio.c];
	int incr = 0;

	int difAlt =  altura[fin.f][fin.c] - altura[inicio.f][inicio.c];

	switch(a){
	
	case (TURN_SR):

		switch(terr){

		case('A'):
			coste = 16;
			break;
		case('T'):
			coste = 3;
			break;
		default:
			coste = 1;
			break;
		}
		break;
	
	case (TURN_L):

		switch(terr){
		case('A'):
			coste = 30;
			break;
		case('T'):
			coste = 5;
			break;
		default:
			coste = 1;
			break;
		}
		break;
	
	case (WALK):

		switch(terr){
		case('A'):
			
			if( difAlt<0 ) incr = -10;
			else if( difAlt>0 ) incr = 10;
			else incr = 0;

			coste = 100 + incr;
			break;
		
		case('T'):

			if( difAlt<0 ) incr = -5;
			else if( difAlt>0 ) incr = 5;
			else incr = 0;

			coste = 20 + incr;
			break;
		
		case('S'):
			if( difAlt<0 ) incr = -1;
			else if( difAlt>0 ) incr = 1;
			else incr = 0;

			coste = 2 + incr;
			break;
		
		default: 
			coste = 1;
			break;

		}
		break;

	case (RUN):

		switch(terr){
			case('A'):
			
			if( difAlt<0 ) incr = -15;
			else if( difAlt>0 ) incr = 15;
			else incr = 0;

			coste = 150 + incr;
			break;
		
		case('T'):

			if( difAlt<0 ) incr = -5;
			else if( difAlt>0 ) incr = 5;
			else incr = 0;

			coste = 35 + incr;
			break;
		
		case('S'):
			if( difAlt<0 ) incr = -2;
			else if( difAlt>0 ) incr = 2;
			else incr = 0;

			coste = 3 + incr;
			break;
		
		default: 
			coste = 1;
			break;

		}
		break;
	}

	return (coste);
}

/************************************************************************/
list<Action> DijkstraR( const EstadoR &inicio, const EstadoR &fin, vector<vector<unsigned char>> &terreno, vector<vector<unsigned char>> &altura )
{
	priority_queue<NodoR> pq;
    
	//matriz cuatridimensional con 8 rumbos por cada casilla(fil,col), con el coste minimo para llegar a esa fil,col,rumbo
	vector<vector<vector<vector<int>>>> coste_min( terreno.size(),vector<vector<vector<int>>>( terreno[0].size(), vector<vector<int>>(8, vector<int>(2, numeric_limits<int>::max()))));
	
	coste_min[inicio.f][inicio.c][inicio.brujula][inicio.zapatillas ? 1 : 0 ] = 0;
	pq.push({inicio, 0, {}});

	while( !pq.empty() ){

		auto it = pq.top();
		pq.pop();
		
		//si el nodo esta sobre el destino
		if( it.estado.f == fin.f && it.estado.c == fin.c ){
			return(it.secuencia);
		}
		
		if( terreno[it.estado.f][it.estado.c] == 'D' ){
			it.estado.zapatillas = true;
		}

		for( Action a: {WALK, RUN, TURN_L, TURN_SR}){
				
			EstadoR next = applyR(a, it.estado, terreno, altura);

			if( !(next == it.estado) ){ 							//si son iguales, no es casilla accesible
				
				int c = costeEnergiaR( it.estado, next, a, terreno, altura );
				
				if( c+it.coste < coste_min[next.f][next.c][next.brujula][next.zapatillas ? 1 : 0] ){ //si es la forma menos costosa de alcazar esa fil,col,rumbo (asi no tendremos repetidos)

					coste_min[next.f][next.c][next.brujula][next.zapatillas ? 1 : 0] = c+it.coste;   //actualizamos

					list<Action> sec = it.secuencia;
					sec.push_back(a);
					pq.push({next, c+it.coste, sec});                  //añadimos nuevo nodo candidato
				}
			}
		}
	}

	return (list<Action>());
}

/************************************************************************/
Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan){
		// Invocar al método de búsqueda
		EstadoR inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = DijkstraR(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio,plan);
		hayPlan = plan.size() != 0 ;
	}
	if (hayPlan && plan.size()>0){
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size()== 0){
		hayPlan = false;
	}

	return accion;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
	return (IDLE);
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}