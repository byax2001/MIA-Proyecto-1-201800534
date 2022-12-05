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
using namespace std;
namespace fs = std::filesystem;

vector<int> id_signature;
// Apartado para los comandos
typedef struct
{

    string Comando = "";
    int tamano = 0;
    char ajuste_particion = 'f';
    string dimensional = "m";
    string path;     // path
    int opcionFdisk; // crear particion, aumentar tamañod de esta o eliminar
    string id;
    int s;    // tamaño
    string f; // ajuste_particion
    string u; // dimensional

} Parametros;

// Structs
struct Partition
{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_s;
    char part_name[16];
} partition_,vacioP;

struct DiscoMBR
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_dsk_signature;
    char dsk_fit;
    Partition mbr_partition_1;
    Partition mbr_partition_2;
    Partition mbr_partition_3;
    Partition mbr_partition_4;
} disco, disco2;

typedef struct
{
    char part_status;
    char part_fit;
    int part_start;
    int part_s;
    int part_next;
    char part_name[16];
} Ebr;

class Comandos
{
public:
    Parametros param;
    Comandos();
    void CreateDisk(int tamano, char t_particion, string dim, string dir);
    void ShowDisco();
    int num_aleatorio();
    void mostrar(DiscoMBR disco);
    void ejecutarInst(Parametros p);
    void DeleteFile(string path);
    void Fdisk(int OpFdisk, string path, int tamano, char dimensional, char tparticion, char TAjuste, char id[]);

private:
};

#endif