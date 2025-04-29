#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>

#include "comportamientos/comportamiento.hpp"

class ComportamientoRescatador : public Comportamiento
{

public:
  /*************************************************************************/
  //inicializar variables de estado niveles 0,1,4
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size)
  {
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;

    for( int i=0; i<size; i++){
      vector<int> tmp;
      for( int j=0; j<size; j++){
        tmp.push_back(0);
      }
      visitada.push_back(tmp);
    }

    for( int i=0; i<16; i++){
      visitada_local.push_back(0);
    }
 
    // vistazo1 = false;
    // vistazo2 = false;

    umbral = 5;
    permitidoUmbral = false;

    contadorGiroIzq = 0;

  }

  /*************************************************************************/
  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
  }

  /*************************************************************************/
  ComportamientoRescatador(const ComportamientoRescatador &comport) : Comportamiento(comport) {}

  /*************************************************************************/
  ~ComportamientoRescatador() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  Action ComportamientoRescatadorNivel_0(Sensores sensores);
  Action ComportamientoRescatadorNivel_1(Sensores sensores);
  Action ComportamientoRescatadorNivel_2(Sensores sensores);
  Action ComportamientoRescatadorNivel_3(Sensores sensores);
  Action ComportamientoRescatadorNivel_4(Sensores sensores);

private:
  // Variables de Estado, son la memoria que tendra el agente de iteraciones/movimientos anteriores
  Action last_action; //ultimo movimiento realizado por el agente
  bool tiene_zapatillas;
  int giro45izq;  // Turn_L + Turn_SR, cuando haya que hacerlo, hacemos Turn_L y ponemos 
                  // la variable a uno para indicar que estamos en medio de un Turn_SL 
                  // y qe se haga Turn_SR en el proximo paso
  vector<vector<int>> visitada; //vector con true si la casilla es visitada y false si no (tam=16)
  vector<int> visitada_local;
  // bool vistazo1, vistazo2;
  // int giro_der;

  int umbral;
  bool permitidoUmbral;
  int contadorGiroIzq;
};

#endif
