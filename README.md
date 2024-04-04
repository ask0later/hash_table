#  Хеш-таблица
## 1 часть. Написание хеш-таблицы

Эта такая структура данных, которая позволяет выполнять операции поиска, вставки и удаления элемента с временной сложностью алгоритма O(1). Если, конечно, хорошо подобрана хеш-функция.

Сама хеш-таблица представляет из себя массив bucket'ов, то есть списков (реализацию я делал [в этом проекте](https://github.com/ask0later/List.git) ранее) и размер хеш-таблицы, определяющийся числом этих списков. Количество элементов в bucket'е называется colisium.

Принцип каждой операции следующий: элемент рассматривается как массив байт, хеш-функция проходится по этому массиву и определенным образом считает некоторое число (например, сумма ASCII-кодов символов). Каждому элементу соответсвует свой хеш. Затем номер списка, с которым проводится операция, равняется остатку от деления хеша элемента на размер хеш-таблицы. Вставка элемента при любой хеш-функции имеет алгоритмическую сложность О(1). Если эта функция "плохая", и большое количество элементов находится в одному списке, то сложность будет достигать О(n).



Чтобы лучше понять это рассмотрим некоторые из них и сравним их дисперсию(отклонение от среднего значения) и load-фактор(среднее количество элементов в списке среди непустых):

### ZeroHashFunction
Данная функция всегда возвращает число ноль. Следовательно все элементы будут находится в одном bucket'е.

Реализация функции:
~~~
size_t ZeroHash(char* /*buffer*/)
{
        return 0;
}
~~~

![zero_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/Zero_HashFunction.png)

load-factor = 4679. dispersion = 458.


### FirstSymbolHashFunction
Данная функция возвращает ASCII-код первого символа.

Реализация функции:
~~~
size_t FirstHash(char* buffer)
{
        return (size_t) buffer[0];
}
~~~

![first_symbol_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/ASCII_code_first_symbol_HashFunction.png)

load-factor = 167. dispersion = 100.


### LenghtHashFunction
Данная функция возвращает длину элемента.

Реализация функции:
~~~
size_t LenghtHash(char* buffer)
{
        return strlen(buffer);
}
~~~

![lenght_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/Lenght_HashFunction.png)

load-factor = 334. dispersion = 164.


### SumHashFunction
Данная функция возвращает сумму ASCI-кодов символов.

Реализация функции:
~~~
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
~~~

![sum_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/Symbol_Sum_HashFunction.png)

load-factor = 45. dispersion = 7.

На первый взляд эта функция имеет равномерное распределение, но давайте посмотрим как она будет себя вести, если увеличить размер хеш-таблици.

### SumHashFunction c большим размером хеш-таблици

![sum_with_big_size_hash](https://github.com/ask0later/hash_table/blob/fbdea56c887895f8a80b9a854d3db108dcefccc7/image/Sum_HashFunction_Big_size.png)

Как видим, большинство bucket'ов не заполнено, поэтому функция не является подходящей.



### AverageHashFunction
Данная функция возвращает среднее число ASCI-кодов символов.

Реализация функции:
~~~
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
~~~

![average_hash](https://github.com/ask0later/hash_table/blob/b482ea65fe5d105a4e427337b9f87878191b21fd/image/Average_HashFunction.png)

load-factor = 141. dispersion = 131.

### ROLHashFunction
Данная функция использует циклический сдвиг влево и XOR (исключающее или).

hash[i] = (hash[i] << 1) \XOR buffer[i]
~~~
size_t ROLHash(char* buffer)
{
        size_t hash = 0;

        for (size_t i = 0; buffer[i] != '\0'; i++)
        {
                hash = ROL(hash) ^ (size_t) buffer[i];
        }
        
        return hash;
}
~~~

![rol_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/ROL_HashFunction.png)

load-factor = 45. dispersion = 6.

### RORHashFunction
Данная функция использует циклический сдвиг вправо и XOR (исключающее или).

hash[i] = (hash[i] >> 1) \XOR buffer[i]
~~~
size_t RORHash(char* buffer)
{
        size_t hash = 0;

        for (size_t i = 0; buffer[i] != '\0'; i++)
        {
                hash = ROR(hash) ^ (size_t) buffer[i];
        }

        return hash;
}
~~~
![ror_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/ROR_HashFunction.png)

load-factor = 45. dispersion = 15.

Данная функция в сравнении с функцией, использующей циклический сдвиг влево, имеет такой же load-фактор, но большую dispersion.

### GNUHashFunction

~~~
size_t GNUHash(char* buffer)
{
        char* s = buffer;
        size_t h = 5381;

        for (unsigned char c = *s; c != '\0'; c = *++s)
                h = h + (h << 5) + c;

        return h;
}
~~~

![gnu_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/GNU_HashFunction.png)

load-factor = 45. dispersion = 6.

### Сравнение хеш-функций
 
| Хеш-функции | ZeroHash | FirstHash | LenghtHash | SumHash | AverageHash | ROLHash | RORHash | GNUHash|
| ------      | :------: | :-------: | :--------: | :-----: | :---------: | :-----: | :------:|:------:| 
| load-factor |   4679   |    167    |    334     |    45   |     141     |    45   |    45   |   45   |
| dispersion  |    458   |    100    |    164     |     7   |     131     |     6   |    15   |    6   |

Самым лучшим выбором хеш-функции будет ROLHash или GNUHash.

