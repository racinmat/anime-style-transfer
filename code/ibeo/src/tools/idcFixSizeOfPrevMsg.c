//======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//======================================================================

const int sizeOfMagicWord = 4;
const int sizeOfSize = 4;

char MagicWord[sizeOfMagicWord] = {0xAF, 0xFE, 0xC0, 0xC2};
char newSizeOfPrevMsg[sizeOfSize] = {0,0,0,0};

//======================================================================

int main(int argc, char* argv[])
{
  if (argc != 2) {
    puts("Wrong number of arguments");
    puts("program <file>");
    exit(1);
  }

  char* filename = argv[1];

  FILE* f = fopen(filename, "r+");
  if (!f) {
    printf("Could not open file %s\n",filename);
    exit(2);
  }

  char magicWord[sizeOfMagicWord];
  if (fread(magicWord, 1, sizeOfMagicWord, f) != sizeOfMagicWord) {
    printf("Could not read magic word from file %s\n",filename);
    exit(3);
  }
  if (strncmp(magicWord, MagicWord, sizeOfMagicWord) != 0) {
    printf("File %s does not start with a magic word\n",filename);
    exit(4);
  }
  fseek(f, sizeOfMagicWord, SEEK_SET);
  if (fwrite(newSizeOfPrevMsg, 1, sizeOfSize, f) != sizeOfSize) {
    printf("Failed to fix sizeOfPrevMsg in file %s\n",filename);
    exit(5);    
  }
  printf("Sucessfully repaired %s\n", filename);
  exit(0);
}

//======================================================================
