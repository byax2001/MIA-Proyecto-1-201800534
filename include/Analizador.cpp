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
    //mkdisk -s->3000 -u->K -path->/home/brandon/hola2/Disco3.dsk
    //rmdisk -path->/home/brandon/hola/Disco1.dsk
    while (entradaCmd!="exit")
    {
        cout << "-----------------------------------------" <<endl;
        cout << "================Proyecto 1==================" <<endl;
        cout << "-----------------------------------------" <<endl;
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
            if (segment.find("\"")==0){
                aux=1;
                aux=segment;
            }else{
                splited.push_back(segment);
            }
            
        }else{
            if (segment.find("\"")==0){
                splited.push_back(aux);
                aux1=0;
            }else{
                aux=aux+" "+segment;
            }
        }
        

    }
    return splited;
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
    if(comando == "mkdisk"){
        cmd.param.Comando = "mkdisk";
        cmd.param.ajuste_particion='f';
        cmd.param.dimensional='m';
        for (int i = 0; i < parametros.size(); i++)
        {
            param=parametros.at(i);
            if(param.find("-s->")==0){
                
                param=replace_txt(param,"-s->","");
                cmd.param.tamano=stoi(param.c_str());///convertir valor a int
            }else if  (param.find("-f->")==0){
                param=replace_txt(param,"-f->","");
                //aqui los ajusten son bf, ff, wf  pero como se toma solo laprimera letra como identificador da igual como venga
                cmd.param.ajuste_particion=param.at(0); //el at devuelve un char
            }else if(param.find("-u->")==0){
                param=replace_txt(param,"-u->","");
                cmd.param.dimensional=param;
            }else if ( param.find("-path->")==0){
                param=replace_txt(param,"-path->","");
                param=replace_txt(param,"\"","");
                cmd.param.path=param;
            }
        }
        // Ejecucion de metodo
        cmd.ejecutarInst(cmd.param);  //SE MANDA A EJECUTAR EL METODO

    } else if(comando == "rep"){
        cmd.param.Comando = "rep";
        cmd.ejecutarInst(cmd.param);
    } else if(comando == "exec"){
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
    }else if(comando=="rmdisk"){
        cmd.param.Comando = "rmdisk";
        int nParams=1;//parametros necesarios para este comando
        for(int i=0; i<parametros.size(); i++){
            param=parametros.at(i);
            if(param.find("-path->")==0){  
                param=replace_txt(param,"-path->","");
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
    }else if (comando=="fdisk"){
        cmd.param.Comando=="fdisk";
        int add_Delete=0;  //0 es add y 1 es delete
        int aux=0;
        //revisar que no venga add  y delete en el mismo apartado
        for (size_t i = 0; i < parametros.size(); i++)
        {
            if (parametros.at(i)=="add"){
                add_Delete=0;
                aux++;
            }else if (parametros.at(i)=="delete"){
                add_Delete=1;
                aux++;
            }
        }
        int nparametros=0;
        if (aux==0){//creacion de una particion
            nparametros=3;
            
        }else if (aux<2){
            if(add_Delete==0){//si es add
                nparametros=3;
            }else{//si es delete
                nparametros=2;
                
            }
        }else{
            cout<<"parametros incompatibles en la misma linea"<<endl;
        }
    }else if(comando == "exit"){
        cout<<"Exit"<<endl;
    }else{
        cout<<"Comando no reconocido"<<endl;
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