#ifndef REPORTE_H
#define REPORTE_H
#include <string>
#include <bits/stdc++.h>
#include "../lib/Comandos.h"
#include "../lib/Mount.h"
#include "../lib/shared.h"
#include "../lib/structs.h"



using namespace std;
class Reporte{
    public:
        Reporte();
        Mount::_MD mountedDisk;
        void MBR_EBR(string id, string pathf, Mount mount);
        void DiskRep(string id,string pathf, Mount mount);
        void crearDirectorio(string path);
        Structs::MBR getDisk(string id, string *p);
        string rString(char caracter);
        void DrawEspLibre(ofstream *archivo,int porcentaje);
        void DrawUltimoLibre(Structs::Partition partitions, int vacia, int mbr_tamano,int init_vacia, ofstream *archivo);
    private:
        Shared shared;
        Structs _struct;
        Comandos comandos;
};


#endif