/*
 * Hashing assignment - a double hashing method is used to 
 * create a dictionary that can then be searched. 
 * The two hash functions are at the very bottom.
 */ 
#include "dhash.h"

/* Memory for a pointer to the hash table. */ 
HashTable *memory_for_HashTable(void)
{
  HashTable *table = NULL ; 
  
  table = (HashTable *)calloc(1, sizeof(HashTable)) ; 
  
  if(table == NULL){
    fprintf(stderr, ERROR_HT_MEM) ; 
    exit(EXIT_FAILURE) ; 
  }
  
  return table ; 
}

/* Initialises values associated the the hash table. */ 
void initialise_hash_table(HashTable *table)
{ 
  table->word_count = 0 ; 
  
  table->size = STARTING_SIZE ; 
  
  table->capacity_limit = CAPACITY ; 
  
  table->total_words_to_find = 0 ; 
  
  table->total_look_ups = 0 ;
  
  table->average_look_up = 0 ;  
  
  /* Initially we want to fill the hash table. */ 
  table->operation = FILLING ; 
  
  table->elements = table_contents_space(table->size) ;
  
  return ; 
}

/* Memory for an array of pointers of type Node. */ 
Node **table_contents_space(unsigned int size)
{
  Node **elements = NULL ; 
  
  elements = (Node **)calloc(size, sizeof(Node*)) ; 

  if(elements == NULL){
    fprintf(stderr, ERROR_NODES_MEM, size) ; 
    exit(EXIT_FAILURE) ; 
  }
  
  return elements ; 
}

/* Finds words index, inserts and checks capacity. */
void insert_word_into_table(HashTable *table, char *word)
{   
  find_index_and_insert(table, word) ; 
  
  if(at_capacity(table)){ 
    create_new_table(table) ;   
  }
  
  return ; 
}

/* Once unique index found, create memory for word. */ 
void find_index_and_insert(HashTable *table, char *word) 
{
  unsigned int index, *indx ;
  
  index = 0 ; 
  indx = &index ;  
  
  (table->word_count)++ ;

  find_index(table, word, indx) ; 

  table->elements[index] = memory_for_Node(word) ; 

  return ; 
} 

/* Determines primary index and whether probing required. */
void find_index(HashTable *table, char *word, \
unsigned int *indx)
{  
  FunctionType test_if_probe_needed ; 

  *indx = (primary_hash_function(word) % table->size) ; 
  
  /* Choose relevant test for either filling or searching.*/ 
  switch(table->operation){
    case FILLING : test_if_probe_needed = &fill ; break ; 
    case SEARCHING : test_if_probe_needed = &search ; break; 
    default : fprintf(stderr, UNEXPECTED_CHOICE_ERROR, \
    table->operation) ; exit(EXIT_FAILURE) ;  
  }
  
  test_if_probe_needed(table, word, indx) ; 
  
  return ;  
}

/* If filling table, test if relevant index is empty. */
void fill(HashTable *table, char *word, \
unsigned int *indx)
{
  /* If it is empty, then first index is fine. */ 
  if(table->elements[*indx] != NULL){  
    second_hash_and_probe(table, word, indx) ; 
  }

  return  ; 
}

/* If searching table, test if relevant index is word. */
void search(HashTable *table, char *word, \
unsigned int *indx)
{
  /* If it is empty, then word doesn't exist in table. */ 
  if(table->elements[*indx] == NULL){
    return ; 
  }
  
  /* If not empty, but not word, search further. */ 
  if(NOT_SAME(table->elements[*indx]->word, word)){
    second_hash_and_probe(table, word, indx) ; 
  }
  
  return ; 
}

