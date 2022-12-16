#include "../lib/Reporte.h"

#include <iostream>
#include <stdlib.h>
#include "string"
#include <locale>

using namespace std;

Reporte::Reporte() {
}

void Reporte::crearDirectorio(string path){
    FILE *archivo_bin;
    archivo_bin = fopen(path.c_str(), "w");
    if (archivo_bin == NULL)
    {   // si es nulo es que el directorio no ha sido creado
        // creo directorio
        // cout<<"Direccion: "<<path<<" :)"<<endl;
        string comando1 = "mkdir -p \"" + path + "\"";
        string comando2 = "rmdir \"" + path + "\"";
        system(comando1.c_str());
        system(comando2.c_str());
        FILE *file = fopen(path.c_str(), "w+b");
        fclose(file);
    }
    else
    {
        fclose(archivo_bin);
    }
}

void Reporte::MBR_EBR(string id, string pathf, Mount mount){
    string name_final = pathf;
    string aux;
    string arch_dot = name_final.substr(0, name_final.length() - 3) + "dot";
    crearDirectorio(arch_dot);
    ofstream archivo;
    archivo.open(arch_dot, ios::out);
    if (archivo.fail())
    {
        shared.handler("REP", "No se pudo crear el reporte de particiones");
        exit(1);
    }
    Structs::MBR disk = mount.getDisk(id,&pathf);
    char path[150];
    strcpy(path, pathf.c_str());
    // REPORTE MBR
    // tamaño
    int mbr_tamano = disk.mbr_tamano;
    // fecha de creacin
    struct tm *tm;
    tm = localtime(&disk.mbr_fecha_creacion);
    char mbr_fecha_creacion[20];
    strftime(mbr_fecha_creacion, 20, "%Y/%m/%d %H:%M:%S", tm);
    // signature
    int mbr_signature = disk.mbr_dsk_signature;
    cout<<arch_dot<<" FILE REP 2AAAAA333A"<<endl;
    archivo << "digraph RepMEBR{\n";
    archivo << "node [shape=plaintext, fontname=Arial]\n";
    archivo << "tabla [label=<<TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"1\">\n";
    archivo << "<TR>\n";
    archivo << "<TD PORT=\"c\" BGCOLOR=\"purple\">REPORTE MBR</TD>\n";
    archivo << "<TD PORT=\"c\" BGCOLOR=\"purple\"> </TD>\n";
    archivo << "</TR>\n";
    archivo << "<TR>\n";
    archivo << "<TD PORT=\"c\" >mbr_tamano</TD>\n";
    archivo << "<TD PORT=\"c\" >" + to_string(mbr_tamano) + "</TD>\n";
    archivo << "</TR>\n";
    archivo << "<TR>\n";
    archivo << "<TD PORT=\"c\" >mbr_fecha_creacion</TD>\n";
    archivo << "<TD PORT=\"c\" >" + string(mbr_fecha_creacion) + "</TD>\n";
    archivo << "</TR>\n";
    archivo << "<TR>\n";
    archivo << "<TD PORT=\"c\" >mbr_fsignature</TD>\n";
    archivo << "<TD PORT=\"c\" >" + to_string(mbr_signature) + "</TD>\n";
    archivo << "</TR>\n";
    cout<<arch_dot<<" FILE REP 5aaaAAA333A"<<endl;
    vector<Structs::Partition> partitions;

    partitions.push_back(disk.mbr_partition_1);
    partitions.push_back(disk.mbr_partition_2);
    partitions.push_back(disk.mbr_partition_3);
    partitions.push_back(disk.mbr_partition_4);

    for (int i = 0; i < 4; i++)
    {
        // ES PRINCIPAL O EXTENDIDA
        Structs::Partition pAct = partitions[i];
        char part_status = pAct.part_status;
        char part_type = pAct.part_type;
        char part_fit = pAct.part_fit;
        int part_start = pAct.part_start;
        int part_size = pAct.part_s;
        char part_name[16];
        strcpy(part_name, pAct.part_name);

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" BGCOLOR=\"purple\">Particion</TD>\n";
        archivo << "<TD PORT=\"c\" BGCOLOR=\"purple\"> </TD>\n";
        archivo << "</TR>\n";
        archivo << "<TR>\n";

        archivo << "<TD PORT=\"c\" >part_status</TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string((char)part_status) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_type</TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string((char)part_type) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_fit</TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string((char)part_fit) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_start</TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string(part_start) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_size</TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string(part_size) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_name </TD>\n";
        string str(part_name);
        archivo << "<TD PORT=\"c\" >" + str + "</TD>\n";
        archivo << "</TR>\n";

        if (pAct.part_type == 'e')
        {
            // GRAFICAR LOGICAS
            // EBR SON LAS LOGICAS
            vector<Structs::EBR> Logicas;
            Logicas = comandos.getlogics(pAct, path);
            for (int j = 0; j < sizeof(Logicas); j++)
            {
                Structs::EBR LogAct = Logicas[j];
                char part_status = LogAct.part_status;
                int part_next = LogAct.part_next;
                char part_fit = LogAct.part_fit;
                int part_start = LogAct.part_start;
                int part_size = LogAct.part_s;
                char part_name[16];
                strcpy(part_name, LogAct.part_name);

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" BGCOLOR=\"pink\"Particion Logica</TD>\n";
                archivo << "<TD PORT=\"c\" BGCOLOR=\"pink\"> </TD>\n";
                archivo << "</TR>\n";
                archivo << "<TR>\n";

                archivo << "<TD PORT=\"c\" >part_status</TD>\n";
                archivo << "<TD PORT=\"c\" >" + to_string((char)part_status) + "</TD>\n";
                archivo << "</TR>\n";

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" >part_next</TD>\n";
                archivo << "<TD PORT=\"c\" >" + to_string(part_next) + "</TD>\n";
                archivo << "</TR>\n";

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" >part_fit</TD>\n";
                archivo << "<TD PORT=\"c\" >" + to_string((char)part_fit) + "</TD>\n";
                archivo << "</TR>\n";

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" >part_start</TD>\n";
                archivo << "<TD PORT=\"c\" >" + to_string(part_start) + "</TD>\n";
                archivo << "</TR>\n";

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" >part_size</TD>\n";
                archivo << "<TD PORT=\"c\" >" + to_string(part_size) + "</TD>\n";
                archivo << "</TR>\n";

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" >part_name/TD>\n";
                string str(part_name);
                archivo << "<TD PORT=\"c\" >" + str + "</TD>\n";
                archivo << "</TR>\n";
            }
        }
    }

    archivo << "</TABLE>>];\n";
    archivo << "}";
    archivo.close();
    string comandof = "dot -Tpng " + arch_dot + " -o " + name_final;
    cout<<"ARCH DOT "<<arch_dot;
    cout<<"NAME FINAL: "<<name_final;
    cout<<"COMANDO consola "<<comandof;
    shared.response("REP","Creacion de reporte correcto");
    system(comandof.c_str());
}


