#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
  Inputs:   char * filename
  struct matrix * transform,
  struct matrix * pm,
  screen s
  Returns:

  Goes through the file named filename and performs all of the actions listed in that file.
  The file follows the following format:
  Every command is a single character that takes up a line
  Any command that requires arguments must have those arguments in the second line.
  The commands are as follows:
  line: add a line to the edge matrix -
  takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
  circle: add a circle to the edge matrix -
  takes 3 arguments (cx, cy, r)
  hermite: add a hermite curve to the edge matrix -
  takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
  bezier: add a bezier curve to the edge matrix -
  takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
  ident: set the transform matrix to the identity matrix -
  scale: create a scale matrix,
  then multiply the transform matrix by the scale matrix -
  takes 3 arguments (sx, sy, sz)
  translate: create a translation matrix,
  then multiply the transform matrix by the translation matrix -
  takes 3 arguments (tx, ty, tz)
  xrotate: create an x-axis rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 1 argument (theta)
  yrotate: create an y-axis rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 1 argument (theta)
  zrotate: create an z-axis rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 1 argument (theta)
  apply: apply the current transformation matrix to the
  edge matrix
  display: draw the lines of the edge matrix to the screen
  display the screen
  save: draw the lines of the edge matrix to the screen
  save the screen to a file -
  takes 1 argument (file name)
  quit: end parsing

  See the file script for an example of the file format


  IMPORTANT MATH NOTE:
  the trig functions int math.h use radian mesure, but us normal
  humans use degrees, so the file will contain degrees for rotations,
  be sure to conver those degrees to radians (M_PI is the constant
  for PI)
  ====================*/
void parse_file ( char * filename,
                  struct matrix * transform,
                  struct matrix * pm,
                  screen s) {

  FILE *f;
  char line[256];
  color c;
  c.red = 45;
  c.green = 160;
  c.blue = 201;

  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 )
    f = stdin;
  else
    f = fopen(filename, "r");

  while ( fgets(line, 255, f) != NULL ) {
    if (strstr(line, "\n")) {
      line[strlen(line)-1]='\0';
      printf(":%s:\n",line);
    }
    if ( !strcmp( line, "quit")) {
      fclose(f);
      exit(0);
    }
    else if (!strcmp(line, "line")) {
      double x0,y0,z0,x1,y1,z1;
      fgets(line, 255, f);
      sscanf(line, "%lf %lf %lf %lf %lf %lf", &x0,&y0,&z0,&x1,&y1,&z1);
      add_edge( pm, x0, y0, z0, x1, y1, z1 );
    }
    else if (!strcmp(line, "circle") || !strcmp(line, "c" )) {
      double cx, cy, r;
      fgets(line, 255, f);
      sscanf(line, "%lf %lf %lf", &cx,&cy,&r);
      add_circle( pm, cx, cy, r,.01 );
    }
    else if (!strcmp(line, "hermite")) {
      double x0, y0, x1, y1, dx0, dy0, dx1, dy1;
      fgets(line, 255, f);
      sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", &x0,&y0,&dx0,&dy0,&x1,&y1,&dx1,&dy1);
      add_curve(pm, x0, y0, dx0-x0, dy0-y0, x1, y1, dx1-x1, dy1-y1, .01, HERMITE_MODE);
    }
    else if (!strcmp(line, "bezier") || !strcmp(line, "b")) {
      double x0, y0, x1, y1, x2, y2, x3, y3;
      fgets(line, 255, f);
      sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
      add_curve(pm, x0, y0, x1, y1, x2, y2, x3, y3, .01, BEZIER_MODE);
    }
    else if (!strcmp(line, "ident"))
      ident(transform);
    else if (!strcmp(line, "scale")) {
      fgets(line, 255, f);
      double sx, sy, sz;
      sscanf(line, "%lf %lf %lf", &sx, &sy, &sz);
      struct matrix *scale = make_scale(sx, sy, sz);
      matrix_mult(scale, transform);
    }
    else if (!strcmp(line, "translate")) {
      fgets(line, 255, f);
      double tx, ty, tz;
      sscanf(line, "%lf %lf %lf", &tx, &ty, &tz);
      struct matrix *translate = make_translate(tx, ty, tz);
      matrix_mult(translate, transform);
    }
    else if (!strcmp(line, "xrotate")) {
      fgets(line, 255, f);
      double theta;
      sscanf(line, "%lf", &theta);
      struct matrix *rotx = make_rotX(theta * M_PI / 180);
      matrix_mult(rotx, transform);
    }
    else if (!strcmp(line, "yrotate")) {
      fgets(line, 255, f);
      double theta;
      sscanf(line, "%lf", &theta);
      struct matrix *roty = make_rotY(theta * M_PI / 180);
      matrix_mult(roty, transform);
    }
    else if (!strcmp(line, "zrotate")) {
      fgets(line, 255, f);
      double theta;
      sscanf(line, "%lf", &theta);
      struct matrix *rotz = make_rotZ(theta * M_PI / 180);
      matrix_mult(rotz, transform);
    }
    else if (!strcmp(line, "apply")){
      matrix_mult(transform, pm);
    }
    else if (!strcmp(line, "display")) {
      draw_lines(pm,s,c);
      display(s);
    }
    else if (!strcmp(line, "save")) {
      draw_lines(pm,s,c);
      display(s);
      save_extension(s, "cool.png");
    }
    else if (!strcmp(line, "clear")) {
      clear_screen(s);
    }
  }
}
