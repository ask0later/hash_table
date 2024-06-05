
#include "error_allocator.h"
#include "hash_table.h"
#include "hash_funcs.h"
#include "list.h"

const char* text_files[] ={"data/data_zero_hash.csv",
                           "data/data_first_hash.csv",
                           "data/data_lenght_hash.csv",
                           "data/data_sum_hash.csv",
                           "data/data_aver_hash.csv",
                           "data/data_rol_hash.csv",
                           "data/data_ror_hash.csv",
                           "data/data_gnu_hash.csv",
                           "data/data_crc32.csv"};

int main()
{
        hash_table table = {};
        err_allocator err_alloc = {};
        CtorErrorAllocator(&err_alloc);

        TestHashFunction(&table, CRC32Hash, "data/data_crc32.csv", &err_alloc);

        DtorErrorAllocator(&err_alloc);
        return 0;
}