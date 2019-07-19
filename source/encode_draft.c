//
// Created by zacsc on 19/07/2019.
//

#include <stdio.h>
#include <string.h>

void printChar(char c) {
    printf("%c", c ? c : '_');
}

int main()
{
    char key[] = "HELP";

    char table[26];
    for (int i=0; i<26; i++) table[i] = '_';

    int tableIndex = strlen(key); // init offset

    for (int keyIndex = 0; keyIndex < strlen(key); ++keyIndex, ++tableIndex) {
        if (tableIndex > 26) tableIndex = 0; // reset index
        table[tableIndex] = key[keyIndex];
    }

    char prevChar = (tableIndex != 0) ? table[tableIndex - 1] : table[0];

    // While we have not written to this location yet
    while (table[tableIndex] == '_') {

        do prevChar = (prevChar != 'Z') ? prevChar + 1 : 'A';
        while (strchr(table, prevChar) != NULL); // Keep incrementing chars till we haven't used one.

        table[tableIndex] = prevChar;

        ++tableIndex;
        if (tableIndex > 25) tableIndex = 0; // increment or reset index
    }

    printf("Table: ");
    for (int i=0; i<26; i++) printChar(table[i]);

    return 5;

}
