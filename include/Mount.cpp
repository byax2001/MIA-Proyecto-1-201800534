#include "../lib/Mount.h"

#include <iostream>
#include <stdlib.h>
#include "string"
#include <locale>

using namespace std;
Mount::Mount(){
}

void Mount::mount(vector<string> context) {
    if (context.empty()) {
        listmount();
        return;
    }
    vector<string> required = {"name", "path"};
    string path;
    string name;
    for (auto current : context) {
        string id = shared.lower(current.substr(0, current.find('=')));
        current.erase(0, id.length() + 1);
        if (current.substr(0, 1) == "\"") {
            current = current.substr(1, current.length() - 2);
        }
        if(id[0]=='-'){
            id=id.substr(1, current.length() - 1);
        }
        printf(id.c_str());

        if (shared.compare(id, "name")) {
            if (count(required.begin(), required.end(), id)) {
                auto itr = find(required.begin(), required.end(), id);
                required.erase(itr);
                name = current;
            }
        } else if (shared.compare(id, "path")) {
            if (count(required.begin(), required.end(), id)) {
                auto itr = find(required.begin(), required.end(), id);
                required.erase(itr);
                path = current;
            }
        }
    }
    if (required.size() != 0) {
        shared.handler("MOUNT", "requiere ciertos parámetros obligatorios");
        return;
    }
    mount(path, name);
}


void Mount::mount(string p, string n) {
    try {
        FILE *validate = fopen(p.c_str(), "r");
        if (validate == NULL) {
            throw runtime_error("disco no existente");
        }

        Structs::MBR disk;
        //PONE EL BUFFER AL INICIO EL DOCUMENTO
        rewind(validate);
        fread(&disk, sizeof(Structs::MBR), 1, validate);
        fclose(validate);

        Structs::Partition partition = dsk.findby(disk, n, p);
        if (partition.part_type == 'E') {
            vector<Structs::EBR> ebrs = dsk.getlogics(partition, p);
            if (!ebrs.empty()) {
                Structs::EBR ebr = ebrs.at(0);
                n = ebr.part_name;
                //shared.handler("", "se montará una partición lógica");
            } else {
                throw runtime_error("no se puede montar una extendida");
            }
        }
        //recorrer los 99 discos montados
        for (int i = 0; i < 99; i++) {
            if (mounted[i].path == p) {
                //recorrer las 26 particiones de cada disco
                for (int j = 0; j < 26; j++) {
                    if (Mount::mounted[i].mpartitions[j].status == '0') {
                        mounted[i].mpartitions[j].status = '1';
                        mounted[i].mpartitions[j].letter = alfabeto.at(j);
                        strcpy(mounted[i].mpartitions[j].name, n.c_str());
                        //numero de disco montado+1 + letra 
                        string re = to_string(i + 1) + alfabeto.at(j);
                        shared.response("MOUNT", "se ha realizado correctamente el mount -id=34" + re);
                        shared.Pause_press_to_continue();
                        return;
                    }
                }
            }
        }
        //recorrer los 99 discos montados
        for (int i = 0; i < 99; i++) {
            if (mounted[i].status == '0') {
                mounted[i].status = '1';
                strcpy(mounted[i].path, p.c_str());
                //recorrer las 26 particiones
                for (int j = 0; j < 26; j++) {
                    if (Mount::mounted[i].mpartitions[j].status == '0') {
                        mounted[i].mpartitions[j].status = '1';
                        mounted[i].mpartitions[j].letter = alfabeto.at(j);
                        strcpy(mounted[i].mpartitions[j].name, n.c_str());
                        //numero de disco montado+1 + letra 
                        string re = to_string(i + 1) + alfabeto.at(j);
                        shared.response("MOUNT", "se ha realizado correctamente el mount -id=34" + re);
                        shared.Pause_press_to_continue();
                        return;
                    }
                }
            }
        }
    }
    catch (exception &e) {
        shared.handler("MOUNT", e.what());
        return;
    }
}

void Mount::unmount(vector<string> context) {
    vector<string> required = {"id"};
    string id_;

    for (int i = 0; i < context.size(); i++) {
        string current = context.at(i);
        string id = current.substr(0, current.find("="));
        current.erase(0, id.length() + 1);

        if (shared.compare(id, "id")) {
            auto itr = find(required.begin(), required.end(), id);
            required.erase(itr);
            id_ = current;
        }
    }
    if (required.size() != 0) {
        shared.handler("UNMOUNT", "requiere ciertos parámetros obligatorios");
        return;
    }
    unmount(id_);
}

void Mount::unmount(string id) {
    try {

        //2018005 34
        if (!(id[0] == '3' && id[1] == '4')) {
            throw runtime_error("el primer identificador no es válido");
        }
        string past = id;
        char letter = id[id.length() - 1];
        id.erase(0, 2); //borrar los primeros dos digitos
        id.pop_back();
        //PARA SABER A QUE DISCO SE REFIERE, POSICIONADO EN UNA PARTE DEL ARRAY IGUAL A N - 1
        int i = stoi(id) - 1;

        if (i < 0) {
            throw runtime_error("identificador de disco inválido");
        }

        for (int j = 0; j < 26; j++) {
            //EL VALOR DE I SE SABE CON EL NUMERO EN EL NOMBRE DE LA PARTICION MONTADA
            if (mounted[i].mpartitions[j].status == '1') {
                if (mounted[i].mpartitions[j].letter == letter) {

                    MountedPartition mp = MountedPartition();
                    mounted[i].mpartitions[j] = mp;
                    shared.response("UNMOUNT", "se ha realizado correctamente el unmount -id=" + past);
                    shared.Pause_press_to_continue();
                    return;
                }
            }
        }
        throw runtime_error("id no existente, no se desmontó nada");
    }
    catch (invalid_argument &e) {
        shared.handler("UNMOUNT", "identificador de disco incorrecto, debe ser entero");
        return;
    }
    catch (exception &e) {
        shared.handler("UNMOUNT", e.what());
        return;
    }
}

Structs::Partition Mount::getmount(string id, string *p) {

    if (!(id[0] == '3' && id[1] == '4')) {
        throw runtime_error("el primer identificador no es válido");
    }
    string past = id;
    char letter = id[id.length() - 1];
    id.erase(0, 2);
    id.pop_back();
    int i = stoi(id) - 1;
    if (i < 0) {
        throw runtime_error("identificador de disco inválido");
    }

    for (int j = 0; j < 26; j++) {
        if (mounted[i].mpartitions[j].status == '1') {
            if (mounted[i].mpartitions[j].letter == letter) {

                FILE *validate = fopen(mounted[i].path, "r");
                if (validate == NULL) {
                    throw runtime_error("disco no existente");
                }

                Structs::MBR disk;
                rewind(validate);
                fread(&disk, sizeof(Structs::MBR), 1, validate);
                fclose(validate);
                *p = mounted[i].path;
                return dsk.findby(disk, mounted[i].mpartitions[j].name, mounted[i].path);
            }
        }
    }
    throw runtime_error("partición no existente");
}

void Mount::listmount() {
    cout << "\n<-------------------------- MOUNTS -------------------------->"
         << endl;
    for (int i = 0; i < 99; i++) { //discos montados
        for (int j = 0; j < 26; j++) { //particiones por cada disco
            if (mounted[i].mpartitions[j].status == '1') {
                cout << "> 54" << i + 1 << alfabeto.at(j) << ", " << mounted[i].mpartitions[j].name << endl;
            }
        }
    }
    shared.Pause_press_to_continue();
}