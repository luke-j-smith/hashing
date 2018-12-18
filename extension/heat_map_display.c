/*
 * Displays a head map for each hash table using SDL.
 * The darker the area, the more densely populated.
 */
#include "heat_map_display.h"
#include "compare_dhash.h"
#include "neillsdl2_ls.h"

#define RECTWIDTH 200
#define RECTLENGTH 700
#define MILLISECONDDELAY 10

void determine_density_colour(HeatMap *map) ;
void moving_average(HeatMap *map) ;
unsigned int wrap(int num, HeatMap *map) ;
void set_block_colour(Block *content) ;

void display_heatmap(HeatMap *map)
{
  unsigned int i ;
  SDL_Simplewin sw ;
  SDL_Rect rectangle ;
  rectangle.w = RECTWIDTH + 2 ;
  rectangle.h = RECTLENGTH + 2 ;

  Neill_SDL_Init(&sw) ;

  do{
    /* Sleep for a short time */
    SDL_Delay(MILLISECONDDELAY);

    rectangle.x = 100 ;
    rectangle.y = 60 ;
    SDL_RenderFillRect(sw.renderer, &rectangle) ;

    Neill_SDL_SetDrawColour(&sw, 0, 0, 0) ;

    rectangle.x = 100 ;
    rectangle.y = 60 ;
    SDL_RenderDrawRect(sw.renderer, &rectangle) ;

    determine_density_colour(map) ;

    for(i = 0 ; i < map->num_blocks ; i++){

      rectangle.h = map->block_length ;
      rectangle.w = RECTWIDTH ;

      Neill_SDL_SetDrawColour(&sw, map->contents[i]->red, \
       map->contents[i]->green, map->contents[i]->blue);

      /* Unfilled Rectangle, fixed size, random position */
      rectangle.x = 101 ;
      rectangle.y = 61 + (i*map->block_length) ;
      SDL_RenderFillRect(sw.renderer, &rectangle);

    }

    SDL_RenderPresent(sw.renderer);
    SDL_UpdateWindowSurface(sw.win);

    Neill_SDL_Events(&sw);

   }while(!sw.finished);


   /* Clear up graphics subsystems */
   atexit(SDL_Quit);

  return ;
}


/* Moving average over five blocks to 'smooth' map out. */
void moving_average(HeatMap *map)
{
  unsigned int i ;

  /* Average over five, plus one to always round up. */
  for(i = 0 ; i < map->num_blocks ; i++){
    map->contents[i]->moving_average = \
    (map->contents[wrap(i - 2, map)]->density + \
    map->contents[wrap(i - 1, map)]->density + \
    map->contents[wrap(i, map)]->density + \
    map->contents[wrap(i + 1, map)]->density + \
    map->contents[wrap(i + 2, map)]->density)/5 + 1;
  }
  return ;
}

/* As modulus used, wrap around ends for moving average. */
unsigned int wrap(int num, HeatMap *map){
  if(num == -2){
    return (map->num_blocks - 2) ;
  }

  if(num == -1){
    return (map->num_blocks - 1) ;
  }

  if(num == (int) map->num_blocks){
    return 0 ;
  }

  if(num == (int) (map->num_blocks + 1)){
    return 1 ;
  }

  return num ;
}

void determine_density_colour(HeatMap *map)
{
  unsigned int i ;

  moving_average(map) ;

  for(i = 0 ; i < map->num_blocks ; i++){
    map->contents[i]->density_percentage = \
    (double)map->contents[i]->moving_average/ \
    map->contents_size ;

    set_block_colour(map->contents[i]) ;
  }

  return ;
}

/* Defines the colour intensity of each block. */
void set_block_colour(Block *content)
{

  if(content->density <= 0.0001){
    content->red = COLOUR_RED0 ;
    content->green = COLOUR_GREEN0 ;
    content->blue = COLOUR_BLUE0 ;
  }

  if((0.0001 < content->density_percentage) && \
  (content->density_percentage <= 0.0002)){
    content->red = COLOUR_RED1 ;
    content->green = COLOUR_GREEN1 ;
    content->blue = COLOUR_BLUE1 ;
  }

  if((0.0002 < content->density_percentage) && \
  (content->density_percentage <= 0.0003)){
    content->red = COLOUR_RED2 ;
    content->green = COLOUR_GREEN2 ;
    content->blue = COLOUR_BLUE2 ;
  }

  if((0.0003 < content->density_percentage) && \
  (content->density_percentage <= 0.0004)){
    content->red = COLOUR_RED3 ;
    content->green = COLOUR_GREEN3 ;
    content->blue = COLOUR_BLUE3 ;
  }

  if((0.0004 < content->density_percentage) && \
  (content->density_percentage <= 0.0005)){
    content->red = COLOUR_RED4 ;
    content->green = COLOUR_GREEN4 ;
    content->blue = COLOUR_BLUE4 ;
  }

  if((0.0004 < content->density_percentage) && \
  (content->density_percentage <= 0.0005)){
    content->red = COLOUR_RED5 ;
    content->green = COLOUR_GREEN5 ;
    content->blue = COLOUR_BLUE5 ;
  }

  if((0.0005 < content->density_percentage) && \
  (content->density_percentage <= 0.0006)){
    content->red = COLOUR_RED6 ;
    content->green = COLOUR_GREEN6 ;
    content->blue = COLOUR_BLUE6 ;
  }

  if((0.0006 < content->density_percentage) && \
  (content->density_percentage <= 0.0007)){
    content->red = COLOUR_RED7 ;
    content->green = COLOUR_GREEN7 ;
    content->blue = COLOUR_BLUE7 ;
  }

  if((0.0007 < content->density_percentage) && \
  (content->density_percentage <= 0.0008)){
    content->red = COLOUR_RED8 ;
    content->green = COLOUR_GREEN8 ;
    content->blue = COLOUR_BLUE8 ;
  }

  if(content->density_percentage > 0.0008){
    content->red = COLOUR_RED9 ;
    content->green = COLOUR_GREEN9 ;
    content->blue = COLOUR_BLUE9 ;
  }

  return ;
}


