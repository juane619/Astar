#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>

using namespace std;

/*CLASE NODO*/
Nodo::Nodo(const pair<int, int> &p_pos, int g_p, const pair<int, int> p_obj = pair<int, int>(0, 0), Nodo* p_padre = nullptr) {
    pos.first = p_pos.first;
    pos.second = p_pos.second;
    padre = p_padre;
    h = distancia(pos, p_obj);

    if (padre == nullptr)
        g = 0;
    else
        g = padre->g + g_p;

    f = g + h;
}

bool Nodo::operator==(const Nodo& otro) {
    return otro.pos.first == this->pos.first && otro.pos.second == this->pos.second;
}

int Nodo::distancia(const pair <int, int> &a, const pair <int, int> &b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

void Nodo::imp_nodo() {
    cout << pos.first << " " << pos.second << " " << g << " " << h << " " << f << " padre: ";
    if (padre == nullptr)
        cout << "null" << endl;
    else
        cout << (padre->pos).first << " " << (padre->pos).second << endl;
}

/*CLASE AESTRELLA*/
AEstrella::AEstrella(Nodo* p_ini, Nodo* p_fin, const vector< std::vector< unsigned char> > &p_mapa, const vector< std::vector< unsigned char> > &p_mapas, bool p_go_rey, int ori_p) {
    mapa_t = p_mapa;
    mapa_s = p_mapas;
    inicio = p_ini;
    fin = p_fin;
    go_rey = p_go_rey;
    ori = ori_p;
    can = true;
    sol_en = false;
    fallo = false;

    cerrada.push_back(inicio);

    list<Nodo*> vecinos_ini = getVecinos(inicio);

    //cout << vecinos_ini.size() << endl;
    
    if (can){
        for (auto it = vecinos_ini.begin(); it != vecinos_ini.end(); it++){
            (*it)->imp_nodo();
            abierta.insert(*it);
        }
    }
    
    imp_lista(abierta);
    cin.get();
    //cout << abierta.size() << endl;
    //cout << "LLega1.." << endl;
    while (!fallo && !sol_en) {
        if (abierta.empty()) {
            fallo = true;
            break;
        }

        //cout << "LLega2.." << endl;
        buscar();

        if (solucionEncontrada()) {
            sol_en = true;
            break;
        }
        //cout << "LLega3.." << endl;
    }
    //        cout << "LLega22.." << endl;
}

list<Nodo*> AEstrella::getVecinos(Nodo* n) {
    list<Nodo*> vecinos;

    if (ori == 0 || ori == 2) {
        if (mapa_t.size() > 101) {
            if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), 1, fin->pos, n));

            if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), 1, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), 3, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), 3, fin->pos, n));

        } else if (!go_rey) {
            //cout << mapa_s[n->pos.first][n->pos.second + 1] << " " << mapa_s[n->pos.first][n->pos.second -1] << " " << mapa_s[n->pos.first+1][n->pos.second] << " " << mapa_s[n->pos.first-1][n->pos.second] << endl;

            if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P' && mapa_s[n->pos.first + 1][n->pos.second] != 'r' && mapa_t[n->pos.first + 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), 1, fin->pos, n));

            if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P' && mapa_s[n->pos.first - 1][n->pos.second] != 'r' && mapa_t[n->pos.first - 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), 1, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'P' && mapa_s[n->pos.first][n->pos.second - 1] != 'r' && mapa_t[n->pos.first][n->pos.second - 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), 3, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'P' && mapa_s[n->pos.first][n->pos.second + 1] != 'r' && mapa_t[n->pos.first][n->pos.second + 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), 3, fin->pos, n));
        } else {
            if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P' && mapa_t[n->pos.first + 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), 1, fin->pos, n));

            if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P' && mapa_t[n->pos.first - 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), 1, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'P' && mapa_t[n->pos.first][n->pos.second - 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), 3, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'P' && mapa_t[n->pos.first][n->pos.second + 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), 3, fin->pos, n));
        }
    } else {
        if (mapa_t.size() > 101) {
            if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), 3, fin->pos, n));

            if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), 3, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), 1, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'P')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), 1, fin->pos, n));

        } else if (!go_rey) {
            //cout << mapa_s[n->pos.first][n->pos.second + 1] << " " << mapa_s[n->pos.first][n->pos.second -1] << " " << mapa_s[n->pos.first+1][n->pos.second] << " " << mapa_s[n->pos.first-1][n->pos.second] << endl;

            if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P' && mapa_s[n->pos.first + 1][n->pos.second] != 'r' && mapa_t[n->pos.first + 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), 3, fin->pos, n));

            if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P' && mapa_s[n->pos.first - 1][n->pos.second] != 'r' && mapa_t[n->pos.first - 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), 3, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'P' && mapa_s[n->pos.first][n->pos.second - 1] != 'r' && mapa_t[n->pos.first][n->pos.second - 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), 1, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'P' && mapa_s[n->pos.first][n->pos.second + 1] != 'r' && mapa_t[n->pos.first][n->pos.second + 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), 1, fin->pos, n));
        } else {
            if (mapa_t[n->pos.first + 1][n->pos.second] != 'M' && mapa_t[n->pos.first + 1][n->pos.second] != 'B' && mapa_t[n->pos.first + 1][n->pos.second] != 'A' && mapa_t[n->pos.first + 1][n->pos.second] != 'P' && mapa_t[n->pos.first + 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first + 1, n->pos.second), 3, fin->pos, n));

            if (mapa_t[n->pos.first - 1][n->pos.second] != 'M' && mapa_t[n->pos.first - 1][n->pos.second] != 'B' && mapa_t[n->pos.first - 1][n->pos.second] != 'A' && mapa_t[n->pos.first - 1][n->pos.second] != 'P' && mapa_t[n->pos.first - 1][n->pos.second] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first - 1, n->pos.second), 3, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second - 1] != 'M' && mapa_t[n->pos.first][n->pos.second - 1] != 'A' && mapa_t[n->pos.first][n->pos.second - 1] != 'B' && mapa_t[n->pos.first][n->pos.second - 1] != 'P' && mapa_t[n->pos.first][n->pos.second - 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second - 1), 1, fin->pos, n));

            if (mapa_t[n->pos.first][n->pos.second + 1] != 'M' && mapa_t[n->pos.first][n->pos.second + 1] != 'A' && mapa_t[n->pos.first][n->pos.second + 1] != 'B' && mapa_t[n->pos.first][n->pos.second + 1] != 'P' && mapa_t[n->pos.first][n->pos.second + 1] != '?')
                vecinos.push_back(new Nodo(pair<int, int>(n->pos.first, n->pos.second + 1), 1, fin->pos, n));
        }
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

    if (!vecinos.empty()) {
        can = true;
    } else {
        can = false;
    }

    return vecinos;
}

bool AEstrella::solucionEncontrada() {
    for (auto it = abierta.begin(); it != abierta.end(); it++) {
        if (fin == (*it))
            return true;
    }
    return false;
}

void AEstrella::f_menor() { //mover mejor nodo de abiertos a cerrados
    cerrada.push_back(*abierta.begin());
    abierta.erase(abierta.begin());
}

bool AEstrella::enLista(const Nodo* n, const list<Nodo*> &lista) {
    for (auto it = lista.begin(); it != lista.end(); it++)
        if (n == (*it))
            return true;

    return false;
}

bool AEstrella::enLista(const Nodo* n, const multiset<Nodo*, mycomparison> &lista) {
    for (auto it = lista.begin(); it != lista.end(); it++)
        if (n == (*it))
            return true;

    return false;
}

