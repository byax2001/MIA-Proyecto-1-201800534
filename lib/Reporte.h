#ifndef REPORTE_H
#define REPORTE_H
#include <string>
#include <bits/stdc++.h>
#include <Comandos.h>
#include <shared.h>
#include <structs.h>
#include <Mount.h>
#include <Comandos.h>
#include <Mount.h>

using namespace std;
class Reporte{
    public:
        Reporte();
        Mount::_MD mountedDisk;
        void MBR_EBR(string id, string pathf);
        void DiskRep(string id,string pathf);
    private:
        Shared shared;
        Structs _struct;
        Comandos comandos;
        Mount mount;
};


#endif