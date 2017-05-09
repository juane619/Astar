#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include <cstdlib>
#include <list>
#include <set>
#include <iostream>
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

    pair<int, int> getCercano(const set<pair< int, int> > &p);

    void pathFinding(const pair<int, int> &pos_orig, const pair<int, int> &point_objetivo, Action &act, const Sensores &sens, const vector< std::vector< unsigned char> > &p_mapa, const vector< std::vector< unsigned char> > &p_mapas, bool go_rey);

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


public:
    pair<int, int> pos;
    int h;
    int g;
    int f;
    Nodo* padre;

    Nodo(const pair<int, int> &p_pos, const pair<int, int> p_obj = pair<int, int>(0, 0), Nodo* p_padre = nullptr) {
        pos.first = p_pos.first;
        pos.second = p_pos.second;
        padre = p_padre;
        h = distancia(pos, p_obj);

        if (padre == nullptr)
            g = 0;
        else
            g = padre->g + 1;

        f = g + h;
    }

    int distancia(const pair <int, int> &a, const pair <int, int> &b) {
        return abs(a.first - b.first) + abs(a.second - b.second);
    }

    void imp_nodo() {
        cout << pos.first << " " << pos.second << " " << g << " " << h << " " << f << " padre: ";
        if (padre == nullptr)
            cout << "null" << endl;
        else
            cout << (padre->pos).first << " " << (padre->pos).second << endl;
    }


};

class AEstrella {
private:
    std::vector< std::vector< unsigned char> > mapa_t;
    std::vector< std::vector< unsigned char> > mapa_s;
    Nodo* inicio;
    Nodo* fin;
    list<Nodo*> abierta;
    list<Nodo*> cerrada;
    bool can;
    bool go_rey;
public:

    AEstrella(Nodo* p_ini, Nodo* p_fin, const vector< std::vector< unsigned char> > &p_mapa, const vector< std::vector< unsigned char> > &p_mapas, bool p_go_rey) {
        mapa_t = p_mapa;
        mapa_s = p_mapas;
        inicio = p_ini;
        go_rey = p_go_rey;
        fin = p_fin;
        can = true;

        cerrada.push_back(inicio);

        list<Nodo*> vecinos_ini = getVecinos(inicio);
        
        //        cout << vecinos_ini.size() << endl;

        if (can) {
            for (auto it = vecinos_ini.begin(); it != vecinos_ini.end(); it++)
                abierta.push_back(*it);
        }

        //cout << abierta.size() << endl;
//cout << "LLega1.." << endl;
        while (objetivo()) {
            //cout << "LLega1.." << endl;
            if (!buscar())
                break;
            //cout << "LLega2.." << endl;
        }
//        cout << "LLega22.." << endl;
    }

