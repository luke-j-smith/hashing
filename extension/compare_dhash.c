/* 
 * Contains code to enable comparison between different hash    
 * functions. Also functions to create a 'heat map'. 
 */
#include "compare_dhash.h"

/* Memory for an array of pointers to hash tables. */ 
HashTable **memory_for_ComparisionHashTables(void)
{
  HashTable **tables = NULL ; 
  
  tables = (HashTable **)calloc(1, sizeof(HashTable*)) ; 
  
  if(tables == NULL){
    fprintf(stderr, ERROR_HTS_MEM, TO_COMPARE) ; 
    exit(EXIT_FAILURE) ; 
  }
  
  return tables ; 
}

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
void initialise_hash_table(HashTable *table, \
unsigned int i)
{ 
  table->word_count = 0 ; 
  
  table->size = STARTING_SIZE ; 
  
  table->capacity_limit = CAPACITY ; 
  
  table->total_words_to_find = 0 ; 
  
  table->total_look_ups = 0 ;
  
  table->average_look_up = 0 ;  
  
  /* Initially we want to fill the hash table. */ 
  table->operation = FILLING ; 
  
  table->comparison = i ; 
  
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
  HashFunction primary_hash ; 
  FunctionType test_if_probe_needed ; 
  
  switch(table->comparison){
    case 0 : primary_hash = &djb2_hash_function ; break ; 
    case 1 : primary_hash = &djb2_hash_function ; break ; 
    case 2 : primary_hash = &sdbm_hash_function ; break ; 
    case 3 : primary_hash = &sdbm_hash_function ; break ; 
    case 4 : primary_hash = &lose_lose_hash_function ;
    break ; 
    case 5 : primary_hash = &lose_lose_hash_function ;
    break ; 
    default : fprintf(stderr, UNEXPECTED_HASH_ERROR, \
    table->comparison) ; exit(EXIT_FAILURE) ;  
  }
  
  *indx = (primary_hash(word) % table->size) ; 
  
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
  HashFunction secondary_hash ; 
  unsigned int probe ; 
  
  switch(table->comparison){
    case 0 : secondary_hash = &sdbm_hash_function ; break ; 
    case 1 : secondary_hash = &lose_lose_hash_function ;
    break ; 
    case 2 : secondary_hash = &djb2_hash_function ; break ; 
    case 3 : secondary_hash = &lose_lose_hash_function ;
    break ; 
    case 4 : secondary_hash = &djb2_hash_function ; break ; 
    case 5 : secondary_hash = &sdbm_hash_function ; break ; 
    default : fprintf(stderr, UNEXPECTED_HASH_ERROR, \
    table->comparison) ; exit(EXIT_FAILURE) ;  
  }
  
  probe = (secondary_hash(word) % table->size) ; 

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
  
  decide_which_block(table->map, index) ; 
  
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
  
  /* Free memory of old heat map, create larger one. */
  free(table->map->contents) ; 
  
  set_up_HeatMap(table->map, table->size) ; 

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
void free_all_allocated_memory(HashTable **table)
{
  unsigned int i ;  
    
  for(i = 0 ; i < TO_COMPARE ; i++){
    if(table[i] != NULL){
      free(table[i]) ;
    } 
  }

  free(table) ;
  
  return ; 
}

/*
 * The djb2 hash function. Taken from the following website:
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned int djb2_hash_function(char *str)
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
unsigned int sdbm_hash_function(char *str) 
{
  unsigned int hash = 0 ; 
  int i, n = strlen(str) ; 

  for(i = 0 ; i < n ; i++){
    hash = str[i] + (hash << SDBM_BITWISE1) + \
    (hash << SDBM_BITWISE2) - hash ;
  }

  /* Return value will need to be modded with table size.*/
  return hash ; 
}

/*
 * The lose lose hash function. Taken from the 
 * following website: http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned int lose_lose_hash_function(char *str) 
{
	unsigned int hash = 0 ;
	int i, n = strlen(str) ;
	
  for(i = 0 ; i < n ; i++){
    hash += str[i] ;
  }
  
  /* Return value will need to be modded with table size.*/
  return hash ;
}

HeatMap *memory_for_HeatMap(void)
{
  HeatMap *distribution = NULL ; 
  
  distribution = (HeatMap *)malloc(sizeof(HeatMap)) ; 
  
  if(distribution == NULL){
    fprintf(stderr, ERROR_HM_MEM) ; 
    exit(EXIT_FAILURE) ; 
  }
  
  return distribution ; 
}

void set_up_HeatMap(HeatMap *map, unsigned int size)
{
  map->contents_size = size ; 
  
  map->block_length = decide_block_length(size) ; 
  
  map->num_blocks = DISPLAY_LENGTH/map->block_length ; 
    
  map->contents = memory_for_each_section(size) ; 

  return ; 
}

Block **memory_for_each_section(unsigned int size) 
{
  unsigned int i, length, sections ; 
  Block **section = NULL ; 
  
  length = decide_block_length(size) ; 
  
  sections = DISPLAY_LENGTH/length ; 
  
  section = (Block **)\
  malloc(sizeof(Block*)*sections) ; 
  
  if(section == NULL){
    fprintf(stderr, ERROR_SECTIONS_MEM, sections) ; 
    exit(EXIT_FAILURE) ; 
  }
  
  for(i = 0 ; i < sections ; i++){
    section[i] = memory_for_each_Block() ; 
  }
  
  return section ; 
}

Block *memory_for_each_Block(void) 
{
  Block *box = NULL ; 
  
  box = (Block *)malloc(sizeof(Block)) ; 
  
  if(box == NULL){
    fprintf(stderr, ERROR_BLOCK_MEM) ; 
    exit(EXIT_FAILURE) ; 
  }
  
  box->density = 0 ; 
  
  box->x = 0 ; 

  box->y = 0 ; 
  
  return box ; 
}

unsigned int decide_block_length(unsigned int size)
{
  unsigned int length ; 
  
  if(size < DISPLAY_LENGTH){
    length = DISPLAY_LENGTH/size ; 
  }
  /* If hash table is larger than display, max size = 1. */ 
  else{
    length = 1 ;  
  }

  return length ; 
}

/* Determines the most appropriate position in map. */ 
void decide_which_block(HeatMap *map, unsigned int index) 
{
  float step_size ; 
  unsigned int position ;
    
  if(map->contents_size <= DISPLAY_LENGTH){
    step_size = \
    (double) map->contents_size/map->num_blocks ; 
    position = index/step_size ; 
  }
  else{
    step_size = (double) DISPLAY_LENGTH/map->contents_size ;
    position = index*step_size ; 
  }
  
  (map->contents[position]->density)++ ; 
  
  return ; 
}

/* Sets the (x,y) position on SDL of each block. */ 
void set_table_position(HeatMap *map, unsigned int px, unsigned int py)
{
  unsigned int i ; 
  
  for(i = 0 ; i < map->num_blocks ; i++){
    map->contents[i]->y = py ; 
    map->contents[i]->x = px + (i*(map->block_length)) ; 
  }

  return ; 
}
