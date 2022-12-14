#ifndef REPORTE_H
#define REPORTE_H
#include <string>
#include <bits/stdc++.h>
#include <Comandos.h>
#include <shared.h>
#include <structs.h>
#include <Mount.h>
#include <Comandos.h>

using namespace std;
class Reporte{
    public:
        Reporte();
        void graph(vector<Structs::Partition> partitions,Structs::MBR mbr);
        void MBR_EBR(Mount:: _MD mounted, string pathf);
        void Disk(Mount:: _MD mounted, string pathf);
    private:
        Shared shared;
        Structs _struct;
        Comandos comandos;
};


#endif