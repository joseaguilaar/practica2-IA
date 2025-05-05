#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <iostream>

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
    giroSR = 0;
    giro135izq = 0;
    giro180 = 0;

    for( int i=0; i<size; i++){
      vector<int> tmp;
      for( int j=0; j<size; j++){
        tmp.push_back(0);
      }
      mapaVisitadas.push_back(tmp);
    }

    visitada_local.assign(8, 0);
    superficie_local.assign(8, ' ');
    cotas_local.assign(8, 0);
    entidades_local .assign(8, ' ');
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
  int giroSR;  
  int giro135izq;
  int giro180;
  vector<vector<int>> mapaVisitadas; //matriz con el numero de veces que se ha visitado cada casilla

  //vectores con la informacion de las 8 casillas de alrededor del agente
  vector<int> visitada_local; 
  vector<unsigned char> superficie_local; //estos 3 me hacen falta para la funcion viable
  vector<unsigned char> cotas_local;
  vector<unsigned char> entidades_local;

  vector<pair<int,bool>> visitada_CV; //vector de pares con las veces visitadas y viabilidad de 
                                      //las casillas del cono de vision

};

#endif
