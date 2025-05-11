#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
#include "motorlib/util.h"

using namespace std;

Action ComportamientoAuxiliar::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoAuxiliarNivel_0 (sensores);
		break;
	case 1:
		accion = ComportamientoAuxiliarNivel_1 (sensores);
		break;
	case 2:
		accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		accion = ComportamientoAuxiliarNivel_3 (sensores);
		//accion = ComportamientoAuxiliarNivel_E (sensores);
		break;
	case 4:
		accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//--------------------------------NIVELES 0,1-----------------------------------
//
////////////////////////////////////////////////////////////////////////////////


/************************************************************************/
// Devuelve la casilla mas interesante de entre las 3 que tiene delante, en orden
// primero la del centro, izquierda y derecha. Y en orden de importancia de casillas:
// objetivo y camino
int VeoCasillaInteresanteA_Nivel0( vector<int> v_l, vector<unsigned char> s_l )
{	
	//buscamos objetivo
	for( int i=0; i<8; i++ ){
		if( s_l[i] == 'X' ){
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
void ViableA (vector<int> &v_l, vector<unsigned char> &s_l, vector<unsigned char> &c_l, int cota_orig, vector<unsigned char> &e_l)
{
	for( int i=0; i<8; i++ ){

		if(v_l[i] == numeric_limits<int>::max()) 
			s_l[i] = 'P';
		else if( ! ( abs(c_l[i] - cota_orig)<=1 && e_l[i]!='r' ) )
			s_l[i] = 'P'; //modificamos a precipicio
	}
}

/************************************************************************/
// Actualiza las matrices MapaResultado y MapaCotas
void SituarSensorEnMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, vector<vector<unsigned char>> &e, vector<vector<int>> &v, 
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
void InicializarVisitadaLocalA( vector<vector<unsigned char>> &m, vector<vector<int>> &v, vector<int> &v_l, Sensores sensores)
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
void InicializarSuperficieLocalA( vector<vector<unsigned char>> &m, vector<unsigned char> &s_l, Sensores sensores)
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
void InicializarCotasLocalA( vector<vector<unsigned char>> &a, vector<unsigned char> &c_l, Sensores sensores)
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
void InicializarEntidadesLocalA( vector<vector<unsigned char>> &e, vector<unsigned char> &e_l, Sensores sensores)
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
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	// El comportamiento de seguir por un Auxiliar por un camino hasta encontrar un puesto base.
	Action accion;
	
	//Fase 1, actualizar variables de estado
	SituarSensorEnMapaA( mapaResultado, mapaCotas, mapaEntidades, mapaVisitadas, sensores );

	InicializarVisitadaLocalA( mapaResultado, mapaVisitadas, visitada_local, sensores );
	InicializarSuperficieLocalA( mapaResultado, superficie_local, sensores );
	InicializarCotasLocalA( mapaCotas, cotas_local, sensores );
	InicializarEntidadesLocalA( mapaEntidades, entidades_local, sensores );

	//Fase 2, determinar siguiente accion a realizar
	if( sensores.superficie[0] == 'X' ) {
		accion = IDLE;
	}
	else if( giroSR != 0 ){ //estamos en medio de un giro TURN_SL
		accion = TURN_SR;
		giroSR--;
	}
	else{

		ViableA( visitada_local, superficie_local, cotas_local, sensores.cota[0], entidades_local );

		int pos = VeoCasillaInteresanteA_Nivel0( visitada_local, superficie_local );

		switch(pos)
		{	
			case 1:
				accion = WALK;      //WALK
				break;
			case 0:			
				giroSR = 6;
				accion = TURN_SR;
				break;
			case 2:		
				accion = TURN_SR;   //TURN_SR
				break;
			case 7:                 //TURN_L
				giroSR = 5;
				accion = TURN_SR;
				break;
			case 3:					//TURN_R
				giroSR = 1;
				accion = TURN_SR;
				break;
			case 6: 			    //2*TURN_L + TURN_SR
				giroSR = 4;
				accion = TURN_SR;
				break;
			case 4:                 //3*TURN_SR
				giroSR = 2;
				accion = TURN_SR;
				break;
			case 5:					//2*TURN_L
				giroSR = 3;
				accion = TURN_SR;
				break;
			case -1:                //si no hay ninguna casilla interesante gira hacia la izquierda para seguir buscando
				accion = TURN_SR;
				break;
		}
	}

	last_action = accion;
	return (accion);
}

/************************************************************************/
// Devuelve la casilla mas interesante de entre las 3 que tiene delante, en orden
int VeoCasillaInteresanteA_Nivel1( bool zap, vector<int> v_l, vector<unsigned char> s_l )
{
	//buscamos zapatillas
	for( int i=0; i<8 && !zap ; i++ ){
		if( s_l[i] == 'D' ){
			return i;
		}
	}

	//mejor resultado:
	vector<pair<pair<char,int>, int>> prioridad({ {{s_l[1],1},v_l[1]-1}, {{s_l[2],2},v_l[2]+1}, {{s_l[0],0},v_l[0]+1}, 
												  {{s_l[3],3},v_l[3]+2}, {{s_l[7],7},v_l[7]+2}, {{s_l[4],4},v_l[4]+3}, 
												  {{s_l[6],6},v_l[6]+3}, {{s_l[5],5},v_l[5]+5}  
	});


	//ordenamos en funcion del valor entero, pero manteniendose en la misma posicion
	//los enteros que sean iguales (por defecto: {1,0,2,7,3,6,4,5})
	stable_sort(prioridad.begin(), prioridad.end(), 
	[](const std::pair<pair<char,int>, int>& a, const std::pair<pair<char,int>, int>& b) { return a.second < b.second; });

	if ( prioridad[0].first.first =='C' || prioridad[0].first.first=='S' || prioridad[0].first.first=='X' || prioridad[0].first.first=='D' || (zap && prioridad[0].first.first=='B') ) return prioridad[0].first.second;
	else if ( prioridad[1].first.first=='C' || prioridad[1].first.first=='S' || prioridad[1].first.first=='X' || prioridad[1].first.first=='D' || (zap && prioridad[1].first.first=='B') ) return prioridad[1].first.second;
	else if ( prioridad[2].first.first=='C' || prioridad[2].first.first=='S' || prioridad[2].first.first=='X' || prioridad[2].first.first=='D' || (zap && prioridad[2].first.first=='B') ) return prioridad[2].first.second;
	else if ( prioridad[3].first.first=='C' || prioridad[3].first.first=='S' || prioridad[3].first.first=='X' || prioridad[3].first.first=='D' || (zap && prioridad[3].first.first=='B') ) return prioridad[3].first.second;
	else if ( prioridad[4].first.first=='C' || prioridad[4].first.first=='S' || prioridad[4].first.first=='X' || prioridad[4].first.first=='D' || (zap && prioridad[4].first.first=='B') ) return prioridad[4].first.second;
	else if ( prioridad[5].first.first=='C' || prioridad[5].first.first=='S' || prioridad[5].first.first=='X' || prioridad[5].first.first=='D' || (zap && prioridad[5].first.first=='B') ) return prioridad[5].first.second;
	else if ( prioridad[6].first.first=='C' || prioridad[6].first.first=='S' || prioridad[6].first.first=='X' || prioridad[6].first.first=='D' || (zap && prioridad[6].first.first=='B')  ) return prioridad[6].first.second;
	else if ( prioridad[7].first.first=='C' || prioridad[7].first.first=='S' || prioridad[7].first.first=='X' || prioridad[7].first.first=='D' || (zap && prioridad[7].first.first=='B') ) return prioridad[7].first.second;
	else return -1;
}

/************************************************************************/
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
	// El comportamiento de seguir por un Auxiliar por un camino hasta encontrar un puesto base.
	Action accion;
	
	//Fase 1, actualizar variables de estado
	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaA( mapaResultado, mapaCotas, mapaEntidades, mapaVisitadas, sensores );

	InicializarVisitadaLocalA( mapaResultado, mapaVisitadas, visitada_local, sensores );
	InicializarSuperficieLocalA( mapaResultado, superficie_local, sensores );
	InicializarCotasLocalA( mapaCotas, cotas_local, sensores );
	InicializarEntidadesLocalA( mapaEntidades, entidades_local, sensores );

	//Fase 2, determinar siguiente accion a realizar
	if(sensores.energia <= 3){
		accion = IDLE;
	}
    else if( giroSR != 0 ){ //estamos en medio de un giro TURN_SL
		accion = TURN_SR;
		giroSR--;
	}
	else{

		ViableA( visitada_local, superficie_local, cotas_local, sensores.cota[0], entidades_local );

		int pos = VeoCasillaInteresanteA_Nivel1( tiene_zapatillas, visitada_local, superficie_local );

		switch(pos)
		{	
			case 1:
				accion = WALK;      //WALK
				break;
			case 0:			
				giroSR = 6;
				accion = TURN_SR;
				break;
			case 2:		
				accion = TURN_SR;   //TURN_SR
				break;
			case 7:                 //TURN_L
				giroSR = 5;
				accion = TURN_SR;
				break;
			case 3:					//TURN_R
				giroSR = 1;
				accion = TURN_SR;
				break;
			case 6: 			    //2*TURN_L + TURN_SR
				giroSR = 4;
				accion = TURN_SR;
				break;
			case 4:                 //3*TURN_SR
				giroSR = 2;
				accion = TURN_SR;
				break;
			case 5:					//2*TURN_L
				giroSR = 3;
				accion = TURN_SR;
				break;
			case -1:                //si no hay ninguna casilla interesante gira hacia la izquierda para seguir buscando
				accion = TURN_SR;
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
bool Find (const NodoA & st, const list<NodoA> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !((*it) == st)){
	it++;
	}
	return (it != lista.end());
}

/************************************************************************/
void AnularMatrizA(vector<vector<unsigned char>> &m)
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
EstadoA NextCasillaAuxiliar(const EstadoA &st){
	EstadoA siguiente = st;

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
bool CasillaAccesibleAuxiliar(const EstadoA &st, const vector<vector<unsigned char>> &terreno,
							  const vector<vector<unsigned char>> &altura)
{
	EstadoA next = NextCasillaAuxiliar(st);
	bool check1 = false, check2 = false, check3 = false;

	check1 = terreno[next.f][next.c] != 'P' && terreno[next.f][next.c] != 'M';
	check2 = terreno[next.f][next.c] != 'B' || (terreno[next.f][next.c] =='B' && st.zapatillas);
	check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;

	return check1 && check2 && check3;
}

/************************************************************************/
EstadoA applyA(Action accion, const EstadoA & st, const vector<vector<unsigned char>> &terreno,
			   const vector<vector<unsigned char>> &altura)
{
	EstadoA next = st;

	switch(accion){
	case WALK:
		if (CasillaAccesibleAuxiliar(st,terreno,altura)){
			next = NextCasillaAuxiliar(st);
		}
		break;
	case TURN_SR:
		next.brujula = (next.brujula+1)%8;
		break;
	}
	return next;
}

/************************************************************************/
list<Action> ComportamientoAuxiliar::AnchuraAuxiliar(
        const EstadoA &inicio, const EstadoA &final,
        const vector<vector<unsigned char>> &terreno,
        const vector<vector<unsigned char>> &altura)
{
    NodoA current_node;
    list<NodoA> frontier; //nodos pendientes de explorar
    set<NodoA> explored; //nodos ya explorados
    list<Action> path;    //solucion

    // Nodo raíz
    current_node.estado = inicio;           // Estado inicial
    frontier.push_back(current_node);

    bool SolutionFound = (current_node.estado.f == final.f &&
                          current_node.estado.c == final.c);

    while (!SolutionFound && !frontier.empty()) {

        frontier.pop_front();
        explored.insert(current_node);

        // Si cae en una casilla con Zapatillas, las adquiere
        if (terreno[current_node.estado.f][current_node.estado.c] == 'D') {
            current_node.estado.zapatillas = true;
        }

        /* ---------- 1. Genero hijo WALK ---------- */
        NodoA child_WALK = current_node;
        child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);

        if (child_WALK.estado.f == final.f && child_WALK.estado.c == final.c) {
            // ¡Encontrada la solución!
            child_WALK.secuencia.push_back(WALK);
            current_node    = child_WALK;
            SolutionFound   = true;
        }
        else if ( explored.find(child_WALK) == explored.end() ) {
            // Añadimos el hijo a la fronteras
            child_WALK.secuencia.push_back(WALK);
            frontier.push_back(child_WALK);
        }

        /* ---------- 2. Genero hijo TURN_SR ---------- */
        if (!SolutionFound) {
            NodoA child_TURN_SR = current_node;
            child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);

            if ( explored.find(child_TURN_SR) == explored.end() ) {
                child_TURN_SR.secuencia.push_back(TURN_SR);
                frontier.push_back(child_TURN_SR);
            }
        }

        /* ---------- 3. Paso al siguiente nodo ---------- */
        if (!SolutionFound && !frontier.empty()) {
            current_node  = frontier.front();
            while( explored.find(current_node) != explored.end() && !frontier.empty() ){
				frontier.pop_front();
				current_node = frontier.front();
			}
        }
    }

    // Si hemos encontrado solución, devolvemos la secuencia
    if (SolutionFound) path = current_node.secuencia;

    return path;
}

/************************************************************************/
void ComportamientoAuxiliar::VisualizaPlan(const EstadoA &st, const list<Action> &plan)
{
	AnularMatrizA(mapaConPlan);
	EstadoA cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
		case WALK:
			switch (cst.brujula)
			{
			case 0:
				cst.f--;
				break;
			case 1:
				cst.f--;
				cst.c++;
				break;
			case 2:
				cst.c++;
				break;
			case 3:
				cst.f++;
				cst.c++;
				break;
			case 4:
				cst.f++;
				break;
			case 5:
				cst.f++;
				cst.c--;
				break;
			case 6:
				cst.c--;
				break;
			case 7:
				cst.f--;
				cst.c--;
				break;
			}
			mapaConPlan[cst.f][cst.c] = 2;
			break;

		case TURN_SR:
			cst.brujula = (cst.brujula + 1) % 8;
			break;
		}
		it++;

	}
}

