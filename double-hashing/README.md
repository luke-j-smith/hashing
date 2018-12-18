# Double Hashing
The program uses a double hashing approach for the spell checker. It reads in the dictionary file from ```argv[1]``` and the words to check file from ```argv[2]```. The program:
1. Does not make any assumptions about the maximum size of the dictionary file;
2. Chooses an initial, prime, size for the dictionary array;
3. Creates a new dictionary array that is roughly twice the size (but still prime) if the existing array if it has become more than 60% full;
4. Rehashes all of the words from the existing array into the new one;
5. Reads in the list of words to check once the dictionary is built; and
6. Reports on the average number of lookups required - this number is the only output (we note that a perfect hash will require exactly 1.0 look-up).

## Usage
The parser can be compiled using the *makefile*:
```
make
```

The parser can then be run using the *makefile*, with the input files specified:
```
make run DICTIONARY="words_to_check_against.txt" WORDS_TO_CHECK="words_to_check.txt"
```

Two example lists have been provided in the files ```34words.txt``` and ```eowl_shuffle.txt```. Note that using ```34words.txt``` as the dictionary results in a much faster runtime than ```eowl_shuffle.txt```.

### Requirements
The compiler used in the *makefile* is GCC (https://gcc.gnu.org/) and you will need to have this installed in order to use it.
