// Program that uncovers a secret message from a bitmap
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 3) {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n"); // a clue file and an output filename must be provided as command line arguments
        return 1;
    }

    // remember filenames
    char *infile = argv[1];
    char *outfile = argv[2];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL) {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL) {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER - this will write all the bytes to the struct's fields, it is order-based
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr); // reads first 14 bytes

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr); // reads the next 40 bytes

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0 - checking already imported values inside both objects
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0) {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    int header1 = sizeof(BITMAPFILEHEADER); // for each struct, there is a memory allocated for each of its fields hence size = 14
    fwrite(&bf, header1, 1, outptr);

    // write outfile's BITMAPINFOHEADER
    int header2 = sizeof(BITMAPINFOHEADER); // for each struct, there is a memory allocated for each of its fields hence size = 40
    fwrite(&bi, header2, 1, outptr);

    // determine padding for scanlines
    int rgb = sizeof(RGBTRIPLE);
    int padding = (4 - (bi.biWidth * rgb) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++) {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++) {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr); // buffering data into the 'triple' file and then writing it to the output file

			// filtering all the red color out
            if (triple.rgbtBlue == 0 && triple.rgbtGreen == 0 && triple.rgbtRed == 255) {
                triple.rgbtBlue = 255;
                triple.rgbtGreen = 255;
                triple.rgbtRed = 255;
            }

			// strengthening the hidden text by changing it to black
            if (triple.rgbtRed != 0 && triple.rgbtRed != 255) {
                triple.rgbtBlue = 0;
                triple.rgbtGreen = 0;
                triple.rgbtRed = 0;
            }

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR); // steps over the padding in this line - we are currently finished with the line

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
            fputc(0x00, outptr);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
