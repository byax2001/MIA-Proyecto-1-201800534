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
        cout<<endl<<"comando1: "<<comando1<<" comando2: "<<comando2<<endl;
        shared.Pause_press_to_continue();
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
        archivo << "\n/* INICIO PARTICION */\n";
        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" BGCOLOR=\"purple\">Particion</TD>\n";
        if(part_type == 'e'){
            archivo << "<TD PORT=\"c\" BGCOLOR=\"purple\"> Extendida</TD>\n";
        }else{
            archivo << "<TD PORT=\"c\" BGCOLOR=\"purple\"> Principal </TD>\n";
        }
        archivo << "</TR>\n";
        archivo << "<TR>\n";

        archivo << "<TD PORT=\"c\" >part_status</TD>\n";
        archivo << "<TD PORT=\"c\" >" + rString(part_status) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_type</TD>\n";
        archivo << "<TD PORT=\"c\" >" + rString(part_type) + "</TD>\n";
        archivo << "</TR>\n";

        archivo << "<TR>\n";
        archivo << "<TD PORT=\"c\" >part_fit</TD>\n";
        archivo << "<TD PORT=\"c\" >" + rString(part_fit) + "</TD>\n";
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
                strncpy(part_name,LogAct.part_name,16);
                cout<<"SIZE: "<<part_size<<" name "<<part_name<<" CARACTER "<<(int)part_name[0]<<endl;
                
                if(part_size<0 || (int)part_name[1]<32){
                    continue;
                }
                bool part_warning=false;
                for (int i = 0; i < 16; i++)
                {
                    cout<<"NAME CHAR: "<<part_name<<" CARACTER: "<<(int)part_name[i]<<endl;
                
                    if((int)part_name[i]<48 && (int)part_name[i]!=0){
                        part_warning=true;
                    }
                }
                if(part_warning==true){
                    continue;
                }

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" BGCOLOR=\"pink\"> Particion </TD>\n";
                archivo << "<TD PORT=\"c\" BGCOLOR=\"pink\"> Logica </TD>\n";
                archivo << "</TR>\n";
                archivo << "<TR>\n";

                archivo << "<TD PORT=\"c\" >part_status</TD>\n";
                archivo << "<TD PORT=\"c\" >" + rString(part_status) + "</TD>\n";
                archivo << "</TR>\n";

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" >part_next</TD>\n";
                archivo << "<TD PORT=\"c\" >" + to_string(part_next) + "</TD>\n";
                archivo << "</TR>\n";

                archivo << "<TR>\n";
                archivo << "<TD PORT=\"c\" >part_fit</TD>\n";
                archivo << "<TD PORT=\"c\" >" + rString(part_fit) + "</TD>\n";
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
                archivo << "<TD PORT=\"c\" >part_name</TD>\n";
                string str(part_name);
                archivo << "<TD PORT=\"c\" >" + str + "</TD>\n";
                archivo << "</TR>\n";
            }
        
        }
        archivo << "\n/*FIN PARTICION */\n";
    }

    archivo << "</TABLE>>];\n";
    archivo << "}";
    archivo.close();
    string comandof = "dot -Tpng " + arch_dot + " -o " + name_final;
    cout<<"\nArchivo generado: "<<name_final<<endl;
    shared.response("REP","Creacion de reporte correcto");
    system(comandof.c_str());
}


//#######################################################################3

