#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"


#include <iostream>
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
		// accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoRescatadorNivel_3 (sensores);
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

/************************************************************************/
// Devuelve la casilla mas interesante de entre las 3 que tiene delante, en orden
// primero la del centro, izquierda y derecha. Y en orden de importancia de casillas:
// objetivo, zapatillas(si no las tiene) y camino
int VeoCasillaInteresanteR_Nivel0( char i, char c, char d, bool zap)
{
	if( c=='X') return 2;
	else if (i=='X') return 1;
	else if (d=='X') return 3;
	else if (!zap){
		if( c=='D') return 2;
		else if (i=='D') return 1;
		else if (d=='D') return 3;
	}
	if (c=='C') return 2;
	else if (i=='C') return 1;
	else if (d=='C') return 3;
	else return 0;
}

/************************************************************************/
//Devuelve la casilla destino si es viable el acceso o precicipio si no lo es
char ViableR (char casilla, char cota_orig, char cota_dest, bool zap, char agente_dest)
{
	if( (abs(cota_dest - cota_orig)<=1 || (zap && abs(cota_dest - cota_orig)<=2))  && agente_dest!='a' )
		return (casilla);
	else
		return ('P'); //precipicio
}

/************************************************************************/
// Actualiza las matrices MapaResultado y MapaCotas
void SituarSensorEnMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, vector<vector<bool>> &v, vector<bool> &v_l,
						 Sensores sensores)
{
	m[sensores.posF][sensores.posC] = sensores.superficie[0];
	a[sensores.posF][sensores.posC] = sensores.cota[0];
	v[sensores.posF][sensores.posC] = true;
	
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
Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	Action accion;

	//Fase 1, actualizar variables de estado
	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaR( mapaResultado, mapaCotas, visitada, visitada_local, sensores );

	//Fase 2, determinar siguiente accion a realizar
	if( sensores.superficie[0] == 'X' ) {
		accion = IDLE;
	}
	else if( giro45izq != 0 ){ //estamos en medio de un giro TURN_SL
		accion = TURN_SR;
		giro45izq--;
	}
	else{

		char i = ViableR(sensores.superficie[1], sensores.cota[0], sensores.cota[1], tiene_zapatillas, sensores.agentes[1]);
		char c = ViableR(sensores.superficie[2], sensores.cota[0], sensores.cota[2], tiene_zapatillas, sensores.agentes[2]);
		char d = ViableR(sensores.superficie[3], sensores.cota[0], sensores.cota[3], tiene_zapatillas, sensores.agentes[3]);

		int pos = VeoCasillaInteresanteR_Nivel0( i, c, d, tiene_zapatillas);

		switch(pos)
		{	
			case 2:
				accion = WALK;
				break;
			case 1:			
				giro45izq = 1;
				accion = TURN_L;
				break;
			case 3:		
				accion = TURN_SR;
				break;
			case 0:            //si no hay ninguna casilla interesante gira hacia la izquierda para seguir buscando
				accion = TURN_L;
				break;
		}
	}

	last_action = accion;
	return (accion);
}



/************************************************************************/
// Devuelve la casilla mas interesante de entre las 3 que tiene delante, en orden
int VeoCasillaInteresanteR_Nivel1( char i, char c, char d, bool zap, vector<bool> v_l)
{
	if (!zap){
		if( c=='D') return 2;
		else if (i=='D') return 1;
		else if (d=='D') return 3;
	}
	if ((c=='C' || c=='S' || c=='X' ) && !v_l[2] ) return 2;
	else if ((i=='C' || i=='S' || i=='X' ) && !v_l[1] ) return 1;
	else if ((d=='C' || d=='S' || d=='X' ) && !v_l[3]) return 3;
	else if (c=='C' || c=='S' || c=='X' ) return 2;
	else if (i=='C' || i=='S' || i=='X' ) return 1;
	else if (d=='C' || d=='S' || d=='X' ) return 3;
	else return 0;
}

/************************************************************************/
//Funcion que actualiza la variable de estado conocida, actualizando ese vector
// con las casillas a las que se tenga vision actualmente

// la idea de este nivel es que se elija con mayor prioridad las casillas de tipo
// camino desconocidas, tambien hay que tener en cuenta las situaciones en las que vienes de un camino, te encuentras una pared y vuelves por el mismo camino en vez de por un camino que hay a la derecha,
// hacer algo asi como dar una vuelta entera en busqueda de solo casillas desconocidas antes de volver por el camino anterior: Solucion a esto:
// cuando no se vean casillas interantes, hacer primero turn_L, si sigue sin haber nada hacer dos turn_R y si siguen si ver nada
// hacer otro turn_R para volver por el mismo sitio ( mejor primero mirar a la derecha y luego izquierda por lo menos en rescatador)
void actualizaConocidasR( vector<bool> &c, vector<vector<unsigned char>> &m, Sensores sensores )
{
}


/************************************************************************/
Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{
	Action accion;

	//Fase 1: actualizar variables de estado
	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaR( mapaResultado, mapaCotas, visitada, visitada_local, sensores );

	//Fase 2: definir proxima accion a realizar

	if( giro45izq != 0 ){ //estamos en medio de un giro TURN_SL
		accion = TURN_SR;
		giro45izq--;
	}
	else if( giro_der != 0 ){
		accion = TURN_SR;
		giro_der--;
	}
	else{

		char i = ViableR(sensores.superficie[1], sensores.cota[0], sensores.cota[1], tiene_zapatillas, sensores.agentes[1]);
		char c = ViableR(sensores.superficie[2], sensores.cota[0], sensores.cota[2], tiene_zapatillas, sensores.agentes[2]);
		char d = ViableR(sensores.superficie[3], sensores.cota[0], sensores.cota[3], tiene_zapatillas, sensores.agentes[3]);

		int pos = VeoCasillaInteresanteR_Nivel1( i, c, d, tiene_zapatillas, visitada_local);

		switch(pos)
		{	
			case 2:
				accion = WALK;
				vistazo1 = false;
				vistazo2 = false;
				break;
			case 1:			
				giro45izq = 1;
				accion = TURN_L;
				vistazo1 = false;
				vistazo2 = false;
				break;
			case 3:		
				accion = TURN_SR;
				vistazo1 = false;
				vistazo2 = false;
				break;
			case 0:            //si no hay ninguna casilla interesante gira hacia la izquierda para seguir buscando

				if(vistazo1){          //iniciamos 4 giros hasta la derecha del todo (para mirar hacia el otro lado)
					accion = TURN_SR;
					giro_der = 1;
					vistazo1 = false;
					vistazo2 = true;
				}
				else if(vistazo2){	  //si en el otro lado no hay nada, volvemos a hacer otro giro para volver por donde hemos venido(mas o menos)
					accion = TURN_SR;
					giro_der=1;
				}
				else{
					accion = TURN_L;  //accion inicial al no ver ninguna casilla interesante
					giro_der = 1;
					vistazo1 = true;
				}
				break;
		}
	}

	last_action = accion;
	return (accion);
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}