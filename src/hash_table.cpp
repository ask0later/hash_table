#include "hash_table.h"
#include "hash_funcs.h"
#include "list.h"
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

// nasm -f elf64  print.asm
// g++ -no-pie main.cpp print.o

int CtorHashTable(hash_table* table, size_t (*hash_function) (char*), err_allocator* err_alloc)
{
        table->hash_function = hash_function;
        table->size = HASH_TABLE_SIZE;
        table->elem_num = 0;

        table->bucket = (List**) calloc(HASH_TABLE_SIZE, sizeof(List*));
        if (!table->bucket)
        {       
               INSERT_ERROR_NODE(err_alloc, "dynamic alloc failed");
               return 1; 
        }

        for (size_t i = 0; i < table->size; i++)
        {
                CtorList(&(table->bucket[i]));
        }

        return 0;
}



void DtorHashTable(hash_table* table)
{
        table->hash_function = NULL;

        for (size_t i = 0; i < HASH_TABLE_SIZE; i++)
        {
                DtorList(table->bucket[i]);
        }
        
        free(table->bucket);

        return;
}




int TestHashFunction(hash_table* table, size_t (*hash_function) (char*), const char text_file[], err_allocator* err_alloc)
{
        CtorHashTable(table, hash_function, err_alloc);
        CompleteHashTable(HAMLET_FILE, table, err_alloc);

        FILE* To = fopen(text_file, "w");
        if (To == NULL) printf("error");
        DumpHashTable(To, table);
        fclose(To);

        

        FILE* general = fopen(GENERAL_FILE, "a");
        if (general == NULL) printf("error");
        fprintf(general, "load factor = %lu, dispersion =  %lu, elem_num = %lu, table_size = %lu\n", GetLoadFactor(table), GetDispersion(table), table->elem_num, table->size);
        fclose(general);

        DtorHashTable(table);

        return 0;
}


int CompleteHashTable(const char file[], hash_table* table, err_allocator* err_alloc)
{
        Text buf = {};

        CtorBuffer(&buf, file, err_alloc);
        size_t elem_num = 0;

        char   word[WORD_LENGTH] = {};
        char*  word_ptr   = nullptr;
        size_t word_size  = 0; 

        size_t i_text = 0;

        List* current_list = NULL;

        while (i_text <= buf.size_buffer)
        {
                if (sscanf(buf.str + i_text, "%s", word) == 0)
                        break;

                word_size = strlen(word); 
                
                i_text += word_size + 1;

                word_ptr = (char*) calloc(WORD_LENGTH, sizeof(char));
                strncpy(word_ptr, word, word_size);

                size_t list_index = ((size_t) AsmGetHashCRC32((unsigned char*) word_ptr)) % table->size;//table->hash_function(word_ptr) % table->size; // ((size_t) AsmGetHashCRC32((unsigned char*) word_ptr)) % table->size; 
                memset(word, 0, WORD_LENGTH);

                current_list = table->bucket[list_index];
                
                Iterator it;
                for (size_t it_num = 0; it_num < SEARCH_ITERATIONS_NUM; it_num++)
                {
                        it = IntrinsicFindElem(current_list, word_ptr);
                }

                if (it.index == -1)
                {
                        ListPushBack(current_list, word_ptr, &it);
                        elem_num++;
                }
                else 
                {
                        free(word_ptr);
                }
        }

        table->elem_num = elem_num;
        
        DtorBuffer(&buf);
        return 0;
}


//------------------------------------------------------------------------
// Dump part


int DumpHashTable(FILE* To, hash_table* table)
{
        for (size_t i = 0; i < table->size; i++)
        {
                fprintf(To, "%lu,%lu\n", i, table->bucket[i]->num_elem);
        }

        return 0;
}



size_t GetDispersion(hash_table* table)
{
        size_t avarage = table->elem_num / table->size;
        size_t dispersion = 0;
        size_t temp = 0;

        for (size_t i = 0; i < table->size; i++)
        {
                temp = (table->bucket[i]->num_elem - avarage);
                temp *= temp;

                dispersion += temp;
        }

        dispersion /= table->size;
        
        return (size_t) sqrt(dispersion); 
}

size_t GetLoadFactor(hash_table* table)
{
        size_t empty_lists_num = 0;

        for (size_t i = 0; i < table->size; i++)
        {
                if (table->bucket[i]->num_elem != 0)
                {
                        empty_lists_num++;
                }
        }
        
        return table->elem_num / empty_lists_num;
}