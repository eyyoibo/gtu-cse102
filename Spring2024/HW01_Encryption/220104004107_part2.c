#include <stdio.h>

char alphabet[61] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '(', '<', '=', '+', ')', '[', '*', '/', ']', '{', '>', '!', '-', '}', '?', '\\', '&', '|', '%', '_', ';', '"', '#', '.', '\'', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

int main() {
    int key = 3;           
    int numberofchar = 0;  
    char character = 0;           
    char yorumsat = '/';    

    /* Loop to read characters until the end of the input */
    while (scanf("%c", &character) != EOF) {
        if (character == yorumsat) { 
            /* Loop to count characters in the comment block */
            while (scanf("%c", &character) != EOF && character != '*') {
                if (character != ' ' && character != '\n') {
                    numberofchar++;
                }
            }

            if (character == '*') {
                printf(" /* There are %d characters in the comment. */", numberofchar);
            } else {
                printf(" /* Invalid comment format. */");
            }

         
            numberofchar = 0;
        } else {
            /* Loop to find the index of the character in the alphabet array and apply decryption */
            int i = 0;
            while (i < 61) {
                if (character == alphabet[i]) {
                    printf("%c", alphabet[(i - key + 61) % 61]); 
                    break;
                }
                i++;
            }
        }

        if (character == '\n' || character == ' ') {
            printf("%c", character); /* Print newline or space characters as they are */
        }
    }

    return 0; 
}
