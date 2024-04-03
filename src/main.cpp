
#include "hash_table.h"
#include "hash_funcs.h"

const char* text_files[NUM_HASH_FUNCS] = {"data/data_zero_hash.csv",
                                          "data/data_first_hash.csv",
                                          "data/data_lenght_hash.csv",
                                          "data/data_sum_hash.csv",
                                          "data/data_aver_hash.csv",
                                          "data/data_rol_hash.csv",
                                          "data/data_ror_hash.csv",
                                          "data/data_gnu_hash.csv"};

size_t (*hash_funcs[NUM_HASH_FUNCS]) (char*) =  {ZeroHash, 
                                                 FirstHash,
                                                 LenghtHash,
                                                 SumHash,
                                                 AverageHash,
                                                 ROLHash,
                                                 RORHash,
                                                 GNUHash};




int main()
{
        hash_table table = {};
        err_allocator err_alloc = {};
        error_allocator_Ctor(&err_alloc);
        
        for (size_t i = 0; i < NUM_HASH_FUNCS; i++)
        {
                CreateHashTable(&table, hash_funcs[i], text_files[i], &err_alloc);
        }

        error_allocator_Dtor(&err_alloc);
        return 0;
}