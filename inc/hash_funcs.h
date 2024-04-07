#ifndef HASH_FUNCS_LIB
#define HASH_FUNCS_LIB

#include <stdlib.h>
#include <string.h>

#define ROL(x) ((x << 1) | (x >> (8 * sizeof(size_t) - 1)))
#define ROR(x) ((x >> 1) | (x << (8 * sizeof(size_t) - 1)))

size_t      ZeroHash(char* buffer);
size_t     FirstHash(char* buffer);
size_t    LenghtHash(char* buffer);
size_t       SumHash(char* buffer);
size_t   AverageHash(char* buffer);
size_t       ROLHash(char* buffer);
size_t       RORHash(char* buffer);
size_t       GNUHash(char* buffer);

typedef size_t (*function_type)(char*);
const function_type hash_funcs[] = {ZeroHash, 
                                    FirstHash,
                                    LenghtHash,
                                    SumHash,
                                    AverageHash,
                                    ROLHash,
                                    RORHash,
                                    GNUHash};

#endif