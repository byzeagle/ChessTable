# How to build this project

This project comes with a makefile. Just run `make` and it will produce a `chess.exe` executable to be run in Linux platforms. You can run this executable with `-h` or `-p` command-line options. 

Example:

    ./chess.exe -h
    Please enter relative or absolute file path to input file
    
or

    ./chess.exe -p ./board3.txt
     Siyah : 56, Beyaz : 112.5

## Valgrind output 

    ==3684== Memcheck, a memory error detector
    ==3684== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==3684== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
    ==3684== Command: ./chess.exe -p board3.txt
    ==3684== 
    Siyah : 56, Beyaz : 112.5
    ==3684== 
    ==3684== HEAP SUMMARY:
    ==3684==     in use at exit: 0 bytes in 0 blocks
    ==3684==   total heap usage: 107 allocs, 107 frees, 86,886 bytes allocated
    ==3684== 
    ==3684== All heap blocks were freed -- no leaks are possible
    ==3684== 
    ==3684== For lists of detected and suppressed errors, rerun with: -s
    ==3684== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
