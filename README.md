# NeuroLanguage
This project is based on the [Language Recognition Application](https://github.com/abbas-rahimi/HDC-Language-Recognition) by [Abbas Rahimi](https://github.com/abbas-rahimi), 
which uses Hyperdimesnional Computing principles for language detection. The program was initially developed in Matlab, and has been programmed 
in Python and C. These two languages proved to be significantly faster than the Matlab implementation, where Python performed ~2x faster and C ~8x faster. 
*This is from recordings on a Windows 10 Intel i7 laptop with 16 GB Ram (Matlab cannot run on the Raspberry PI 4)*. After initially creating the program 
in these respective languages, optimizations were made to better the ability of the application. The biggest change was that for both Python and C, 
where training and testing was split and a cache directory was enabled for the program. Training accounted for a significant portion of the
total time, especially for large dimensions, such as 10,000 (accounted for around 80% of total time). When a cache already exists, the training
memory is pulled and testing can be done immediately. Additionally, the C program made expansive changes to include more languages and a wider
itemMemory (more information in the C folder).

## Prerequisites
* GCC 8.3.0
* Python 3.8
* PHP 7.2.3

## Running the Program
**For C:** Most of the directories include a Makefile, which if you have no experience with, compiles the header files and main file for you. Therefore,
all you need to do is type `make` in the cmd line (default to windows format). -O5 is recommended for fastest performance 
(for NonSplit versions that require cmd line compilation)

**For Python:**

## Authors
* Ian Birn - [ianbirn](https://github.com/ianbirn)
* Ben Lucas - [blucas6](https://github.com/blucas6)
* Caroline MacLaren - [cmaclaren](https://github.com/cmaclaren)
* Andrew Osburn - [AndrewOsburn](https://github.com/AndrewOsburn)

## Acknowledgments
* [Abbas Rahimi's](https://github.com/abbas-rahimi) [Language Recognition Application](https://github.com/abbas-rahimi/HDC-Language-Recognition)