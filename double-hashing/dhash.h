/*
 * Hashing assignment - a double hashing method is used to 
 * create a dictionary that can then be searched. 
 * Hash table is scrapped and rebuilt when 60% full. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A list of potential errors that could occur. */ 
#define ERROR_HT_MEM "\nERROR: Not enough memory to create"\
" a structure of type HashTable.\n\n"
#define ERROR_NODES_MEM "\nERROR: Not enough memory to " \
"create an array of %d Nodes.\n\n"
#define ERROR_1NODE_MEM "\nERROR: Not enough memory to " \
"create a single Node for a new word.\n\n"
#define ERROR_WORD_MEM "\nERROR: Not enough memory to " \
"a word of length: %d.\n\n"
#define UNEXPECTED_CHOICE_ERROR "\nERROR: Unexpected " \
"input (ie, '%d') for choice as to whether filling or " \
"searching - input value must be either '0' or '1'.\n\n" \
"the hash table. Input \n\n" 
#define PROBE_ERROR "\nERROR: Probe size of zero when " \
"conducting probe using double hashing.\n\n" 

/* Hash table starts as relatively small prime number. */
#define STARTING_SIZE 103

/* Maximum capacity of the hash table defined as 60%. */
#define CAPACITY 0.60

/* Used to switch between filling and searching table. */ 
#define FILLING 0
#define SEARCHING 1

/* Returning TRUE when two words are different. */
#define NOT_SAME(a, b) (strcmp(a, b) != 0)

/* Whether the hash table has reached maximum capacity. */
#define FULL 1 
#define STILL_ROOM 0

/* New table must be at least double size of current. */
#define DOUBLE(current_size) (2*current_size)

/* Every number divisible by 1, so start search at 2. */ 
#define PRIME_CHECK_START 2 

#define NO_REMAINDER(a, b) ((a % b) == 0)

/* Prime if and only if divisible by 1 and itself. */ 
#define PRIME(divisible) (divisible == 2)

#define NOT_PRIME 1 
#define IS_PRIME 0

/*
 * Values used in the primary and secondary hash functions. 
 * Taken from: http://www.cse.yorku.ca/~oz/hash.html
 */
#define DJB2_START 5381
#define DJB2_BITWISE 5
#define SDBM_BITWISE1 6
#define SDBM_BITWISE2 16

/* Structure used to store the words entered into table. */ 
struct node{
  char *word ; 
} ; 
typedef struct node Node ; 

/* Contains all relevant information for hash table. */ 
struct hashtable{
  /* Essentially a dynamic array of pointers to strings. */
  Node **elements ; 
  /* All other figures for filling and searching. */ 
  unsigned int word_count ; 
  unsigned int size ; 
  double capacity_limit ;
  unsigned int total_words_to_find ; 
  unsigned int total_look_ups ;
  float average_look_up ;
  unsigned int operation ; 
} ; 
typedef struct hashtable HashTable ; 

/* Used to swap test functions when filling or searching.*/
typedef void (*FunctionType) (HashTable *, char *, \
unsigned int *) ; 


HashTable *memory_for_HashTable(void) ; 
void initialise_hash_table(HashTable *table) ; 
Node **table_contents_space(unsigned int size) ; 
void insert_word_into_table(HashTable *table, char *word) ;
void find_index_and_insert(HashTable *table, char *word) ; 
void find_index(HashTable *table, char *word, \
unsigned int *indx) ; 
void fill(HashTable *table, char *word, \
unsigned int *indx) ; 
void search(HashTable *table, char *word, \
unsigned int *indx) ; 
void second_hash_and_probe(HashTable *table, char *word, \
unsigned int *index) ; 
unsigned int determine_probe(HashTable *table, char *word) ; 
unsigned int probe_to_insert(HashTable *table, int indx, \
unsigned int probe) ; 
void probing_search(HashTable *table, int indx, \
unsigned int probe, char *word) ; 
int probing_wrapper(int indx, unsigned int probe, \
unsigned int i, unsigned int size) ; 
void insert_word(HashTable *table, char *word) ; 
Node *memory_for_Node(char *word_to_input) ; 
char *memory_for_word(char *word_to_input) ; 
unsigned int at_capacity(HashTable *table) ; 
void create_new_table(HashTable *table) ; 
unsigned int next_largest_prime(unsigned int current_size) ;
unsigned int next_prime(unsigned int current_number) ; 
void look_for_word_in_table(HashTable *table, char *word) ; 
void free_all_allocated_memory(HashTable *table) ; 
unsigned int primary_hash_function(char *str) ; 
unsigned int second_hash_function(char *str) ;
