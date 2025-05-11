#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <iostream>
#include <list>


#include "comportamientos/comportamiento.hpp"

struct EstadoR {
  int f;
  int c;
  int brujula;
  bool zapatillas;
  bool operator==(const EstadoR &st) const{
  return f == st.f && c == st.c && brujula == st.brujula && zapatillas ==
  st.zapatillas;
  }
};

struct NodoR{
  EstadoR estado;
  int coste;
  list<Action> secuencia;

  bool operator==(const NodoR &node) const{ return estado == node.estado; }
  bool operator<(const NodoR& n) const { return coste > n.coste; } 
};

struct NodoR4{
  EstadoR estado;
  int coste;
  int costeYheur;
  list<Action> secuencia;

  bool operator==(const NodoR4 &node) const{ return estado == node.estado; }
  bool operator<(const NodoR4& n) const { return coste > n.coste; } 
};

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

    permite_agua = false;

    hayPlan = false;

    avisado = false;
    recargando = false;
    last_point = {-1,-1};
    rentaRecarga = true;


  }

  /*************************************************************************/
  // Inicializar Variables de Estado Niveles 2,3
  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    hayPlan = false;
    tiene_zapatillas = false;
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

  Action ComportamientoReactivo_R(Sensores sensores);


  void PintaPlan(const list<Action> &plan, bool zap);
  void VisualizaPlan(const EstadoR &st,
                     const list<Action> &plan);



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

  bool permite_agua;
  
  list<Action> plan;
  bool hayPlan;

  bool recargando;
  bool rentaRecarga;

  bool avisado;
  pair<int,int> last_point;

};

#endif
