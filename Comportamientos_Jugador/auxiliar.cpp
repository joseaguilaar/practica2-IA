#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include <algorithm>
#include "motorlib/util.h"

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
int VeoCasillaInteresanteA_Nivel0( char i, char c, char d, vector<int> v_l, int &umbral, bool &permitido_umbral )
{
	if( c=='X') return 2;
	else if (i=='X') return 1;
	else if (d=='X') return 3;

	vector<pair<pair<char,int>, int>> prioridad({ {{c,2},v_l[2]}, {{i,1},v_l[1]} , {{d,3},v_l[3]} } );

	//ordenamos en funcion del valor entero, pero manteniendose en la misma posicion
	//los enteros que sean iguales (por defecto: {c,i,d})
	stable_sort(prioridad.begin(), prioridad.end(), 
	[](const std::pair<pair<char,int>, int>& a, const std::pair<pair<char,int>, int>& b) { return a.second < b.second; });

	//devolveremos la casilla con mas prioridad
	if(permitido_umbral){
		//devolveremos la casilla con mas prioridad
		if ( prioridad[0].first.first =='C' ) return prioridad[0].first.second;
		else if (prioridad[1].first.first=='C' ) return prioridad[1].first.second;
		else if (prioridad[2].first.first=='C' ) return prioridad[2].first.second;
		else return 0;
	}
	else{
		if ( prioridad[0].first.first =='C' && prioridad[0].second < umbral ) return prioridad[0].first.second;
		else if (prioridad[1].first.first=='C' && prioridad[1].second < umbral ) return prioridad[1].first.second;
		else if (prioridad[2].first.first=='C' && prioridad[2].second < umbral ) return prioridad[2].first.second;
		else return 0;
	}
}

/************************************************************************/
//Devuelve la casilla destino si es viable el acceso o precicipio si no lo es
char ViableA (char casilla, char cota_orig, char cota_dest, char agente_dest)
{
	if( abs(cota_dest-cota_orig)<=1 && agente_dest!='r' )
		return (casilla);
	else
		return ('P'); //precipicio
}

/************************************************************************/
// Actualiza las matrices MapaResultado y MapaCotas
void SituarSensorEnMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, vector<vector<int>> &v, vector<int> &v_l,
						 Sensores sensores)
{
	m[sensores.posF][sensores.posC] = sensores.superficie[0];
	a[sensores.posF][sensores.posC] = sensores.cota[0];
	v[sensores.posF][sensores.posC] = v[sensores.posF][sensores.posC]+1;
	
	int pos = 1;
	switch (sensores.rumbo)
	{
	case norte:
		for( int i=1; i<=3; i++ ){
			for( int j=-i; j<=i; j++ ){
				m[sensores.posF - i][sensores.posC + j] = sensores.superficie[pos];
				a[sensores.posF - i][sensores.posC + j] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF - i][sensores.posC + j];
				pos++;
			}
		}
		break;

	case noreste:
		for( int i=1; i<=3; i++ ){
			
			for( int j=0; j<=i; j++ ){
				m[sensores.posF - i][sensores.posC + j] = sensores.superficie[pos];
				a[sensores.posF - i][sensores.posC + j] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF - i][sensores.posC + j];
				pos++;
			}

			for( int k = i-1; k>=0; k-- ){
				m[sensores.posF - k][sensores.posC + i] = sensores.superficie[pos];
				a[sensores.posF - k][sensores.posC + i] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF - k][sensores.posC + i];
				pos++;
			}
		}
		break;

	case este:
		for( int i=1; i<=3; i++ ){
			for( int j=-i; j<=i; j++ ){
				m[sensores.posF + j][sensores.posC + i] = sensores.superficie[pos];
				a[sensores.posF + j][sensores.posC + i] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF + j][sensores.posC + i];
				pos++;
			}
		}
		break;

	case sureste:
		for (int i = 1; i <= 3; ++i) {

			for (int j = 0; j <= i; ++j) {
				m[sensores.posF + j][sensores.posC + i] = sensores.superficie[pos];
				a[sensores.posF + j][sensores.posC + i] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF + j][sensores.posC + i];
 				pos++;
			}

			for (int k = i - 1; k >= 0; --k) {
				m[sensores.posF + i][sensores.posC + k] = sensores.superficie[pos];
				a[sensores.posF + i][sensores.posC + k] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF + i][sensores.posC + k];
				pos++;
			}
		}
		break;
	
	case sur:
		for (int i = 1; i <= 3; ++i) {
			for (int j = -i; j <= i; ++j) {
				m[sensores.posF + i][sensores.posC - j] = sensores.superficie[pos];
				a[sensores.posF + i][sensores.posC - j] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF + i][sensores.posC - j];
				pos++;
			}
		}
		break;
	
	case suroeste:
		for (int i = 1; i <= 3; ++i) {

			for (int j = 0; j <= i; ++j) {
				m[sensores.posF + i][sensores.posC - j] = sensores.superficie[pos];
				a[sensores.posF + i][sensores.posC - j] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF + i][sensores.posC - j];
				pos++;
			}

			for (int k = i - 1; k >= 0; --k) {
				m[sensores.posF + k][sensores.posC - i]    = sensores.superficie[pos];
				a[sensores.posF + k][sensores.posC - i]    = sensores.cota[pos];
				v_l[pos] = v[sensores.posF + k][sensores.posC - i];
				pos++;
			}
		}
		break;
	
	case oeste:
		for (int i = 1; i <= 3; ++i) {
			for (int j = -i; j <= i; ++j) {
				m[sensores.posF - j][sensores.posC - i] = sensores.superficie[pos];
				a[sensores.posF - j][sensores.posC - i] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF - j][sensores.posC - i];
				pos++;
			}
		}
		break;
	
	case noroeste:
		for (int i = 1; i <= 3; ++i) {

			for (int j = 0; j <= i; ++j) {
				m[sensores.posF - j][sensores.posC - i] = sensores.superficie[pos];
				a[sensores.posF - j][sensores.posC - i] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF - j][sensores.posC - i];
				pos++;
			}
			for (int k = i - 1; k >= 0; --k) {
				m[sensores.posF - i][sensores.posC - k] = sensores.superficie[pos];
				a[sensores.posF - i][sensores.posC - k] = sensores.cota[pos];
				v_l[pos] = v[sensores.posF - i][sensores.posC - k];
				pos++;
			}
		}
		break;
	}
}

