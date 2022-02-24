Сдать решение задачи H2.7*-external-sort-Внешняя сортировка.
===
* Полный балл:	200
* Штраф за посылку:	1
* Бонусные баллы:	20 15 10 5
* Имя входного файла:	input.txt
* Имя выходного файла:	output.txt
* Ограничение времени:	2 с
* Ограничение реального времени:	5 с
* Ограничение памяти:	4M
* Ограничение размера стека:	1M

## Внешняя сортировка
В файле "input.txt" содержатся строки символов, длина каждой строки не превышает 10000 байт. Файл нужно отсортировать в лексикографическом порядке и вывести результат в файл "output.txt". Вот беда, файл занимает много мегабайт, а в вашем распоряжение оказывается вычислительная система с памятью всего в 256 килобайт. Но файл должен быть отсортирован!

# Формат входных данных
Будьте внимательны, в этой задаче придётся использовать файловый ввод/вывод. Входной файл "input.txt" содержит неотсортированные строки символов без управляющих и пробельных (ASCII коды от 33 до 126).

## Формат результата
Отсортированная версия входного файла.

## Примеры
### Входные данные в файле input.txt
```bash
aksjdfhaskjdh
alsdajsldjaslkdjaslkdj
asldkjaslkdjlkjq
qweqweqweqweqweqwe
pqiwepoiqwpoeiqwpeoiqwpeo
qpeoiqwpoeiqwpe
```

### Результат работы в файле output.txt
```bash
aksjdfhaskjdh
alsdajsldjaslkdjaslkdj
asldkjaslkdjlkjq
pqiwepoiqwpoeiqwpeoiqwpeo
qpeoiqwpoeiqwpe
qweqweqweqweqweqwe
```