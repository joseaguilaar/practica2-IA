#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>

#include "comportamientos/comportamiento.hpp"

struct EstadoA {
  int f;
  int c;
  int brujula;
  bool zapatillas;
  bool operator==(const EstadoA &st) const{
  return f == st.f && c == st.c && brujula == st.brujula and zapatillas ==
  st.zapatillas;
  }
};

struct NodoA{
  EstadoA estado;
  int coste;
  int costeYheur;
  list<Action> secuencia;

  bool operator==(const NodoA &node) const{ return estado == node.estado; }
  bool operator<(const NodoA& n) const { return coste > n.coste; } 
};

class ComportamientoAuxiliar : public Comportamiento
{

public:

  /*************************************************************************/
  // Inicializar Variables de Estado Niveles 0,1,4
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
  {
    last_action = IDLE;
    tiene_zapatillas = false;
    giroSR = 0;

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
  // Inicializar Variables de Estado Niveles 2,3
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    hayPlan = false;
    tiene_zapatillas = false;
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

  Action ComportamientoAuxiliarNivel_E(Sensores sensores);

  list<Action> AnchuraAuxiliar( const EstadoA &inicio, const EstadoA &final,
                                const vector<vector<unsigned char>> &terreno,
                                const vector<vector<unsigned char>> &altura);

  void VisualizaPlan(const EstadoA &st, const list<Action> &plan);




private:
  // Definir Variables de Estado
  Action last_action; 
  bool tiene_zapatillas;
  int giroSR;

  vector<vector<int>> mapaVisitadas; //matriz con el numero de veces que se ha visitado cada casilla

  //vectores con la informacion de las 8 casillas de alrededor del agente
  vector<int> visitada_local; 
  vector<unsigned char> superficie_local; //estos 3 me hacen falta para la funcion viable
  vector<unsigned char> cotas_local;
  vector<unsigned char> entidades_local;


  //nivel 2,3,4
  list<Action> plan;
  bool hayPlan;
};

#endif