void Reporte::DiskRep(string id,string pathf, Mount mount)
{
    // CON EL PATH DEL DISCO MONTADO SE SELECCIONA EL DISCO Y SE OBTIENEN TODOS LOS DATOS PARA EL GRAFICO
    string aux;
    int vacia =0;
    int init_vacia=0;
    
    ofstream archivo;
    string name_final = pathf;
    string arch_dot = name_final.substr(0, name_final.length()-3)+"dot";
    crearDirectorio(arch_dot);
    archivo.open(arch_dot, ios::out);
    if (archivo.fail())
    {
        shared.handler("REP", "No se pudo crear el reporte de particiones");
        exit(1);
    }

    Structs::MBR disk = mount.getDisk(id,&pathf);
    char path[150];

    strcpy(path, mount.getPath(id).c_str()); //PATH DEL DISCO DE DONDE SE MONTO LA PARTICION
    
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
                vacia=1; //----para indicar que la particion actual esta vacia
                init_vacia = sizeof(Structs::MBR)+1;//----para ver de donde comienzan las particiones
                archivo << "<td rowspan='2'>Espacio Libre</td>\n";
                continue;
            }else{
                init_vacia = partitions[i].part_s+partitions[i].part_start+1;
            }

            if (partitions[i].part_type == 'e'){
                // GRAFICAR LOGICAS
                // EBR SON LAS LOGICAS
                archivo<<"\n/*PARTICION EXTENDIDA*/\n";
                archivo << "<td>\n";
                archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";

                float porcentaje = (partitions[i].part_s / (float)mbr_tamano) * 100;
                archivo << "<tr><td align=\"left\"><b> EXTENDIDA </b></td></tr>\n";
                //ROW PARA LAS LOGICAS
                archivo << "<tr>\n";
                vector<Structs::EBR> Logicas;
                Logicas = comandos.getlogics(pAct, path);
                for (size_t i = 0; i < Logicas.size(); i++)
                {
                    Structs::EBR LogAct = Logicas[i];
                    int part_next = LogAct.part_next;
                    int part_start = LogAct.part_start;
                    int part_size = LogAct.part_s;
                    archivo << "<td rowspan='2'>EBR</td>\n";
                    archivo << "<td>\n";
                    archivo << "<table border= \'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
                    float porcentaje = (part_size / (float)mbr_tamano) * 100;
                    archivo << "<tr><td align=\"left\"><b>Logica</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\"><b>" + to_string(porcentaje) + "%</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                    archivo << "</table>\n";
                    archivo << "</td>\n";
                }
                archivo << "</tr>\n";
                archivo << "</table>\n";
                archivo << "</td>\n";
                archivo<<"\n/*FIN PARTICION EXTENDIDA*/\n";

            }else{
                string aux;
                archivo<<"\n/* PARTICION PRINCIPAL*/\n";
                archivo << "<td>\n";
                archivo << "<table border= \'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
                float porcentaje = (partitions[i].part_s / (float)mbr_tamano) * 100;
                cout<<"\n "<<partitions[i].part_s <<" SIZE"<<endl;
                cout<<"\n "<<mbr_tamano<<" SIZET"<<endl;     
                archivo << "<tr><td align=\"left\"><b>Principal</b></td></tr>\n";
                archivo << "<tr><td align=\"left\">" + to_string(porcentaje) + "  %</td></tr>\n";
                archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                archivo << "</table>\n";
                archivo << "</td>\n";
                archivo<<"\n/*FIN PARTICION PRINCIPAL*/\n";
            }
        }else{
            //inicio_deb = la particion actual deberia de iniciar aqui
            if(partitions[i].part_s==0){//si por si acaso viene vacia la particion
                vacia =1;
                if(i==3){
                    DrawUltimoLibre(partitions[i],vacia,mbr_tamano,init_vacia,&archivo);
                }
                continue;
            }else if (partitions[i].part_s != 0 && vacia == 1){
                float porcentaje = ((partitions[i].part_start - init_vacia)/(float)mbr_tamano) * 100;
                DrawEspLibre(&archivo, porcentaje);
                vacia = 0;
                init_vacia = partitions[i].part_start + partitions[i].part_s + 1;
            }else if (vacia == 0 && partitions[i].part_s != 0 ){
                int inicio_deb = partitions[i - 1].part_start + partitions[i - 1].part_s;
                if (partitions[i].part_start != inicio_deb + 1 || partitions[i].part_s == '0'){
                    // graficar un apartado del disco llamado ESPACIO LIBRE entre inicio deb y  partitions[i].start
                    float porcentaje = (partitions[i].part_start - inicio_deb) / (float)mbr_tamano * 100;
                    DrawEspLibre(&archivo, porcentaje);
                }
            }

            if (partitions[i].part_type == 'e'){
                // GRAFICAR LOGICAS
                archivo << "<td>\n";
                archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";

                float porcentaje = (partitions[i].part_s / (float)mbr_tamano) * 100;
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

                    archivo << "<td rowspan='2'>EBR</td>\n";
                    archivo << "<td>\n";
                    archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
                    float porcentaje = (part_size / (float)mbr_tamano) * 100;
                    archivo << "<tr><td align=\"left\"><b>Logica</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\"><b>" + to_string(porcentaje) + "%</b></td></tr>\n";
                    archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                    archivo << "</table>\n";
                    archivo << "</td>\n";
                }
                archivo << "</tr>\n";
                archivo << "</table>\n";
                archivo << "</td>\n";
            }else{
                string aux;
                archivo<<"\n/*PARTICION PRINCIPAL*/\n";
                archivo << "<td>\n";
                archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
                float porcentaje = (partitions[i].part_s / (float)mbr_tamano) * 100;
                archivo << "<tr><td align=\"left\"><b>Principal</b></td></tr>\n";
                archivo << "<tr><td align=\"left\"><b>" + to_string(porcentaje) + "  % </b></td></tr>\n";
                archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
                archivo << "</table>\n";
                archivo << "</td>\n";
                archivo<<"\n/*FIN PARTICION PRINCIPAL*/\n";
            }
            //POR SI ACASO NO SE LLENO EL DISCO HABRA UN ESPACIO ENTRE LA ULTIMA PARTICION Y EL RESTO DEL DISCO
            if (i == 3)
            {   //ultima parte del disco usada
                DrawUltimoLibre(partitions[i],vacia,mbr_tamano,init_vacia,&archivo);
            }
            

        }
    }
    archivo << "</TR>\n";
    archivo << "</TABLE>>];\n";
    archivo << "}";
    archivo.close();
    string comandof = "dot -Tpng \"" + arch_dot + "\" -o \"" + name_final + "\"";
    
    shared.response("REP","Creacion de reporte correcto");
    cout<<"\nArchivo: "<<name_final<<endl;
    system(comandof.c_str());
}

