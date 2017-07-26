#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include <cstdlib>
#include <list>
#include <set>
#include <vector>


#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento {
public:

    ComportamientoJugador(unsigned int size);

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport) {
    }

    ~ComportamientoJugador() {
    }

    Action think(Sensores sensores);

    int interact(Action accion, int valor);

    ComportamientoJugador * clone() {
        return new ComportamientoJugador(*this);
    }

    void act_pos(std::pair <int, int> &p, const Sensores &sensores);

    void act_mapa(std::vector< std::vector< unsigned char> > &mapa, const std::vector< unsigned char> &v, int posx, int posy);

    bool see_int(const std::vector< std::vector< unsigned char> > &mapa, int posx, int posy);

    bool planing(const list<pair<int, int> > &pares, const pair<int, int> &p);

    void update_ori(const Action &accion);

    void reiniciar();

    int see_pk(const std::vector< unsigned char> &t, const std::vector< unsigned char> &s, const Sensores &sens);

    pair<int, int> make_point(int pos_elem, int ori_elem, const pair<int, int> &p);

    void check_acceso(const std::vector< unsigned char> &t, int pos, int ori);

    Action girarPre();

    Action girar(const Sensores &s);

    void act_precipicios();

    void save_inf(const std::vector< unsigned char> &t, const std::vector< unsigned char> &s);

    bool ir_apuerta(const pair<int, int> &p);

    void revelar(const Sensores &s);

    pair<int, int> getCercano(const vector<pair< int, int> > &p);

    pair<int, int> getCercano(const multiset<pair< int, int> > &p);

    void pathFinding(const pair<int, int> &pos_orig, const pair<int, int> &point_objetivo, Action &act, const Sensores &sens, const vector< std::vector< unsigned char> > &p_mapa, const vector< std::vector< unsigned char> > &p_mapas, bool go_rey, int ori_p);

    void ejecutaPlan(Action &act, const Sensores &sens);

    void equipaObjeto(unsigned char objeto, Action &act, const Sensores & sens);

    void cogeObjeto(unsigned char obj, Action &accion, const Sensores & sens);

    bool tengoObjeto(unsigned char obj);

    void tiraObjeto(unsigned char objeto, Action &act, const Sensores & sens);

    bool can_advance(unsigned char justo_delante_ter, unsigned char justo_delante_sup, const Sensores &sens);
private:
    //ENCONTRAR PK
    bool pk_found; //Guarda si has encontrado ya el pk
    int pk_orientation; //Guarda el lado del pk VISTO
    bool go_pk;


    //DE USO GENERAL
    const int MAX_PASOS_DESCUBRIR = 15000;
    int pasos_totales;
    int pasos;
    unsigned brujula; //0: norte, 1: este, 2:sur, 3:oeste
    Action last_accion; //Guarda la ultima accion
    std::pair <int, int> pos_actual;
    int mochila;
    bool hay_bosque;
    bool hay_agua;
    bool hay_puertas;
    bool hay_reyes;
    bool hay_regalos;
    bool cogiendo_obj;
    bool tirando_obj;
    int cont_lobo;
    int cont_aldeano;
    int cont_regalos;
    int cont_puertas;
    int cont_reyes;
    int cont_plan;
    int cont_tira_objeto;
    int puertas = 0; //Controlamos si estamos dentro de puerta o no

    /*OBJETOS*/
    int tengo_regalo;
    bool tengo_biquini;
    bool tengo_zapatillas;
    bool tengo_llave;
    bool tengo_hueso;
    bool suelto_objeto;
    bool go_puerta;

    /*GIRAR CORRECTAMENTE*/
    bool girando;
    bool debe_avanzar;
    int contador;
    int contador_mismo_objeto;
    Action giro_norte;
    Action giro_sur;
    Action giro_este;
    Action giro_oeste;
    int avanzando;
    pair<int, int> pos_ant;
    int cont_misma_pos;
    bool atrancado;

    //A ESTRELLA
    list<Action> plan;
    bool go_point;
    bool ejecutando_plan;

    //DESCUBRIR MAPA AL VER EL PK
    int topn, tope, tops, topo;
    std::vector< std::vector< unsigned char> > mapa_aux;
    std::pair <int, int> pos_aux;

    //MISIONES
    bool go_regalo;
    set<pair<int, int> > reyes_seen;
    bool goto_rey;
};

class Nodo {
    friend class AEstrella;
private:
    pair<int, int> pos;
    int h;
    int g;
    int f;
    Nodo* padre;

public:

    Nodo(const pair<int, int> &p_pos, int g_p, const pair<int, int> p_obj, Nodo* p_padre);

    bool operator==(const Nodo& otro);
    
    int distancia(const pair <int, int> &a, const pair <int, int> &b);

    void imp_nodo();
};

struct mycomparison {

    bool operator()(const Nodo* lhs, const Nodo* rhs) {
        return (lhs->g+lhs->h)<(rhs->g+rhs->h);
    }
};

class AEstrella {
private:
    std::vector< std::vector< unsigned char> > mapa_t;
    std::vector< std::vector< unsigned char> > mapa_s;
    Nodo* inicio;
    Nodo* fin;
    multiset<Nodo*, mycomparison > abierta;
    list<Nodo*> cerrada;
    int ori;
    bool can;
    bool sol_en, fallo;
    bool go_rey;
public:
    AEstrella(Nodo* p_ini, Nodo* p_fin, const vector< std::vector< unsigned char> > &p_mapa, const vector< std::vector< unsigned char> > &p_mapas, bool p_go_rey, int ori_p);

    list<Nodo*> getVecinos(Nodo* n);
    bool solucionEncontrada();
    void f_menor();
    bool enLista(const Nodo* n, const list<Nodo*> &lista);
    bool enLista(const Nodo* n, const multiset<Nodo*, mycomparison> &lista);
    void ruta(Nodo* select, const list<Nodo*> &vecinos_select);
    void buscar();
    list<pair<int, int> > camino();
    bool getCan();
    void imp_lista(const list<Nodo*> &list);
    void imp_lista(const multiset<Nodo*, mycomparison> &list);
};



#endif
