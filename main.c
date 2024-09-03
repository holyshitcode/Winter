#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

const char *horizonBar = "+------------------------------+";

void sendMenuBar(const char *schemeName, char **columns, int count);
void makeScheme();


int main(void) {
    char *status = malloc(sizeof(char) * 50);
    printf("Enter your status:(scheme, .. , .. , ..):");
    scanf("%s", status);

    if(strcmp(status, "scheme") == 0) {
        makeScheme();
    }

    return 0;
}





void sendMenuBar(const char *schemeName, char **columns, int count) {
    printf("           [%s]\n", schemeName);
    printf("%s\n", horizonBar);

    for (int i = 0; i < 10; i++) {
        if (i < count) {
            printf("| %-28s |\n", columns[i]);  // 왼쪽 정렬하여 출력
        } else {
            printf("|                              |\n");
        }
    }

    printf("%s\n", horizonBar);
}


void makeScheme() {
    char *columns[50];  // 포인터 배열
    int count = 0;
    char schemeName[50];

    printf("Enter the scheme name: ");
    scanf("%49s", schemeName);  // 최대 49자까지 입력

    while (true) {
        char temp[50];

        printf("Please enter a column name (or 'quit' to exit): ");
        scanf("%49s", temp);  // 최대 49자까지 입력

        if (strcmp(temp, "quit") == 0) {
            break;
        }

        // 각 포인터에 메모리 할당 및 문자열 복사
        columns[count] = malloc(strlen(temp) + 1);
        if (columns[count] == NULL) {
            printf("Memory allocation failed\n");
        }
        strcpy(columns[count], temp);
        count++;

        if (count >= 50) {
            printf("Maximum number of columns reached\n");
            break;
        }
    }

    printf("Columns entered:\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", columns[i]);
    }

    sendMenuBar(schemeName, columns, count);

    // 동적으로 할당된 메모리 해제
    for (int i = 0; i < count; i++) {
        free(columns[i]);
    }
}



