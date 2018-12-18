# Separate Chaining
This program is an adaptation of the *Double Hashing* program where collisions are handled by creating linked lists out from a base array. This program:
1. Uses a linked-list approach;
2. Does not make any assumptions about the maximum size of the dictionary file;
3. Uses the same hash and double hash functions as in the *Double Hashing* program;
4. Reads in the list of words to check once the dictionary is built; and
5. Reports on the average number of lookups required - this number is the only output (we note that a perfect hash will require exactly 1.0 look-up).

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
