#include "../lib/Reporte.h"

#include <iostream>
#include <stdlib.h>
#include "string"
#include <locale>

using namespace std;

Reporte::Reporte() {}

void Reporte::MBR_EBR(Mount::_MD mounted, string pathf)
{
    ofstream archivo;
    string name_final = pathf;
    string arch_dot = pathf.substr(0, sizeof(pathf) - 3) + "dot";

    archivo.open(arch_dot, ios::out);
    if (archivo.fail())
    {
        shared.handler("REP", "No se pudo crear el reporte de particiones");
        exit(1);
    }

    // CON EL PATH DEL DISCO MONTADO SE SELECCIONA EL DISCO Y SE OBTIENEN TODOS LOS DATOS PARA EL GRAFICO
    char path[150];
    strcpy(path, mounted.path);
    FILE *validate = fopen(path, "r");
    if (validate == NULL)
    {
        shared.handler("REP", "Disco no existe");
        return;
    }

    Structs::MBR disk;
    // PONE EL BUFFER AL INICIO EL DOCUMENTO
    rewind(validate);
    // leer documento
    fread(&disk, sizeof(Structs::MBR), 1, validate);
    fclose(validate);

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

    vector<Structs::Partition> partitions;

    partitions.push_back(disk.mbr_partition_1);
    partitions.push_back(disk.mbr_partition_2);
    partitions.push_back(disk.mbr_partition_3);
    partitions.push_back(disk.mbr_partition_4);

    for (size_t i = 0; i < 4; i++)
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
        archivo << "<TD PORT=\"c\" BGCOLOR=\"purple\"Particion</TD>\n";
        archivo << "<TD PORT=\"c\" BGCOLOR=\"purple\"> </TD>\n";
        archivo << "</TR>\n";
        archivo << "<TR>\n";

        archivo << "<TD PORT=\"c\" >part_status</TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string(part_status) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_type</TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string(part_type) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_fit</TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string(part_fit) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_start</TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string(part_start) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_size/TD>\n";
        archivo << "<TD PORT=\"c\" >" + to_string(part_size) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_name/TD>\n";
        string str(part_name);
        archivo << "<TD PORT=\"c\" >" + str + "</TD>\n";
        archivo << "</TR>\n";

        if (partitions[i].part_type != 'p')
        {
            // GRAFICAR LOGICAS
            // EBR SON LAS LOGICAS
            vector<Structs::EBR> Logicas;
            Logicas = comandos.getlogics(pAct, path);
            for (size_t i = 0; i < sizeof(Logicas); i++)
            {
                Structs::EBR LogAct = Logicas[i];
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
                archivo << "<TD PORT=\"c\" >" + to_string(part_status) + "</TD>\n";
                archivo << "</TR>\n";

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" >part_next</TD>\n";
                archivo << "<TD PORT=\"c\" >" + to_string(part_next) + "</TD>\n";
                archivo << "</TR>\n";

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" >part_fit</TD>\n";
                archivo << "<TD PORT=\"c\" >" + to_string(part_fit) + "</TD>\n";
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
    system(comandof.c_str());
}

void Reporte::Disk(Mount::_MD mounted, string pathf)
{
    // CON EL PATH DEL DISCO MONTADO SE SELECCIONA EL DISCO Y SE OBTIENEN TODOS LOS DATOS PARA EL GRAFICO
    ofstream archivo;
    string name_final = pathf;
    string arch_dot = pathf.substr(0, sizeof(pathf) - 3) + "dot";

    archivo.open(arch_dot, ios::out);
    if (archivo.fail())
    {
        shared.handler("REP", "No se pudo crear el reporte de particiones");
        exit(1);
    }

    char path[150];
    strcpy(path, mounted.path);
    FILE *validate = fopen(path, "r");
    if (validate == NULL)
    {
        shared.handler("REP", "Disco no existe");
        return;
    }

    Structs::MBR disk;
    // PONE EL BUFFER AL INICIO EL DOCUMENTO
    rewind(validate);
    // leer documento
    fread(&disk, sizeof(Structs::MBR), 1, validate);
    fclose(validate);
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

    int inicio_aux=0;
    

    for (size_t i = 0; i < 4; i++)
    {
        // ES PRINCIPAL O EXTENDIDA
        Structs::Partition pAct = partitions[i];
        int part_start = pAct.part_start;
        int part_size = pAct.part_s;

        if (partitions[i].part_type != 'p')
        {
            // GRAFICAR LOGICAS
            // EBR SON LAS LOGICAS
            vector<Structs::EBR> Logicas;
            Logicas = comandos.getlogics(pAct, path);
            for (size_t i = 0; i < sizeof(Logicas); i++)
            {
                Structs::EBR LogAct = Logicas[i];
                int part_next = LogAct.part_next;
                int part_start = LogAct.part_start;
                int part_size = LogAct.part_s;
            }
        }
        else
        {   
            string aux;
            archivo << "label=<<table border="+ to_string(1)+" cellborder=" +to_string(0)+ " cellspacing="+ '1' +">";

            float porcentaje = (partitions[i].part_s/mbr_tamano) * 100;
            archivo<<"<tr><td align=\"left\"><b>Principal</b></td></tr>";
            archivo<<"<tr><td align=\"left\"><b>"+to_string(porcentaje)+"  % </b></td></tr>";
            archivo<< "<tr><td align=\"left\">  del Disco </td></tr>";
     
   
   </table>>];
        }
    }
    archivo << "</TR>\n";
    archivo << "</TABLE>>];\n";
    archivo << "}";
    archivo.close();
    string comandof = "dot -Tpng \"" + arch_dot + "\" -o \"" + name_final + "\"";
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