#ifndef STRUCTS_H
#define STRUCTS_H
#include <algorithm>

#include <string>
#include <ctime>

using namespace std;

class Structs {
public:
    Structs();
    // Structs
struct Transition{

};
struct Partition
{
    char part_status='0';
    char part_type;
    char part_fit;
    int part_start=-1;
    int part_s=0;
    char part_name[16];
} partition_,vacioP;

struct MBR
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_dsk_signature;
    char dsk_fit;
    Partition mbr_partition_1;
    Partition mbr_partition_2;
    Partition mbr_partition_3;
    Partition mbr_partition_4;
} disco, disco2;

struct EBR
{
    char part_status='0';
    char part_fit;
    int part_start;
    int part_s=0;
    int part_next=-1;
    char part_name[16];
} ebr;

};

#endif