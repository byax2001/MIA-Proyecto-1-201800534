#include "../lib/Comandos.h"

int startValue;

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
//MKDISK
void Comandos::CreateDisk(int tamano, char t_particion, string dim, string path)
{
    //-s: tamaño del disco
    //-f:bf,ff,wf
    //-u: unidad
    //-path: direccion 
    Structs::MBR disco;
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
    string nameComandos = path.substr(indexDoc,path.length());
    cout<<"Disco" <<nameComandos<<" creado con exito!"<<endl;
    FILE *file = fopen(path.c_str(), "w+b");
    fread(&disco, sizeof(disco), 1, archivo_bin);
    cout<<"Tamaño: "<<disco.mbr_tamano<<" bytes"<<endl;
    cout<<"Fit: "<<disco.dsk_fit<<endl;
    struct tm *tm;
    tm = localtime(&disco.mbr_fecha_creacion);
    char mostrar_fecha [20];
    strftime(mostrar_fecha, 20, "%Y/%m/%d %H:%M:%S",tm); 
    cout<<"Fecha de creacion: "<<mostrar_fecha<<endl;
    cout<<"Signature "<<disco.mbr_dsk_signature<<endl;
    cout<<"Path: "<<path<<endl;
    fclose(file);
    shared.Pause_press_to_continue();//presione cualquier tecla para continuar
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
    Structs::MBR disco,disco2;
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

//FDISK
void Comandos::fdisk(char FdiskOption,int s,char u,string path, char tPart,char fit,string name,int add,string _delete)
{
    if(FdiskOption=='c'){
        //s, u, path, t,f
        generatepartition(s,u,path,tPart,fit,name);
    }else if(FdiskOption=='c'){
        addpartition(add,u,name,path);
    }else{
        deletepartition(_delete,path,name);
    }
}

void Comandos::generatepartition(int s,char u, string p, char t, char f, string n){
    try
    {
        startValue = 0;
        if(s<=0){
            throw runtime_error("-size debe de ser mayor que 0");
        }
        if(u=='b' || u=='k'|| u=='m'){
            if(!u=='b'){
                s *= (u=='k') ? 1024: 1024*1024;
            }
        }else{
            throw runtime_error("-u no contiene los valores esperados...");
        }

        if(!(t=='p' ||  t=='e' || t=='l')){
            throw runtime_error("-t no contiene los valores esperados...");
        }
        if(!(f=='b' || f=='f' || f=='w')){
            throw runtime_error("-f no contiene los valores esperados...");
        }
        Structs::MBR disco; //
        FILE *file = fopen(p.c_str(), "rb+");
        if(file!=NULL) //Verifica que el disco exista
        {
            rewind(file);
            fread(&disco, sizeof(disco),1,file);
        }else{
            shared.handler("FDISK", "Disco inexistente!");
            return;
        }
        fclose(file);
        //get partitions retorna un vector de structs Particion ya existentes
        vector<Structs::Partition> partitions = getPartitions(disco); // status=0;
        vector<Transition> between;//vector entre transiciones

        int used = 0; //usado
        int ext = 0; //particion extendida
        int c = 1; //particion 1 
        int base = sizeof(disco); // peso del disco 
        Structs::Partition extended; //extendida
        for(Structs::Partition prttn: partitions){ //recorrer cada particion recolectada anteriormente
            if(prttn.part_status == '1'){ //estatus l 
                Transition trn; //transicion
                trn.partition = c; //particion1
                trn.start = prttn.part_start; //inicio
                trn.end = prttn.part_start + prttn.part_s; //final de la particion

                trn.before = trn.start - base; //resta entre el inicio de la particion y el tamanio del disco en la primera iteracion
                base = trn.end; //nuevo valor  (lugar de inicio de la nueva particion)

                if(used !=0)
                {
                    between.at(used-1).after = trn.start - (between.at(used-1).end);
                }
                between.push_back(trn);
                used++;
                //para saber si existe particion extendida
                if(prttn.part_type == 'e' || prttn.part_type == 'E'){
                    ext++;
                    extended = prttn;
                }
            }
            //LIMITE E PARTICIONES
            if(used == 4 && !t=='l'){
                throw runtime_error("Limite de particiones alcanzado");
            }else if(ext==1 && t=='e'){
                throw runtime_error("Solo se puede crear una particion Extendida");
            }
            c++;
        }

        if (ext == 0 && t=='l') {
            throw runtime_error("No existe particion Extendida para crear la Logica");
        }
        if (used != 0) {
            between.at(between.size() - 1).after = disco.mbr_tamano - between.at(between.size() - 1).end;
        }

        try {
            findby(disco, n, p);
            shared.handler("FDISK", " Este nombre ya esta en uso");
            return;
        } catch (exception &e) {
        }
        //Nueva particion la que el usuario esta mandando
        Structs::Partition transition;
        transition.part_status = '1';
        transition.part_s = 'size';
        transition.part_type = t; //P,E,L
        transition.part_fit = f;
        strcpy(transition.part_name, n.c_str());
        //buenos pases
        if (t=='l') {
            logic(transition, extended, p);
            return;
        }
        disco = adjust(disco, transition, between, partitions, used); 

        FILE *bfile = fopen(p.c_str(), "rb+");
        if (bfile != NULL) {
            fseek(bfile, 0, SEEK_SET);
            fwrite(&disco, sizeof(Structs::MBR), 1, bfile);
            if (t=='e') {
                Structs::EBR ebr;
                ebr.part_start = startValue;
                fseek(bfile, startValue, SEEK_SET);
                fwrite(&ebr, sizeof(Structs::EBR), 1, bfile);
            }
            fclose(bfile);
            shared.response("FDISK", "partición creada correctamente");
        }
    }
    catch (invalid_argument &e) {
        shared.handler("FDISK", "-size debe ser un entero");
        return;
    }
    catch (exception &e) {
        shared.handler("FDISK", e.what());
        return;
    }
}

vector<Structs::Partition> Comandos::getPartitions(Structs::MBR disco) {
    vector<Structs::Partition> v;
    v.push_back(disco.mbr_partition_1);
    v.push_back(disco.mbr_partition_2);
    v.push_back(disco.mbr_partition_3);
    v.push_back(disco.mbr_partition_4);
    return v;
}
Structs::MBR

Comandos::adjust(Structs::MBR mbr, Structs::Partition p, vector<Transition> t, vector<Structs::Partition> ps, int u) {
    if (u == 0) {
        p.part_start = sizeof(mbr);
        startValue = p.part_start;
        mbr.mbr_partition_1 = p;
        return mbr;
    } else {
        Transition toUse;
        int c = 0;
        for (Transition tr : t) {
            if (c == 0) {
                toUse = tr;
                c++;
                continue;
            }

            if (toupper(mbr.dsk_fit) == 'F') {
                if (toUse.before >= p.part_s || toUse.after >= p.part_s) {
                    break;
                }
                toUse = tr;
            } else if (toupper(mbr.dsk_fit) == 'B') {
                if (toUse.before < p.part_s || toUse.after < p.part_s) {
                    toUse = tr;
                } else {
                    if (tr.before >= p.part_s || tr.after >= p.part_s) {
                        int b1 = toUse.before - p.part_s;
                        int a1 = toUse.after - p.part_s;
                        int b2 = tr.before - p.part_s;
                        int a2 = tr.after - p.part_s;

                        if ((b1 < b2 && b1 < a2) || (a1 < b2 && a1 < a2)) {
                            c++;
                            continue;
                        }
                        toUse = tr;
                    }
                }
            } else if (toupper(mbr.dsk_fit) == 'W') {
                if (!(toUse.before >= p.part_s) || !(toUse.after >= p.part_s)) {
                    toUse = tr;
                } else {
                    if (tr.before >= p.part_s || tr.after >= p.part_s) {
                        int b1 = toUse.before - p.part_s;
                        int a1 = toUse.after - p.part_s;
                        int b2 = tr.before - p.part_s;
                        int a2 = tr.after - p.part_s;

                        if ((b1 > b2 && b1 > a2) || (a1 > b2 && a1 > a2)) {
                            c++;
                            continue;
                        }
                        toUse = tr;
                    }
                }
            }
            c++;
        }
        if (toUse.before >= p.part_s || toUse.after >= p.part_s) {
            if (toupper(mbr.dsk_fit) == 'F') {
                if (toUse.before >= p.part_s) {
                    p.part_start = (toUse.start - toUse.before);
                    startValue = p.part_start;
                } else {
                    p.part_start = toUse.end;
                    startValue = p.part_start;
                }
            } else if (toupper(mbr.dsk_fit) == 'B') {
                int b1 = toUse.before - p.part_s;
                int a1 = toUse.after - p.part_s;

                if ((toUse.before >= p.part_s && b1 < a1) || !(toUse.after >= p.part_start)) {
                    p.part_start = (toUse.start - toUse.before);
                    startValue = p.part_start;
                } else {
                    p.part_start = toUse.end;
                    startValue = p.part_start;
                }
            } else if (toupper(mbr.dsk_fit) == 'W') {
                int b1 = toUse.before - p.part_s;
                int a1 = toUse.after - p.part_s;

                if ((toUse.before >= p.part_s && b1 > a1) || !(toUse.after >= p.part_start)) {
                    p.part_start = (toUse.start - toUse.before);
                    startValue = p.part_start;
                } else {
                    p.part_start = toUse.end;
                    startValue = p.part_start;
                }
            }
            Structs::Partition partitions[4];
            for (int i = 0; i < ps.size(); i++) {
                partitions[i] = ps.at(i);
            }
            for (auto &partition : partitions) {
                if (partition.part_status == '0') {
                    partition = p;
                    break;
                }
            }

            Structs::Partition aux;
            for (int i = 3; i >= 0; i--) {
                for (int j = 0; j < i; j++) {
                    if ((partitions[j].part_start > partitions[j + 1].part_start)) {
                        aux = partitions[j + 1];
                        partitions[j + 1] = partitions[j];
                        partitions[j] = aux;
                    }
                }
            }

            for (int i = 3; i >= 0; i--) {
                for (int j = 0; j < i; j++) {
                    if (partitions[j].part_status == '0') {
                        aux = partitions[j];
                        partitions[j] = partitions[j + 1];
                        partitions[j + 1] = aux;
                    }
                }
            }
            mbr.mbr_partition_1 = partitions[0];
            mbr.mbr_partition_2 = partitions[1];
            mbr.mbr_partition_3 = partitions[2];
            mbr.mbr_partition_4 = partitions[3];
            return mbr;
        } else {
            throw runtime_error("no hay espacio suficiente");
        }
    }
}

Structs::Partition Comandos::findby(Structs::MBR mbr, string name, string path) {
    Structs::Partition partitions[4];
    partitions[0] = mbr.mbr_partition_1;
    partitions[1] = mbr.mbr_partition_2;
    partitions[2] = mbr.mbr_partition_3;
    partitions[3] = mbr.mbr_partition_4;

    bool ext = false;
    Structs::Partition extended;
    for (auto &partition : partitions) {
        if (partition.part_status == '1') {
            if (shared.compare(partition.part_name, name)) {
                return partition;
            } else if (partition.part_type == 'E') {
                ext = true;
                extended = partition;
            }
        }
    }
    if (ext) {
        vector<Structs::EBR> ebrs = getlogics(extended, path);
        for (Structs::EBR ebr : ebrs) {
            if (ebr.part_status == '1') {
                if (shared.compare(ebr.part_name, name)) {
                    Structs::Partition tmp;
                    tmp.part_status = '1';
                    tmp.part_type = 'L';
                    tmp.part_fit = ebr.part_fit;
                    tmp.part_start = ebr.part_start;
                    tmp.part_s = ebr.part_s;
                    strcpy(tmp.part_name, ebr.part_name);
                    return tmp;
                }
            }
        }
    }
    throw runtime_error("la partición no existe");
}

void Comandos::logic(Structs::Partition partition, Structs::Partition ep, string p) {
    Structs::EBR nlogic;
    nlogic.part_status = '1';
    nlogic.part_fit = partition.part_fit;
    nlogic.part_s = partition.part_s;
    nlogic.part_next = -1;
    strcpy(nlogic.part_name, partition.part_name);

    FILE *file = fopen(p.c_str(), "rb+");
    rewind(file);
    Structs::EBR tmp;
    fseek(file, ep.part_start, SEEK_SET);
    fread(&tmp, sizeof(Structs::EBR), 1, file);
    int size;
    do {
        size += sizeof(Structs::EBR) + tmp.part_s;
        if (tmp.part_status == '0' && tmp.part_next == -1) {
            nlogic.part_start = tmp.part_start;
            nlogic.part_next = nlogic.part_start + nlogic.part_s + sizeof(Structs::EBR);
            if ((ep.part_s - size) <= nlogic.part_s) {
                throw runtime_error("no hay espacio para más particiones lógicas");
            }
            fseek(file, nlogic.part_start, SEEK_SET);
            fwrite(&nlogic, sizeof(Structs::EBR), 1, file);
            fseek(file, nlogic.part_next, SEEK_SET);
            Structs::EBR addLogic;
            addLogic.part_status = '0';
            addLogic.part_next = -1;
            addLogic.part_start = nlogic.part_next;
            fseek(file, addLogic.part_start, SEEK_SET);
            fwrite(&addLogic, sizeof(Structs::EBR), 1, file);
            shared.response("FDISK", "partición creada correctamente");
            fclose(file);
            return;
        }
        fseek(file, tmp.part_next, SEEK_SET);
        fread(&tmp, sizeof(Structs::EBR), 1, file);
    } while (true);
}

vector<Structs::EBR> Comandos::getlogics(Structs::Partition partition, string p) {
    vector<Structs::EBR> ebrs;

    FILE *file = fopen(p.c_str(), "rb+");
    rewind(file);
    Structs::EBR tmp;
    fseek(file, partition.part_start, SEEK_SET);
    fread(&tmp, sizeof(Structs::EBR), 1, file);
    do {
        if (!(tmp.part_status == '0' && tmp.part_next == -1)) {
            if (tmp.part_status != '0') {
                ebrs.push_back(tmp);
            }
            fseek(file, tmp.part_next, SEEK_SET);
            fread(&tmp, sizeof(Structs::EBR), 1, file);
        } else {
            fclose(file);
            break;
        }
    } while (true);
    return ebrs;
}

void Comandos::deletepartition(string _delete, string p, string n) {
    try {

        if (p.substr(0, 1) == "\"") {
            p = p.substr(1, p.length() - 2);
        }

        if (!(shared.compare(d, "fast") || shared.compare(d, "full"))) {
            throw runtime_error("-delete necesita valores específicos");
        }

        FILE *file = fopen(p.c_str(), "rb+");
        if (file == NULL) {
            throw runtime_error("disco no existente");
        }

        Structs::MBR disk;
        rewind(file);
        fread(&disk, sizeof(Structs::MBR), 1, file);

        findby(disk, n, p);

        Structs::Partition partitions[4];
        partitions[0] = disk.mbr_partition_1;
        partitions[1] = disk.mbr_partition_2;
        partitions[2] = disk.mbr_partition_3;
        partitions[3] = disk.mbr_partition_4;

        Structs::Partition past;
        bool fll = false;
        for (int i = 0; i < 4; i++) {
            if (partitions[i].part_status == '1') {
                if (partitions[i].part_type == 'P') {
                    if (shared.compare(partitions[i].part_name, n)) {
                        if (shared.compare(d, "fast")) {
                            partitions[i].part_status = '0';
                        } else {
                            past = partitions[i];
                            partitions[i] = Structs::Partition();
                            fll = true;
                        }
                        break;
                    }
                } else {
                    if (shared.compare(partitions[i].part_name, n)) {
                        if (shared.compare(d, "fast")) {
                            partitions[i].part_status = '0';
                        } else {
                            past = partitions[i];
                            partitions[i] = Structs::Partition();
                            fll = true;
                        }
                        break;
                    }
                    vector<Structs::EBR> ebrs = getlogics(partitions[i], p);
                    int count = 0;
                    for (Structs::EBR ebr : ebrs) {
                        if (shared.compare(ebr.part_name, n)) {
                            ebr.part_status = '0';
                        }
                        fseek(file, ebr.part_start, SEEK_SET);
                        fwrite(&ebr, sizeof(Structs::EBR), 1, file);
                        count++;
                    }
                    shared.response("FDISK", "partición eliminada correctamente -" + d);
                    return;
                }
            }
        }

        Structs::Partition aux;
        for (int i = 3; i >= 0; i--) {
            for (int j = 0; j < i; j++) {
                if (partitions[j].part_status == '0') {
                    aux = partitions[j];
                    partitions[j] = partitions[j + 1];
                    partitions[j + 1] = aux;
                }
            }
        }

        disk.mbr_partition_1 = partitions[0];
        disk.mbr_partition_2 = partitions[1];
        disk.mbr_partition_3 = partitions[2];
        disk.mbr_partition_4 = partitions[3];

        rewind(file);
        fwrite(&disk, sizeof(Structs::MBR), 1, file);
        if (fll) {
            fseek(file, past.part_start, SEEK_SET);
            int num = static_cast<int>(past.part_s / 2);
            fwrite("\0", sizeof("\0"), num, file);
        }
        shared.response("FDISK", "partición eliminada correctamente -" + d);
        fclose(file);
    }
    catch (exception &e) {
        shared.handler("FDISK", e.what());
        return;
    }
}

void Comandos::addpartition(int add, char u, string n, string p) {
    try {
        int i = stoi(add);

        if (shared.compare(u, "b") || shared.compare(u, "k") || shared.compare(u, "m")) {

            if (!shared.compare(u, "b")) {
                i *= (shared.compare(u, "k")) ? 1024 : 1024 * 1024;
            }
        } else {
            throw runtime_error("-u necesita valores específicos");
        }


        FILE *file = fopen(p.c_str(), "rb+");
        if (file == NULL) {
            throw runtime_error("disco no existente");
        }

        Structs::MBR disk;
        rewind(file);
        fread(&disk, sizeof(Structs::MBR), 1, file);

        findby(disk, n, p);

        Structs::Partition partitions[4];
        partitions[0] = disk.mbr_partition_1;
        partitions[1] = disk.mbr_partition_2;
        partitions[2] = disk.mbr_partition_3;
        partitions[3] = disk.mbr_partition_4;


        for (int i = 0; i < 4; i++) {
            if (partitions[i].part_status == '1') {
                if (shared.compare(partitions[i].part_name, n)) {
                    if ((partitions[i].part_s + (i)) > 0) {
                        if (i != 3) {
                            if (partitions[i + 1].part_start != 0) {
                                if (((partitions[i].part_s + (i) +
                                      partitions[i].part_start) <=
                                     partitions[i + 1].part_start)) {
                                    partitions[i].part_s += i;
                                    break;
                                } else {
                                    throw runtime_error("se sobrepasa el límite");
                                }
                            }
                        }
                        if ((partitions[i].part_s + i +
                             partitions[i].part_start) <= disk.mbr_tamano) {
                            partitions[i].part_s += i;
                            break;
                        } else {
                            throw runtime_error("se sobrepasa el límite");
                        }

                    }
                }
            }
        }

        disk.mbr_partition_1 = partitions[0];
        disk.mbr_partition_2 = partitions[1];
        disk.mbr_partition_3 = partitions[2];
        disk.mbr_partition_4 = partitions[3];

        rewind(file);
        fwrite(&disk, sizeof(Structs::MBR), 1, file);
        shared.response("FDISK", "la partición se ha aumentado correctamente");
        fclose(file);
    }
    catch (exception &e) {
        shared.handler("FDISK", e.what());
        return;
    }

}
