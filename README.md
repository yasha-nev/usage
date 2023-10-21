# ***APPLE Mac os CPU/RAM USAGE using mach***
---

The program collects data on processor load, used and free RAM memory and saves the data to a csv file once a minute.

## using
* XNU Mach kernel
* Threads
* CMake

## build
```
$ cmake -B build
$ cmake --build build -j
```
## start programm

```
$ cd build/
$ ./main
```

## function

* quit
```
$ quit
```
