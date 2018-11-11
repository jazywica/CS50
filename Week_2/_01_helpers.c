// Helper functions for converting notes (D4@1/8, E4@1/4 or F#4@1/2 etc.) into 'wav' files
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "helpers.h"


// Converts a fraction formatted as X/Y to eighths
int duration(char *fraction)
{
    int nominator = atoi(&fraction[0]); // retrieving an it from a string
    int denominator = atoi(&fraction[2]);
    return nominator * (8 / denominator);
}


// Calculates frequency (in Hz) of a note
int frequency(char *note)
{
    char n[3];
    int idx = 0;
    while (!isdigit(note[idx])) {
        n[idx] = note[idx]; // Collecting the necessary information about the tone
        idx++;
    }
    n[idx] = '\0';
    int octave = atoi(&note[idx]); // retrieving an octave as a string
    float freq = 440;

    if (strlen(n) > 1) {
        if (n[1] == '#') // adjusting tones for accidentals
            freq = freq * pow(2, 1 / 12.0);
        else if (n[1] == 'b')
            freq = freq / pow(2, 1 / 12.0);
    }

    if (octave != 4) {
        int adjust = octave - 4; // adjusting tones for octaves
        freq *= pow(2, adjust);
    }

    if (n[0] == 'B') // adjusting tones themselves in relation to note A
        freq = freq * pow(2, 2 / 12.0);
    else if (n[0] == 'G')
        freq = freq / pow(2, 2 / 12.0);
    else if (n[0] == 'F')
        freq = freq / pow(2, 4 / 12.0);
    else if (n[0] == 'E')
        freq = freq / pow(2, 5 / 12.0);
    else if (n[0] == 'D')
        freq = freq / pow(2, 7 / 12.0);
    else if (n[0] == 'C')
        freq = freq / pow(2, 9 / 12.0);

    return round(freq);
}


// Determines whether a string represents a rest
bool is_rest(char *s)
{
    if (strcmp(s, "") == 0)
        return true;

    return false;
}
