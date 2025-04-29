#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <thread>

#include "comportamientos/comportamiento.hpp"

class ComportamientoAuxiliar : public Comportamiento
{

public:

  /*************************************************************************/
  // Inicializar Variables de Estado Niveles 0,1,4
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
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

    umbral = 10;
    permitidoUmbral = false;
    contadorGiroCompleto = 0;
  }

  /*************************************************************************/
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
  }

  /*************************************************************************/
  ComportamientoAuxiliar(const ComportamientoAuxiliar &comport) : Comportamiento(comport) {}
  ~ComportamientoAuxiliar() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  Action ComportamientoAuxiliarNivel_0(Sensores sensores);
  Action ComportamientoAuxiliarNivel_1(Sensores sensores);
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);
  Action ComportamientoAuxiliarNivel_3(Sensores sensores);
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);

private:
  // Definir Variables de Estado
  Action last_action; 
  bool tiene_zapatillas;
  int giro45izq;

  vector<vector<int>> visitada; //vector con true si la casilla es visitada y false si no (tam=16)
  vector<int> visitada_local;

  int umbral;
  bool permitidoUmbral;
  int contadorGiroCompleto;
};

#endif
