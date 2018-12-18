/*
 * Hashing assignment - an open chaining double method is 
 * used to create a dictionary that can then be searched. 
 * The single hash function is at the very bottom. 
 */
#include "shash.h"

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
  
  /* Open chain density - how many words with same index. */
  table->density = 0 ;  
  
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
  insert_word(table, word) ; 
  
  if(at_capacity(table)){ 
    create_new_table(table) ;   
  }
  
  return ; 
}

/* Finds index and inserts word into the table. */ 
void insert_word(HashTable *table, char *word) 
{
  unsigned int index ;
  
  (table->word_count)++ ; 
  
  index = (primary_hash_function(word) % table->size) ;
   
  /* If element is empty, create space for the word. */
  if(table->elements[index] == NULL){
    table->elements[index] = memory_for_Node(word) ;
  } 
  /* If not, insert word into a linked list from element.*/
  else{
    insert_into_open_chain(table->elements[index], word, \
    table) ; 
  }

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
  
  /* Ensure next pointer is NULL by default. */ 
  element->next = NULL ; 
  
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

void insert_into_open_chain(Node *element, char *word, \
HashTable *table)
{ 
  /* Reset chain density to be 1, as element is occupied. */
  table->density = 1 ;  

  /*
   * Move along the open chain until the end is reached. 
   * Increase density for each step (ie, each word). 
   */
  while(element->next != NULL){
    element = element->next ; 
    (table->density)++ ; 
  }
  
  /* Add the next word to end of chain. */ 
  element->next = memory_for_Node(word) ; 
    
  return ;
}

/* Determines whether hash table needs to be rebuilt. */
unsigned int at_capacity(HashTable *table)
{  
  /* 
   * When a number of words all have the same index, ie when 
   * the chain density becomes too high level, must rebuild.
   */   
  if(table->density > table->capacity_limit){
    return FULL ; 
  }
  
  return STILL_ROOM ; 
}

/* If table is deemed to be full, create a larger one.*/
void create_new_table(HashTable *table)
{
  unsigned int i, previous_size ; 
  Node **old = NULL ;
  Node *element = NULL ;  
  
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
      element = old[i] ; 
      
      /* If element not empty, go through the whole chain.*/
      while(element != NULL){
        insert_word(table, element->word) ; 
        element = element->next ; 
      }
    } 
    /* Free the memory of the whole open chain. */
    free_open_chain(old[i]) ; 
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

/* Walk through a linked list and free all of the memory. */
void free_open_chain(Node *element)
{
  Node *clear ; 
  
  while((clear = element) != NULL){
    element = element->next ;
    free(clear->word) ; 
    free(clear) ;  
  }
  
  return ; 
}

/* Look for word from second file. */
void look_for_word_in_table(HashTable *table, char *word) 
{
  unsigned int index = 0 ; 
  Node *element ; 
   
  /* Find the index and element where word would be. */      
  index = (primary_hash_function(word) % table->size) ;
        
  (table->total_look_ups)++ ; 

  /* If this is empty, then word is not in hash table. */
  if(table->elements[index] == NULL){
    return ; 
  }
  
  else{
    element = table->elements[index] ;
    /*
     * If not empty, then search through open chain until, 
     * either the end is reached (and word doesn't exist in
     * table), or the word is found in the linked list. 
     */
    while((element != NULL) && (NOT_SAME(element->word, \
    word))){
      (table->total_look_ups)++ ; 
      element = element->next ; 
    }
  }

  return ; 
} 

/* Ensure that all memory is freed when finished. */ 
void free_all_allocated_memory(HashTable *table)
{
  unsigned int i ;
  
  for(i = 0 ; i < table->size ; i++){
    free_open_chain(table->elements[i]) ; 
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
