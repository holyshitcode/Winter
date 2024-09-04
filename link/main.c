#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum dataType {
    STRING,
    INTEGER,
    CHAR
};

typedef struct node {
    void *data;
    struct node *next;
    enum dataType type;
} node;

node *head = NULL;
node *tail = NULL;

void insertData(void *data, enum dataType type) {
    node *temp = malloc(sizeof(node));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    temp->data = data;
    temp->type = type;
    temp->next = NULL;

    if (head == NULL) {
        head = temp;
        tail = temp;
    } else {
        tail->next = temp;
        tail = temp;
    }
}

void showData() {
    node *temp = head;
    while (temp != NULL) {
        if (temp->data != NULL) {
            // 여기가 중요: 삽입된 데이터가 어떤 타입인지 확인하여 출력
            if (temp->type == STRING ) {
                printf("String Data: %s\n", (char *)temp->data);
            } else if (temp->type == INTEGER) {
                printf("Integer Data: %d\n", *(int *)temp->data);
            }
        }
        temp = temp->next;
    }
}

void freeList() {
    node *temp = head;
    while (temp != NULL) {
        node *next = temp->next;
        free(temp->data);  // 동적 메모리 해제
        free(temp);
        temp = next;
    }
    head = tail = NULL;
}

int main(void) {

    bool flag = true;

    while (flag) {
        int intData;
        char charData;

        printf("Please enter an integer: ");
        scanf("%d", &intData);

        // 버퍼에 남아있는 개행 문자를 제거합니다.
        while (getchar() != '\n');

        printf("Please enter a character: ");
        scanf("%c", &charData);

        // 문자열 데이터 동적 할당
        char *stringData = malloc(intData * sizeof(char));
        if (stringData == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        printf("Please enter a string: ");
        scanf("%s", stringData);

        // 데이터를 삽입
        insertData(stringData,STRING);

        // int 값을 저장하기 위해 동적 메모리 할당
        int *intPtr = malloc(sizeof(int));
        if (intPtr == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        *intPtr = intData;
        insertData(intPtr,INTEGER);

        // 종료를 위한 플래그 설정
        flag = false;
    }

    // 데이터 출력
    showData();

    // 리스트 메모리 해제
    freeList();

    return 0;
}