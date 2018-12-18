/* 
 * Spell checker created using double hashing. Returns the
 * average number of look-ups for. 
 */
#include <stdio.h>
#include <stdlib.h>
#include "shash.h"

/* Details of the potential errors that could occur. */ 
#define ARGC_ERROR "\nERROR: Unexpected input.\nPlease " \
"try something like: %s dictionary.txt\n\n"
#define ERROR_OPENING_FILE "\nERROR: Cannot open file: " \
"%s.\nPlease try again.\n\n"
#define WORD_SIZE_ERROR "\nERROR: Word from file in " \
"position: %d, is longer than the maximum length " \
"of: %d.\n\n"
#define ERROR_CLOSING_FILE "\nERROR: Cannot close file: "\
"%s.\nPlease try again.\n\n"

/* Longest word a major dictionary is 45 letters long. */
#define MAX_WORD_LENGTH 46

#define REMAINING 1

/* Characters that would indicate the end of a/next word. */
#define WORD(c) ((c != ' ') && (c != '\n') \
&& (c != '\r') && (c != '\0') && (c != EOF))

void fill_table(HashTable *table, FILE *bank, char *argv1) ; 
void open_file(FILE **word_bank, char *argv1) ;
void words_in_file_into_table(HashTable *table, FILE *bank,\
char *argv) ;
int take_word_from_file(FILE *word_bank, char *temp_word, \
char *argv) ; 
void check_string_length(FILE *word_bank, char *argv) ; 
void close_file(FILE **word_bank, char *argv1) ; 
void search_table(HashTable *table, FILE *look, \
char *argv2) ;
void determine_average_look_ups(HashTable *table) ; 

int main(int argc, char **argv)
{
  FILE *word_bank = NULL, *look_up = NULL ; 
  HashTable *table = NULL ; 
    
  if(argc == 3){
    table = memory_for_HashTable() ;  

    /* argv[1] contains our "dictionary" of words. */ 
    fill_table(table, word_bank, argv[1]) ;  
    
    /* argv[2] contains the words we are looking for. */ 
    search_table(table, look_up, argv[2]) ;
    
    determine_average_look_ups(table) ; 
    
    free_all_allocated_memory(table) ; 
  }
  
  else{
    fprintf(stderr, ARGC_ERROR, argv[0]) ;
    return 1 ; 
  }
  
  return 0 ; 
}

/* Fill the table with words from dictionary. */ 
void fill_table(HashTable *table, FILE *bank, char *argv1)  
{
  open_file(&bank, argv1) ;
  
  initialise_hash_table(table) ; 
                
  words_in_file_into_table(table, bank, argv1) ;

  close_file(&bank, argv1) ;
    
  return ; 
}

void open_file(FILE **word_bank, char *argv1)
{ 
  if((*word_bank = fopen(argv1, "r")) == NULL){
    fprintf(stderr, ERROR_OPENING_FILE, argv1) ; 
    exit(EXIT_FAILURE) ; 
  }
  return ; 
}

/* Fills table with words in from the first file. */ 
void words_in_file_into_table(HashTable *table, FILE *bank,\
char *argv)
{
  char temp_word[MAX_WORD_LENGTH] ; 

  /* While not EOF, take one word at a time and insert. */ 
  while(take_word_from_file(bank, temp_word, \
  argv) == REMAINING){
    insert_word_into_table(table, temp_word) ; 
  }

  return ; 
}

/* If words within length limit, take from file. */ 
int take_word_from_file(FILE *word_bank, char *temp_word, \
char *argv)
{ 
  check_string_length(word_bank, argv) ; 
      
  if(fscanf(word_bank, "%s", temp_word) == EOF){
    return EOF ; 
  }
  
  /* Avoids length check starting with a non-letter char.*/
  fseek(word_bank, 1, SEEK_CUR) ; 
        
  return REMAINING ;  
}

/* Ensure that word from file not above max length. */ 
void check_string_length(FILE *word_bank, char *argv)
{
  static unsigned int word_in_file = 0 ; 
  int counter ;
  char c = '\0' ; 
  
  word_in_file++ ; 
  
  counter = 0 ; 
  do{
    c = fgetc(word_bank) ; 
    counter++ ; 
  } while(WORD(c)) ; 
  
  /* If word to large, exit and report position in file. */
  if(counter >= MAX_WORD_LENGTH){
    fprintf(stderr, WORD_SIZE_ERROR, word_in_file, \
    MAX_WORD_LENGTH) ;
    close_file(&word_bank, argv) ; 
    exit(EXIT_FAILURE) ;   
  }
  
  /* Rewind file pointer to start of the word. */ 
  fseek(word_bank, -counter, SEEK_CUR) ; 
  
  return ;  
}

void close_file(FILE **word_bank, char *argv1)
{
  if(fclose(*word_bank) != 0){
    fprintf(stderr, ERROR_CLOSING_FILE, argv1) ;
    exit(EXIT_FAILURE) ; 
  } 
  return ; 
}

/* Searches the table with words in from the second file. */ 
void search_table(HashTable *table, FILE *look, char *argv2)  
{
  char temp_word[MAX_WORD_LENGTH] ; 
    
  open_file(&look, argv2) ;
                            
  while(take_word_from_file(look, temp_word, \
  argv2) == REMAINING){
  
    (table->total_words_to_find)++ ; 
    
    look_for_word_in_table(table, temp_word) ; 
  }

  close_file(&look, argv2) ;
  
  return ; 
}

/* Determine and print average number of look-ups. */ 
void determine_average_look_ups(HashTable *table) 
{
  table->average_look_up = (double) \
  (table->total_look_ups)/(table->total_words_to_find) ; 
 
  printf("\nThe average look-up is: %f\n\n", \
  table->average_look_up) ; 

  return ; 
}