/************************************************************************/
int costeEnergiaA( EstadoA &inicio, EstadoA &fin, Action &a, const vector<vector<unsigned char>> &terreno,
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

		case('?'):
			coste = 1000;    //para que la prioridad sea ir por caminos conocidos
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
list<Action> AEstrella( const EstadoA &inicio, const EstadoA &fin, vector<vector<unsigned char>> &terreno, vector<vector<unsigned char>> &altura )
{
	priority_queue<NodoA> pq;
    
	//matriz cuatridimensional con 8 rumbos por cada casilla(fil,col), con el coste minimo para llegar a esa fil,col,rumbo
	vector<vector<vector<vector<int>>>> coste_min( terreno.size(),vector<vector<vector<int>>>( terreno[0].size(), vector<vector<int>>(8, vector<int>(2, numeric_limits<int>::max()))));
	
	coste_min[inicio.f][inicio.c][inicio.brujula][inicio.zapatillas ? 1 : 0 ] = 0;

	int heur = abs(fin.f - inicio.f) + abs(fin.c - inicio.c); 

	pq.push({inicio, 0, heur, {}});

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

		for( Action a: {WALK, TURN_SR}){
				
			EstadoA next = applyA(a, it.estado, terreno, altura);

			if( !(next == it.estado) ){ 							//si son iguales, no es casilla accesible
				
				int c = costeEnergiaA( it.estado, next, a, terreno, altura );
				
				if( c+it.coste < coste_min[next.f][next.c][next.brujula][next.zapatillas ? 1 : 0] ){ //si es la forma menos costosa de alcazar esa fil,col,rumbo (asi no tendremos repetidos)

					coste_min[next.f][next.c][next.brujula][next.zapatillas ? 1 : 0] = c+it.coste;   //actualizamos

					list<Action> sec = it.secuencia;
					sec.push_back(a);

					heur = abs(fin.f - next.f) + abs(fin.c - next.c);     //calculamos heuristica
					
					pq.push({next, c+it.coste, c+it.coste + heur, sec});                  //añadimos nuevo nodo candidato
				}
			}
		}
	}

	return (list<Action>());
}

