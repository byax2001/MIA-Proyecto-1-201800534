#ifndef ANALIZADOR_H
#define ANALIZADOR_H
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include<bits/stdc++.h>
#include <string>
#include <string.h>
#include <vector>
#include "../lib/Comandos.h"
#include "../lib/Mount.h"
#include "../lib/shared.h"
#include <fstream>
#include <iostream>
#include <Reporte.h>
using namespace std; 

class Analizador
{
public:
    //aqui se declaran los metodos y structs completos y funciones para poder usarlos en el cpp 
    Analizador();
    void start();
    void analisis(string entrada);
    int BookId;
    vector<string> split_txt(string text);
    string replace_txt(string str, const string& from, const string& to);
    void identificarParametros(string comando, vector<string> parametros);
    void LeerScript(string path);
    void print(string s);
private:
    Mount mount;
    Shared shared;
    Reporte reporte;
};

#endif