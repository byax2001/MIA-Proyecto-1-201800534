#include "../lib/Analizador.h"
#include "../lib/Analizador.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace std;



Analizador::Analizador()
{
    //Este es el constructor 
}

void Analizador::start(){
    string entradaCmd="";
    //mkdisk -s=3500 -u=K -path=/home/brandon/hola2/Disco3.dsk
    //rmdisk -path=/home/brandon/hola2/disco3.dsk
    //fdisk -s=300 -path=/home/Disco1.dsk -name=Particion1
    //fdisk -t=E -path=/home/brandon/hola2/disco3.dsk-u=K -name=Particion2 -s=300
    //fdisk -s=1 -t=L -u=M -f=BF -path="/home/brandon/hola2/disco3.dsk"-name="Particion3"
    //fdisk -t=E -path=/home/brandon/hola2/disco3.dsk-name=Part3 -u=K -s=200
    //fdisk -delete=fast -name="Particion1" -path=/home/brandon/hola2/disco3.dsk
    //fdisk -name=Particion1 -delete=full -path=/home/brandon/hola2/disco3.dsk
    //fdisk -add=-500 -s=10 -u=K -path=/home/brandon/hola2/disco3.dsk -name=Particion4
    //fdisk -add=1 -u=M -path=/home/brandon/hola2/disco3.dsk -name="Particion 4"
    //mount 

    while (entradaCmd!="exit")
    {
        printf("\033c");
        cout << "--------------------------------------------" <<endl;
        cout << "================Proyecto 1==================" <<endl;
        cout << "--------------------------------------------" <<endl;
        cout << "[brandon@moon]: " ;
        getline(cin,entradaCmd);
        analisis(entradaCmd);
    }   
    
}

vector<string> Analizador::split_txt(string text){ // Split para separar tipo de comando y parametros
    stringstream ss(text);  //split este texto
    string segment; //variable donde se guardara cada parte
    string aux;
    int aux1=0;
    vector<string> splited;

    while(std::getline(ss, segment, ' ')){
        if (aux1==0){
            if (segment.find("\"")!=string::npos){
                aux1=1;
                aux=segment;
            }else{
                splited.push_back(segment);
            }
            
        }else{
            if (segment.find("\"")!=string::npos){
                aux=aux+" "+segment;
                splited.push_back(aux);
                aux1=0;
            }else{
                aux=aux+" "+segment;
            }
        }
        

    }
    //eliminar comentarios que estan en la misma linea;
    vector<string> splited_aux;
    for (string segment_:splited){
        if(segment_.find("#")!=string::npos){
            break;
        }
        splited_aux.push_back(segment_);
    }
    return splited_aux;
}

