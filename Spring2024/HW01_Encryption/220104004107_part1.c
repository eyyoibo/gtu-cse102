#include <stdio.h>

char alphabet[61] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '(', '<', '=', '+', ')', '[', '*', '/', ']', '{', '>', '!', '-', '}', '?', '\\', '&', '|', '%', '_', ';', '"', '#', '.', '\'', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

int main() {
    int counter = 0; 
    int key = 3;     /* static key is the sum of the numbers of my school number 2+2+0+1+0+4+0+0+4+1+0+7 = 21 , 2+1 = 3 */
    char character = 0;     
    char yorumsat = '@'; 

    /* Loop to read characters until the end of the input */
    while (scanf("%c", &character) != EOF) {
        if (character == '/') { 
            scanf("%c", &character);

            if (character == '*') {
                printf("%c", yorumsat); 

                /* Loop to read characters until the end of the comment block */
                while (scanf("%c", &character) != EOF && character != '*') {
                    if (character != ' ') {
                        counter++; 
                    }
                }

                if (character == '*') {
                    printf("%d", counter); /* Print the count of non-space characters in comments */
                }
            }
        } else {
            /* Loop to find the index of the character in the alphabet array and apply encryption */
            int i = 0;
            while (i < 61) {
                if (character == alphabet[i]) {
                    printf("%c", alphabet[(i + key) % 61]); 
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
