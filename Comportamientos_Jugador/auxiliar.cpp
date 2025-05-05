#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include <algorithm>
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
		// accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoAuxiliarNivel_3 (sensores);
		accion = ComportamientoAuxiliarNivel_E (sensores);
		break;
	case 4:
		// accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

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

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
}

list<Action> AvanzaASaltosDeCaballo()
{
	list<Action> secuencia;
	secuencia.push_back(WALK);
	secuencia.push_back(WALK);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(WALK);
	return secuencia;
}

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
/**
 * @brief Primera versión del algoritmo de Búsqueda en Anchura para el agente Auxiliar.
 *
 * @param inicio  Estado inicial de la búsqueda
 * @param final   Estado final de la búsqueda (solo importan fila y columna)
 * @param terreno Matriz con el tipo de terreno de cada casilla
 * @param altura  Matriz con la cota de cada casilla
 * @return        La secuencia de acciones que lleva desde @p inicio hasta @p final
 * @note          Devuelve un plan vacío si no encuentra un camino válido
 */
list<Action> ComportamientoAuxiliar::AnchuraAuxiliar(
        const EstadoA &inicio, const EstadoA &final,
        const vector<vector<unsigned char>> &terreno,
        const vector<vector<unsigned char>> &altura)
{
    NodoA current_node;
    list<NodoA> frontier;
    list<NodoA> explored;
    list<Action> path;

    // Nodo raíz
    current_node.estado = inicio;           // Estado inicial
    frontier.push_back(current_node);

    bool SolutionFound = (current_node.estado.f == final.f &&
                          current_node.estado.c == final.c);

    while (!SolutionFound && !frontier.empty()) {

        frontier.pop_front();
        explored.push_back(current_node);

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
        else if (!Find(child_WALK, frontier) && !Find(child_WALK, explored)) {
            // Añadimos el hijo a la frontera
            child_WALK.secuencia.push_back(WALK);
            frontier.push_back(child_WALK);
        }

        /* ---------- 2. Genero hijo TURN_SR ---------- */
        if (!SolutionFound) {
            NodoA child_TURN_SR = current_node;
            child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);

            if (!Find(child_TURN_SR, frontier) && !Find(child_TURN_SR, explored)) {
                child_TURN_SR.secuencia.push_back(TURN_SR);
                frontier.push_back(child_TURN_SR);
            }
        }

        /* ---------- 3. Paso al siguiente nodo ---------- */
        if (!SolutionFound && !frontier.empty()) {
            current_node  = frontier.front();
            SolutionFound = (current_node.estado.f == final.f &&
                             current_node.estado.c == final.c);
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
	if (hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size()== 0){
		hayPlan = false;
	}

	return accion;
}
