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
												  {{s_l[3],3},v_l[3]+2}, {{s_l[7],7},v_l[7]+2}, {{s_l[4],4},v_l[4]+4}, 
												  {{s_l[6],6},v_l[6]+4}, {{s_l[5],5},v_l[5]+5}  
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