// Program that resizes a bitmap image
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4) {
        fprintf(stderr, "Usage: ./resize f infile outfile\n"); // a factor (0.0, 100.0], input file and an output filename must be provided as command line arguments
        return 1;
    }

    // read the resizing factor and remember filenames
    float factor = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // check the resizing factor correctness
    if (factor <= 0 || factor > 100) {
        fprintf(stderr, "The resizing factor has to be in (0.0, 100.0].\n");
        return 1;
    }

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

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0) {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    int f; // storage for the corrected factor
    int origWidth = bi.biWidth;
    int origHeight = abs(bi.biHeight);
    int origPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int isPositive = 0;

    if (factor >= 1) {
        f = round(factor);
        bi.biWidth *= f;
        bi.biHeight *= f;
        isPositive = 1;
    }
    else {
        f = round(1 / factor);

		// ensure that downsizing the image will not cut off any data
        if (bi.biWidth % f != 0 || bi.biHeight % f != 0) {
            fclose(outptr);
            fclose(inptr);
            fprintf(stderr, "Downsizing factor does not match the image dimensions.\n");
            return 5;
        }
        bi.biWidth /= f;
        bi.biHeight /= f;
    }

    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; // determine padding for scanlines
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight); // updating the file size
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // 1. Upsizing - iterate over infile's scanlines
    if (isPositive) {
        for (int i = 0; i < origHeight; i++) {
            for (int j = 0; j < f; j++) { // loop that resizes the image vertically
                for (int k = 0; k < origWidth; k++) { // loop that resizes the image horizontally
                    RGBTRIPLE triple; // temporary storage
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr); // read RGB triple from infile
                    for (int l = 0; l < f; l++)
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); // write RGB triple to outfile, 'f' times
                }
                fseek(inptr, - sizeof(RGBTRIPLE) * origWidth, SEEK_CUR); // reverrse back to start of line to resize vertically
                for (int m = 0; m < padding; m++)
                    fputc(0x00, outptr);
            }
            // since we go bac to SOL at the end of each line, now we have to actually move forward to the next line
            fseek(inptr, (origWidth * sizeof(RGBTRIPLE)) + origPadding, SEEK_CUR);
        }
    }
	// 2. Downsizing - iterate over infile's scanlines
    else {
        for (int i = 0; i < abs(bi.biHeight); i++) {
            for (int j = 0; j < bi.biWidth; j++) {
                RGBTRIPLE triple; // temporary storage
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr); // read RGB triple from infile
                fseek(inptr, sizeof(RGBTRIPLE) * (f - 1), SEEK_CUR); // skip the bytes that are irrelevant
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); // write RGB triple to outfile, but only the filtered ones
            }

            fseek(inptr, origPadding, SEEK_CUR); // skip over the padding in the imported line

            for (int k = 0; k < padding; k++)
                fputc(0x00, outptr);

            fseek(inptr, (((origWidth * sizeof(RGBTRIPLE)) + origPadding) * (f - 1)), SEEK_CUR); // skip over irrelevant lines
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
