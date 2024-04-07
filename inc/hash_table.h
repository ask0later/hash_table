#ifndef HASH_MAP_LIB
#define HASH_MAP_LIB

#include <math.h>

#include "error_allocator.h"
#include "list.h"
#include "read_file.h"

typedef char* Elem_t;


const char    TEST_FILE[] = "text/test.txt";
const char  HAMLET_FILE[] = "text/hamlet.txt";
const char GENERAL_FILE[] = "data/general.txt";

const size_t HASH_TABLE_SIZE = 103;
const size_t MAX_SIZE_WORD   = 20;

struct hash_table
{
        size_t (*hash_function) (char*);
        List**    bucket;
        size_t      size;
        size_t  elem_num;
};



int  CtorHashTable(hash_table* table, size_t (*hash_function) (char*), err_allocator* err_alloc);
void DtorHashTable(hash_table* table);


int CompleteHashTable(const char file[], hash_table* table, err_allocator* err_alloc);
int DeleteTableNodeData(hash_table* table);

int RemoveElem(Elem_t elem, hash_table* table, err_allocator* err_alloc);







int TestHashFunction(hash_table* table, size_t (*hash_function) (char*), const char text_file[], err_allocator* err_alloc);

//------------------------------------------------------------
int DumpHashTable(FILE* To, hash_table* table);

//------------------------------------------------------------
size_t GetDispersion(hash_table* table);
size_t GetLoadFactor(hash_table* table);


#endif