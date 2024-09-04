#include <stdio.h>
#include <stdlib.h>

/**
 * 연결리스트는 데이터와 그 좌표를 가지고있으므로
 * 구조체내부에 그다음 데이터의 구조체를 넣어 탐색한다.
 * 시작부분과 끝부분을 표시해주지않으면 동적구조로 알수가없음으로
 * head, tail 로 구분하도록하겠다.
 */
typedef struct node {
    void *data;
    struct node *next;
} node;

node *head = NULL;
node *tail = NULL;

void insertData(void *data) {
    node *temp = malloc(sizeof(node));
    temp->data = data;
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
        printf("Data: %p\n", temp->data);
        temp = temp->next;
    }
}

int main(void) {
    while(1) {
        int intData;
        char charData;

        printf("Please enter an integer: ");
        scanf("%d", &intData);
        printf("Please enter a character: ");
        scanf("%c", &charData);
        char *stringData = malloc(intData * sizeof(char));

        insertData(stringData);
        insertData((int*)intData);

    }
}

