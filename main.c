#include <stdio.h>

const char *horizonBar = "+------------------------------+";

void sendMenuBar();



int main(void) {
    char tables[];

    /*
     * column names
     */

    sendMenuBar();
    return 0;
}

void sendMenuBar(char *schemeName, char *tables ) {
    char *scheme = schemeName;
    char menuTables;
    int height = 10;
    printf("%s\n", horizonBar);
    for (int i = 0; i < height; i++) {
        printf("|                              |\n");
    }
    printf("%s\n", horizonBar);
}

char* makeColumns(*char columns) {
    return ;
}

/**
 * ToDo
 * 반환타입 리스트로 반환하게만들기
 */
 
