#include <iostream>
#include "stdlib.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <string.h>
#include "Analizador.cpp"
using namespace std;

int main(int argc, char const *argv[])

{   
    Analizador analizador;
    string entradaCmd="";
    //mkdisk -s->3000 -u->K -path->/home/brandon/hola/Disco1.dsk
    //rmdisk -path->/home/brandon/hola/Disco1.dsk
    while (entradaCmd!="exit")
    {
        cout << "-----------------------------------------" <<endl;
        cout << "================Proyecto 1==================" <<endl;
        cout << "-----------------------------------------" <<endl;
        cout << "[brandon@moon]: " ;
        getline(cin,entradaCmd);
        analizador.analisis(entradaCmd);
    }   
    
   
    return 0;
}
