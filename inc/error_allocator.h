#ifndef error_allocatorlib
#define error_allocatorlib

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define INSERT_ERROR_NODE(err_alloc, message) error_insert(err_alloc, message, __FILE__, __FUNCTION__, __LINE__)
#define ERR_ALLOC_TERMINATE(err_alloc) dump_error(err_alloc); error_allocator_Dtor(err_alloc);

const size_t MAX_SIZE_MESSAGE  = 30;
const size_t NUMBERS_OF_ERRORS = 3;

struct err_info
{
    int     code;
    char    message[MAX_SIZE_MESSAGE];
    char    file[MAX_SIZE_MESSAGE]; 
    char    func[MAX_SIZE_MESSAGE];   
    int     line;
};


struct err_node
{
    ssize_t      next;
    err_info    error;
};

struct err_allocator
{
    size_t      size;
    err_node*   nodes;
};

int error_allocator_Ctor(err_allocator* err_alloc);
int error_allocator_Dtor(err_allocator* err_alloc);

int error_insert(err_allocator* err_alloc, const char message[], const char file[], const char func[], const int line);
int  error_erase(err_allocator* err_alloc);


int dump_error(err_allocator* err_alloc);

#endif