void AEstrella::ruta(Nodo* select, const list<Nodo*> &vecinos_select) {
    //cout << vecinos_select.size() << endl;
    for (auto it = vecinos_select.begin(); it != vecinos_select.end(); it++) {
        if (enLista(*it, cerrada)) {
            //cout << "En cerrada ya..\n";
            continue;
        } else if (!enLista(*it, abierta)) {
            //cout << "Añadiendo a abierta..\n";
            abierta.insert(*it);
        } else { //Si esta en la lista abierta
            //cout << "En abierta ya..\n";
            for (auto itaux = abierta.begin(); itaux != abierta.end(); itaux++) {
                if ((*it) == (*itaux)) {
                    if ((*it)->g < (*itaux)->g) {
                        itaux = abierta.erase(itaux);
                        abierta.insert(*it);
                        break;
                    }
                }
            }
        }
    }
}

void AEstrella::buscar() {
    //cout << "LLega1.." << endl;
    f_menor(); //Pasa el elemento con f menor de la lista abierta a la cerrada
    //cout << "LLega2.." << endl;
    Nodo* select = cerrada.back();
    
    list<Nodo*> vecinos = getVecinos(select);
    //imp_lista(vecinos);
    //cin.get();
    if (!vecinos.empty())
        ruta(select, vecinos);
    //cout << "LLega2.." << endl;
}

