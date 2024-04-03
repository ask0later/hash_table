#include "hash_map.h"
#include "list.h"
#include <cmath>
#include <cstdio>


int CreateHashTable(hash_table* table, size_t (*hash_function) (char*), const char text_file[], err_allocator* err_alloc)
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
        DumpHashTable(To, table);
        fclose(To);

        FILE* general = fopen(GENERAL_FILE, "a");
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
                
                if (FindElem(word_ptr, table, err_alloc) == -1)
                {
                        if (AddElem(word_ptr, table, err_alloc) != 0)
                        {
                                INSERT_ERROR_NODE(err_alloc, "function AddElem is failed");
                                DeleteBuffer(&buf);
                                return 1;
                        }
                        elem_num++;
                }
                else 
                {
                        free(word_ptr);
                }
        }

        table->elem_num = elem_num - 1; 

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

int FindElem(Elem_t elem, hash_table* table, err_allocator* /*err_alloc*/)
{
        size_t index_list = table->hash_function(elem) % table->size;
        
        List* list = table->bucket[index_list];

        ssize_t counter = list->nodes[0].next;
        
        while(counter != 0)
        {
                if (strcmp(list->nodes[counter].data, elem) == 0)
                        return (int) counter;
                
                counter = list->nodes[counter].next;
        }

        return -1;
}      

int AddElem(Elem_t elem, hash_table* table, err_allocator* err_alloc)
{
        size_t index_list = table->hash_function(elem) % table->size;
        
        List* list = table->bucket[index_list];

        Push_Back(list, elem);

        if (list->errors != 0)
        {
                INSERT_ERROR_NODE(err_alloc, "list lib contains an error");
                return 1;
        }

        return 0;
}


int RemoveElem(Elem_t elem, hash_table* table, err_allocator* err_alloc)
{
        size_t index_list = table->hash_function(elem) % (table->size);
        
        List* list = table->bucket[index_list];

        size_t size_elem = strlen(elem);

        ssize_t counter = 0;
        do
        {
                if (strncmp(list->nodes[counter].data, elem, size_elem) == 0)
                        break;
                counter = list->nodes[counter].next;
        } while (counter != 0);

        if (counter == 0)
        {       
                // elem is not find
                return 1; 
        }
        else 
        {
                Iterator it = {};
                it.list  = list;
                it.index = counter;
                ListErase(&it);

                if (it.list->errors != 0)
                {
                        INSERT_ERROR_NODE(err_alloc, "list lib contains an error");
                        return 1;
                }
        }

        return 0;
}



//------------------------------------------------------------------------
// Dump part


int DumpHashTable(FILE* To, hash_table* table)
{
        for (size_t i = 0; i < table->size; i++)
        {
                fprintf(To, "%lu,%lu\n", i, table->bucket[i]->num_elem);

                // fprintf(To, "%3lu have %4lu elements\n", i, table->bucket[i]->num_elem);
                // TextDumpList(table->bucket[i]);
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
        // size_t elem_sum = 0;
        size_t empty_lists_num = 0;

        for (size_t i = 0; i < table->size; i++)
        {
                if (table->bucket[i]->num_elem != 0)
                {
                        // elem_sum += table->bucket[i]->size;
                        empty_lists_num++;
                }
        }
        // printf("%lu and %d and %lu\n", table->size, (int)table->elem_num, empty_lists_num);
        return table->elem_num / empty_lists_num;
}