//#######################################################################3

void Reporte::DiskRep(string id,string pathf, Mount mount)
{
    // CON EL PATH DEL DISCO MONTADO SE SELECCIONA EL DISCO Y SE OBTIENEN TODOS LOS DATOS PARA EL GRAFICO
    string aux;
    ofstream archivo;
    string name_final = pathf;
    string arch_dot = name_final.substr(0, name_final.length()-3)+"dot";
    archivo.open(arch_dot, ios::out);
    if (archivo.fail())
    {
        shared.handler("REP", "No se pudo crear el reporte de particiones");
        exit(1);
    }

    Structs::MBR disk = mount.getDisk(id,&pathf);
    char path[150];

    strcpy(path, pathf.c_str());
    
    // GRAPH- INIT ------------------------------------------
    int mbr_tamano = disk.mbr_tamano;
    archivo << "digraph DiskG{\n";
    archivo << "node [shape=plaintext, fontname=Arial]\n";
    archivo << "tabla [label=<<TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"1\">\n";
    archivo << "<TR>\n";
    archivo << "<TD rowspan='2'>MBR</TD>\n";

    vector<Structs::Partition> partitions;

    partitions.push_back(disk.mbr_partition_1);
    partitions.push_back(disk.mbr_partition_2);
    partitions.push_back(disk.mbr_partition_3);
    partitions.push_back(disk.mbr_partition_4);

    int inicio_aux = 0;

    for (size_t i = 0; i < 4; i++)
    {
        // ES PRINCIPAL O EXTENDIDA
        Structs::Partition pAct = partitions[i];
        int part_start = pAct.part_start;
        int part_size = pAct.part_s;
        if (i == 0)
        {   
            //si por si acaso viene vacia la particion
            if(partitions[i].part_s==0){
                archivo << "<td rowspan='2'>Espacio Libre</td>\n";
                continue;
            }

            if (partitions[i].part_type == 'e'){
                // GRAFICAR LOGICAS
                // EBR SON LAS LOGICAS
                archivo << "<td>\n";
                archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";

                float porcentaje = (partitions[i].part_s / mbr_tamano) * 100;
                archivo << "<tr><td align=\"left\"><b> EXTENDIDA </b></td></tr>\n";
                //ROW PARA LAS LOGICAS
                archivo << "<tr>\n";
                vector<Structs::EBR> Logicas;
                Logicas = comandos.getlogics(pAct, path);
                for (size_t i = 0; i < sizeof(Logicas); i++)
                {
                    Structs::EBR LogAct = Logicas[i];
                    int part_next = LogAct.part_next;
                    int part_start = LogAct.part_start;
                    int part_size = LogAct.part_s;
                    archivo << "<td rowspan='2'>EBR</td>\n";
                    archivo << "<td>\n";
                    archivo << "<table border= \'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
                    float porcentaje = (part_size / mbr_tamano) * 100;
                    archivo << "<tr><td align=\"left\"><b>Logica</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\"><b>" + to_string(porcentaje) + "%</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                    archivo << "</table>\n";
                    archivo << "</td>\n";
                }
                archivo << "</tr>\n";
                archivo << "</table>";
                archivo << "</td>\n";


            }else{
                string aux;
                archivo << "<td>\n";
                archivo << "<table border= \'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
                float porcentaje = (partitions[i].part_s / mbr_tamano) * 100;
                cout<<"\n "<<partitions[i].part_s <<" SIZE"<<endl;
                cout<<"\n "<<mbr_tamano<<" SIZET"<<endl;
                
                archivo << "<tr><td align=\"left\"><b>Principal</b></td></tr>\n";
                archivo << "<tr><td align=\"left\">" + to_string(porcentaje) + "  %</td></tr>\n";
                archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                archivo << "</table>\n";
                archivo << "</td>\n";
            }
        }else{
            //inicio_deb = la particion actual deberia de iniciar aqui
            if(partitions[i].part_s==0){//si por si acaso viene vacia la particion
                archivo << "<td rowspan='2'>Espacio Libre</td>\n";
                continue;
            }
            int inicio_deb = partitions[i-1].part_start+partitions[i-1].part_s;
            if(partitions[i].part_start!=inicio_deb+1 || partitions[i].part_s=='0'){
                //graficar un apartado del disco llamado ESPACIO LIBRE entre inicio deb y  partitions[i].start
                archivo << "<td>\n";
                archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
                float porcentaje = (partitions[i].part_start-inicio_deb) * 100;
                archivo << "<tr><td align=\"left\"><b>Libre</b></td></tr>\n";
                archivo << "<tr><td align=\"left\"><b>" + to_string(porcentaje) + "%</b></td></tr>\n";
                archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                archivo << "</table>\n";
                archivo << "</td>\n";
            }if (partitions[i].part_type == 'e'){
                // GRAFICAR LOGICAS
                archivo << "<td>\n";
                archivo << "table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";

                float porcentaje = (partitions[i].part_s / mbr_tamano) * 100;
                archivo << "<tr><td align=\"left\"><b> EXTENDIDA </b></td></tr>\n";
                //ROW PARA LAS LOGICAS
                archivo << "<tr>";
                // EBR SON LAS LOGICAS
                vector<Structs::EBR> Logicas;
                Logicas = comandos.getlogics(pAct, path);
                for (size_t i = 0; i < sizeof(Logicas); i++)
                {
                    Structs::EBR LogAct = Logicas[i];
                    int part_next = LogAct.part_next;
                    int part_start = LogAct.part_start;
                    int part_size = LogAct.part_s;

                    archivo << "td rowspan='2'>EBR</td>\n";
                    archivo << "<td>\n";
                    archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
                    float porcentaje = (part_size / mbr_tamano) * 100;
                    archivo << "<tr><td align=\"left\"><b>Logica</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\"><b>" + to_string(porcentaje) + "%</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                    archivo << "</td>\n";
                }
                archivo << "</tr>\n";
                archivo << "</table>\n";
                archivo << "</td>\n";
            }else{
                string aux;
                archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
                float porcentaje = (partitions[i].part_s / mbr_tamano) * 100;
                archivo << "<tr><td align=\"left\"><b>Principal</b></td></tr>\n";
                archivo << "<tr><td align=\"left\"><b>" + to_string(porcentaje) + "  % </b></td></tr>\n";
                archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                archivo << "</table>\n";
            }

            //POR SI ACASO NO SE LLENO EL DISCO HABRA UN ESPACIO ENTRE LA ULTIMA PARTICION Y EL RESTO DEL DISCO
            if (i == 3)
            {   //ultima parte del disco usada
                int u_pdisk_usado = partitions[i].part_start+partitions[i].part_s;
                if (u_pdisk_usado!=mbr_tamano)
                {
                    //APARTADO PARA CREAR UN ESPACIO LIBRE 
                    archivo << "<td>\n";
                    archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'>\n";
                    float porcentaje = (mbr_tamano-u_pdisk_usado) * 100;
                    archivo << "<tr><td align=\"left\"><b>Libre</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\"><b>" + to_string(porcentaje) + "%</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                    archivo << "</table>\n";
                    archivo << "</td>\n";
                }
                
            }
            

        }
    }
    archivo << "</TR>\n";
    archivo << "</TABLE>>];\n";
    archivo << "}";
    archivo.close();
    string comandof = "dot -Tpng \"" + arch_dot + "\" -o \"" + name_final + "\"";
    cout<<"ARCH DOT "<<arch_dot;
    cout<<"NAME FINAL: "<<name_final;
    cout<<"COMANDO consola "<<comandof;
    shared.response("REP","Creacion de reporte correcto");
    system(comandof.c_str());
}