list<pair<int, int> > AEstrella::camino() {
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

bool AEstrella::getCan() {

    return can;
}

void AEstrella::imp_lista(const list<Nodo*> &list) {

    for (auto it = list.begin(); it != list.end(); it++)
        (*it)->imp_nodo();

    cout << endl << endl;
}

void AEstrella::imp_lista(const multiset<Nodo*, mycomparison> &list) {

    for (auto it = list.begin(); it != list.end(); it++)
        (*it)->imp_nodo();

    cout << endl << endl;
}

/*COnstructor por defecto*/
ComportamientoJugador::ComportamientoJugador(unsigned int size) : Comportamiento(size) {
    /*Rellenamos los precipicios que ya sabemos donde estan*/
    std::vector< unsigned char> pre1(size, 'P');
    mapaResultado[0] = pre1;
    mapaResultado[1] = pre1;
    mapaResultado[2] = pre1;

    for (int i = 3; i < 97; i++) {
        for (int j = 0; j < 3; j++)
            mapaResultado[i][j] = 'P';
        for (int k = 97; k < 100; k++)
            mapaResultado[i][k] = 'P';
    }

    mapaResultado[97] = pre1;
    mapaResultado[98] = pre1;
    mapaResultado[99] = pre1;
    ////////////////////////////////////////////////////////////////

    //USO GENERAL
    brujula = 0;
    last_accion = actIDLE;
    mochila = 0;
    pos_actual.first = 99;
    pos_actual.second = 99;
    hay_bosque = false;
    hay_agua = false;
    hay_puertas = false;
    hay_reyes = false;
    hay_regalos = false;
    pasos_totales = 0;
    cogiendo_obj = false;
    tirando_obj = false;
    cont_lobo = 0;
    cont_aldeano = 0;
    cont_puertas = 0;
    cont_reyes = 0;
    cont_tira_objeto = 0;
    puertas = 0; //Controlamos si estamos dentro de puerta o no

    //OBJETOS
    tengo_regalo = 0;
    tengo_biquini = false;
    tengo_zapatillas = false;
    tengo_llave = false;
    tengo_hueso = false;
    suelto_objeto = false;


    //ENCONTRAR_PK
    pk_found = false;
    pk_orientation = -1;
    go_pk = false;


    //GIRAR CORRECTAMENTE
    giro_norte = actTURN_L;
    giro_sur = actTURN_R;
    giro_oeste = actTURN_R;
    giro_este = actTURN_L;
    avanzando = 0;
    girando = false;
    debe_avanzar = false;
    contador = 1 + rand() % (11 - 1);
    contador_mismo_objeto = 0;
    cont_misma_pos = 0;
    atrancado = false;

    //DESCUBRIR MAPA AL VER PK
    topn = tope = topo = tops = 99;

    /*Rellenamos mapa_aux*/
    std::vector< unsigned char> aux(200, '?');

    for (unsigned int i = 0; i < 200; i++) {

        mapa_aux.push_back(aux);
    }
    /////

    pos_aux.first = 99;
    pos_aux.second = 99;

    //A ESTRELLA
    go_regalo = false;
    go_point = false;
    ejecutando_plan = false;
    goto_rey = false;
    cont_plan = 0;
}
/******************************************************************************/

/*********************DETECTAR Y ENCONTRAR PK**********************************/

//ver si tenemos en nuestra vision un PK
bool tengoObjeto(unsigned char obj);

int ComportamientoJugador::see_pk(const std::vector< unsigned char> &t, const std::vector< unsigned char> &s, const Sensores &sens) {
    int pos = -1;
    bool encontrado = false;

    for (unsigned i = 1; i < 16 && !encontrado; i++) {
        if (i == 2)
            continue;

        if (!pk_found) {
            if (t[i] == 'K') {
                go_point = true;
                go_pk = true;
                encontrado = true;
                pos = i;
            }
        } else {
            //cout << "Go objeto.." << endl;
            if (s[i] == 'r' && tengoObjeto('4')) {
                //cout << "Go rey.." << endl;
                encontrado = true;
                pos = i;
                go_point = true;
                goto_rey = true;
                //ejecutando_plan = true;
            }
            if (!encontrado && t[i] == 'D' && (pasos_totales > 2400 && tengoObjeto('3')) && cont_puertas == 0 && (sens.vida > 120 || puertas % 2 != 0)) {
                //cout << "go puerta.." << endl;
                encontrado = true;
                pos = i;
                go_point = true;
            }
            if (!encontrado && sens.vida > 120) {
                if (s[i] == '3' && hay_puertas && pasos_totales > 2400 && !tengo_llave) {
                    //cout << "GO llave.." << endl;
                    encontrado = true;
                    pos = i;
                    go_point = true;
                }
                if (!encontrado && s[i] == '2' && hay_bosque && !tengo_zapatillas) {
                    //cout << "Go ZAPA.." << endl;
                    encontrado = true;
                    pos = i;
                    go_point = true;
                }
                if (!encontrado && s[i] == '1' && hay_agua && !tengo_biquini) {
                    //cout << "Go biquini.." << endl;
                    encontrado = true;
                    go_point = true;
                    pos = i;
                }
                if (!encontrado && s[i] == '4' && hay_reyes && !tengo_regalo) {
                    //cout << "Go regalo.." << endl;

                    encontrado = true;
                    go_point = true;
                    pos = i;
                }
            }
        }
    }

    return pos;
}

/*Ver si el pk esta a la izquierda o a la derecha
 devuelve: 0->norte ; 1->oeste; 2->este*/
unsigned get_pk_orientation(int pos_pk) {
    if (pos_pk == 1 || pos_pk == 4 || pos_pk == 5 || pos_pk == 9 || pos_pk == 10 || pos_pk == 11)
        return 1;
    else if (pos_pk == 3 || pos_pk == 7 || pos_pk == 8 || pos_pk == 13 || pos_pk == 14 || pos_pk == 15)
        return 2;

    else
        return 0;
}

/*Guardamos informacion relevante de lo que vemos*/
void ComportamientoJugador::save_inf(const std::vector< unsigned char> &t, const std::vector< unsigned char> &s) {
    bool encontrado = false;

    for (unsigned i = 0; i < 16 && !encontrado; i++) {
        if (t[i] == 'D') {
            hay_puertas = true;
            //if (pk_found)
            //puertas_seen.insert(make_point(i, get_pk_orientation(i)));
        } else if (t[i] == 'B')
            hay_bosque = true;
        else if (t[i] == 'A')
            hay_agua = true;
        else if (s[i] == 'r') {
            hay_reyes = true;
            //go_point = true;
            if (pk_found)
                reyes_seen.insert(make_point(i, get_pk_orientation(i), pos_actual));
        } else if (s[i] == '4') {

            hay_regalos = true;
        }
    }
}

/*DIstancia real entre dos tuplas*/
int distancia_r(const pair <int, int> &a, const pair <int, int> &b) {

    return abs(a.first - b.first) + abs(a.second - b.second);
}

/*Obtener regalo mas cercano*/
pair<int, int> ComportamientoJugador::getCercano(const vector<pair< int, int> > &p) {
    pair<int, int> cerca = *p.begin();
    auto it = p.begin();
    it++;

    for (; it != p.end(); it++) {

        if (distancia_r(pos_actual, *it) < distancia_r(pos_actual, cerca))
            cerca = *it;
    }


    return cerca;
}

/*Obtener regalo mas cercano*/
pair<int, int> ComportamientoJugador::getCercano(const set<pair< int, int> > &p) {
    pair<int, int> cerca = *p.begin();
    auto it = p.begin();
    it++;

    for (; it != p.end(); it++) {

        if (distancia_r(pos_actual, *it) < distancia_r(pos_actual, cerca))
            cerca = *it;
    }

    return cerca;
}

/*Obtener plan de acciones a realizar*/
bool ComportamientoJugador::planing(const list<pair<int, int> > &pares, const pair<int, int> &pos_actuall) {
    plan.clear();
    pair<int, int> pos_aux = pos_actuall;
    int bruj_aux = brujula;

    for (auto it = pares.begin(); it != pares.end(); it++) {
        while (pos_aux.first != it->first) {
            if (it->first > pos_aux.first) {
                if (bruj_aux == 1) {
                    plan.push_back(actTURN_R);
                    bruj_aux = 2;
                } else if (bruj_aux == 3) {
                    plan.push_back(actTURN_L);
                    bruj_aux = 2;
                } else if (bruj_aux == 0) {
                    plan.push_back(actTURN_R);
                    plan.push_back(actTURN_R);
                    bruj_aux = 2;
                } else {
                    pos_aux.first++;
                    plan.push_back(actFORWARD);
                }
            } else if (it->first < pos_aux.first) {
                if (bruj_aux == 1) {
                    plan.push_back(actTURN_L);
                    bruj_aux = 0;
                } else if (bruj_aux == 3) {
                    plan.push_back(actTURN_R);
                    bruj_aux = 0;
                } else if (bruj_aux == 2) {
                    plan.push_back(actTURN_R);
                    plan.push_back(actTURN_R);
                    bruj_aux = 0;
                } else {
                    pos_aux.first--;
                    plan.push_back(actFORWARD);
                }
            }
        }
        while (pos_aux.second != it->second) {
            if (it->second > pos_aux.second) {
                if (bruj_aux == 0) {
                    plan.push_back(actTURN_R);
                    bruj_aux = 1;
                } else if (bruj_aux == 2) {
                    plan.push_back(actTURN_L);
                    bruj_aux = 1;
                } else if (bruj_aux == 3) {
                    plan.push_back(actTURN_R);
                    plan.push_back(actTURN_R);
                    bruj_aux = 1;
                } else {
                    pos_aux.second++;
                    plan.push_back(actFORWARD);
                }
            } else if (it->second < pos_aux.second) {
                if (bruj_aux == 0) {
                    plan.push_back(actTURN_L);
                    bruj_aux = 3;
                } else if (bruj_aux == 2) {
                    plan.push_back(actTURN_R);
                    bruj_aux = 3;
                } else if (bruj_aux == 1) {
                    plan.push_back(actTURN_R);
                    plan.push_back(actTURN_R);
                    bruj_aux = 3;
                } else {

                    pos_aux.second--;
                    plan.push_back(actFORWARD);
                }
            }
        }
    }
    return true;
}

/*Ejecutar plan*/
bool es_object(unsigned char i);

void ComportamientoJugador::ejecutaPlan(Action &act, const Sensores &sens) {
    //cout << "Ejecutando plan" << endl;
    if (plan.size() > 0) {
        act = plan.front();
        if (act == actFORWARD) {
            if (sens.terreno[2] == 'P') {
                plan.clear();
                act = girarPre();
            } else if (sens.terreno[2] == 'A') {
                plan.clear();
                act = girar(sens);
            } else if (sens.terreno[2] == 'M') {
                plan.clear();
                act = girar(sens);
            } else if (sens.superficie[2] == 'a') {
                act = actIDLE;
                plan.push_front(actFORWARD);
            } else if (sens.colision && go_pk && sens.superficie[2] == 'd') {
                plan.push_front(actIDLE);
                act = girar(sens);
            } else if (sens.colision && sens.superficie[2] == 'd' && !tengo_llave) {
                //                
                if (go_regalo && sens.regalos.size() > 1) {
                    cont_regalos = ((cont_regalos + 1) % sens.regalos.size());
                    pathFinding(pos_actual, sens.regalos[cont_regalos], act, sens, mapaResultado, mapaEntidades, false, brujula);
                } else if (goto_rey && reyes_seen.size() > 1) {
                    cont_reyes = ((cont_reyes + 1) % reyes_seen.size());
                    pair<int, int> nuevo_objetivo;
                    int i = 0;

                    for (auto it = reyes_seen.begin(); it != reyes_seen.end(), i < cont_reyes; i++, it++)
                        nuevo_objetivo = *(it);

                    //cout << "Problema 2 .. " << hay_llaves << " " << cont_reyes << " " << nuevo_objetivo.first << " " << nuevo_objetivo.second << endl;

                    pathFinding(pos_actual, nuevo_objetivo, act, sens, mapaResultado, mapaEntidades, true, brujula);
                } else {
                    //cout << "Problema 3.." << endl;
                    act = girar(sens);
                    plan.clear();
                }
                //}
            } else if (sens.colision && sens.superficie[2] == 'd' && tengoObjeto('3')) {
                if (sens.objetoActivo == '3') {
                    plan.push_back(actFORWARD);
                    act = actGIVE;
                } else
                    equipaObjeto('3', act, sens);

            } else if (!sens.colision && sens.terreno[2] == 'D' && tengoObjeto('3')) {
                if (sens.objetoActivo != '3')
                    equipaObjeto('3', act, sens);
                else {
                    act = actFORWARD;
                }
            }//            else if (sens.superficie[2] == '4' && !tengoObjeto('4')) {
                //                cogeObjeto('4', act, sens);
                //                if (sens.objetoActivo == '4')
                //                    plan.push_front(actFORWARD);
                //            }
                //            else if (sens.colision && (sens.superficie[2]=='4')) {
                //                cogeObjeto(sens.superficie[2], act, sens);
                //                if (sens.objetoActivo == sens.superficie[2])
                //                    plan.push_front(actFORWARD);
                //            } 
            else {
                //cout << "LLega 1.." << endl;
                //ejecutando_plan = false;
                plan.pop_front();
                //cout << "LLega 2.." << endl;
            }
        } else {
            //cout << "LLega 2.." << endl;
            plan.pop_front();
        }
    }
    if (plan.size() == 0) {

        goto_rey = false;
        go_regalo = false;
        ejecutando_plan = false;
        //cout << "PAramos de ejecutar.." << endl;
        cont_plan = 10;
    }
}

void PintaPlan(list<Action> plan) {
    // cout << "Pintando plan.. " << endl;
    auto it = plan.begin();
    while (it != plan.end()) {
        if (*it == actFORWARD) {
            cout << "A ";
        } else if (*it == actTURN_R) {
            cout << "D ";
        } else if (*it == actTURN_L) {
            cout << "I ";
        } else {

            cout << "- ";
        }
        it++;
    }
    cout << endl;
}

Action girar(const Sensores & s);

/*PATHFINDING: A ESTRELLA*/
void ComportamientoJugador::pathFinding(const pair<int, int> &pos_orig, const pair<int, int> &point_objetivo, Action &act, const Sensores &sens, const vector< std::vector< unsigned char> > &p_mapa, const vector< std::vector< unsigned char> > &p_mapas, bool go_rey, int ori_p) {
    if (cont_plan == 0) {
        cout << "Llamando a PATHFINDING.." << endl;

        cout << point_objetivo.first << "  " << point_objetivo.second << endl;
        cont_plan = 0;

        Nodo orig(pos_orig, 0, point_objetivo);
        Nodo dest(point_objetivo, 0);
        cout << "Llega 1.." << endl;
        AEstrella aprueba(&orig, &dest, p_mapa, p_mapas, go_rey, ori_p);
        cout << "Llega 2.." << endl;
        if (aprueba.getCan()) {
            //cout << "PLanificamos camino.." << endl;
            list<pair<int, int> > cam = aprueba.camino();

            ejecutando_plan = planing(cam, pos_orig);

            act = plan.front();

            if (act == actFORWARD) {
                if (sens.terreno[2] == 'P') {
                    act = girarPre();
                } else if (sens.terreno[2] == 'A') {
                    act = girar(sens);
                } else if (sens.colision && go_regalo) {
                    //cout << "PLANIFICANDO: colision ejecutando NO  PUERTA chocando.." << sens.regalos.size() << " " << cont_regalos << endl;
                    if (sens.regalos.size() > 1) {
                        cont_regalos = ((cont_regalos + 1) % sens.regalos.size());
                    }
                    //ejecutando_plan=false;
                }
            }

            if (plan.size() > 0)
                plan.pop_front();
        }
    }
}

/*Obtener el punto en el mapa de algo divisado*/
pair<int, int> ComportamientoJugador::make_point(int pos_elem, int ori_elem, const pair<int, int> &pos_actuall) {
    pair<int, int> point(pos_actuall);

    //Si vamos al norte
    if (brujula == 0) {
        //Obtenermos la columna
        if (ori_elem == 1) {
            if (pos_elem > 8)
                point.second = pos_actuall.second - (12 - pos_elem);
            else if (pos_elem > 3)
                point.second = pos_actuall.second - (6 - pos_elem);
            else
                point.second = pos_actuall.second - 1;
        } else if (ori_elem == 2) {
            if (pos_elem > 12)
                point.second = pos_actuall.second + (pos_elem - 12);
            else if (pos_elem > 6)
                point.second = pos_actuall.second + (pos_elem - 6);
            else if (pos_elem > 0)
                point.second = pos_actuall.second + 1;
        }
        //Obtenemos la fila
        if (pos_elem > 8)
            point.first = pos_actuall.first - 3;
        else if (pos_elem > 3)
            point.first = pos_actuall.first - 2;
        else if (pos_elem > 0)
            point.first = pos_actuall.first - 1;

    } else if (brujula == 1) { //Si vamos al este
        //Obtenermos la fila
        if (ori_elem == 1) {
            if (pos_elem > 8)
                point.first = pos_actuall.first - (12 - pos_elem);
            else if (pos_elem > 3)
                point.first = pos_actuall.first - (6 - pos_elem);
            else if (pos_elem > 0)
                point.first = pos_actuall.first - 1;
        } else if (ori_elem == 2) {
            if (pos_elem > 12)
                point.first = pos_actuall.first + (pos_elem - 12);
            else if (pos_elem > 6)
                point.first = pos_actuall.first + (pos_elem - 6);
            else if (pos_elem > 0)
                point.first = pos_actuall.first + 1;
        }
        //Obtenemos la columna
        if (pos_elem > 8)
            point.second = pos_actuall.second + 3;
        else if (pos_elem > 3)
            point.second = pos_actuall.second + 2;
        else if (pos_elem > 0)
            point.second = pos_actuall.second + 1;

    } else if (brujula == 2) { //Si vamos al sur
        //Obtenermos la columna
        if (ori_elem == 1) {
            if (pos_elem > 8)
                point.second = pos_actuall.second + (12 - pos_elem);
            else if (pos_elem > 3)
                point.second = pos_actuall.second + (6 - pos_elem);
            else if (pos_elem > 0)
                point.second = pos_actuall.second + 1;
        } else if (ori_elem == 2) {
            if (pos_elem > 12)
                point.second = pos_actuall.second - (pos_elem - 12);
            else if (pos_elem > 6)
                point.second = pos_actuall.second - (pos_elem - 6);
            else if (pos_elem > 0)
                point.second = pos_actuall.second - 1;
        }
        //Obtenemos la fila
        if (pos_elem > 8)
            point.first = pos_actuall.first + 3;
        else if (pos_elem > 3)
            point.first = pos_actuall.first + 2;
        else if (pos_elem > 0)
            point.first = pos_actuall.first + 1;

    } else { //Si vamos al oeste
        //Obtenermos la fila
        if (ori_elem == 1) {
            if (pos_elem > 8)
                point.first = pos_actuall.first + (12 - pos_elem);
            else if (pos_elem > 3)
                point.first = pos_actuall.first + (6 - pos_elem);
            else if (pos_elem > 0)
                point.first = pos_actuall.first + 1;
        } else if (ori_elem == 2) {
            if (pos_elem > 12)
                point.first = pos_actuall.first - (pos_elem - 12);
            else if (pos_elem > 6)
                point.first = pos_actuall.first - (pos_elem - 6);
            else if (pos_elem > 0)
                point.first = pos_actuall.first - 1;
        }

        //Obtenemos la columna
        if (pos_elem > 8)
            point.second = pos_actuall.second - 3;
        else if (pos_elem > 3)
            point.second = pos_actuall.second - 2;
        else

            if (pos_elem > 0)
            point.second = pos_actuall.second - 1;
    }

    return point;
}

//////////////////////////////////////////////////////////////////////////////////

/********************ACCIONES DE ACTUALIZACION**************************/

void ComportamientoJugador::act_precipicios() {

    std::vector< unsigned char> aux(30, 'P');
    mapaResultado[0] = aux;
}

/*Actualizar mapa una vez encontrado el PK*/
void ComportamientoJugador::act_mapa(std::vector< std::vector< unsigned char> > &mapa, const std::vector< unsigned char> &v, int posx, int posy) {
    mapa[posx][posy] = v[0];

    if (brujula == 0) {
        mapa[posx - 1][posy - 1] = v[1];
        mapa[posx - 1][posy] = v[2];
        mapa[posx - 1][posy + 1] = v[3];
        mapa[posx - 2][posy - 2] = v[4];
        mapa[posx - 2][posy - 1] = v[5];
        mapa[posx - 2][posy] = v[6];
        mapa[posx - 2][posy + 1] = v[7];
        mapa[posx - 2][posy + 2] = v[8];
        mapa[posx - 3][posy - 3] = v[9];
        mapa[posx - 3][posy - 2] = v[10];
        mapa[posx - 3][posy - 1] = v[11];
        mapa[posx - 3][posy] = v[12];
        mapa[posx - 3][posy + 1] = v[13];
        mapa[posx - 3][posy + 2] = v[14];
        mapa[posx - 3][posy + 3] = v[15];
    } else if (brujula == 1) {
        mapa[posx - 1][posy + 1] = v[1];
        mapa[posx][posy + 1] = v[2];
        mapa[posx + 1][posy + 1] = v[3];
        mapa[posx - 2][posy + 2] = v[4];
        mapa[posx - 1][posy + 2] = v[5];
        mapa[posx][posy + 2] = v[6];
        mapa[posx + 1][posy + 2] = v[7];
        mapa[posx + 2][posy + 2] = v[8];
        mapa[posx - 3][posy + 3] = v[9];
        mapa[posx - 2][posy + 3] = v[10];
        mapa[posx - 1][posy + 3] = v[11];
        mapa[posx][posy + 3] = v[12];
        mapa[posx + 1][posy + 3] = v[13];
        mapa[posx + 2][posy + 3] = v[14];
        mapa[posx + 3][posy + 3] = v[15];
    } else if (brujula == 2) {
        mapa[posx + 1][posy + 1] = v[1];
        mapa[posx + 1][posy] = v[2];
        mapa[posx + 1][posy - 1] = v[3];
        mapa[posx + 2][posy + 2] = v[4];
        mapa[posx + 2][posy + 1] = v[5];
        mapa[posx + 2][posy] = v[6];
        mapa[posx + 2][posy - 1] = v[7];
        mapa[posx + 2][posy - 2] = v[8];
        mapa[posx + 3][posy + 3] = v[9];
        mapa[posx + 3][posy + 2] = v[10];
        mapa[posx + 3][posy + 1] = v[11];
        mapa[posx + 3][posy] = v[12];
        mapa[posx + 3][posy - 1] = v[13];
        mapa[posx + 3][posy - 2] = v[14];
        mapa[posx + 3][posy - 3] = v[15];
    } else if (brujula == 3) {

        mapa[posx + 1][posy - 1] = v[1];
        mapa[posx][posy - 1] = v[2];
        mapa[posx - 1][posy - 1] = v[3];
        mapa[posx + 2][posy - 2] = v[4];
        mapa[posx + 1][posy - 2] = v[5];
        mapa[posx][posy - 2] = v[6];
        mapa[posx - 1][posy - 2] = v[7];
        mapa[posx - 2][posy - 2] = v[8];
        mapa[posx + 3][posy - 3] = v[9];
        mapa[posx + 2][posy - 3] = v[10];
        mapa[posx + 1][posy - 3] = v[11];
        mapa[posx][posy - 3] = v[12];
        mapa[posx - 1][posy - 3] = v[13];
        mapa[posx - 2][posy - 3] = v[14];
        mapa[posx - 3][posy - 3] = v[15];
    }
}

/*Descubrir lo visto hasta ver un PK*/
void ComportamientoJugador::revelar(const Sensores & s) {
    topn -= 3;
    tops += 3;
    topo -= 3;
    tope += 3;

    int iaux = topn, jaux = topo;
    int fil = abs((pos_aux.first - s.mensajeF) - topn);
    int col = abs((pos_aux.second - s.mensajeC) - topo);

    for (int i = fil; i <= (s.mensajeF + (tops - pos_aux.first)); i++, iaux++) {
        for (int j = col, jaux = topo; j <= (s.mensajeC + (tope - pos_aux.second)); j++, jaux++) {
            //            cout << "real: " <<  i << " " << j << endl 
            //                    << "aux: " << iaux << " " << jaux << endl << endl;

            if (mapaResultado[i][j] == '?' && mapa_aux[iaux][jaux] != '?')
                mapaResultado[i][j] = mapa_aux[iaux][jaux];
        }
    }
}

/*Actualizamos posicion segun la orientacion, solo cuando hallamos avanzado(precondicion)
 y tambien actualizamos la brujula si hemos girado*/
void ComportamientoJugador::act_pos(std::pair <int, int> &p, const Sensores & sensores) {
    switch (last_accion) {
        case actFORWARD:
            if (!sensores.colision) {
                switch (brujula) {
                    case 0: // Norte
                        p.first--;
                        if (!pk_found)
                            if (p.first < topn)
                                topn = p.first;
                        break;
                    case 1: // Este
                        p.second++;
                        if (!pk_found)
                            if (p.second > tope)
                                tope = p.second;
                        break;
                    case 2: // Sur
                        p.first++;
                        if (!pk_found)
                            if (p.first > tops)
                                tops = p.first;
                        break;
                    case 3: // Oeste
                        p.second--;
                        if (!pk_found)
                            if (p.second < topo)
                                topo = p.second;
                        break;
                }
            }
            break;
        case actTURN_L:
            brujula = (brujula + 3) % 4;
            break;
        case actTURN_R:
            brujula = (brujula + 1) % 4;

            break;
    }
}
/*******************************************************************/

//Asignar contador

int asignar_contador() {

    return 4 + rand() % (10 - 4);
}

/******************Acciones de movimiento lateral***********/

/*Girar cuando hay muro o precipicio*/
Action ComportamientoJugador::girarPre() {
    Action accion;
    if (brujula == 0) {
        girando = true;
        accion = giro_norte;
        if (giro_norte == actTURN_R) {
            giro_sur = actTURN_L;
            giro_este = actTURN_R;
        } else if (giro_norte == actTURN_L) {
            giro_oeste = actTURN_L;
            giro_sur = actTURN_R;
        }
    } else if (brujula == 1) {
        accion = giro_este;
        //if (girando) {
        //girando = false;
        contador = asignar_contador();
        giro_sur = actTURN_R;
        //giro_oeste = actTURN_R;
        giro_norte = actTURN_L;

        if (rand() % 2 == 0)
            giro_este = actTURN_L;
        else
            giro_este = actTURN_R;
        // } else {
        //girando = true;
        //giro_este = actTURN_R;
        //}
    } else if (brujula == 2) {
        girando = true;
        accion = giro_sur;
        if (giro_sur == actTURN_R) {
            giro_norte = actTURN_L;
            giro_oeste = actTURN_R;
        } else if (giro_sur == actTURN_L) {
            giro_este = actTURN_L;
            giro_norte = actTURN_R;
        }
    } else if (brujula == 3) {
        accion = giro_oeste;
        //if (girando) {
        //girando = false;
        contador = asignar_contador();
        giro_sur = actTURN_L;
        giro_norte = actTURN_R;

        if (rand() % 2 == 0)
            giro_oeste = actTURN_L;

        else
            giro_oeste = actTURN_R;
        //giro_este = actTURN_R;
        //} else {
        //girando = true;
        //giro_oeste = actTURN_L;
        //}
    }
    return accion;
}

Action ComportamientoJugador::girar(const Sensores & s) {
    //cout << "girO..\n";
    Action act = actTURN_L;

    if (atrancado) {
        //cout << "atrancado" << endl;
        if (rand() % 2 == 0)
            act = actTURN_R;
        else
            act = actTURN_L;
    } else {

        if ((s.terreno[1] == 'P' || s.terreno[1] == 'M') && (s.terreno[3] == 'P' || s.terreno[3] == 'M') && (s.terreno[5] == 'M' || s.terreno[5] == 'P') && (s.terreno[7] == 'M' || s.terreno[7] == 'P') && (s.terreno[2] != 'M' && s.terreno[2] != 'P'))
            act = actFORWARD;
        else {
            if (s.terreno[2] == 'M') {
                if (brujula == 0) { //Si al azar sale 1, girará para el otro lado
                    if ((1 + rand() % (4 - 1)) == 1)
                        act = giro_sur;
                    else
                        act = giro_norte;
                } else if (brujula == 2) {
                    if ((1 + rand() % (5 - 1)) == 1)
                        act = giro_norte;
                    else
                        act = giro_sur;
                } else {
                    if (rand() % 2 == 0)
                        act = actTURN_R;
                    else
                        act = actTURN_L;
                }
            } else if ((s.terreno[1] != 'S' && s.terreno[1] != 'T') && (s.terreno[3] == 'S' || s.terreno[3] == 'T')) {
                //cout << "Para derecha.." << endl;
                if (debe_avanzar) {
                    act = actFORWARD;
                    debe_avanzar = false;
                } else {
                    act = actTURN_R;
                }
            } else if ((s.terreno[3] != 'S' && s.terreno[3] != 'T') && (s.terreno[1] == 'S' || s.terreno[1] == 'T')) {
                //cout << "Para izquierda.." << endl;
                if (debe_avanzar) {
                    act = actFORWARD;
                    debe_avanzar = false;
                } else
                    act = actTURN_L;
            } else if ((s.terreno[1] != 'S' && s.terreno[1] != 'T') || (s.terreno[3] != 'S' && s.terreno[3] != 'T')) {
                if (s.terreno[2] == 'S' || s.terreno[2] == 'T') {
                    //cout << "debe avanzar.." << endl;
                    act = actFORWARD;
                    debe_avanzar = true;
                } else {
                    //out << "Aleatorio.." << endl;
                    if (rand() % 2 == 0)
                        act = actTURN_R;
                    else
                        act = actTURN_L;
                }
            } else if ((s.terreno[1] != 'S' && s.terreno[1] != 'T') && (s.terreno[3] != 'S' && s.terreno[3] != 'T')) {
                if (s.terreno[2] == 'S' || s.terreno[2] == 'T') {
                    //cout << "debe avanzar.." << endl;
                    act = actFORWARD;
                    debe_avanzar = true;
                } else {
                    //out << "Aleatorio.." << endl;
                    if (rand() % 2 == 0)
                        act = actTURN_R;
                    else
                        act = actTURN_L;
                }
            } else {
                //out << "Aleatorio.." << endl;
                if (rand() % 2 == 0)
                    act = actTURN_R;
                else
                    act = actTURN_L;
            }
        }
        if (act == actFORWARD) {
            if (s.terreno[2] == 'A' || s.terreno[2] == 'P' || s.superficie[2] != '_') {
                if (rand() % 2 == 0)
                    act = actTURN_R;

                else
                    act = actTURN_L;
            }
        }
    }
    return act;
}
/////////////////////////////////////////////////////////////

bool tengoObjeto(unsigned char obj);

/*Comprobamos si podemos avanzar o no*/
bool ComportamientoJugador::can_advance(unsigned char justo_delante_ter, unsigned char justo_delante_sup, const Sensores & sens) {
    if (justo_delante_sup != '_' && justo_delante_sup != 'd' && justo_delante_sup != 'r')
        return false;
    else {
        if (justo_delante_ter == 'T' || justo_delante_ter == 'S' || justo_delante_ter == 'K') {
            if (justo_delante_sup == 'r')
                return sens.objetoActivo == '4';

            return true;
        } else {
            if (justo_delante_ter == 'A')
                return sens.objetoActivo == '1';
            else if (justo_delante_ter == 'B')
                return sens.objetoActivo == '2';
            else if (justo_delante_ter == 'D') {
                if (sens.objetoActivo == '3') {
                    cont_puertas = 40;

                    return sens.objetoActivo == '3';
                }
            }
        }
    }
}

/******************************OBJETOS********************************/

/*Equipamos un objeto. Precondicion: que tengamos el objeto en la mochila*/
void ComportamientoJugador::equipaObjeto(unsigned char objeto, Action &act, const Sensores & sens) {
    //cout << "EQUIPANDO OBJETO.. " << mochila << " " << tengo_hueso << " " << tengo_biquini << " " << tengo_zapatillas << " " << tengo_llave << " " << tengo_regalo << endl;
    if (mochila < 4) {
        if (last_accion == actPUSH)
            act = actPOP;
        else
            act = actPUSH;
    } else
        act = girar(sens);
}

/*Ver si es un objeto el char pasado*/
bool es_object(unsigned char i) {

    return i == '0' || i == '1' || i == '2' || i == '3' || i == '4';
}

/*Coge objeto*/
void ComportamientoJugador::cogeObjeto(unsigned char obj, Action &act, const Sensores & sens) {
    if (cont_tira_objeto == 0) {
        cogiendo_obj = true;
        if (mochila < 4) {
            //cout << "COGIENDO OBJETO.. " << mochila << " " << tengo_biquini << " " << tengo_zapatillas << " " << tengo_llave << " " << tengo_regalo << endl;

            if (sens.objetoActivo == '_' && (obj != '0' || pasos_totales > 15000 || sens.vida < 200)) {
                contador_mismo_objeto = 0;

                if (obj != '4')
                    mochila++;
                else if (obj == '4' && tengo_regalo == 0)
                    mochila++;

                if (obj == '0') {
                    tengo_hueso = true;
                } else if (obj == '1')
                    tengo_biquini = true;
                else if (obj == '2')
                    tengo_zapatillas = true;
                else if (obj == '3') {
                    //cout << "coge llave.." << endl;
                    tengo_llave = true;
                } else if (obj == '4')
                    tengo_regalo++;

                act = actPICKUP;
                //cout << "COGIENDO OBJETO.. " << mochila << " " << tengo_hueso << " " << tengo_biquini << " " << tengo_zapatillas << " " << tengo_llave << " " << tengo_regalo << endl;
            } else if (obj != '0' || pasos_totales > 15000 || sens.vida < 200) {
                if (tengo_hueso)
                    tiraObjeto('0', act, sens);
                else
                    act = actPUSH;
            }
        } else {
            if (obj == '4') {
                tiraObjeto(sens.objetoActivo, act, sens);
            } else
                act = girar(sens);
        }
    }
}

/*Tira objeto*/
void ComportamientoJugador::tiraObjeto(unsigned char obj, Action &act, const Sensores & sens) {

    if (tengoObjeto(obj)) {
        tirando_obj = true;
        //cout << "TIRA OBJETO.. " << mochila << " " << tengo_hueso << " " << tengo_biquini << " " << tengo_zapatillas << " " << tengo_llave << " " << tengo_regalo << endl;
        if (sens.objetoActivo == obj) {
            mochila--;

            if (obj == '0')
                tengo_hueso = false;
            else if (obj == '1')
                tengo_biquini = false;
            else if (obj == '2')
                tengo_zapatillas = false;
            else if (obj == '3') {
                tengo_llave = false;
            } else if (obj == '4')
                tengo_regalo--;

            act = actTHROW;
            cont_tira_objeto = 5;
            //cout << "TIRA OBJETO.. " << mochila << " " << tengo_hueso << " " << tengo_biquini << " " << tengo_zapatillas << " " << tengo_llave << " " << tengo_regalo << endl << endl;
        } else if (mochila < 4)
            act = actPUSH;

        else
            act = girar(sens);
    }
}

/*Comprobamos si tenemos ese objeto*/
bool ComportamientoJugador::tengoObjeto(unsigned char obj) {

    if (obj == '0')
        return tengo_hueso;
    else if (obj == '1')
        return tengo_biquini;
    else if (obj == '2')
        return tengo_zapatillas;
    else if (obj == '3')
        return tengo_llave;
    else if (obj == '4')

        return tengo_regalo;
}

////////////////////////////////////////////////

/*Reiniciamos los valores de la memoria si morimos*/
void ComportamientoJugador::reiniciar() {
    //USO GENERAL
    last_accion = actIDLE;
    pasos = 0;
    brujula = 0;
    mochila = 0;
    pos_actual.first = 99;
    pos_actual.second = 99;
    cogiendo_obj = false;
    tirando_obj = false;
    cont_lobo = 0;
    cont_aldeano = 0;
    cont_puertas = 0;
    cont_reyes = 0;
    cont_tira_objeto = 0;
    puertas = 0; //Controlamos si estamos dentro de puerta o no

    //OBJETOS
    tengo_regalo = 0;
    tengo_biquini = false;
    tengo_zapatillas = false;
    tengo_llave = false;
    suelto_objeto = false;
    tengo_hueso = false;


    //ENCONTRAR_PK
    pk_found = false;
    pk_orientation = -1;
    go_pk = false;

    //GIRAR CORRECTAMENTE
    if (giro_norte == actTURN_L) {
        giro_norte = actTURN_R;
        giro_sur = actTURN_L;
        giro_oeste = actTURN_L;
        giro_este = actTURN_R;
    } else if (giro_norte == actTURN_R) {
        giro_norte = actTURN_L;
        giro_sur = actTURN_R;
        giro_oeste = actTURN_R;
        giro_este = actTURN_R;
    }

    avanzando = 0;
    girando = false;
    debe_avanzar = false;
    contador = 1 + rand() % (11 - 1);
    contador_mismo_objeto = 0;

    cont_misma_pos = 0;
    atrancado = false;

    //DESCUBRIR MAPA AL VER PK
    topn = tope = topo = tops = 99;

    /*Rellenamos mapa_aux*/
    mapa_aux.clear();
    std::vector< unsigned char> aux(200, '?');

    for (unsigned int i = 0; i < 200; i++) {

        mapa_aux.push_back(aux);
    }
    /////

    pos_aux.first = 99;
    pos_aux.second = 99;

    //A ESTRELLA
    go_point = false;
    ejecutando_plan = false;
    go_regalo = false;
    goto_rey = false;
    cont_plan = 0;
}

////////////////////////////////
/*Pensamiento de nuestro agente*/
////////////////////////////////

Action ComportamientoJugador::think(Sensores sensores) {
    pasos_totales++;
    cogiendo_obj = false;
    tirando_obj = false;


    if (sensores.reset) {
        cout << "MUERTO! " << pasos_totales << endl;
        reiniciar();
    }

    if (!ejecutando_plan)
        if (cont_plan > 0)
            cont_plan--;

    if (cont_tira_objeto > 0)
        cont_tira_objeto--;

    if (!ejecutando_plan)
        go_point = false;

    Action accion = actFORWARD; //Por defecto, avanzamos
    unsigned char justo_delante_ter = sensores.terreno[2];
    unsigned char justo_delante_sup = sensores.superficie[2];

    if (pk_found) {
        act_pos(pos_actual, sensores);

        if (pos_actual.first == pos_ant.first && pos_actual.second == pos_ant.second)
            cont_misma_pos++;
        else
            cont_misma_pos = 0;
    } else
        act_pos(pos_aux, sensores);

    if (cont_misma_pos > 10)
        atrancado = true;

    if (atrancado)
        if (cont_misma_pos > 0) {
            cont_misma_pos--;
            if (cont_misma_pos == 0)
                atrancado = false;
        }


    if (sensores.terreno[0] == 'D')
        puertas++;

    save_inf(sensores.terreno, sensores.superficie);

    if (last_accion == actTURN_L || last_accion == actTURN_R)
        avanzando = 0;

    if (brujula == 0 || brujula == 2) //Solo estariamos girando cuando vamos hacia este u oeste
        girando = false;

    if (!can_advance(justo_delante_ter, justo_delante_sup, sensores)) { //Aqui decidimos lo que hacemos cuando no podemos avanzar por algo
        //cout << "No podemos avanzar.." << endl;
        //Estamos atrancados
        if (atrancado)
            if (rand() % 2 == 0)
                accion = actTURN_R;
            else
                accion = actTURN_L;
            //Chocamos contra terreno intransitable
        else if (justo_delante_ter == 'P') //Precipicio
            accion = girarPre();
        else if (justo_delante_ter == 'M') //Muro
            accion = girar(sensores);
        else if (justo_delante_ter == 'A') { //Agua
            girando = false;
            contador = asignar_contador();
            if (sensores.terreno[0] != 'B') {
                if (tengo_biquini) {
                    equipaObjeto('1', accion, sensores);
                } else
                    accion = girar(sensores);
            } else
                accion = girar(sensores);
        } else if (justo_delante_ter == 'B') { //Bosque
            girando = false;
            contador = asignar_contador();
            if (sensores.terreno[0] != 'A') {
                if (tengo_zapatillas) {
                    equipaObjeto('2', accion, sensores);
                } else
                    accion = girar(sensores);
            } else
                accion = girar(sensores);
        } else if (justo_delante_ter == 'D') { //Puerta
            //cout << "justo delante puerta CHOCANDO" << endl;
            girando = false;
            contador = asignar_contador();

            if (sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B') {
                if (tengo_llave && (sensores.vida > 120 || puertas % 2 != 0)) {
                    equipaObjeto('3', accion, sensores);
                } else
                    accion = girar(sensores);
            } else
                accion = girar(sensores);
        } else { //Si no puedes avanzar y no se cumple nada de lo anterior
            //cout << "No puedes avanzar, pero no lo controlamos" << endl;
            accion = girar(sensores);
        }

        if (es_object(justo_delante_sup)) { //Chocamos contra objetos
            cogiendo_obj = true;
            //cout << "No puede avanzar por objeto.." << endl;
            if ((!tengoObjeto(justo_delante_sup) && sensores.vida > 120) || (contador_mismo_objeto >= 8 && !suelto_objeto)) {
                if (sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B') {
                    //cout << "Intentando coger objeto.." << endl;
                    if (justo_delante_sup == '3' && (pasos_totales > 2400 || contador_mismo_objeto >= 8))
                        cogeObjeto(justo_delante_sup, accion, sensores);
                    else if (justo_delante_sup != '3')
                        cogeObjeto(justo_delante_sup, accion, sensores);
                    else {
                        contador_mismo_objeto++;
                        accion = girar(sensores);
                    }
                } else
                    accion = girar(sensores);
            } else {
                //out << "Mismo objeto.." << endl;
                contador_mismo_objeto++;
                accion = girar(sensores);
            }
        } else { //CHOCAMOS CONTRA PERSONAJES
            if (justo_delante_sup == 'l') { //Lobo
                //cout << "Lobo a la vista.." << "\n";
                cont_lobo++;
                if (cont_lobo > 8) {
                    cont_lobo = 0;
                    accion = girar(sensores);
                } else
                    accion = actIDLE;
            } else if (justo_delante_sup == 'a') { //Aldeano
                //cout << "Delante aldeano.." << "\n";
                cont_aldeano++;
                if (cont_aldeano > 8) {
                    cont_aldeano = 0;
                    accion = girar(sensores);
                } else
                    accion = actIDLE;
            } else if (justo_delante_sup == 'r' && (sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B')) { //Rey
                //cout << "justo delante_rey chocando ..\n";
                if (tengo_regalo) {
                    equipaObjeto('4', accion, sensores);
                } else
                    accion = girar(sensores);
            }
        }

        if (ejecutando_plan && (!cogiendo_obj)) {
            //cout << "Ejecutando plan choque.." << endl;
            ejecutaPlan(accion, sensores);
        }
    } else { //Podemos avanzar
        if (justo_delante_ter == 'D') { //Puerta
            //cout << "Puerta avanzando.." << endl;
            if (justo_delante_sup == 'd' && (sensores.vida > 120 || puertas % 2 != 0))
                if (sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B' && cont_puertas == 0) {
                    accion = actGIVE;
                } else
                    accion = girar(sensores);

        } else if (justo_delante_sup == 'r') { //Rey_re
            //cout << "justo delante_rey sin chocar 1..\n";
            if (sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B') {
                //cout << "justo delante_rey sin chocar 2..\n";
                ejecutando_plan = false;
                goto_rey = false;
                go_regalo = false;
                plan.clear();
                accion = actGIVE;
                tengo_regalo--;
                mochila--;
            } else
                accion = girar(sensores);
        } else if (girando) { //Girando
            //cout << "Avanzamos girando.." << contador << "\n";
            if (last_accion == actFORWARD && accion == actFORWARD && contador != 0) {
                contador--;
            }

            if (!contador) {
                girando = false;
                contador = 4 + rand() % (8 - 4);
                ;
                accion = girar(sensores);
            }
        } else { //Avanzamos sin obstaculos
            if (cont_puertas > 0)
                cont_puertas--;

            avanzando++;
            //cout << "Avance sin obstaculo.." << avanzando << "\n";
            if (!tengo_hueso) {
                if (pk_found) {
                    if (brujula == 0 || brujula == 2) {
                        if (avanzando >= (15 + rand() % (45 - 15)))
                            accion = girar(sensores);
                    } else {
                        if (avanzando >= asignar_contador()) {
                            accion = girar(sensores);
                        }
                    }
                } else {
                    if (brujula == 0 || brujula == 2) {
                        if (avanzando >= 40)
                            accion = girar(sensores);
                    } else {
                        if (avanzando >= asignar_contador()) {
                            accion = girar(sensores);
                        }
                    }
                }
            } else
                tiraObjeto('0', accion, sensores);
        }

        if (ejecutando_plan && !cogiendo_obj && !tirando_obj) {
            //cout << "Ejecutando sin choque..\n";
            ejecutaPlan(accion, sensores);
        }

    }


    /*ACTUALIZACION DE MAPAS*/
    if (!pk_found) {
        if (sensores.terreno[0] == 'K') {
            pk_found = true;
            go_pk = false;
            pos_actual.first = sensores.mensajeF;
            pos_actual.second = sensores.mensajeC;

            revelar(sensores); //Revelamos mapa visto antes de ver PK

        } else {
            act_mapa(mapa_aux, sensores.terreno, pos_aux.first, pos_aux.second); //Actualizamos mapa auxiliar
        }
    }

    //cout << ejecutando_plan << endl;
    if (pk_found) { //Actualizamos mapas
        act_mapa(mapaResultado, sensores.terreno, pos_actual.first, pos_actual.second);
        act_mapa(mapaEntidades, sensores.superficie, pos_actual.first, pos_actual.second);
    }
    /***********************************/

    /*PLANIFICACION DE CAMINOS*/
    if (!cogiendo_obj) {
        if (!go_point && sensores.vida > 80 && pk_found && !sensores.colision && tengo_regalo and !ejecutando_plan and hay_reyes && accion != actGIVE && justo_delante_sup == '_' && pasos_totales > MAX_PASOS_DESCUBRIR && reyes_seen.size() > 0) {
            //cout << "HACIA EL REY!" << pasos_totales << endl;
            goto_rey = true;
            pair<int, int> pos_objetivo = getCercano(reyes_seen);
            if (mapaResultado[pos_objetivo.first][pos_objetivo.second] != '?' && mapaResultado[pos_objetivo.first][pos_objetivo.second] != 'A' && mapaResultado[pos_objetivo.first][pos_objetivo.second] != 'B')
                pathFinding(pos_actual, pos_objetivo, accion, sensores, mapaResultado, mapaEntidades, true, brujula);
        } else if (sensores.vida > 80 && !go_point && !sensores.colision && pk_found and !tengo_regalo && pasos_totales > MAX_PASOS_DESCUBRIR and !ejecutando_plan and sensores.regalos.size() > 0 and hay_regalos && accion != actGIVE && justo_delante_sup == '_') {
            //cout << "HACIA EL REGALO! " << pasos_totales << endl;
            go_regalo = true;
            pair<int, int> pos_objetivo = getCercano(sensores.regalos);
            if (mapaResultado[pos_objetivo.first][pos_objetivo.second] != '?' && mapaResultado[pos_objetivo.first][pos_objetivo.second] != 'A' && mapaResultado[pos_objetivo.first][pos_objetivo.second] != 'B')
                pathFinding(pos_actual, pos_objetivo, accion, sensores, mapaResultado, mapaEntidades, false, brujula);
        }

        if (!ejecutando_plan && !sensores.colision && accion != actGIVE) { //Comprobamos si hay que planificar camino
            if ((last_accion == actFORWARD || last_accion == actIDLE || last_accion == actTURN_L || last_accion == actTURN_R) && sensores.terreno[0] != 'K') {
                int pos_pk;
                //cout << "BUSCANDO.." << endl;

                if ((pos_pk = see_pk(sensores.terreno, sensores.superficie, sensores)) != -1) { //Si vemos algo para ir

                    //Planificamos camino
                    pk_orientation = get_pk_orientation(pos_pk);

                    if (go_pk) { //Camino hacia PK(mapa auxiliar)
                        pair<int, int> pos_objetivo = make_point(pos_pk, pk_orientation, pos_aux);
                        //cout << "PLanificando.." << pos_objetivo.first << " " << pos_objetivo.second << endl;
                        if (mapa_aux[pos_objetivo.first - 1][pos_objetivo.second] != 'B' && mapa_aux[pos_objetivo.first + 1][pos_objetivo.second] != 'B' && mapa_aux[pos_objetivo.first][pos_objetivo.second - 1] != 'B' && mapa_aux[pos_objetivo.first + 1][pos_objetivo.second + 1] != 'B')
                            pathFinding(pos_aux, pos_objetivo, accion, sensores, mapa_aux, mapaEntidades, false, brujula);

                    } else if (go_point) { //Camino hacia lo demas(mapaResultado y bien posicionado)
                        go_point = false;
                        pair<int, int> pos_objetivo = make_point(pos_pk, pk_orientation, pos_actual);
                        if (goto_rey) {
                            //cout << "Planificamos rey.." << endl;
                            if (mapaResultado[pos_objetivo.first][pos_objetivo.second] != '?' && mapaResultado[pos_objetivo.first][pos_objetivo.second] != 'A' && mapaResultado[pos_objetivo.first][pos_objetivo.second] != 'B')
                                pathFinding(pos_actual, pos_objetivo, accion, sensores, mapaResultado, mapaEntidades, true, brujula);
                        } else {
                            //cout << "Planificamos.." << endl;
                            if (mapaResultado[pos_objetivo.first][pos_objetivo.second] != '?' && mapaResultado[pos_objetivo.first][pos_objetivo.second] != 'A' && mapaResultado[pos_objetivo.first][pos_objetivo.second] != 'B')
                                pathFinding(pos_actual, pos_objetivo, accion, sensores, mapaResultado, mapaEntidades, false, brujula);
                        }
                    }
                }
            }
        }
    }


    //Suelta la llave antes de morir
    if (sensores.vida < 55) {
        if ((justo_delante_ter == 'S' || justo_delante_ter == 'T' || justo_delante_ter == 'K') && justo_delante_sup == '_' && puertas % 2 == 0) {
            if (sensores.objetoActivo == '3' || sensores.objetoActivo == '4') {
                suelto_objeto = true;
                if (sensores.objetoActivo == '3')
                    tengo_llave = false;
                else if (sensores.objetoActivo == '4')
                    tengo_regalo--;
                mochila--;
                accion = actPUTDOWN;
            } else if (tengoObjeto('3')) {
                if (!tengo_regalo)
                    suelto_objeto = true;
                equipaObjeto('3', accion, sensores);
            } else if (tengoObjeto('4')) {
                suelto_objeto = true;
                equipaObjeto('4', accion, sensores);
            } else if (suelto_objeto) {
                suelto_objeto = false;
                if (rand() % 2 == 0)
                    accion = actTURN_L;
                else
                    accion = girar(sensores);
            }
        }
    }

    if (pk_found) {
        pos_ant.first = pos_actual.first;
        pos_ant.second = pos_actual.second;
    }

    last_accion = accion;

    return accion;
}

int ComportamientoJugador::interact(Action accion, int valor) {
    return false;
}