/************************************************************************/
bool CasillaAccesibleA_nivel4(const EstadoA &st, const vector<vector<unsigned char>> &terreno,
							  const vector<vector<unsigned char>> &altura, const vector<vector<unsigned char>> &entidades, bool planeando)
{
	EstadoA next = NextCasillaAuxiliar(st);
	bool check1 = false, check2 = false, check3 = false, check4 = false;

	if( planeando ){
		if( !(next.f>=3 && next.f<terreno.size()-3 && next.c>=3 && next.c<terreno.size()-3)){ return false; }
		if( terreno[next.f][next.c] == '?'){ return true; }
	}

	check1 = terreno[next.f][next.c] != 'P' && terreno[next.f][next.c] != 'M';
	check2 = terreno[next.f][next.c] != 'B' || (terreno[next.f][next.c] =='B' && st.zapatillas);
	check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;
	check4 = (entidades[next.f][next.c] != 'r');

	return (check1 && check2 && check3 && check4);
}

/************************************************************************/
EstadoA applyA_nivel4(Action accion, const EstadoA &st, const vector<vector<unsigned char>> &terreno,
			   const vector<vector<unsigned char>> &altura, const vector<vector<unsigned char>> &agentes)
{
	EstadoA next = st;

	switch(accion){
	case TURN_SR:
		next.brujula = (next.brujula+1)%8;
		break;

	case WALK:				
	
		if (CasillaAccesibleA_nivel4(st,terreno,altura,agentes, true)){
			next = NextCasillaAuxiliar(st);
		}
		break;
	}
	return (next);
}

