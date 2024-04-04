#  Хеш-таблица
## 1 часть. Написание хеш-таблицы

Эта такая структура данных, которая позволяет выполнять операции поиска, вставки и удаления элемента с временной сложностью алгоритма O(1). Если, конечно, хорошо подобрана хеш-функция.

Сама хеш-таблица представляет из себя массив bucket'ов, то есть списков (реализацию я делал [в этом проекте](https://github.com/ask0later/List.git) ранее) и размер хеш-таблицы, определяющийся числом этих списков. Количество элементов в bucket'е называется colisium.

Принцип каждой операции следующий: элемент рассматривается как массив байт, хеш-функция проходится по этому массиву и определенным образом считает некоторое число (например, сумма ASCII-кодов символов). Каждому элементу соответсвует свой хеш. Затем номер списка, с которым проводится операция, равняется остатку от деления хеша элемента на размер хеш-таблицы. Вставка элемента при любой хеш-функции имеет алгоритмическую сложность О(1). Если эта функция "плохая", и большое количество элементов находится в одному списке, то сложность будет достигать О(n).



Чтобы лучше понять это рассмотрим некоторые из них и сравним их дисперсию(отклонение от среднего значения) и load-фактор(среднее количество элементов в списке среди непустых):

## ZeroHashFunction
Данная функция всегда возвращает число ноль. Следовательно все элементы будут находится в одном bucket'е.

![zero_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/Zero_HashFunction.png)




![first_symbol_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/ASCII_code_first_symbol_HashFunction.png)

![lenght_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/Lenght_HashFunction.png)

![sum_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/Symbol_Sum_HashFunction.png)

![average_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/Average_HashFunction.png)

![rol_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/ROL_HashFunction.png)

![ror_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/ROR_HashFunction.png)

![gnu_hash](https://github.com/ask0later/hash_table/blob/1f4f591cee359f590a712a751f5bc399fa4e8812/image/GNU_HashFunction.png)