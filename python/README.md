# Python Implementation of Language Recognition
This directory contains the relevant information for the Python Language Recognition application. The development
was completed on a Raspberry PI 4 and on a Windows 10 machine, tested with the latest version (Python 3.8)

# Structure
This directory contains different versions of the program. The follow is an overview of the different directories.

`cachedTraining/` - folder which stores the training files (itemMemory and language files)

`langRecBase.py` - base program with no optimizations. Training and testing are together

`langRecTesting.py` - base program split for testing saved files

`langRecTestingIM.py` - optimized testing program split for testing saved files

`langRecTraining.py` - split training file