/************************************************************************/
list<Action> AEstrellaA_nivel4( const EstadoA &inicio, const EstadoA &fin, vector<vector<unsigned char>> &terreno, vector<vector<unsigned char>> &altura, vector<vector<unsigned char>> &agentes )
{
	priority_queue<NodoA> pq;
    
	//matriz cuatridimensional con 8 rumbos por cada casilla(fil,col), con el coste minimo para llegar a esa fil,col,rumbo
	vector<vector<vector<vector<int>>>> coste_min( terreno.size(),vector<vector<vector<int>>>( terreno[0].size(), vector<vector<int>>(8, vector<int>(2, numeric_limits<int>::max()))));
	
	coste_min[inicio.f][inicio.c][inicio.brujula][inicio.zapatillas ? 1 : 0 ] = 0;

	int heur = abs(fin.f - inicio.f) + abs(fin.c - inicio.c); 

	pq.push({inicio, 0, heur, {}});

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

		for( Action a: {WALK, TURN_SR}){
				
			EstadoA next = applyA_nivel4(a, it.estado, terreno, altura, agentes);

			if( !(next == it.estado) ){ 							//si son iguales, no es casilla accesible
				
				int c = costeEnergiaA( it.estado, next, a, terreno, altura );
				
				if( c+it.coste < coste_min[next.f][next.c][next.brujula][next.zapatillas ? 1 : 0] ){ //si es la forma menos costosa de alcazar esa fil,col,rumbo (asi no tendremos repetidos)

					coste_min[next.f][next.c][next.brujula][next.zapatillas ? 1 : 0] = c+it.coste;   //actualizamos

					list<Action> sec = it.secuencia;
					sec.push_back(a);

					heur = abs(fin.f - next.f) + abs(fin.c - next.c);     //calculamos heuristica
					
					pq.push({next, c+it.coste, c+it.coste + heur, sec});                  //añadimos nuevo nodo candidato
				}
			}
		}
	}

	return (list<Action>());
}


