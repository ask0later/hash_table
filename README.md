##  Хеш-таблица
# 1 часть. Написание хеш-таблицы

Эта такая структура данных, которая позволяет выполнять операции поиска, вставки и удаления элемента с временной сложностью алгоритма O(1). Если, конечно, хорошо подобрана хеш-функция.

Сама хеш-таблица представляет из себя массив списков (реализацию я делал [в этом проекте](https://github.com/ask0later/List.git) ранее) и размер хеш-таблицы, определяющийся числом этих списков.

Принцип каждой операции следующий: элемент рассматривается как массив байт, хеш-функция проходится по этому массиву и определенным образом считает некоторое число (например, сумма ASCII-кодов символов). Каждому элементу соответсвует свой хеш. Затем номер списка, с которым проводится операция, равняется остатку от деления хеша элемента на размер хеш-таблицы. Вставка элемента при любой хеш-функции имеет алгоритмическую сложность О(1). Если эта функция "плохая" и большое количество элементов находится в одному списке, то сложность будет достигать О(n).

Чтобы лучше понять это и выбрать наилучшую хеш-функцию рассмотрим некоторые из них:


