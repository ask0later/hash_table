
#include "hash_funcs.h"


size_t ZeroHash(char* /*buffer*/)
{
        return 0;
}

size_t FirstHash(char* buffer)
{
        return (size_t) buffer[0];
}

size_t LenghtHash(char* buffer)
{
        return strlen(buffer);
}

size_t SumHash(char* buffer)
{
        size_t sum = 0;
        size_t   i = 0;
        while (buffer[i] != '\0')
        {
                sum += (size_t) buffer[i];
                i++;
        }
        return sum;
}

size_t AverageHash(char* buffer)
{
        size_t sum = 0;
        size_t   i = 0;
        while (buffer[i] != '\0')
        {
                sum += (size_t) buffer[i++];
        }

        return sum / i;
}

size_t ROLHash(char* buffer)
{
        size_t hash = 0;

        for (size_t i = 0; buffer[i] != '\0'; i++)
        {
                hash = ROL(hash) ^ (size_t) buffer[i];
        }
        
        return hash;
}

size_t RORHash(char* buffer)
{
        size_t hash = 0;

        for (size_t i = 0; buffer[i] != '\0'; i++)
        {
                hash = ROR(hash) ^ (size_t) buffer[i];
        }

        return hash;
}

size_t GNUHash(char* buffer)
{
        char* s = buffer;
        size_t h = 5381;

        for (unsigned char c = (unsigned char) *s; c != '\0'; c = (unsigned char) *++s)
                h = h + (h << 5) + c;

        return h;
}