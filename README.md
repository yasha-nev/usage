# ***APPLE Mac os CPU/RAM USAGE using mach***
---

Старый проект был немного допилен, чтоб удовлетворять условиям практической работы №2.
(Использование мьютексов было изменено)

Программа собирает данные загружености процессора, пямяти и сохраняет их в файл формата CSV.

## Что используется:
* XNU Mach kernel
* Threads
* CMake

## Как собрать:
```
$ cmake -B build
$ cmake --build build -j
```
## Как запустить:

```
$ cd build/
$ ./main
```

## Функционал:

* quit
```
$ quit
```
