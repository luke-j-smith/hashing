# Extension
For the extension, I decided to compare the effectiveness of six different combinations of hash functions whilst using a double hashing approach. The three hash functions I have used are: 
1. djb2; 
2. sdbm; and 
3. lose lose. 
```All are taken from: http://www.cse.yorku.ca/~oz/hash.html```

The combinations investigated are:

| Primary Hash | Secondary Hash |
| -------------|:--------------:|
| djb2         | sdbm           |
| djb2         | lose lose      |
| sdbm         | djb2           |
| sdbm         | lose lose      |
| lose lose    | djb2           |
| lose lose    | sdbm           |

After results for the six different combinations have been calculated they are printed to the terminal. The user is then able to select whether they would like to see the final 'heat map' for each of the six hash tables, if they do, this is displayed using SDL. 

In the heat map, the darker the colour, the higher the density of words in that area. In order to 'smooth out' the graphics on SDL, I have used a simple moving average. 

## TO-DO

1. Not all of the ```callocs``` have been freed. Due to time constraints, I did not have time to write a function to properly take care of the ```HeatMap``` structure within ```HashTable``` before submission. This results in the heat map not being properly displayed via SDL each time - sometimes it results in a segmentation fault and therefore, you may have to try running the program multiple times to view the heat map.
2. The lack of time also contributed to some unpleasant code which I did not have time improve. I apologise for this, however, hope that the idea of the extension comes across!

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
The compiler used in the *makefile* is GCC (https://gcc.gnu.org/) and you will need to have this installed in order to use it. In order to display the 'turtle', SDL2 is used and will also need to be installed (https://wiki.libsdl.org/Installation).

### neillsdl2
There are two files ```neillsdl2.c``` and ```neillsdl2.h``` that I have slightly modified but were not written by me. These were written by Neill Campbell (http://www.bristol.ac.uk/engineering/people/neill-w-campbell/index.html) to facilitate the use of SDL2 in the assignment (as the direct use of this library was not being assessed).
