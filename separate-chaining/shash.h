/*
 * Hashing assignment - an open chaining double method is 
 * used to create a dictionary that can then be searched. 
 * Hash table is rebuilt when open chain becomes > 4.  
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

/* Hash table starts as relatively small prime number. */
#define STARTING_SIZE 103
/* 
 * Used to define the capacity of an open chain, ie what is
 * the maximum number of words allowed in the open chain.
 */
#define CAPACITY 4

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

/* Structure used to store the words entered into table. */ 
struct node{ 
  char *word ; 
  /* Includes a *next pointer to allow for open chaining. */
  struct node *next ; 
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
  /* Keeps a total of the max number words in one chain. */ 
  unsigned int density ; 
} ; 
typedef struct hashtable HashTable ; 


HashTable *memory_for_HashTable(void) ; 
void initialise_hash_table(HashTable *table) ; 
Node **table_contents_space(unsigned int size) ; 
void insert_word_into_table(HashTable *table, char *word) ; 
void insert_word(HashTable *table, char *word) ; 
Node *memory_for_Node(char *word_to_input) ; 
char *memory_for_word(char *word_to_input) ; 
void insert_into_open_chain(Node *element, char *word, \
HashTable *table) ; 
unsigned int at_capacity(HashTable *table) ; 
void create_new_table(HashTable *table) ; 
unsigned int next_largest_prime(unsigned int current_size) ;
unsigned int next_prime(unsigned int current_number) ; 
void free_open_chain(Node *element) ;
void look_for_word_in_table(HashTable *table, char *word) ;
void free_all_allocated_memory(HashTable *table) ; 
unsigned int primary_hash_function(char *str) ; 
