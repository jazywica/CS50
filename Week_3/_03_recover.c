// Program that recovers JPG images (multiples of 512 byts) from a memory card
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t BYTE; // this is required as we can't really use an 'int' (of size 4) when we want to read single bytes from the memory card


int main(int argc, char *argv[])
{
	 // ensure proper usage
    if (argc != 2) {
        fprintf(stderr, "Usage: ./recover image\n"); // an input RAW file must be provided as command line arguments
        return 1;
    }

    int size = 512;
    int counter = 0;
    int foundFirst = 0; // flag for indicating if we found a first file
    char filename[8]; // storage for increasing filenames

    FILE *file = fopen(argv[1], "r");
    BYTE *buffer = malloc(size); //BYTE buffer[size]; // we can also use a static array here, but 'fwrite' has to get a pointer i.e. "&buffer" 
    FILE *img;

    while (fread(buffer, 1, size, file) == 512) { // 'fread' returns the size of the imported bytes, it will stop automatically when we reach EOL
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) { // we use a bitwise op. 'AND'
            if (!foundFirst) {
                sprintf(filename, "%03i.jpg", counter);
                img = fopen(filename, "w");
                fwrite(buffer, size, 1, img);
                foundFirst = 1; // from this point onwards, we only import the files as continuous data flow
            }
            else {
                fclose(img); // if we are already in the importing mode and we get another jpg, we have to close the previous one
                counter++;
                sprintf(filename, "%03i.jpg", counter);
                img = fopen(filename, "w");
                fwrite(buffer, size, 1, img);
            }
        }

        else if (foundFirst) // condition that writes to an already detected, open file
            fwrite(buffer, size, 1, img);
    }

    free(buffer); // freeing the memory (only valid for dynamically allocated objects)
    fclose(file);
    fclose(img);
}