/************************************************************************/
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	// El comportamiento de seguir por un Auxiliar por un camino hasta encontrar un puesto base.
	Action accion;
	
	//Fase 1, actualizar variables de estado
	SituarSensorEnMapaA(mapaResultado, mapaCotas, visitada, visitada_local, sensores);

	//Fase 2, determinar siguiente accion a realizar
	if( sensores.superficie[0] == 'X' ) {
		accion = IDLE;
	}
	else if( giro45izq != 0 ){ //estamos en medio de un giro TURN_SL
		accion = TURN_SR;
		giro45izq--;
	}
	else{

		char i = ViableA(sensores.superficie[1], sensores.cota[0], sensores.cota[1], sensores.agentes[1]);
		char c = ViableA(sensores.superficie[2], sensores.cota[0], sensores.cota[2], sensores.agentes[2]);
		char d = ViableA(sensores.superficie[3], sensores.cota[0], sensores.cota[3], sensores.agentes[3]);

		int pos = VeoCasillaInteresanteA_Nivel0( i, c, d, visitada_local, umbral, permitidoUmbral );

		switch(pos)
		{	
			case 2:
				accion = WALK;
				contadorGiroCompleto=0;
				permitidoUmbral = false;
				break;
			case 1:			
				giro45izq = 6;
				accion = TURN_SR;
				contadorGiroCompleto=0;
				permitidoUmbral = false;
				break;
			case 3:		
				accion = TURN_SR;
				contadorGiroCompleto=0;
				permitidoUmbral = false;
				break;
			case 0:            //si no hay ninguna casilla interesante hace TURN_SR
				accion = TURN_SR;
				contadorGiroCompleto++;
				if( contadorGiroCompleto >= 8 ){
					permitidoUmbral = true;
					contadorGiroCompleto = 0;
				}
				break;
		}
	}

	last_action = accion;
	return (accion);
}



/************************************************************************/
// Devuelve la casilla mas interesante de entre las 3 que tiene delante, en orden
int VeoCasillaInteresanteA_Nivel1( char i, char c, char d, vector<int> v_l )
{
	vector<pair<pair<char,int>, int>> prioridad({ {{c,2},v_l[2]}, {{i,1},v_l[1]} , {{d,3},v_l[3]} } );

	//ordenamos en funcion del valor entero, pero manteniendose en la misma posicion
	//los enteros que sean iguales (por defecto: {c,i,d})
	stable_sort(prioridad.begin(), prioridad.end(), 
	[](const std::pair<pair<char,int>, int>& a, const std::pair<pair<char,int>, int>& b) { return a.second < b.second; });

	//devolveremos la casilla con mas prioridad
	if ( prioridad[0].first.first =='C'|| prioridad[0].first.first=='S' || prioridad[0].first.first=='X' ) return prioridad[0].first.second;
	else if (prioridad[1].first.first=='C' || prioridad[1].first.first=='S' || prioridad[1].first.first=='X' ) return prioridad[1].first.second;
	else if (prioridad[2].first.first=='C' || prioridad[2].first.first=='S' || prioridad[2].first.first=='X' ) return prioridad[2].first.second;
	else return 0;
}

/************************************************************************/
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
	Action accion;

	//Fase 1: actualizar variables de estado
	SituarSensorEnMapaA( mapaResultado, mapaCotas, visitada, visitada_local, sensores );

	//Fase 2: definir proxima accion a realizar
	if( giro45izq != 0 ){ //estamos en medio de un giro TURN_SL
		accion = TURN_SR;
		giro45izq--;
	}
	else{

		char i = ViableA(sensores.superficie[1], sensores.cota[0], sensores.cota[1], sensores.agentes[1]);
		char c = ViableA(sensores.superficie[2], sensores.cota[0], sensores.cota[2], sensores.agentes[2]);
		char d = ViableA(sensores.superficie[3], sensores.cota[0], sensores.cota[3], sensores.agentes[3]);

		int pos = VeoCasillaInteresanteA_Nivel1( i, c, d, visitada_local );

		switch(pos)
		{	
			case 2:
				accion = WALK;
				break;
			case 1:			
				giro45izq = 6;
				accion = TURN_SR;
				break;
			case 3:		
				accion = TURN_SR;
				break;
			case 0:            //si no hay ninguna casilla interesante hace TURN_SR
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