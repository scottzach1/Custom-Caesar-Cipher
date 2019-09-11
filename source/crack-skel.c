#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define TEXT_SIZE 200000  // Note, the longer the text the more likely you will get a good 'decode' from the start.
#define ALEN 26         // Number of chars in ENGLISH alphabet
#define ETA "ETAONRISHDLFCMUGYPWBVKJXQZ" // Characters in order of appearance in English documents.
#define ABC "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/* Program developed for NWEN243, Victoria University of Wellington
   Author: Kris Bubendorfer, this extended version (c) 2015
   LAB: 2

   This program applies a basic frequency analysis on a cyphertext.  It has been extened over the 2014 version to
   solve polyalphabetic cyphers - by brute force.  In this case, it applies the frequency analysis for different 
   numbers of n keys (polyalphabetic Caeser).  Obviously it will need a cypher of about n times
   the typical length for a monoalphabetic cypher.

   Program is used like this:

   Compile:  gcc -o crack crack.c

   Test file (ctext): JWRLS, XSSH PZK JH HES BJFV, UZU (this is not a realistic length piece of cypher text)

   crack n

   Argument:

   n number of keys to try

   ---

   % cat ctext | crack 1
   ALICE, MEET YOU AT THE PARK, BOB   <-- of course it won't be this correct.  Don't worry about that for the -d option.
   AMFDE, UEET LNH AT TIE RASC, ONO   <-- this is what it really looks like, a larger sample is better, this is short.


 */

void splitText(char *text, char **subtexts, int keyLen);
void frequencyAnalysis(char *text, char *mappings);

char upcase(char ch){
  if(islower(ch))
    ch -= 'a' - 'A';
  return ch;
}

int main(int argc, char **argv) {

    // first allocate some space for our input text (we will read from stdin).

    char *text = (char *) malloc(sizeof(char) * TEXT_SIZE + 1);
    char chStart;
    int maxKeyLength, iStart;

    if (argc > 1 && (maxKeyLength = atoi(argv[1])) > 0);
    else {
        fprintf(stderr, "Malformed argument, use: crack [nStart], nStart > 0\nStart");
        exit(-1);
    } // get the command line argument nStart

    // Now read TEXT_SIZE or feof worth of characters (whichever is smaller) and convert to uppercase as we do it.
    // Added: changed to count frequencies as we read it in

    for (iStart = 0, chStart = fgetc(stdin); iStart < TEXT_SIZE && !feof(stdin); iStart++, chStart = fgetc(stdin)) {
        text[iStart] = (chStart = (isalpha(chStart) ? upcase(chStart) : chStart));
    }
    text[iStart] = '\0'; // terminate the string properly.

    /* At this point we have two things,
     *   1. The input cyphertext in "text"
     *   2. The maximum number of keys to try (nStart) - we'll be trying 1..nStart keys.
     *
     * What you need to do is as follows:
     *   1. create a for-loop that will check key lengths from 1..nStart
     *   2. for each i <= nStart, spit the cypher text into i sub-texts.  For i = 1, 1 subtext, for i = 2, 2 subtexts, of alternating characters etc.
     *   3. for each subtext:
     *          a. count the occurance of each letter
     *          b. then map this onto the CHFREQ, to create a map between the sub-text and english
     *          c. apply the new map to the subtext
     *   4. merge the subtexts
     *   5. output the 'possibly' partially decoded text to stdout.  This will only look OK if i was the correct number of keys
     *
     * what you need to output (sample will be provided) - exactly:
     * i maps -> stderr
     * i 'possible' translations
     *
     * You would be wise to make seperate functions that perform various sub-tasks, and test them incrementally.  Any other approach will likely
     * make your brain revolt.  This isn't a long program, mine is 160 lines, with comments (and written in a very verbose style) - if yours is
     * getting too long, double check you're on the right track.
     *
     */

    // Your code here...


    // Attempt All Key Lengths.
    for (int keyLenIndex = 0; keyLenIndex < maxKeyLength; ++keyLenIndex) {
        int keyLen = keyLenIndex + 1;

        // Allocate Memory
        char **subtexts = (char **) malloc(sizeof(char *) * keyLen);

        for (int i = 0; i < keyLen; ++i) {
            subtexts[i] = (char *) malloc(sizeof(char) * TEXT_SIZE);
        }

        char mappings[keyLen][ALEN];
        char solution[TEXT_SIZE];

        // Split the text into subtexts.
        splitText(text, subtexts, keyLen);

        // Calculate Frequencies and generate map.
        for (int keyModIndex = 0; keyModIndex < keyLen; ++keyModIndex) {
            frequencyAnalysis(subtexts[keyModIndex], mappings[keyModIndex]);
        }

        // Decode Text
        for (int textIndex = 0; textIndex < TEXT_SIZE; ++textIndex) {
            int keyModIndex = textIndex % keyLen;

            char c = text[textIndex];
            if ((isalpha(c) != 0)) // Ignore non-alphabetic characters.
                c = mappings[keyModIndex][c - 'A'];

            solution[textIndex] = c;
        }

        // Yeet to console.
        printf("%s\n", solution);

        free(subtexts);
    }
}

void frequencyAnalysis(char *text, char *mappings) {

    // Count Frequencies
    int *keyFrequencies = (int *) calloc(ALEN, sizeof(int));

    for (int i=0; i<TEXT_SIZE || !text[i]; ++i) {
        char c = text[i];
        if (isalpha(c) != 0)
            ++keyFrequencies[c - 'A'];
    }

    // Sort the Frequencies: Bubble Sort
    char keysByFrequency[] = ABC;

    for (int i = 0; i < ALEN - 1; ++i) {
        for (int j = 0; j  < ALEN - 1 - i; ++j) {
            if (keyFrequencies[keysByFrequency[j] - 'A'] < keyFrequencies[keysByFrequency[j + 1] - 'A']) {
                char temp = keysByFrequency[j];
                keysByFrequency[j] = keysByFrequency[j + 1];
                keysByFrequency[j + 1] = temp;
            }
        }
    }

    // Map to ETA
    char trueFrequencies[] = ETA;

    for (int i = 0; i < ALEN; ++i)
        mappings[keysByFrequency[i] - 'A'] = trueFrequencies[i];

    free(keyFrequencies);
}

void splitText(char *text, char **subtexts, int keyLen) {
    for (int textIndex = 0; textIndex < TEXT_SIZE; textIndex++) {
        int keyModIndex = textIndex % keyLen;
        int subtextIndex = textIndex / keyLen;
        subtexts[keyModIndex][subtextIndex] = text[textIndex];
    }
}
