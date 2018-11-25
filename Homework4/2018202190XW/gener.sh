#!/bin/bash
gcc -o dmk dmk.c
./dmk > tmp.c
gcc -Og -S tmp.c
gedit tmp.s
gedit tmp.c
