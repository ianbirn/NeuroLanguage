# NeuroLanguage - Brain Inspired Language Recognition
This project is based on the [Language Recognition Application](https://github.com/abbas-rahimi/HDC-Language-Recognition) by [Abbas Rahimi](https://github.com/abbas-rahimi), 
which uses Hyperdimesnional Computing principles for language detection. The program was initially developed in Matlab, and has been programmed 
in Python and C. These two languages proved to be significantly faster than the Matlab implementation, where Python performed ~2x faster and C ~8x faster. 
*This is from recordings on a Windows 10 Intel i7 laptop with 16 GB Ram (Matlab cannot run on the Raspberry PI 4)*. After initially creating the program 
in these respective languages, optimizations were made to better the ability of the application. The biggest change was that for both Python and C, 
where training and testing was split and a cache directory was enabled for the program. Training accounted for a significant portion of the
total time, especially for large dimensions, such as 10,000 (accounted for around 80% of total time). When a cache already exists, the training
memory is pulled and testing can be done immediately. Additionally, the C program made expansive changes to include more languages and a wider
itemMemory (more information in the C folder). As well, there is a PHP directory for launching a web server to run the program.

## Prerequisites
* GCC 8.3.0
* Python 3.8
* PHP 7.2.3

## Structure
The Python and C directories have README.md for more information on the structure and specifications of files. The following is a basic breakdown
of this repository.

`GUI/` - contains the relevant files for executing a user interface version of the program (contains training ability)

`c/` - contains the relevant directories/files for different variations of the program written in C, as well as executional instructions

`php/` - contains C files, cache, and PHP file for running instantaneous detection from any web client (server was run on Raspberry PI 4 for our purposes)

`python/` - contains the relevant directories/files for different variations of the program written in Python, as well as executional instructions

`testing/` - the testing directory for the expanded version of C (the additional characters and languages)

`testing_texts`/ - the testing directory pulled from Abbas Rahimi's original Matlab implementation (27 characters & 22 languages)

`training/` - the training directory for the expanded version of C (the additional characters and languages)

`training_texts/` - the training directory pulled from Abbas Rahimi's original Matlab implementation (27 characters & 22 languages)

## Running the Program
#### For C: 
Most of the directories include a Makefile, which if you have no experience with, compiles the header files and main file for you. Therefore,
all you need to do is type `make` in the cmd line (default is Linux format). `gcc -O5` is recommended for fastest performance.

**For compiling (non-split):**
> `gcc -O5 langRec_v1.c -o langRec_v1 -lm`

> `gcc -O5 langRec_v2.c -o langRec_v2 -lm`


**For running (non-split) on Linux:**
> `./langRec_v1`

> `./langRec_v2`


**For running (non-split) on Windows:**
> `langRec_v1.exe`

> `langRec_v2.exe`


**For compiling all other versions (using Makefile):**
> `make`

> `make clean`

*NOTE: if you make any changes to N or D (or anywhere else), you will need to do `make` and `make clean` again to refresh langRec*


**For running all versions 1-3 (Linux & Windows respectively):**
> `./langRec`

> `langRec.exe`


**For running v4 (Linux & Windows respectively):**

*KEY: argv[1] = D = dimensionality size (ex: 10000); argv[2] = phrase = unknown you want to detect (NOTE: MUST BE IN QUOTATIONS OR THE PROGRAM WILL ONLY READ THE FIRST WORD)*

> `./langRec D "this is the sentence"`

> `langRec.exe D "this is the sentence"`


#### For Python:
Python running instructions.

## Authors
* Ian Birn - [ianbirn](https://github.com/ianbirn)
* Ben Lucas - [blucas6](https://github.com/blucas6)
* Caroline MacLaren - [cmaclaren](https://github.com/cmaclaren)
* Andrew Osburn - [AndrewOsburn](https://github.com/AndrewOsburn)

## Acknowledgments
* [Abbas Rahimi's](https://github.com/abbas-rahimi) [Language Recognition Application](https://github.com/abbas-rahimi/HDC-Language-Recognition)