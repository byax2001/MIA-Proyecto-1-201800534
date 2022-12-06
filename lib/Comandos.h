#ifndef COMANDOS_H
#define COMANDOS_H
#include <iostream>
#include "stdlib.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
#include "structs.h"


// Apartado para los comandos
struct Parametros
{

    string Comando = "";
    int tamano = 0;
    char ajuste_particion = 'w';
    string dimensional = "k";
    string path;     // path
    int opcionFdisk; // crear particion, aumentar tamañod de esta o eliminar
    string id;
    int s;    // tamaño
    string f; // ajuste_particion
    string u; // dimensional

};

class Comandos
{
public:
    Comandos();
    Parametros param;
    void CreateDisk(int tamano, char t_particion, string dim, string dir);
    int num_aleatorio();
    void ejecutarInst(Parametros p);
    void DeleteFile(string path);
    void Fdisk(int OpFdisk, string path, int tamano, char dimensional, char tparticion, char TAjuste, char id[]);

private:
};

#endif