void second_hash_and_probe(HashTable *table, char *word, \
unsigned int *index) 
{
  int indx ; 
  unsigned int probe ; 
  
  indx = (int) *index ; 
  
  /* If, somehow, probe size is zero, then exit. */ 
  if((probe = determine_probe(table, word)) == 0){
    fprintf(stderr, PROBE_ERROR) ; 
    exit(EXIT_FAILURE) ; 
  }

  switch(table->operation){
    case FILLING : *index = probe_to_insert(table, indx, \
    probe) ; break ; 
    case SEARCHING : probing_search(table, indx, probe, \
    word) ; break ; 
    default : fprintf(stderr, UNEXPECTED_CHOICE_ERROR, \
    table->operation) ; exit(EXIT_FAILURE) ;  
  }

  return ; 
}

/* Used second hash function to determine probe size. */
unsigned int determine_probe(HashTable *table, char *word)
{
  unsigned int probe ; 
  
  probe = (second_hash_function(word) % table->size) ; 

  /* Ensure that probing value is not zero! */ 
  if(probe == 0){
    probe = 1 ; 
  }

  return probe ; 
}

/* When filling, determine which cell is empty. */ 
unsigned int probe_to_insert(HashTable *table, int indx, \
unsigned int probe)
{
  unsigned int i ; 
  
  /* Keep probing for an empty cell. */ 
  i = 1 ; 
  do{
    indx = probing_wrapper(indx, probe, i, table->size) ; 
  } while(table->elements[indx] != NULL) ; 
  
  return (unsigned int) indx ; 
}

/* Keep searching until word is found or empty cell. */ 
void probing_search(HashTable *table, int indx, \
unsigned int probe, char *word) 
{
  unsigned int i ; 
    
  i = 1 ; 
  do{
    indx = probing_wrapper(indx, probe, i, table->size) ; 
    
    (table->total_look_ups)++ ; 
     
    /* If cell is empty, then word not in table. */            
    if(table->elements[indx] == NULL){
      return ; 
    }
  
  /* Keep looking until same word. */   
  } while(NOT_SAME(table->elements[indx]->word, word)) ; 

  return ; 
}

/* Index value cannot be negative - wrap back around.*/ 
int probing_wrapper(int indx, unsigned int probe, \
unsigned int i, unsigned int size)
{
  indx = (indx - (i*probe)) ; 
    
  if(indx < 0){
    indx += i*size ; 
  }

  return indx ; 
}

/* Find unique index for word, create memory and insert. */
void insert_word(HashTable *table, char *word) 
{
  unsigned int index, *indx ;
  
  index = 0 ; 
  indx = &index ;  

  find_index(table, word, indx) ; 

  table->elements[index] = memory_for_Node(word) ; 
  
  (table->word_count)++ ; 

  return ; 
}

/* Allocate memory for type Node containing the word. */ 
Node *memory_for_Node(char *word_to_input)
{
  Node *element = NULL ; 
  
  element = (Node *)calloc(1, sizeof(Node*)) ; 

  if(element == NULL){
    fprintf(stderr, ERROR_1NODE_MEM) ; 
    exit(EXIT_FAILURE) ; 
  }
  
  element->word = memory_for_word(word_to_input) ; 
  
  return element ; 
}

/* Allocate exactly enough memory for the word itself. */ 
char *memory_for_word(char *word_to_input) 
{
  unsigned int length ; 
  char *word ; 
  
  length = strlen(word_to_input) ; 
  
  word = (char *)calloc(length, sizeof(char)) ;
  
  if(word == NULL){
    fprintf(stderr, ERROR_WORD_MEM, length) ; 
    exit(EXIT_FAILURE) ; 
  }
  
  strcpy(word, word_to_input) ; 
  
  return word ; 
}

/* Determines whether hash table needs to be rebuilt. */
unsigned int at_capacity(HashTable *table)
{
  float percentage ;
  
  percentage = \
  (double) (table->word_count)/(table->size) ; 
  
  /* 
   * When the number of words in the hash table has reached 
   * a certain percentage of total size, it is full. 
   */ 
  if(percentage > table->capacity_limit){
    return FULL ; 
  }
  
  return STILL_ROOM ; 
}

