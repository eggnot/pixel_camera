// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;


int main(int argc, char *argv[]) {
  /*
   * Set up GPIO pins. This fails when not running as root.
   */
  GPIO io;
  if (!io.Init())
    return 1;
    
  /*
   * Set up the RGBMatrix. It implements a 'Canvas' interface.
   */
  int rows = 32;    // A 32x32 display. Use 16 when this is a 16x32 display.
  int chain = 1;    // Number of boards chained together.
  int parallel = 1; // Number of chains in parallel (1..3). > 1 for plus or Pi2
  int pwm = 8;
  int delay = 10; 

  int c;
  while ((c = getopt (argc, argv, "p:d:")) != -1)
    switch (c) {
      case 'p':
        pwm = atoi(optarg);
	break;
      case 'd':
	delay = atoi(optarg);
	break;
      default:
	return 0;
    }

 
  RGBMatrix *matrix = new RGBMatrix(&io, rows, chain, parallel);

  matrix->SetPWMBits(pwm);

  printf("pwm bits: %d", matrix->pwmbits());
  printf("pwm correction: %d", matrix->luminance_correct());

  Canvas *canvas = matrix;

  //canvas->Fill(0, 0, 255);
  
  FILE *f = freopen(NULL, "rb", stdin);

  unsigned char buf[3];

  int bytes_read;
  
  while(1) { 
    for(int y=0; y<32; y++) {
      for(int x=0; x<32; x++) {
        do {
          bytes_read = fread(buf, 3, 1, f);
        } while(bytes_read != 1);
          
        canvas->SetPixel(x, y, buf[0], buf[1], buf[2]);
      }
    }
    //vsync? suspend execution for microsecond intervals
    usleep(delay);
  }

  canvas->Clear();
  delete canvas;

  return 0;
}
