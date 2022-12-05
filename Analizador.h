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
#include "Comandos.cpp"
#include <fstream>
#include <iostream>
using namespace std; 

class Analizador
{
public:
    //aqui se declaran los metodos y structs completos y funciones para poder usarlos en el cpp 
    Comandos cmd;
    Analizador();
    int BookId;
    vector<string> split_txt(string text);
    string replace_txt(string str, const string& from, const string& to);
    void analisis(string entrada);
    void identificarParametros(string comando, vector<string> parametros);
    void LeerScript(string path);
private:

};

#endif