    list<Nodo*> getVecinos(Nodo* n) {
        list<Nodo*> vecinos;

        if (mapa_t.size() > 101) {
            if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), fin->pos, n));

            if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), fin->pos, n));

        } else if (!go_rey) {
            //cout << mapa_s[n->pos.first][n->pos.second + 1] << " " << mapa_s[n->pos.first][n->pos.second -1] << " " << mapa_s[n->pos.first+1][n->pos.second] << " " << mapa_s[n->pos.first-1][n->pos.second] << endl;

            if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P' && mapa_s[n->pos.first + 1][n->pos.second] != 'r' && mapa_t[n->pos.first + 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), fin->pos, n));

            if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P' && mapa_s[n->pos.first - 1][n->pos.second] != 'r' && mapa_t[n->pos.first - 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'P' && mapa_s[n->pos.first][n->pos.second - 1] != 'r' && mapa_t[n->pos.first][n->pos.second - 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'P' && mapa_s[n->pos.first][n->pos.second + 1] != 'r' && mapa_t[n->pos.first][n->pos.second + 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), fin->pos, n));
        } else {
            if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P' && mapa_t[n->pos.first + 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), fin->pos, n));

            if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P' && mapa_t[n->pos.first - 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'P' && mapa_t[n->pos.first][n->pos.second - 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'P' && mapa_t[n->pos.first][n->pos.second + 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), fin->pos, n));
        }
        /*PUERTAS ANULADAS
         if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P' && mapa_t[n->pos.first + 1][n->pos.second] != 'D')
            vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), fin->pos, n));

        if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P' && mapa_t[n->pos.first - 1][n->pos.second] != 'D')
            vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), fin->pos, n));

        if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'P' && mapa_t[n->pos.first][n->pos.second - 1] != 'D')
            vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), fin->pos, n));

        if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'P' && mapa_t[n->pos.first][n->pos.second + 1] != 'D')
            vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), fin->pos, n));*/

        if (vecinos.size() > 0) {
            can = true;
        } else {
            can = false;
        }

        return vecinos;
    }

    bool objetivo() {
        for (auto it = abierta.begin(); it != abierta.end(); it++)
            if (fin->pos.first == (*it)->pos.first && fin->pos.second == (*it)->pos.second)
                return false;
        return true;
    }

    bool f_menor() {
        
        if (abierta.size() > 0) {
            Nodo* menor = abierta.front();

            int i = 0;

            auto it = abierta.begin();
            auto it_del = abierta.begin();

            it++;

            for (; it != abierta.end(); it++, i++)
                if ((*it)->f < menor->f) {
                    menor = *it;
                    it_del = it;
                }

            cerrada.push_back(menor);
            it_del = abierta.erase(it_del);
            return true;
        } else
            return false;
    }

    bool enLista(const Nodo* n, const list<Nodo*> &lista) {
        for (auto it = lista.begin(); it != lista.end(); it++)
            if (n->pos.first == (*it)->pos.first && n->pos.second == (*it)->pos.second)
                return true;

        return false;
    }

    void ruta(Nodo* select, const list<Nodo*> &vecinos_select) {
        //cout << vecinos_select.size() << endl;
        for (auto it = vecinos_select.begin(); it != vecinos_select.end(); it++) {
            if (enLista(*it, cerrada)) {
                //cout << "En cerrada ya..\n";
                continue;
            } else if (!enLista(*it, abierta)) {
                //cout << "Añadiendo a abierta..\n";
                abierta.push_front(*it);
            } else { //Si esta en la lista abierta
                //cout << "En abierta ya..\n";
                if ((select->g + 1) < (*it)->g) {
                    for (auto itaux = abierta.begin(); itaux != abierta.end(); itaux++) {
                        if ((*itaux)->pos.first == (*it)->pos.first && (*itaux)->pos.second == (*it)->pos.second) {
                            
                            itaux = abierta.erase(itaux);
                            
                            abierta.push_front(*it);
                            break;
                        }
                    }
                }
            }
        }
    }

    bool buscar() {
        //cout << "LLega1.." << endl;
        if (f_menor()) { //Pasa el elemento con f menor de la lista abierta a la cerrada
            //cout << "LLega2.." << endl;
            if (cerrada.size() == 0) {
                //cout << "problema1.." << endl;
                can = false;
                return false;
            }

            Nodo* select = cerrada.back();
            list<Nodo*> vecinos = getVecinos(select);

            if (can) {
                //cout << "LLega1.." << endl;
                //cout << "LLega2.." << endl;
                ruta(select, vecinos);
                //cout << "LLega2.." << endl;
                return true;
            } else {
                //cout << "problema 2.." << endl;
                return false;
            }
        } else {
            //cout << "problema 3.." << endl;
            can = false;
            return false;
        }
    }

    list<pair<int, int> > camino() {
        list<pair<int, int> > cam;
        Nodo* objetivo = nullptr;
        for (auto itaux = abierta.begin(); itaux != abierta.end(); itaux++) {
            if ((*itaux)->pos.first == fin->pos.first && (*itaux)->pos.second == fin->pos.second) {
                objetivo = *itaux;
            }
        }

        if (objetivo != nullptr) {
            while (objetivo->padre != nullptr) {
                cam.push_back(objetivo->pos);
                objetivo = objetivo->padre;
            }

            cam.reverse();

        }
        return cam;
    }

    bool getCan() {
        return can;
    }

    void imp_lista(const list<Nodo*> &list) {
        for (auto it = list.begin(); it != list.end(); it++)
            (*it)->imp_nodo();

        cout << endl << endl;
    }
};



#endif
