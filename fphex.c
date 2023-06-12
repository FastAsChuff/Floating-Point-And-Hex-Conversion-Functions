#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/********************************************************************************************
// Floating Points As Hex Strings
// ==============================
// Author: Simon Goater May 2023.
//
// The binary representation of floats must be same for source and destination floats.
// If the endianess of source and destination differ, the hex characters must be 
// permuted accordingly.
*/
typedef union {
  float f;
  double d;
  long double ld;
  unsigned char c[16];
} fpchar_t;

const unsigned char hexchar[16] = {0x30, 0x31, 0x32, 0x33, 
    0x34, 0x35, 0x36, 0x37, 
    0x38, 0x39, 0x41, 0x42, 
    0x43, 0x44, 0x45, 0x46};
const unsigned char binchar[23] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 
  0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
    
void fptostring(void* f, unsigned char* string, uint8_t sizeoffp) {
  fpchar_t floatstring;  
  memcpy(&floatstring.c, f, sizeoffp);
  int i, stringix;
  stringix = 0;
  unsigned char thischar;
  for (i=0; i<sizeoffp; i++) {
    thischar = floatstring.c[i];
    string[stringix] = hexchar[thischar >> 4];
    stringix++;
    string[stringix] = hexchar[thischar & 0xf];
    stringix++;
  }
}

void stringtofp(void* f, unsigned char* string, uint8_t sizeoffp) {
  fpchar_t floatstring;
  int i, stringix;
  stringix = 0;
  for (i=0; i<sizeoffp; i++) {
    floatstring.c[i] = binchar[(string[stringix] - 0x30) % 23] << 4;
    stringix++;
    floatstring.c[i] += binchar[(string[stringix] - 0x30) % 23];
    stringix++;
  }
  memcpy(f, &floatstring.c, sizeoffp);
}

_Bool isfpstring(void* f, unsigned char* string, uint8_t sizeoffp) {
  // Validates the floatstring and if ok, copies value to f.
  int i;
  for (i=0; i<2*sizeoffp; i++) {
    if (string[i] < 0x30) return false;
    if (string[i] > 0x46) return false;
    if ((string[i] > 0x39) && (string[i] < 0x41)) return false;
  }
  stringtofp(f, string, sizeoffp);
  return true;
}

/********************************************************************************************
// Floating Points As Hex Strings - END
// ====================================
*/

int main(int argc, char **argv)
{
  //float f = 1.23f;
  //double f = 1.23;
  long double f = 1.23;
  if (argc > 1) f = atof(argv[1]);
  unsigned char floatstring[33] = {0};
  //printf("fpval = %.32f\n", f);
  printf("fpval = %.32Lf\n", f);
  fptostring((void*)&f, (unsigned char*)floatstring, sizeof(f));
  printf("floathex = %s\n", floatstring);
  f = 1.23f;
  //floatstring[0] = 'a';
  if (isfpstring((void*)&f, (unsigned char*)floatstring, sizeof(f))) {
    //printf("fpval = %.32f\n", f);
    printf("fpval = %.32Lf\n", f);
  } else {
    printf("Error converting floating point from hex.\n");
  }
  exit(0);
}
