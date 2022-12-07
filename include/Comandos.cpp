#include "../lib/Comandos.h"

Comandos::Comandos()
{
    // Constructor
}

void Comandos::ejecutarInst(Parametros p)
{
    if (p.Comando == "mkdisk")
    { // Se identifica el comando
        
        CreateDisk(p.tamano, p.ajuste_particion, p.dimensional, p.path);
    }
    else if (p.Comando == "fidsk")
    {
        
        cout<<"hola";
    }
    else if (p.Comando == "rmdisk")
    {
        DeleteFile(p.path);
    }
    else
    {
        cout << "Error comando no conocido" << endl;
        exit(1);
    }
}

void Comandos::CreateDisk(int tamano, char t_particion, string dim, string path)
{
    //-s: tamaño del disco
    //-f:bf,ff,wf
    //-u: unidad
    //-path: direccion 
    Structs::DiscoMBR disco;
    int size_file = tamano;
    char bloque[1024]; //el tamaño de este array lleno es igual a un k
    for (int i = 0; i < 1024; i++)
    {
        bloque[i] = '\0';
    }
    // escribir un bloque es igual a escribir un kilobyte
    if (dim == "k")
    {
        size_file = size_file;
    }
    else if (dim == "m")
    {
        size_file = size_file * 1024;
    }

    // CREACION DEL ARCHIVO
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
    archivo_bin = fopen(path.c_str(), "rb+"); // seek_seet no funciona con ab+  solo con rb+ a la hora de escribir
                                              // ab+ siempre escribe al final, pero a la hora de leer si funciona seek set  al parecer
    int limite = 0;
    while (limite != size_file)
    {
        fwrite(&bloque, 1024, 1, archivo_bin);
        limite++;
    }
    //tamaño del diso
    disco.mbr_tamano = size_file * 1024;
    //fecha de creacion
    disco.mbr_fecha_creacion = time(0);
    //-------------------------------------
    int numrand = rand() % 9999 + 100;
    //id unico al crear disco
    
    disco.mbr_dsk_signature = numrand;
    //-------------------------------------
    //tipo de particiones
    disco.dsk_fit = t_particion;
    //particiones
    disco.mbr_partition_1 = Structs::Partition();
    disco.mbr_partition_2 = Structs::Partition();
    disco.mbr_partition_3 = Structs::Partition();
    disco.mbr_partition_4 = Structs::Partition();

    //ubicarse al inicio del disco rellenado con \0 y luego escribir el mbr
    fseek(archivo_bin, 0, SEEK_SET);
    fwrite(&disco, sizeof(disco), 1, archivo_bin);
    fclose(archivo_bin);

    //indicar que el disco fue creado
    int indexDoc = path.rfind("/");
    string nameDisk = path.substr(indexDoc,path.length());
    cout<<"Disco" <<nameDisk<<" creado con exito!"<<endl;
    getline(cin,nameDisk);
}



int Comandos::num_aleatorio()
{ // numero aleatorio irrepetible
    

    return 2;
}

void Comandos::DeleteFile(string path)
{
    char char_path[path.length() + 1];
    strcpy(char_path, path.c_str());
    if (remove(char_path) != 0)
    {
        cout << "Error al eliminar el archivo " << endl;
    }
    else
    {
        cout << "Se elimino el archivo correctamente" << endl;
    }
}

