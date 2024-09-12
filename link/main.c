#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * 노드의 데이터타입을 구분하는 enum
 */
enum dataType {
    STRING,
    INTEGER,
    CHAR
};
/**
 * 메인프로그램 동작시에 유저가 선택하는 작동
 */
enum operation {
    INPUT,
    SHOW,
    DELETE,
    EXIT
};
/**
 * 메인프로그램 종료플래그
 */
enum exitFlag {
    in,
    out
};

typedef struct node {
    void *data; // generic으로 데이터를 받아서 저장 다른 노드를 사용하지않음
    struct node *next;
    enum dataType type;
} node;

node *head = NULL;
node *tail = NULL;

void insertData(void *data, enum dataType type);
void showData();
void deleteData(void *data, enum dataType type);
void freeList();

int main(void) {
    enum exitFlag exitFlag = in;
    int choice, dataTypeFlag;

    while (exitFlag == in) {
        printf("1. Insert\n2. Show\n3. Delete\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("Enter dataType[1.String, 2.Char, 3.Integer]: ");
                scanf("%d", &dataTypeFlag);

                if (dataTypeFlag == 1) {
                    char *stringData = malloc(100 * sizeof(char));  // 문자열 메모리 할당
                    if (stringData == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Input Data in String: ");
                    scanf("%s", stringData);
                    insertData(stringData, STRING);

                } else if (dataTypeFlag == 2) {
                    char *charData = malloc(sizeof(char));
                    if (charData == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Input Data in Char: ");
                    scanf(" %c", charData);
                    insertData(charData, CHAR);

                } else if (dataTypeFlag == 3) {
                    int *intData = malloc(sizeof(int));
                    if (intData == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Input Data in Integer: ");
                    scanf("%d", intData);
                    insertData(intData, INTEGER);

                } else {
                    printf("Invalid data type.\n");
                }
                break;
            }

            case 2:
                showData();
                break;

            case 3: {
                printf("Enter dataType[1.String, 2.Char, 3.Integer]: ");
                scanf("%d", &dataTypeFlag);

                if (dataTypeFlag == 1) {
                    char stringData[100];
                    printf("Input Data in String: ");
                    scanf("%s", stringData);
                    deleteData(stringData, STRING);

                } else if (dataTypeFlag == 2) {
                    char charData;
                    printf("Input Data in Char: ");
                    scanf(" %c", &charData);
                    deleteData(&charData, CHAR);

                } else if (dataTypeFlag == 3) {
                    int intData;
                    printf("Input Data in Integer: ");
                    scanf("%d", &intData);
                    deleteData(&intData, INTEGER);

                } else {
                    printf("Invalid data type.\n");
                }
                break;
            }

            case 4:
                exitFlag = out;
                break;

            default:
                printf("Invalid choice.\n");
                break;
        }
    }
    freeList();
    return 0;
}

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
            if (temp->type == STRING) {
                printf("String Data: %s\n", (char *)temp->data);
            } else if (temp->type == INTEGER) {
                printf("Integer Data: %d\n", *(int *)temp->data);
            } else if (temp->type == CHAR) {
                printf("Character Data: %c\n", *(char *)temp->data);
            }
        }
        temp = temp->next;
    }
}

void deleteData(void *data, enum dataType type) {
    node *temp = head;
    node *prev = NULL;

    while (temp != NULL) {
        if (temp->type == type) {
            bool toDelete = false;
            switch (type) {
                case STRING:
                    if (strcmp(temp->data, data) == 0)
                        toDelete = true;
                    break;
                case INTEGER:
                    if (*(int *)temp->data == *(int *)data)
                        toDelete = true;
                    break;
                case CHAR:
                    if (*(char *)temp->data == *(char *)data)
                        toDelete = true;
                    break;
            }

            if (toDelete) {
                if (prev == NULL) {
                    head = temp->next;
                    if (temp == tail) {
                        tail = NULL;
                    }
                } else {
                    prev->next = temp->next;
                    if (temp == tail) {
                        tail = prev;
                    }
                }

                free(temp->data);
                free(temp);
                printf("Data deleted successfully.\n");
                return;
            }
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Data not found.\n");
}

void freeList() {
    node *temp = head;
    while (temp != NULL) {
        node *next = temp->next;
        free(temp->data);
        free(temp);
        temp = next;
    }
    head = tail = NULL;
}