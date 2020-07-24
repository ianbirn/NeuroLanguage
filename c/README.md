# C Implementation of Language Recognition
This directory contains the relevant information for the C Language Recognition application. The development
was completed on a Raspberry PI 4 and on a Windows 10 machine, tested with the latest compiler (GCC 8.3.0)

# Results
The C code performed exceptionally fast. In the initial runnings of the C program on the Raspberry PI, training
accounted for over 55% of the total time taken. N=3 is recommended for different variations of the program, for it
maintains the highest accuracy levels across different dimesnionalities (D). D=3000 is suitable enough for a rating
over 94.5% on all versions of the program, though D=10000 will yield the highest accuracy reporting.

# Structure
This directory contains different versions of the program. The follow is an overview of the different directories.

## olderVersions-noSplit/
`olderVersions-noSplit/` - contains original file versions that are not split into header/src files

`olderVersions-noSplit/langRec_v1.c` - original implementation based on Rahimi model (combined training/testing)

`olderVersions-noSplit/langRec_v2.c` - split training/testing, where if `./cachedTraining` will pull memory (if it doesn't exist
or has files missing it'll generate; currently no cache is loaded)

## v1/ *directories working with: ../../training_texts & ../../testing_texts*
`v1/` - an expansion of *olderVersions-noSplit/langRec_v2.c* that is split into header/src files

`v1/cachedTraining/` - a directory of memory for values D=1000,3000,5000,10000 and N=3,4,5

`v1/Makefile` - compiles all the src files into **langRec** (NOTE: if on windows, adjust to langRec.exe)

`v1/computations.c` - src file for the computations done on the language HVs (both for training & testing)

`v1/computations.h` - header file for *compuations.c*

`v1/main.c` - main file that calls for training & testing; contains language pool & itemMemory; times the program

`v1/test.c` - src file that generates an accuracy level from a pool of 21,000 unknown texts with stored training information

`v1/test.h` - header file for *test.c*

`v1/train.c` - src file that checks for training memory & builds it if not found with 22 languages in *main.c*

`v1/train.h` - header file for *train.c*


