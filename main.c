#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

const char *horizonBar = "+------------------------------+";

void sendMenuBar(const char *schemeName, const char *tables, int count);
char *makeColumns(int count, ...);

int main(void) {
    char *columns[50];  // 포인터 배열
    int count = 0;
    char schemeName[50];
    printf("Enter the scheme name: ");
    scanf("%s", schemeName);
    sendMenuBar(schemeName, NULL,0);

    while (true) {
        char quitStatus[10];
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
            return 1;
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

    // `makeColumns` 함수 호출 및 결과 출력
    char *result = makeColumns(count,
                                columns[0],
                                count > 1 ? columns[1] : NULL,
                                count > 2 ? columns[2] : NULL,
                                count > 3 ? columns[3] : NULL,
                                count > 4 ? columns[4] : NULL);

    if (result != NULL) {
        printf("Combined columns: %s\n", result);
        free(result);  // 동적으로 할당된 메모리 해제
    }

    // 동적으로 할당된 메모리 해제
    for (int i = 0; i < count; i++) {
        free(columns[i]);
    }

    sendMenuBar(schemeName, columns, count);

    return 0;
}

void sendMenuBar(const char *schemeName, const char *columns, int count) {
    printf("           [%s]\n", schemeName);
    printf("%s\n", horizonBar);
    for (int i = 0; i < count; i++) {
        printf("|             %s                 |\n",&columns[i]);

    }
    printf("%s\n", horizonBar);
}

char *makeColumns(int count, ...) {
    va_list args;
    va_start(args, count);

    // 총 길이를 계산하기 위한 변수
    int totalLength = 0;
    for (int i = 0; i < count; i++) {
        char *item = va_arg(args, char *);
        if (item != NULL) {
            totalLength += strlen(item) + 2;  // 각 항목 사이의 구분자와 공백을 위한 추가 공간
        }
    }

    // 가변 인수 목록을 다시 시작
    va_end(args);
    va_start(args, count);

    // 결과 문자열을 저장할 메모리 할당
    char *columns = malloc(totalLength + 1);  // null terminator를 포함한 메모리
    if (columns == NULL) {
        printf("Memory allocation failed\n");
        va_end(args);
        return NULL;
    }
    columns[0] = '\0';  // 빈 문자열로 초기화

    // 문자열들을 결합
    for (int i = 0; i < count; i++) {
        char *item = va_arg(args, char *);
        if (item != NULL) {
            strcat(columns, item);
            if (i < count - 1) {
                strcat(columns, ", ");  // 각 항목을 콤마로 구분
            }
        }
    }
    va_end(args);
    return columns;
}

/***
 *ToDo 안에넣는거까진 성공, 제대로된 스트링이 나오게만들기 처음에나오는 박스 카운트 적용안되게
 *조건문넣기
 */