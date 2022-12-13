#ifndef REPORTE_H
#define REPORTE_H
#include <string>
#include <bits/stdc++.h>
#include <Comandos.h>
#include <shared.h>
#include <structs.h>

using namespace std;
class Reporte{
    public:
        Reporte();
        
        void getInf();
        void graph(vector<Structs::Partition> partitions,Structs::MBR mbr);
    private:
        Shared shared;
        Structs _struct;
};


#endif