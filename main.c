#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main( int argc, char **argv ) {

  screen s;
  color c;
  c.red = 255;
  clear_screen( s );
  struct matrix *edges;
  struct matrix *transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);
  add_edge( edges, 100, 100, 100, 200, 200, 200 );
  //add_circle( edges, 250, 250, 100, .01 );
  //add_curve( edges, 200, 250, 150, 50, 300, 250, 300, 250, .01, 1 );
  add_curve( edges, 150, 150, 150, 50, 350, 150, 350, 300, .01, 0 );
  draw_lines( edges, s, c );

  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );

  display( s );
  //save_extension(s, "lines.png");


  free_matrix( transform );
  free_matrix( edges );
}
