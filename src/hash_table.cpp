#include "hash_table.h"
#include "list.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>


int TestHashFunction(hash_table* table, size_t (*hash_function) (char*), const char text_file[], err_allocator* err_alloc)
{
        if (CtorHashTable(table, hash_function, err_alloc) != 0)
        {
                ERR_ALLOC_TERMINATE(err_alloc);
                DtorHashTable(table);
                return 1;
        }
        

        if (CompleteHashTable(HAMLET_FILE, table, err_alloc) != 0)
        {
                INSERT_ERROR_NODE(err_alloc, "hash table completion error");
                ERR_ALLOC_TERMINATE(err_alloc);
                DeleteTableNodeData(table);
                DtorHashTable(table);
                return 1;
        }

        FILE* To = fopen(text_file, "w");
        if (To == NULL)
        {
                INSERT_ERROR_NODE(err_alloc, "file opening is failed");
                ERR_ALLOC_TERMINATE(err_alloc);
                DeleteTableNodeData(table);
                DtorHashTable(table); 
        }
        
        DumpHashTable(To, table);
        fclose(To);

        FILE* general = fopen(GENERAL_FILE, "a");
        if (To == NULL)
        {
                INSERT_ERROR_NODE(err_alloc, "file opening is failed");
                ERR_ALLOC_TERMINATE(err_alloc);
                DeleteTableNodeData(table);
                DtorHashTable(table); 
        }
        
        fprintf(general, "%lu, %lu\n", GetLoadFactor(table), GetDispersion(table));
        
        fclose(general);

        DeleteTableNodeData(table);

        DtorHashTable(table);

        return 0;
}

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


int CompleteHashTable(const char file[], hash_table* table, err_allocator* err_alloc)
{
        Text buf = {};

        if (CreateBuffer(&buf, file, err_alloc) == 1)
        {
                INSERT_ERROR_NODE(err_alloc, "read_file lib is failed");
                DeleteBuffer(&buf);
                return 1;
        }
        
        size_t elem_num = 0;

        char   word[MAX_SIZE_WORD] = {};
        char*  word_ptr   = NULL;
        size_t word_size  = 0; 

        size_t i_text = 0;

        List* current_list = NULL;

        while (i_text <= buf.size_buffer)
        {
                sscanf(buf.str + i_text, "%s", word);
                word_size = strlen(word); 

                i_text += word_size + 1;

                word_ptr = (char*) calloc(word_size + 1, sizeof(char));
                if (!word_ptr)
                {
                        INSERT_ERROR_NODE(err_alloc, "dynamic allocation is fault");
                        DeleteBuffer(&buf);
                        return 1;
                }
                
                strncpy(word_ptr, word, word_size + 1);
                
                size_t list_index = table->hash_function(word_ptr) % table->size;

                current_list = table->bucket[list_index];

                Iterator it = FindElem(current_list, word_ptr);

                if (it.index == -1)
                {
                        it.index = 0;
                        ListPushBack(current_list, word_ptr, &it);
                        elem_num++;
                }
                else 
                {
                        free(word_ptr);
                }
        }

        table->elem_num = elem_num; 
        
        DeleteBuffer(&buf);

        return 0;
}


int DeleteTableNodeData(hash_table* table)
{
        for (size_t i = 0; i < table->size; i++)
        {
                DtorNodeData(table->bucket[i]);
        }
        
        return 0;
}


int RemoveElem(Elem_t elem, hash_table* table, err_allocator* /*err_alloc*/)
{
        size_t index_list = table->hash_function(elem) % (table->size);
        
        List* list = table->bucket[index_list];

        Iterator cur_it = ListBegin(list);

        for (Iterator end_it = ListEnd(list); cur_it.index != end_it.index; cur_it = Next(&cur_it)) 
        {
                if (strcmp(ListGetElem(&cur_it), elem) == 0)
                {
                        ListErase(list, cur_it.index, &cur_it);
                        return 0;
                }
        }


        return 1;
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


//-------------------------------------------------------------------------


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