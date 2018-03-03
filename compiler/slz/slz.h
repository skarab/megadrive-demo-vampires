#ifndef __SLZ_H__
#define __SLZ_H__


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "stdint.h"
#include <cstring>

using namespace std;

// Error codes
enum {
   ERR_NONE,            // No error
   ERR_CANTREAD,        // Can't read from input file
   ERR_CANTWRITE,       // Can't write into output file
   ERR_TOOLARGE16,      // File is too large for SLZ16
   ERR_TOOLARGE24,      // File is too large for SLZ24
   ERR_CORRUPT,         // File is corrupt?
   ERR_NOMEMORY,        // Ran out of memory
   ERR_UNKNOWN          // Unknown error
};

// Possible formats
enum {
   FORMAT_DEFAULT,      // No format specified
   FORMAT_SLZ16,        // SLZ16 (16-bit size)
   FORMAT_SLZ24,        // SLZ24 (24-bit size)
   FORMAT_TOOMANY       // Too many formats specified
};

// Function prototypes
int read_word(FILE *, uint16_t *);
int read_tribyte(FILE *, uint32_t *);
int write_word(FILE *, const uint16_t);
int write_tribyte(FILE *, const uint32_t);

int compress(FILE *infile, FILE *outfile, int format);

#endif