/************************************************************************/
// Devuelve la casilla mas interesante de entre las 3 que tiene delante, en orden
int VeoCasillaInteresanteA_Nivel4( bool zap, vector<int> v_l, vector<unsigned char> s_l )
{
	//buscamos zapatillas
	for( int i=0; i<8 && !zap ; i++ ){
		if( s_l[i] == 'D' ) return i;
	}

	for( int i: {7,0,1} ){
		if( s_l[i] == 'X' ) return i;
	}

	//mejor resultado:
	vector<pair<pair<char,int>, int>> prioridad({ {{s_l[1],1},v_l[1]-1}, {{s_l[2],2},v_l[2]+1}, {{s_l[0],0},v_l[0]+1}, 
												  {{s_l[3],3},v_l[3]+2}, {{s_l[7],7},v_l[7]+2}, {{s_l[4],4},v_l[4]+3}, 
												  {{s_l[6],6},v_l[6]+3}, {{s_l[5],5},v_l[5]+5}  
	});


	//ordenamos en funcion del valor entero, pero manteniendose en la misma posicion
	//los enteros que sean iguales (por defecto: {1,0,2,7,3,6,4,5})
	stable_sort(prioridad.begin(), prioridad.end(), 
	[](const std::pair<pair<char,int>, int>& a, const std::pair<pair<char,int>, int>& b) { return a.second < b.second; });

	if ( prioridad[0].first.first !='P' && prioridad[0].first.first!='M' && ( prioridad[0].first.first!='B' || (zap && prioridad[0].first.first=='B') ) ) return prioridad[0].first.second;
	else if ( prioridad[1].first.first!='P' && prioridad[1].first.first!='M' && ( prioridad[1].first.first!='B' || (zap && prioridad[1].first.first=='B') ) ) return prioridad[1].first.second;
	else if ( prioridad[2].first.first!='P' && prioridad[2].first.first!='M' && ( prioridad[2].first.first!='B' || (zap && prioridad[2].first.first=='B') ) ) return prioridad[2].first.second;
	else if ( prioridad[3].first.first!='P' && prioridad[3].first.first!='M' && ( prioridad[3].first.first!='B' || (zap && prioridad[3].first.first=='B') ) ) return prioridad[3].first.second;
	else if ( prioridad[4].first.first!='P' && prioridad[4].first.first!='M' && ( prioridad[4].first.first!='B' || (zap && prioridad[4].first.first=='B') ) ) return prioridad[4].first.second;
	else if ( prioridad[5].first.first!='P' && prioridad[5].first.first!='M' && ( prioridad[5].first.first!='B' || (zap && prioridad[5].first.first=='B') ) ) return prioridad[5].first.second;
	else if ( prioridad[6].first.first!='P' && prioridad[6].first.first!='M' && ( prioridad[6].first.first!='B' || (zap && prioridad[6].first.first=='B') )  ) return prioridad[6].first.second;
	else if ( prioridad[7].first.first!='P' && prioridad[7].first.first!='M' && ( prioridad[7].first.first!='B' || (zap && prioridad[7].first.first=='B') ) ) return prioridad[7].first.second;
	else return -1;
}