/*

void Reporte::MBR_EBR(Mount::_MD mounted,string pathf){


//CON EL PATH DEL DISCO MONTADO SE SELECCIONA EL DISCO Y SE OBTIENEN TODOS LOS DATOS PARA EL GRAFICO
    char path[150];
    strcpy(path,mounted.path);
    FILE *validate = fopen(path, "r");
    if (validate == NULL) {
        shared.handler("REP","Disco no existe");
        return;
    }

    Structs::MBR disk;
    //PONE EL BUFFER AL INICIO EL DOCUMENTO
    rewind(validate);
    //leer documento
    fread(&disk, sizeof(Structs::MBR), 1, validate);
    fclose(validate);

                    //REPORTE MBR
    //tamaño
    int mbr_tamano = disk.mbr_tamano;
    //fecha de creacin
    struct tm *tm;
    tm = localtime(&disk.mbr_fecha_creacion);
    char mbr_fecha_creacion[20];
    strftime(mbr_fecha_creacion, 20, "%Y/%m/%d %H:%M:%S",tm);
    //signature
    int mbr_signature = disk.mbr_dsk_signature;

    vector<Structs::Partition> partitions;

    partitions.push_back(disk.mbr_partition_1);
    partitions.push_back(disk.mbr_partition_2);
    partitions.push_back(disk.mbr_partition_3);
    partitions.push_back(disk.mbr_partition_4);

    for (size_t i = 0; i < 4; i++)
    {
        //ES PRINCIPAL O EXTENDIDA
        Structs::Partition pAct = partitions[i];
        char part_status = pAct.part_status;
        char part_type = pAct.part_type;
        char part_fit = pAct.part_fit;
        int part_start = pAct.part_start;
        int part_size = pAct.part_s;
        char part_name[16];
        strcpy(part_name,pAct.part_name);

        if (partitions[i].part_type != 'p')
        {
            //GRAFICAR LOGICAS
            //EBR SON LAS LOGICAS
            vector<Structs::EBR> Logicas;
            Logicas = comandos.getlogics(pAct,path);
            for (size_t i = 0; i < sizeof(Logicas); i++)
            {
                Structs::EBR LogAct = Logicas[i];
                char part_status = LogAct.part_status;
                int part_next = LogAct.part_next;
                char part_fit = LogAct.part_fit;
                int part_start = LogAct.part_start;
                int part_size = LogAct.part_s;
                char part_name[16];
                strcpy(part_name,LogAct.part_name);

            }

        }
    }




}
*/