/* If table is deemed to be full, create a larger one.*/
void create_new_table(HashTable *table)
{
  unsigned int i, previous_size ; 
  Node **old = NULL ; 
  
  /* Temp pnter to the old table containing all the words.*/ 
  old = table->elements ; 
  
  previous_size = table->size ; 
  
  table->word_count = 0 ; 
  
  /* Find new table size and create enough space for it. */    
  table->size = next_largest_prime(table->size) ; 
  
  table->elements = table_contents_space(table->size) ; 

  /* Check if entry in each element of old hash table. */  
  for(i = 0 ; i < previous_size ; i++){
    /* If element not empty, hash word into new table. */
    if(old[i] != NULL){
      insert_word(table, old[i]->word) ;  
      /* Free memory created for each word in old table. */
      free(old[i]->word) ; 
    } 
    /* Free memory created for each element of old table. */
    free(old[i]) ; 
  }
  /* Free memory created for the old hash table itself. */
  free(old) ; 

  return ; 
}

/* Finds the next appropriate prime number for table size.*/ 
unsigned int next_largest_prime(unsigned int current_size)
{
  unsigned int prime = current_size ; 

  /* Check each subsequent prime, until large enough. */ 
  while(prime < DOUBLE(current_size)){
    prime = next_prime(prime) ; 
  }
  
  return prime ; 
}

/* Finds the subsequent prime for any inputted number. */ 
unsigned int next_prime(unsigned int current_number)
{
  unsigned int i, divisible, check ; 
  
  check = NOT_PRIME ; 
  
  /* Check each subsequent number until a prime is found. */ 
  while(check == NOT_PRIME){
    current_number++ ; 
    /* All numbers divisible by 1, so set divisible = 1. */ 
    divisible = 1 ;
    /* Then start search from i = 2. */ 
    for(i = PRIME_CHECK_START ; i <= current_number ; i++){
      /* If there is no remainder, then divisible by i. */ 
      if(NO_REMAINDER(current_number, i)){
        divisible++ ; 
      }
    }
    
    /* Prime iff divisible by 1 and itself (divisible =2).*/
    if(PRIME(divisible)){
      check = IS_PRIME ; 
    }
  }
  
  return current_number ; 
}

/* Look for word from second file. */
void look_for_word_in_table(HashTable *table, char *word) 
{
  unsigned int index, *indx ;
  
  index = 0 ; 
  indx = &index ;  
  
  (table->total_look_ups)++ ;  
  
  /* Now looking for words, change operation to searching.*/
  table->operation = SEARCHING ; 

  /* Find index, very similar to when filling. */ 
  find_index(table, word, indx) ; 
    
  return ; 
} 

/* Ensure that all memory is freed when finished. */ 
void free_all_allocated_memory(HashTable *table)
{
  unsigned int i ; 
  
  for(i = 0 ; i < table->size ; i++){
    if(table->elements[i] != NULL){
      free(table->elements[i]->word) ; 
    }
    free(table->elements[i]) ; 
  }
  
  free(table->elements) ; 
  
  free(table) ;
  
  return ; 
}

/*
 * The djb2 hash function. Taken from the following website:
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned int primary_hash_function(char *str)
{
  unsigned int hash = DJB2_START ;
  int i, n = strlen(str) ;

  for(i = 0 ; i < n ; i++){
    hash = ((hash << DJB2_BITWISE) + hash) + str[i] ; 
  }

  /* Return value will need to be modded with table size.*/
  return hash ;
}

/*
 * The sdbm hash function. Taken from the following website:
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned int second_hash_function(char *str)
{
  unsigned int hash = 0 ; 
  int i, n = strlen(str) ; 

  for(i = 0 ; i < n ; i++){
    hash = str[i] + (hash << SDBM_BITWISE1) + \
    (hash << SDBM_BITWISE2) - hash;
  }

  /* Ensure that probe size is not zero. */ 
  if(hash == 0){
    return 1 ; 
  }

  /* Return value will need to be modded with table size.*/
  return hash ; 
}