/************************************************************************/
Action ComportamientoAuxiliar::ComportamientoReactivo_A(Sensores sensores)
{
	// El comportamiento de seguir por un Auxiliar por un camino hasta encontrar un puesto base.
	Action accion;
	
	//Fase 1, actualizar variables de estado
	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaA( mapaResultado, mapaCotas, mapaEntidades, mapaVisitadas, sensores );

	InicializarVisitadaLocalA( mapaResultado, mapaVisitadas, visitada_local, sensores );
	InicializarSuperficieLocalA( mapaResultado, superficie_local, sensores );
	InicializarCotasLocalA( mapaCotas, cotas_local, sensores );
	InicializarEntidadesLocalA( mapaEntidades, entidades_local, sensores );

	//Fase 2, determinar siguiente accion a realizar
	if(sensores.energia <= 3){
		accion = IDLE;
	}
    else if( giroSR != 0 ){ //estamos en medio de un giro TURN_SL
		accion = TURN_SR;
		giroSR--;
	}
	else{

		ViableA( visitada_local, superficie_local, cotas_local, sensores.cota[0], entidades_local );

		int pos = VeoCasillaInteresanteA_Nivel4( tiene_zapatillas, visitada_local, superficie_local );

		switch(pos)
		{	
			case 1:
				accion = WALK;      //WALK
				break;
			case 0:			
				giroSR = 6;
				accion = TURN_SR;
				break;
			case 2:		
				accion = TURN_SR;   //TURN_SR
				break;
			case 7:                 //TURN_L
				giroSR = 5;
				accion = TURN_SR;
				break;
			case 3:					//TURN_R
				giroSR = 1;
				accion = TURN_SR;
				break;
			case 6: 			    //2*TURN_L + TURN_SR
				giroSR = 4;
				accion = TURN_SR;
				break;
			case 4:                 //3*TURN_SR
				giroSR = 2;
				accion = TURN_SR;
				break;
			case 5:					//2*TURN_L
				giroSR = 3;
				accion = TURN_SR;
				break;
			case -1:                //si no hay ninguna casilla interesante gira hacia la izquierda para seguir buscando
				accion = TURN_SR;
				break;
		}
	}

	last_action = accion;
	return (accion);
}