string Analizador::replace_txt(string str, const string& from, const string& to) {
    //size_t es un tipo de valor de datos 
    //string::npos  indica el final de la cadena 
    //replace ( posicion inicial, cantidad de caracteres a sustituir, caracter que reemplazara el antiguo valor)
    //from: valor a sutituir   to: valor que sustituira
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

 /*
        PASOS PARA REALIZAR LOS COMANDOS:
        1: estarandizar minusculas o mayusculas para todos los elementos
        2: separarlos por los espacios y guardar cada parametro en una celda de un array
        3: identificar cual es el comando (el cual es el primer elemento del array)
        4: distinguir el comando de los demas parametros
        5: proceder dependiendo del comando a pedir unos u otros parametros
        6: distinguir parametros con los comandos que luego procederan a ser eliminados con un replace (elemento,"") 
           dejando solo el valor de estos
        7: mandar a otro metodo a realizar una ejecucion dependiendo del comando y los parametros usados.
    */

void Analizador::analisis(string entrada){
    string entradaCmd=entrada;
    //pasar la cadena string a minusculas
    transform(entradaCmd.begin(), entradaCmd.end(), entradaCmd.begin(), ::tolower);
    vector<string> cmdEntrada = split_txt(entradaCmd); //el array del split de la entrada cmd
    vector<string> parametros; //los parametros a realizar
    string comando = ""; //Comando que se va a ejecutar
    for(int i = 0; i< cmdEntrada.size(); i++){
        if(i == 0){
            comando = cmdEntrada.at(i);  //comando=cmdEntrada[0]
        }else{
            parametros.push_back(cmdEntrada.at(i)); //push cmd Entrada
        }
    }
    // Identificacion de comando, parametros y ejecucion
    identificarParametros(comando, parametros);
}

void Analizador::identificarParametros(string comando, vector<string> parametros){
    Comandos cmd;
    string param = "";

    //exec -path=/home/brandon/arch.mia
    //parth12 = 10240  + 1m
    //parth4   = 7680     +500 kbytes
    //mkdisk -s=4000 -u=K -path=/home/brandon/hola2/Disco3.dsk
    //rmdisk -path=/home/brandon/hola2/disco3.dsk
    //fdisk -s=300 -path=/home/brandon/hola2/disco3.dsk -name=Particion1
    //mount -path=/home/brandon/hola2/disco3.dsk -name=Particion1
    //mount -path=/home/brandon/hola2/disco3.dsk -name=Particion2
    //mount -path=/home/brandon/hola2/disco3.dsk –name=Particion3
    //rep -id=341a -Path=/home/brandon/reports/reporte1.jpg -name=mbr
    //rep -id=341a -Path=/home/brandon/reports/reporte1v2.jpg -name=disk
    //rep -id=341a -Path=/home/brandon/reports/reporte1v2.jpg -name=disk
    //rep -id=341b -Path=/home/brandon/reports/report2.pdf -name=disk


    //fdisk -t=E -path=/home/brandon/hola2/disco3.dsk -u=K -name=Particion2 -s=300
    //fdisk -s=1 -t=L -u=M -f=BF -path="/home/brandon/hola2/disco3.dsk"-name="Particion3"
    //fdisk -t=E -path=/home/brandon/hola2/disco3.dsk-name=Part3 -u=K -s=200mount -path=/home/brandon/hola2/disco3.dsk -name=Particion1
    //fdisk -delete=fast -name="Particion1" -path=/home/brandon/hola2/disco3.dsk
    //fdisk -name=Particion1 -delete=full -path=/home/brandon/hola2/disco3.dsk
    //fdisk -add=-500 -s=10 -u=K -path=/home/brandon/hola2/disco3.dsk -name=Particion4
    //fdisk -add=1 -u=M -path=/home/brandon/hola2/disco3.dsk -name="Particion 4"
 

    if(comando.compare("mkdisk")==0){
        cmd.param.Comando = "mkdisk";
        cmd.param.ajuste_particion='f';
        cmd.param.dimensional='m';
        for (int i = 0; i < parametros.size(); i++)
        {
            param=parametros.at(i);
            if(param.find("-s=")==0){
                
                param=replace_txt(param,"-s=","");
                cmd.param.tamano=stoi(param.c_str());///convertir valor a int
            }else if  (param.find("-f=")==0){
                param=replace_txt(param,"-f=","");
                //aqui los ajusten son bf, ff, wf  pero como se toma solo laprimera letra como identificador da igual como venga
                cmd.param.ajuste_particion=param.at(0); //el at devuelve un char
            }else if(param.find("-u=")==0){
                param=replace_txt(param,"-u=","");
                cmd.param.dimensional=param.at(0);
            }else if ( param.find("-path=")==0){
                param=replace_txt(param,"-path=","");
                param=replace_txt(param,"\"","");
                cmd.param.path=param;
            }else{

                shared.handler("MKDISK","Parametros extras no compatibles con la instruccion");
                return;
            }
        }
        // Ejecucion de metodo
        cmd.ejecutarInst(cmd.param);  //SE MANDA A EJECUTAR EL METODO

    } else if(comando.compare("fdisk")==0){
        //parametros opcionales
        cmd.param.dimensional='k';
        cmd.param.typePartition='p';
        cmd.param.fit = 'w';
        
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-s=")==0){
                param=replace_txt(param,"-s=","");
                cmd.param.tamano=stoi(param.c_str());///convertir valor a int
            }else if(param.find("-u=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-u=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.dimensional= param[0];
        
            }else if(param.find("-path=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-path=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }else if(param.find("-t=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-t=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.typePartition=param.c_str()[0];
            }else if(param.find("-f=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-f=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.fit=param.c_str()[0];
            }else if(param.find("-delete=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-delete=", "");
                param = replace_txt(param, "\"", "");
                cmd.param._delete=param;
                cmd.param.opcionFdisk = 'd';
            }else if(param.find("-name=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-name=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.name=param;
            }else if(param.find("-add=")==0){
                param=replace_txt(param,"-add=","");
                cmd.param.add=stoi(param.c_str());///convertir valor a int
                cmd.param.opcionFdisk = 'a';
            }
        }
        cmd.param.Comando = "fdisk";
        cmd.ejecutarInst(cmd.param);
    } else if(comando.compare("mount")==0){
        //mount -path=/home/brandon/hola2/disco3.dsk -name=Particion1
        mount.mount(parametros);
       
    }else if(comando.compare("unmount")==0){
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-id=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-id=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.id = param;
            }
        }
        cmd.param.Comando = "unmount";
        mount.unmount(cmd.param.id);
    }else if(comando.compare("exec")==0){
        cmd.param.Comando = "exec";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-path=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-path=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }
        }
        LeerScript(cmd.param.path);   
    }else if(comando.compare("rmdisk")==0){
        cmd.param.Comando = "rmdisk";
        int nParams=1;//parametros necesarios para este comando
        for(int i=0; i<parametros.size(); i++){
            param=parametros.at(i);
            if(param.find("-path=")==0){  
                param=replace_txt(param,"-path=","");
                param=replace_txt(param,"\"","");
                cmd.param.path=param;///convertir valor a int
                nParams--;
            }
        }
        // Ejecucion de metodo
        if (nParams==0){
            cmd.ejecutarInst(cmd.param);  //SE MANDA A EJECUTAR EL METODO       
        }else{
            cout<<"Error: faltan parametros"<<endl;
        }
    }else if(comando.compare("rep")==0){
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-id=")==0){
                param=replace_txt(param,"-id=",""); 
                cmd.param.id=param;
            }else if(param.find("-name=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-name=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.name = param;
            }else if(param.find("-path=") == 0){  //find devuelve un 0 si se encontro, si no devolvera el tamaño del string completo
                param = replace_txt(param, "-path=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }
        }
        if (cmd.param.name=="mbr"){
            reporte.MBR_EBR(cmd.param.id,cmd.param.path,mount);
        }else if (cmd.param.name=="disk"){
            reporte.DiskRep(cmd.param.id,cmd.param.path,mount);
        }else{
            shared.handler("REP","Tipo de reporte no registrado");
        }
    }else if(comando.compare("exit")==0){
        cout<<"Exit"<<endl;
    }else{
        shared.handler("ERROR","Comando no reconocido");
    }
}

void Analizador::LeerScript(string path){
    string line;
    ifstream archivo;
    cout<<"dir: "<<path<<endl;
    archivo.open(path,ios::in);
    if(archivo.fail()){
        cout<<"Error al abrir el archivo"<<endl;
        exit(1);
    }
    while (!archivo.eof())
    {
        getline(archivo,line);
        if(line!=""){
            if(line.at(0)=='#'){
                cout<<"Comentario: "<<line<<endl;
            }else{
                cout<<"instruction: "<<line<<endl;
                analisis(line);
            }
            
        }
    }
}

void Analizador::print(string s){
    cout<< s<< endl;
}