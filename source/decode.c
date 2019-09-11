#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Program developed for NWEN243, Victoria University of Wellington
   Author: Kris Bubendorfer (c) 2014-15.
   Uses a modified caeser I & II (in 2015)

   Compile:  gcc -o decode decode.c

   See encode for  examples on using it
 */

char upcase(char ch){
  if(islower(ch))
    ch -= 'a' - 'A';
  return ch;
}

char* fixkey(char* s){
  int i, j;
  char plain[26]; // assume key < length of alphabet, local array on stack, will go away!

  for(i = 0, j = 0; i < strlen(s); i++){
    if(isalpha(s[i])){
      plain[j++] = upcase(s[i]);
    }
  }
  plain[j] = '\0';
  return strcpy(s, plain);
}


int in(char c, char* s, int pos){
  // Assume everything is already in the same case
  int i;

  for(i = 0; i < pos; i++)
    if(c == s[i]) return 1;
    
  return 0;
}

void buildencodetable (char* key, char* encode) {

    // This function needs to build an array of mappings in the 'encode' array from plaintext characters
    // to encypered characters.  The encode array will be indexed by the plaintext char.  To
    // make this a useful 0-26 index for the array, 'A' will be stubtracted from it (yes you
    // can do this in C).  You can see this in the main(){} below.  The values in the array
    // will be the cipher value, in the example at the top A -> H, B -> J, etc.

    // You are implementing a Caesar 1 & 2 combo Cypher as given in handout.
    // Your code here:

    // Useful For Knowing where we have been.
    for (int i = 0; i < 26; i++) encode[i] = '_';

    int tableIndex = strlen(key) - 1; // initial offset

    fixkey(key);

    // remove duplicates in key.
    for (char *current = key; current != key + strlen(key); ++current) {
        char *duplicate = strchr((current + 1), *current);
        if (duplicate == NULL) continue; // this is the last index of current

        // offset all subsequent chars to the left,
        for (; duplicate != key + strlen(key) - 1; ++duplicate)
            *duplicate = *(duplicate + 1);

        // clear last character.
        key[strlen(key) - 1] = '\0';
    }

    // Place key in encode.
    for (int keyIndex = 0; keyIndex < strlen(key); ++keyIndex, ++tableIndex) {
        if (tableIndex > 26) tableIndex = 0; // reset index
        encode[tableIndex] = key[keyIndex];
    }

    // Remember Last Char, ternary as it could have reset.
    char prevChar = (tableIndex != 0) ? encode[tableIndex - 1] : encode[0];


    // While we have not written to this location yet, fill with rest of alphabet.
    while (encode[tableIndex] == '_') {

        do prevChar = (prevChar != 'Z') ? prevChar + 1 : 'A';
        while (strchr(encode, prevChar) != NULL); // Keep incrementing chars till we haven't used one.

        encode[tableIndex] = prevChar;

        if (++tableIndex > 25) tableIndex = 0; // increment and/or reset index
    }
}


void buildtable (char* key, char* decode){ // this changed from encode
  
  // This function needs to build an array of mappings in 'encode' from plaintext characters
  // to encihered characters.

  // You are implementing a Caesar 1 & 2 combo Cypher as given in the lab handout.
  // Your code here:

  // probably need to declare some stuff here!

  // the simplest way to do this is to do exactly the same as you did when creating the 
  // encode table, and then look up the encode table to get the translations, and build the
  // decode table from this.  This isn't the most efficient approach, but it will get the 
  // job done unless you want to be fancy.


    // Allocate and create Encode table.
    char* encode = (char*) malloc(sizeof(char) * 26);
    buildencodetable(key, encode);

    // Reverse chars through encode table.
    for (char *letter = encode; letter != encode + 26; ++letter) {
        decode[encode[*letter - 'A'] - 'A'] = *letter;
    }

}

int main(int argc, char **argv){
  // format will be: 'program' key {encode|decode}
  // We'll be using stdin and stdout for files to encode and decode.

  // first allocate some space for our translation table.

  char* decode = (char*)malloc(sizeof(char)*26); // this changed from encode
  char ch;

  if(argc != 2){
    fprintf(stderr,"format is: '%s' key", argv[0]);
    exit(1);
  }

  // Build translation tables, and ensure key is upcased and alpha chars only.

  buildtable(argv[1], decode); // this changed from encode

  // write the key to stderr (so it doesn't break our pipes)

  fprintf(stderr,"key: %s - %d\n", decode, strlen(decode));


  // the following code does the translations.  Characters are read 
  // one-by-one from stdin, translated and written to stdout.

    ch = fgetc(stdin);
    while (!feof(stdin)) {
      if(isalpha(ch))          // only decrypt alpha chars
	fputc(decode[ch-'A'], stdout);
     else 
	fputc(ch, stdout);
      ch = fgetc(stdin);      // get next char from stdin
    }
}

