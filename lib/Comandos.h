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
#include "../lib/structs.h"
#include "../lib/shared.h"


// Apartado para los comandos
struct Parametros
{
    string Comando = "";
    int tamano = 0;
    char ajuste_particion = 'w';
    char dimensional = 'k';
    string path;     // path
    //FDISK
    char opcionFdisk='c'; // crear particion(c), aumentar tamaño (a) de esta o eliminar(d)
    char typePartition;//extendida, logica, primaria
    char fit='w';//fit
    string _delete="";
    int add = 0;
    string name;
    //
    string id;
    int s;    // tamaño
    string f; // ajuste_particion
    string u; // dimensional

};

class Comandos
{
public:
    Comandos();
    typedef struct _Transition {
        int partition;
        int start;
        int end;
        int before;
        int after;
    } Transition;
    Parametros param;
    //MKISK
    void CreateDisk(int tamano, char t_particion, char dim, string dir);
    void ejecutarInst(Parametros p);
    //RMFILE
    void DeleteFile(string path);
    //FDISK
    void Fdisk(int OpFdisk, string path, int tamano, char dimensional, char tparticion, char TAjuste, char id[]);
    void fdisk (char FdiskOption,int s,char u,string path, char tParti,char fit,string name,int add, string _delete);
    void generatepartition(int s, char u, string p, char t, char f, string n);
    void deletepartition(string _delete, string p, string n);
    void addpartition(int add, char u, string n, string p);
    vector<Structs::Partition> getPartitions(Structs::MBR disk);
    Structs::MBR
    adjust(Structs::MBR mbr, Structs::Partition p, vector<Transition> t, vector<Structs::Partition> ps, int u);
    Structs::Partition findby(Structs::MBR mbr, string name, string path);
    void logic(Structs::Partition partition, Structs::Partition ep, string p);
    vector<Structs::EBR> getlogics(Structs::Partition partition, string p);
    void ImprimirParticiones(vector<Structs::Partition> partitions);
private:
    Shared shared;
 
};

#endif