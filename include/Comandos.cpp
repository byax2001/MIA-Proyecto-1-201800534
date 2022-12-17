#include "../lib/Comandos.h"

int startValue;

Comandos::Comandos()
{
    // Constructor
}

void Comandos::ejecutarInst(Parametros p)
    
{
    if (p.Comando == "mkdisk")
    { // Se identifica el coman do
        if(!(p.dimensional=='b' || p.dimensional=='k'|| p.dimensional=='m')){
            shared.handler("MKDISK","-u no contiene los valores esperados...");
            return;
        };
        if(!(p.ajuste_particion=='f' || p.ajuste_particion=='w'|| p.ajuste_particion=='b')){
            shared.handler("MKDISK","Tipo de ajuste de particion erroneos..");
            return;
        };
        
        CreateDisk(p.tamano, p.ajuste_particion, p.dimensional, p.path);
    }
    else if (p.Comando == "fdisk")
    {   

        
        if(p.dimensional!='k' && p.dimensional!='m' && p.dimensional !='b'){
            shared.handler("FDISK","Error la dimensional no tiene parametros correctos ");
            return;
        }
        fdisk(p.opcionFdisk,p.tamano,p.dimensional,p.path,p.typePartition,p.fit,p.name,p.add,p._delete);
    }
    else if (p.Comando == "rmdisk")
    {
        DeleteFile(p.path);
    }else if (p.Comando=="mount"){
       
    }else if (p.Comando == "unmount"){
       
    }else if (p.Comando == "mkfs"){

    }
    else
    {
        cout << "Error comando no conocido" << endl;
        exit(1);
    }
}
//MKDISK
void Comandos::CreateDisk(int tamano, char t_particion, char dim, string path)
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
    if (dim == 'k')
    {
        size_file = size_file;
    }
    else if (dim == 'm')
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
    //STATUS
    disco.mbr_partition_1.part_status='0';
    disco.mbr_partition_2.part_status='0';
    disco.mbr_partition_3.part_status='0';
    disco.mbr_partition_4.part_status='0';
    

    //ubicarse al inicio del disco rellenado con \0 y luego escribir el mbr
    fseek(archivo_bin, 0, SEEK_SET);
    fwrite(&disco, sizeof(disco), 1, archivo_bin);
    fclose(archivo_bin);

    //indicar que el disco fue creado
    int indexDoc = path.rfind("/");
    string nameComandos = path.substr(indexDoc,path.length());
    shared.response("MKDISK","Disco "+nameComandos+" creado con exito!");
    FILE *file = fopen(path.c_str(), "rb+");
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
        shared.handler("RMDISK","Error al eliminar el archivo ");
    }
    else
    {
        shared.response("RMDISK","Se elimino el archivo correctamente ");
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
        //evaluar tamaño correcto
        if(s<=0){
            throw runtime_error("-size debe de ser mayor que 0");
        }
        //evaluar que las dimensionales sean correctas
        if(u=='b' || u=='k'|| u=='m'){
            if(!u=='b'){
                s *= (u=='k') ? 1024: 1024*1024;
            }
        }else{
            throw runtime_error("-u no contiene los valores esperados...");
        }
        
        //Evaluar que el tipo de particiones sea el correcto
        if(!(t=='p' ||  t=='e' || t=='l')){
            throw runtime_error("-t no contiene los valores esperados...");
        }
        //Evaluar que el tipo e fit sea el correcto
        if(!(f=='b' || f=='f' || f=='w')){
            throw runtime_error("-f no contiene los valores esperados...");
        }
        
        //VERIFICAR QUE EL DISCO EXISTA 
        Structs::MBR disco; //structura mbr del disco en realidad
        FILE *file = fopen(p.c_str(), "rb+");
        if(file!=NULL)
        {
            rewind(file);
            fread(&disco, sizeof(disco),1,file);
        }else{
            shared.handler("FDISK", "Disco inexistente!");
            return;
        }
        fclose(file);
        
        //get partitions retorna un vector de structs Particion ya existentes
        vector<Structs::Partition> partitions = getPartitions(disco); 

        //VERIFICIAR QUE NO HALLA MAS DE DOS EXTENDIDAS
        for(Structs::Partition part : partitions){
            cout<<"part_type "<<part.part_type<<" type: "<< t<<endl;
            shared.Pause_press_to_continue();
            if(part.part_type=='e' && t=='e'){
                shared.handler("FDISK","Error no puede haber mas de dos particiones extendidas");
                return;
            }
        }

        vector<Transition> between;//vector entre transiciones

        int used = 0; //usado
        int ext = 0; //particion extendida
        int c = 1; //particion 1 
        int base = sizeof(disco); // peso del disco 
        Structs::Partition extended; //extendida
        for(Structs::Partition prttn: partitions){ //recorrer cada particion recolectada anteriormente
            if(prttn.part_status == '1'){ //estatus l 
                //start = inicio de la particion
                //end = fin de la particion
                //before  (antes)  = start - tam mbr al inicio
                //base = lo que esta antes de la particion
                //after = start - end 
                //used para saber cuantas particiones estan usadas

                Transition trn; //transicion
                trn.partition = c; //particion1
                trn.start = prttn.part_start; //inicio
                //part_s es igual al tamaño,  end = inicio + tamaño de particion
                trn.end = prttn.part_start + prttn.part_s; //final de la particion
                //before (antes) = inicio - tam MBR
                trn.before = trn.start - base; //resta entre el inicio de la particion y el tamanio del disco en la primera iteracion
                base = trn.end; //se sustituye ahora el valor de base 

                if(used !=0)// SI YA SE MODIFICO LA PARTICION 
                {
                    //transicion (between[n]) . despues  = start - transicion[n].end
                    between.at(used-1).after = trn.start - (between.at(used-1).end);
                }
                between.push_back(trn); //INGRESAR AL VECTOR
                used++;
                //para saber si existe particion extendida
                if(prttn.part_type == 'e'){
                    ext++;
                    extended = prttn;
                }
            }
            //SOLO SE PUEDEN CREAR LOGICAS LUEGO DE 4 PARTICIONES
            if(used == 4 && !t=='l'){
                throw runtime_error("Limite de particiones alcanzado");
                return;
            }else if(ext==1 && t=='e'){
                throw runtime_error("Solo se puede crear una particion Extendida");
            }
            c++;
        }
        //SI SE DESEA CREAR UNA LOGICA Y NO HAY EXTENDIDAS DEBE DE TIRAR ERROR
        if (ext == 0 && t=='l') {
            throw runtime_error("No existe particion Extendida para crear la Logica");
        }
        
        //BEETWEEN ES UN VECTOR DE TRANSITIONS 
        if (used != 0) {
            //transicion [n-1]. antes  = disco_tam - transicionv [tamtransicionv -1 ] (ultimapos). end 
            between.at(between.size() - 1).after = disco.mbr_tamano - between.at(between.size() - 1).end;
        }

        try {
            //SE MANDA A USAR ESTE METODO SI TRUENA EL NOMBRE NO ESTA EN USO  POR LO QUE 
            //OCURRIRA UN ERROR QUE ATRAPARA EL CATCH Y SEGUIRA DE FORMA NORMAL EL PROGRMA
            //SI NO TRUENA ENTONCES EL NOMBRE YA ESTA EN USO
            findby(disco, n, p);
            shared.handler("FDISK", " Este nombre ya esta en uso");
            return;
        } catch (exception &e) {}
        
        //Nueva particion la que el usuario esta mandando
        Structs::Partition newPartition;
        newPartition.part_status = '1';
        newPartition.part_s = s;
        newPartition.part_type = t; //P,E,L
        cout<<endl<<"---------TIPO DE PARTICION: "<<newPartition.part_type<<endl;
        newPartition.part_fit = f; //B,F,W
        strcpy(newPartition.part_name, n.c_str());
        
        //LOGICASS---------------------------------------------------
        if (t=='l') {
            //INGRESA LA PARTICION LOGICA ADENTRO DE LA PARTICION EXTENDIDA 
            logic(newPartition, extended, p);
            return;
        }
        //METER NUEVA PARTICION AL MBR  
        disco = adjust(disco, newPartition, between, partitions, used); 
        cout<<"disco:--------------------------"<<endl;
        vector<Structs::Partition> partitions2 = getPartitions(disco); 
        for(Structs::Partition part: partitions2){
            cout<<"###### Partition :" <<part.part_type<<" name: "<<part.part_name<< " status: "<<part.part_status<<endl;;
        }


        FILE *bfile = fopen(p.c_str(), "rb+");
        if (bfile != NULL) {
            fseek(bfile, 0, SEEK_SET);
            fwrite(&disco, sizeof(Structs::MBR), 1, bfile);
            //SI LA PARTICION INGRESADA ES UNA PARTICION EXTENDIDA
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
    //u: used  (cuantas particiones llevan usaas )
    if (u == 0) {
        p.part_start = sizeof(mbr);
        startValue = p.part_start;
        mbr.mbr_partition_1 = p;
        return mbr;
    } else {
        Transition toUse;
        int c = 0;
        for (Transition tr : t) { //transicion por cada elemento del vector de transiciones
            if (c == 0) {
                toUse = tr;
                c++;
                continue;
            }

            if (mbr.dsk_fit == 'f') {
                if (toUse.before >= p.part_s || toUse.after >= p.part_s) {
                    break;
                }
                toUse = tr;
            } else if (mbr.dsk_fit == 'b') {
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
            } else if (mbr.dsk_fit == 'w') {
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
            if (mbr.dsk_fit == 'f') {
                if (toUse.before >= p.part_s) {
                    p.part_start = (toUse.start - toUse.before);
                    startValue = p.part_start;
                } else {
                    p.part_start = toUse.end;
                    startValue = p.part_start;
                }
            } else if (mbr.dsk_fit== 'b') {
                int b1 = toUse.before - p.part_s;
                int a1 = toUse.after - p.part_s;

                if ((toUse.before >= p.part_s && b1 < a1) || !(toUse.after >= p.part_start)) {
                    p.part_start = (toUse.start - toUse.before);
                    startValue = p.part_start;
                } else {
                    p.part_start = toUse.end;
                    startValue = p.part_start;
                }
            } else if (mbr.dsk_fit == 'w') {
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
            
            //ps: partitions mandados desde la llamada
            for (int i = 0; i < ps.size(); i++) {
                  
                partitions[i] = ps.at(i);
            }
            cout<<endl<<"STATUS PARTICION-----------"<<endl;
            for (auto &partition : partitions) {
                 cout<<"STATUS-----------"<<partition.part_status<<endl;;
                if (partition.part_status == '0') {
                    //disco libre mas cercano del array partitions que no es igual a ps 
                    partition = p;
                    break;
                }
            }
            cout<<endl<<"PARTICIONES ADJUST------------"<<endl;
            for (auto partition : partitions) {
                
                cout<<endl<<"name: "<<partition.part_name<<" tipo: "<<partition.part_type
                << " fit: "<<partition.part_fit<<" tam: "<<partition.part_s<<endl;
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
        //obtener las particiones logicas 
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
    fread(&tmp, sizeof(Structs::ebr), 1, file);
    int size=0;
    do {
        cout<<"size "<<size<<"struc ebr:"<<sizeof(Structs::ebr) <<"ebract size:" <<tmp.part_s<<"AAAAAAAAAAAAAAAAA LGOICICICICIC"<<endl;
        int size_aux=sizeof(Structs::ebr) + tmp.part_s;
        size =size+size_aux;
        cout<<"size"<<size<<"AAAAAAAAAAAAAAAAA LGOICICICICIC"<<endl;
        if (tmp.part_status == '0' && tmp.part_next == -1) {
            nlogic.part_start = tmp.part_start;
            nlogic.part_next = nlogic.part_start + nlogic.part_s + sizeof(Structs::ebr);
            //ep = particion extendida

            //size = tamaño STRUCT EBR + particion ebr actual.size 
            //tamno particion extendida mandada - size  <= tam nueva particion
            cout<<endl<<"EP SIZE: "<<ep.part_s<<" tmp tam:"<<tmp.part_s
            <<" SIZE: "<<size<< " NLOGIC TAM "<<nlogic.part_s<<
            "struct ebr: "<<sizeof(Structs::ebr)<<endl;
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
        }else{
            if (strcmp(tmp.part_name,partition.part_name)==0){
                shared.handler("FDISK","Ya existe una particion con dicho nombre");
                return; 
            }
        }
        fseek(file, tmp.part_next, SEEK_SET);
        fread(&tmp, sizeof(Structs::EBR), 1, file);
    } while (true);
}

//retorna un vector de particiones logicas
vector<Structs::EBR> Comandos::getlogics(Structs::Partition partition, string p) {
    vector<Structs::EBR> ebrs;
    FILE *file = fopen(p.c_str(), "rb+");
    rewind(file);
    Structs::EBR tmp;
    fseek(file, partition.part_start, SEEK_SET);
    fread(&tmp, sizeof(Structs::EBR), 1, file);
    //recorrer el archivo hasta hallar datos ilogicos, ahi no habra nada y sera el fin de las particiones
    int n=0;
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

void Comandos::deletepartition(string d, string p, string n) {
    try {
        /*EL AUX DIJO QUE SOLO VA A VENIR FULL EL DELETE SIEMPRE
        if (!(shared.compare(d, "fast") || shared.compare(d, "full"))) {
            throw runtime_error("-delete necesita valores específicos");
        }
        */
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
                       /* if (shared.compare(d, "fast")) {
                            partitions[i].part_status = '0'; //eliminar particion
                        } else {*/
                            past = partitions[i];
                            partitions[i] = Structs::Partition();
                            fll = true;
                        //}
                        break;
                    }
                } else {
                    if (shared.compare(partitions[i].part_name, n)) {
                        /*if (shared.compare(d, "fast")) {
                            partitions[i].part_status = '0';
                        } else {
                            past = partitions[i];
                            partitions[i] = Structs::Partition();
                            fll = true;
                        }*/

                        past = partitions[i];
                        int start = partitions[i].part_start;
                        int tam = partitions[i].part_s;
                        char caracter = '\0';
                        partitions[i] = Structs::Partition();
                        
                        //BORRAR TODO ADENTRO DE LA PARTICION EXTENDIDA
                        fseek(file, start, SEEK_SET);
                        for (int i = 0; i < tam; i++)
                        {
                            fwrite(&caracter,sizeof(caracter),1,file);
                        }
                        fll = true;
                        break; //SI ERA LA EXTENDIDA EL CICLO SE DETIENE ACA
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
        int i = add;
        //tamaño en bytes 
        if (u=='b' || u=='k' || u=='m') {

            if (!u=='b') {
                i *= (u=='k') ? 1024 : 1024 * 1024;
            }
        } else {
            throw runtime_error("-u necesita valores específicos");
        }
        
        int add_tam = i;
        //si el disco existe 
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
                //n = name
                if (shared.compare(partitions[i].part_name, n)) {
                    //part_s = tamaño de la particion
                    //si la particion no esta vacia
                    if ((partitions[i].part_s + (add_tam)) > 0) {
                        //SI LA PARTICION CON EL NOMBRE CORRECTO ESTA ANTES DEL ULTIMO
                        if (i != 3) {
                            if (partitions[i + 1].part_start != 0) {
                                if (add_tam > 0) {
                                    if ((partitions[3].part_s + (add_tam) +partitions[3].part_start) <=
                                    disk.mbr_tamano) {
                                        partitions[3].part_s += add_tam;
                                        break;
                                    } else {
                                        throw runtime_error("se sobrepasa el límite");
                                    }
                                }else{
                                    if (((partitions[i].part_s + (add_tam) +partitions[i].part_start) >
                                        partitions[i].part_start)) {
                                        partitions[i].part_s -= add_tam;
                                        break;
                                    } else {
                                        throw runtime_error("Elimina mas espacio del posible");
                                    }
                                    partitions[i].part_s -= add_tam; 
                                }
                                
                            }
                        }
                        //SI LA PARTICION CON EL NOMBRE BUSCADO ES EL ULTIMO DATO
                        if (add_tam > 0) {
                            if ((partitions[3].part_s + (add_tam) +partitions[3].part_start) <=
                                disk.mbr_tamano) {
                                partitions[3].part_s += add_tam;
                                break;
                            } else {
                                throw runtime_error("se sobrepasa el límite");
                            }
                        }else{
                            if (((partitions[i].part_s + (add_tam) +partitions[i].part_start) >
                                partitions[i].part_start)) {
                                partitions[i].part_s -= add_tam;
                                break;
                            } else {
                                throw runtime_error("Elimina mas espacio del posible");
                            }
                            partitions[i].part_s -= add_tam; 
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
        shared.response("FDISK", "la partición se ha aumentado/disminuido correctamente");
        fclose(file);
    }
    catch (exception &e) {
        shared.handler("FDISK", e.what());
        return;
    }

}