string Reporte::rString(char _char){
    string aux;
    if(_char==0){
        _char='*';
    }
    aux.push_back(_char);
    return aux;
}

void Reporte::DrawEspLibre(ofstream *archivo, int porcentaje){
    *archivo << "<td>\n";
    *archivo << "<table border=\'" + to_string(1) + "\' cellborder=\'" + to_string(0) + "\' cellspacing=\'1\'" + ">\n";
    *archivo << "<tr><td align=\"left\"><b>Libre</b></td></tr>\n";
    *archivo << "<tr><td align=\"left\"><b>" + to_string(porcentaje) + "%</b></td></tr>\n";
    *archivo << "<tr><td align=\"left\">  del Disco </td></tr>\n";
    *archivo << "</table>\n";
    *archivo << "</td>\n";
}
void Reporte::DrawUltimoLibre(Structs::Partition partition, int vacia, int mbr_tamano,int init_vacia, ofstream *archivo)
{
    if (vacia == 0){
        int u_pdisk_usado = partition.part_start + partition.part_s;
        if (u_pdisk_usado != mbr_tamano)
        {
            // APARTADO PARA CREAR UN ESPACIO LIBRE
            float porcentaje = (mbr_tamano - u_pdisk_usado) / (float) mbr_tamano * 100;
            DrawEspLibre(archivo, porcentaje);
        }
    }else{
        float porcentaje = ((mbr_tamano - init_vacia) /(float)mbr_tamano )*100;

        DrawEspLibre(archivo, porcentaje);
    }
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