void Comandos::Fdisk(int OpFdisk, string path, int tamano, char dimensional, char tparticion, char tAjuste, char id[])
{
    //OpFdisk : si se va a añadir o eliminar una particion 
    //-s tamaño
    //-u unidad
    // path path
    //-t tipo de particion
    //-f mejor ajuste
    // delete borrar particion
    // add aumentar tamño de una particion
    // name
    Structs::DiscoMBR disco,disco2;
    Structs::Partition partition_,partmod,*aux,vacioP;
    // revisar que el disco a leer exista
    FILE *archivo_bin;
    archivo_bin = fopen(path.c_str(), "rb+");
    if (archivo_bin != NULL)
    {
        fread(&disco2, sizeof(disco2), 1, archivo_bin);
        //Crear Particion 
        if (OpFdisk == 0)
        {
            if(strcmp(disco2.mbr_partition_1.part_name,id)==0){
                //nombre ya existe error
                cout<<"Ya existe una particion con este id"<<endl;
                return;
            }else if(strcmp(disco2.mbr_partition_2.part_name,id)==0){
                //nombre ya existe error
                cout<<"Ya existe una particion con este id"<<endl;
                return;
            } else if(strcmp(disco2.mbr_partition_3.part_name,id)==0){
                //nombre ya existe error
                cout<<"Ya existe una particion con este id"<<endl;
                return;
            } else if(strcmp(disco2.mbr_partition_4.part_name,id)==0){
                //nombre ya existe error
                cout<<"Ya existe una particion con este id"<<endl;
                return;
            }
            partition_.part_status = 'd'; // desactivado
            partition_.part_type = tparticion;
            partition_.part_fit = tAjuste; // bestfit firsfit  worsfit
            strcpy(partition_.part_name,id);//pasar el contenido de una variable a otra (destino, origen)

            if (disco2.mbr_partition_1.part_status == '\0')
            {
                int inicio = sizeof(disco2) + 1;
                int tamano = sizeof(partition_);
                partition_.part_start = inicio;
                partition_.part_s = tamano;
                disco2.mbr_partition_1 = partition_;
            }
            else if (disco2.mbr_partition_2.part_status == '\0')
            {
                int inicio = disco2.mbr_partition_1.part_start + disco2.mbr_partition_1.part_s + 1;
                int tamano = sizeof(partition_);
                partition_.part_start = inicio;
                partition_.part_s = tamano;
                disco2.mbr_partition_2 = partition_;
            }
            else if (disco2.mbr_partition_3.part_status == '\0')
            {
                int inicio = disco2.mbr_partition_2.part_start + disco2.mbr_partition_2.part_s + 1;
                int tamano = sizeof(partition_);
                partition_.part_start = inicio;
                partition_.part_s = tamano;
                disco2.mbr_partition_2 = partition_;
            }
            else if (disco2.mbr_partition_4.part_status == '\0')
            { //\0 es el valor de las variables char sin inicializar
                int inicio = disco2.mbr_partition_3.part_start + disco2.mbr_partition_3.part_s + 1;
                int tamano = sizeof(partition_);
                partition_.part_start = inicio;
                partition_.part_s = tamano;
                disco2.mbr_partition_2 = partition_;
            }
            fseek(archivo_bin, 0, SEEK_SET);
            fwrite(&disco2, sizeof(disco2), 1, archivo_bin);
            fclose(archivo_bin);
            cout << "Particion creada" << endl;
        }else if (OpFdisk == 1){ // Opcion ADD
            // para que otra variable tome el valor de otra y que esta al modificarle se modifiquen las dos
            // hacer esto:
            //  Punteros:
            //  int varOriginal=0
            //  int *varAux= &varOriginal;        //*varAux retorna el valor de var Original y tambien la modifica
            //  *varAux=3;
            //  varOriginal ahora tendra el valor de 3
            if(strcmp(disco2.mbr_partition_1.part_name,id)==0){
                aux=&disco2.mbr_partition_1;
                (*aux).part_s=(*aux).part_s+tamano; //se esta modificando el struct original para referenciarse en algun atributo usar parentesis (*id).atributo
                disco2.mbr_partition_2.part_start=(*aux).part_start+(*aux).part_s+1;
                disco2.mbr_partition_3.part_start=disco2.mbr_partition_2.part_start+disco2.mbr_partition_2.part_s+1;
                disco2.mbr_partition_4.part_start=disco2.mbr_partition_3.part_start+disco2.mbr_partition_3.part_s+1;
            }else if(strcmp(disco2.mbr_partition_2.part_name,id)==0){
                disco2.mbr_partition_3.part_start=disco2.mbr_partition_2.part_start+disco2.mbr_partition_2.part_s+1;
                disco2.mbr_partition_4.part_start=disco2.mbr_partition_3.part_start+disco2.mbr_partition_3.part_s+1;
            }else if(strcmp(disco2.mbr_partition_3.part_name,id)==0){
                aux=&disco2.mbr_partition_3;
                (*aux).part_s=(*aux).part_s+tamano; //se esta modificando el struct original para referenciarse en algun atributo usar parentesis (*id).atributo
                disco2.mbr_partition_4.part_start=disco2.mbr_partition_3.part_start+disco2.mbr_partition_3.part_s+1;
            }else if(strcmp(disco2.mbr_partition_4.part_name,id)==0){
                aux=&disco2.mbr_partition_4;
                (*aux).part_s=(*aux).part_s+tamano; //se esta modificando el struct original para referenciarse en algun atributo usar parentesis (*id).atributo
            }
        
        }else if (OpFdisk>1){//DELETE 
             if(strcmp(disco2.mbr_partition_1.part_name,id)==0){
                //size tiene el tamaño en bytes (8 bits)
                int tam=disco2.mbr_partition_1.part_s; //El tamaño de las particiones estara en bytes
                int inicio=disco2.mbr_partition_1.part_start;
                fseek(archivo_bin, inicio, SEEK_SET);
                char aux='\0';
                for (size_t i = 0; i < tam; i++)
                {
                    fwrite(&aux,sizeof(aux),1,archivo_bin);
                }
                disco2.mbr_partition_1=vacioP;
                fseek(archivo_bin,0,SEEK_SET);
                fwrite(&disco2,sizeof(disco2),1,archivo_bin);
                fclose(archivo_bin);

             }else if(strcmp(disco2.mbr_partition_2.part_name,id)==0){
                int tam=disco2.mbr_partition_2.part_s;
                int inicio=disco2.mbr_partition_2.part_start;
                fseek(archivo_bin, inicio, SEEK_SET);
                char aux='\0';
                for (size_t i = 0; i < tam; i++)
                {
                    fwrite(&aux,sizeof(aux),1,archivo_bin);
                }
                disco2.mbr_partition_2=vacioP;
                fseek(archivo_bin,0,SEEK_SET);
                fwrite(&disco2,sizeof(disco2),1,archivo_bin);
                fclose(archivo_bin);

             }else if(strcmp(disco2.mbr_partition_3.part_name,id)==0){
                int tam=disco2.mbr_partition_3.part_s;
                int inicio=disco2.mbr_partition_3.part_start;
                fseek(archivo_bin, inicio, SEEK_SET);
                char aux='\0';
                for (size_t i = 0; i < tam; i++)
                {
                    fwrite(&aux,sizeof(aux),1,archivo_bin);
                }
                disco2.mbr_partition_3=vacioP;
                fseek(archivo_bin,0,SEEK_SET);
                fwrite(&disco2,sizeof(disco2),1,archivo_bin);
                fclose(archivo_bin);

             }else if(strcmp(disco2.mbr_partition_4.part_name,id)==0){
                int tam=disco2.mbr_partition_4.part_s;
                int inicio=disco2.mbr_partition_4.part_start;
                fseek(archivo_bin, inicio, SEEK_SET);
                char aux='\0';
                for (size_t i = 0; i < tam; i++)
                {
                    fwrite(&aux,sizeof(aux),1,archivo_bin);
                }
                disco2.mbr_partition_4=vacioP;
                fseek(archivo_bin,0,SEEK_SET);
                fwrite(&disco2,sizeof(disco2),1,archivo_bin);
                fclose(archivo_bin);

             }
        }
    }else
    {
        cout << "El archivo a usar con fdisk no existe" << endl;
    }
}
