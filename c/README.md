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

## olderVersions-noSplit/ *directories working with: ../../training_texts & ../../testing_texts*
`olderVersions-noSplit/` - contains original file versions that are not split into header/src files

`olderVersions-noSplit/langRec_v1.c` - original implementation based on Rahimi model (combined training/testing)

`olderVersions-noSplit/langRec_v2.c` - split training/testing, where */cachedTraining* will pull memory (if it doesn't exist
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

## v2/
`v2/` - an optimized version of v1 that uses .bin versions of training, reports predictions for files thrown in *unknown/* (no current cache)

`v2/training_bin/` - the .bin versions of training files (training directory)

`v2/unknown/` - directory where unknown files can be dumped for detection

`v2/Makefile/` - compiles all the src files into **langRec** (NOTE: if on windows, adjust to langRec.exe)

`v2/computations.c/` - src file for the computations done on the language HVs (both for training & testing)

`v2/computations.h` - header file for *compuations.c*

`v2/main.c` - main file that calls for training & testing; contains language pool & itemMemory; times the program

`v2/test.c` - src file that reports the top 3 predictions for files in *unknown/*

`v2/test.h` - header file for *test.c*

`v2/train.c` - src file that checks for training memory & builds it if not found with 22 languages in *main.c* (not cache currently generated)

`v2/train.h` - header file for *train.c*

## v3/ *directories working with: ../../training & ../../testing*
`v3/` - expanded version of the program, working with 164 characters & 27 languages

`v3/MAIN.c` - main file that calls for training & testing; contains language pool & itemMemory

`v3/Makefile` - compiles all the src files into **langRec** (NOTE: if on windows, adjust to langRec.exe)

`v3/computations.c/` - src file for the computations done on the language HVs (both for training & testing)

`v3/computations.h` - header file for *compuations.c*

`v3/test.c` - src file that generates an accuracy level from a pool of about 27,000 unknown texts with stored training information

`v3/test.h` - header file for *test.c*

`v3/train.c` - src file that checks for training memory & builds it if not found with 27 languages in *MAIN.c* (not cache currently generated)

`v3/train.h` - header file for *train.c*

## v4/ *directories working with: ../../training & ../../testing*
`v4/` - same expansions as *v3/*, but has cmd line arguments for values of D & and unknown language to detection

`v4/MAIN.c` - main file that calls for training & testing; converts first argv to be D input, second to be the text (must be included in ""s; i.e. "sentence here")

`v4/Makefile` - compiles all the src files into **langRec** (NOTE: if on windows, adjust to langRec.exe)

`v4/computations.c/` - src file for the computations done on the language HVs (both for training & testing)

`v4/computations.h` - header file for *compuations.c*

`v4/test.c` - src file that reports the top prediction for inputed text

`v4/test.h` - header file for *test.c*

`v4/train.c` - src file that checks for training memory & builds it if not found with 27 languages in *MAIN.c* (current cache N=3 for D=1000,3000,5000,10000)

`v4/train.h` - header file for *train.c*