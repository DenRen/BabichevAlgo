Сдать решение задачи H6.8-post-Почтовые отделения
Полный балл:	100
Штраф за посылку:	1
Ограничение времени:	1 с
Ограничение реального времени:	4 с
Ограничение памяти:	64M
Ограничение размера стека:	4M
Вдоль прямой дороги расположены деревни. Дорога представляется целочисленной осью, а расположение каждой деревни задается одним целым числом — координатой на этой оси. Никакие две деревни не имеют одинаковых координат. Расстояние между двумя деревнями вычисляется как модуль разности их координат.

В некоторых, не обязательно во всех, деревнях будут построены почтовые отделения. Деревня и расположенное в ней почтовое отделение имеют одинаковые координаты. Почтовые отделения необходимо расположить в деревнях таким образом, чтобы общая сумма расстояний от каждой деревни до ближайшего к ней почтового отделения была минимальной.

Входные данные
В первой строке содержатся два целых числа: количество деревень n (1 ≤ n ≤ 300) и количество почтовых отделений m (1 ≤ m ≤ 30), m ≤ n. Вторая строка содержит n целых чисел в возрастающем порядке, являющихся координатами деревень. Для каждой координаты x верно 1 ≤ x ≤ 104.

Выходные данные
Первая строка выходного файла должна содержать одно целое число — общую сумму расстояний от каждой деревни до её ближайшего почтового отделения. Вторая строка должна содержать m целых чисел в возрастающем порядке. Эти числа являются искомыми координатами почтовых отделений. Если для заданного расположения деревень есть несколько решений, необходимо найти любое из них.

Пример
Входные данные
10 5
1 2 3 6 7 9 11 22 44 50
Выходные данные
9
2 9 22 44 50