/************************************************************************/
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_E(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan){
		// Invocar al método de búsqueda
		EstadoA inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = AnchuraAuxiliar(inicio, fin, mapaResultado, mapaCotas);
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

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
	return (IDLE);
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan){
		// Invocar al método de búsqueda
		EstadoA inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = AEstrella(inicio, fin, mapaResultado, mapaCotas);
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

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
	Action accion = IDLE;
	SituarSensorEnMapaA( mapaResultado, mapaCotas, mapaEntidades, mapaVisitadas, sensores );

	
	if( sensores.destinoF!=-1 && sensores.destinoC!=-1 ){

		if(sensores.superficie[0] == 'X' && sensores.energia < 2500 ) return (IDLE);

		if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

		if (!hayPlan){
			// Invocar al método de búsqueda
			EstadoA inicio, fin;
			inicio.f = sensores.posF;
			inicio.c = sensores.posC;
			inicio.brujula = sensores.rumbo;
			inicio.zapatillas = tiene_zapatillas;
			fin.f = sensores.destinoF;
			fin.c = sensores.destinoC;
			plan = AEstrellaA_nivel4(inicio, fin, mapaResultado, mapaCotas, mapaEntidades);
			VisualizaPlan(inicio,plan);
			hayPlan = plan.size() != 0;
			
		}

		if (hayPlan && plan.size()>0){
			accion = plan.front();
			plan.pop_front();
			
			if( accion == WALK ){

				if( sensores.agentes[2]=='v' || sensores.agentes[2]=='e' ){
					plan.push_front(accion);
					return (IDLE);
				}

				if( !CasillaAccesibleA_nivel4({sensores.posF,sensores.posC,sensores.rumbo,tiene_zapatillas}, mapaResultado, mapaCotas, mapaEntidades, false) ){
					hayPlan = false;
					return(IDLE);
				}
			}
			
		}
		else{
			cerr<<"NOPLAN";
			accion = ComportamientoReactivo_A(sensores);
		}
	
		if (plan.size()== 0){
			hayPlan = false;
		}

	}
	else{
		AnularMatrizA(mapaConPlan);
	}
	return (accion);
}

