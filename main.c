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

  // edges = generate_curve_coefs( 10, 2, 3, 4, BEZIER_MODE );
  // print_matrix(edges);

  //add_edge( edges, 100, 100, 100, 200, 200, 200 );
  add_circle( edges, 250, 250, 100, .01 );
  add_curve( edges, 100, 100, 150, 150, 175, 100, 200, 200, 0.1, BEZIER_MODE);
  draw_lines( edges, s, c );
  display( s );
  clear_screen( s );

  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );

  free_matrix( transform );
  free_matrix( edges );
}
