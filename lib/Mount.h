#ifndef MOUNT_H
#define MOUNT_H

#include <string>
#include <bits/stdc++.h>
#include "../lib/shared.h"
#include "../lib/structs.h"
#include "../lib/Comandos.h"

using namespace std;

class Mount {
    public:
    Mount();

    typedef struct _MP
    {
        char letter;
        char status = '0';
        char name[16];
    }MountedPartition; //particion montada

    typedef struct _MD
    {
        char path[150];
        char status = '0';
        MountedPartition mpartitions[26]; //array de particiones montadas
    }MountedDisc; //Disco montado

    MountedDisc mounted[99]; //array de discos montados

    void mount(vector<string> context);

    void unmount(vector<string> context);

    void mount(string p, string n);

    void unmount(string id);

    void listmount();

    Structs::Partition getmount(string id, string *p);

    private:
    Comandos dsk;
    Shared shared;
    vector<char> alfabeto = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                             's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
};
#endif