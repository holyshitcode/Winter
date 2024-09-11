#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>


/**
 * B-Tree 구현
 */
#define M 3
#define LEFT 0
#define CENTER 1
#define RIGHT 2

typedef struct node {
    int keyValue[M - 1];
    struct node *child[M];
    int currentKeyCount;
    bool leafStatus;
} node;

node *root;

node *createNode(int newValue) {
    node *newNode = malloc(sizeof(node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->keyValue[0] = newValue;
    newNode->currentKeyCount = 1;
    newNode->leafStatus = false;
    return newNode;
}

/*
node *searchNode(node *target, int key) {
    if (target == NULL) {
        return NULL;
    }
    for (int i = 0; i < target->currentKeyCount; i++) {
        if (key == target->keyValue[i]) {
            return target;
        }
    }
    if (target->keyValue[LEFT] == key || target->keyValue[1] == key) {
        return target;
    }
    if (key < target->keyValue[0]) {
        return searchNode(target->child[LEFT], key);
    }
    if (key > target->keyValue[0] && key < target->keyValue[1]) {
        return searchNode(target->child[CENTER], key);
    }
    if (key > target->keyValue[1]) {
        return searchNode(target->child[RIGHT], key);
    }
    return target;
}*/
node *searchNode(node *target, int key) {
    if (target == NULL) {
        return NULL;
    }

    // 현재 노드의 키들과 비교
    for (int i = 0; i < target->currentKeyCount; i++) {
        if (key == target->keyValue[i]) {
            return target; // 키를 찾음
        }
    }

    // 자식 노드로 내려가기 위한 조건
    if (key < target->keyValue[0]) {
        // 첫 번째 키보다 작은 경우
        return searchNode(target->child[LEFT], key);
    } if (target->currentKeyCount == 1) {
        // 하나의 키만 있는 경우, 그 키보다 큰 값은 오른쪽 자식으로
        return searchNode(target->child[RIGHT], key);
    } if (target->currentKeyCount == 2) {
        // 두 개의 키가 있는 경우
        if (target->keyValue[0] <= key <= target->keyValue[1]) {
            // 두 키 사이에 있는 경우 가운데 자식
            return searchNode(target->child[CENTER], key);
        } if (key > target->keyValue[1]) {
            // 두 번째 키보다 큰 경우 오른쪽 자식
            return searchNode(target->child[RIGHT], key);
        }
    }
    return target;
}


node *insertNode(node *node, int mid_key);

/*
node *insertNode(node *target, int key) {
    if (target == NULL) {
        return createNode(key);
    }
    if (target->keyValue[LEFT] == key || target->keyValue[1] == key ) {
        return createNode(target->keyValue[LEFT]);
    }
    if(key < target->keyValue[0]) {
        return insertNode(target->child[LEFT], key);
    }
    if(key > target->keyValue[0] && key < target->keyValue[1]) {
        return insertNode(target->child[CENTER], key);
    }
    if(key > target->keyValue[1]) {
        return insertNode(target->child[RIGHT], key);
    }
    return target;
}
*/
node *split(node *target, int key) {
    int tempKeys[M];
    for (int i = 0; i < target->currentKeyCount; i++) {
        tempKeys[i] = target->keyValue[i];
    }
    tempKeys[target->currentKeyCount] = key;

    for (int i = 0; i < M - 1; i++) {
        for (int j = i + 1; j < M; j++) {
            if (tempKeys[i] > tempKeys[j]) {
                int temp = tempKeys[i];
                tempKeys[i] = tempKeys[j];
                tempKeys[j] = temp;
            }
        }
    }

    int midIndex = M / 2;
    int midKey = tempKeys[midIndex];

    node *leftNode = createNode(tempKeys[0]);
    node *rightNode = createNode(tempKeys[midIndex + 1]);

    for (int i = 1; i < ceil(midIndex); i++) {
        leftNode->keyValue[leftNode->currentKeyCount++] = tempKeys[i];
    }
    for (int i = midIndex + 2; i < M; i++) {
        rightNode->keyValue[rightNode->currentKeyCount++] = tempKeys[i];
    }

    if (!target->leafStatus) {
        for (int i = 0; i <= midIndex; i++) {
            leftNode->child[i] = target->child[i];
        }
        for (int i = midIndex + 1; i < M; i++) {
            rightNode->child[i - (midIndex + 1)] = target->child[i];
        }
        leftNode->leafStatus = false;
        rightNode->leafStatus = false;
    }

    if (target == root) {
        node *newRoot = createNode(midKey);
        newRoot->child[LEFT] = leftNode;
        newRoot->child[RIGHT] = rightNode;
        newRoot->leafStatus = false;
        root = newRoot;
        return newRoot;
    } else {
        return insertNode(root, midKey);
    }
}

bool duplicatedKey(node *target, int key) {
    // 키 중복 여부를 확인
    if (searchNode(target, key) != NULL) {
        printf("duplicated key(%d) found and deleted\n", key);
        return true; // 중복 키가 있음을 표시
    }
    return false; // 중복이 아닐 경우
}

node *insertNode(node *target, int key) {
    if (duplicatedKey(target, key)) {
        return target; // 중복된 키가 있을 경우 삽입하지 않음
    }

    if (target == NULL) {
        return createNode(key); // 새 노드 생성
    }

    if (target->leafStatus) {
        // 리프 노드에 삽입
        if (target->currentKeyCount < M - 1) {
            // 키를 삽입할 공간이 있으면 삽입
            target->keyValue[target->currentKeyCount++] = key;
            return target;
        } else {
            // 공간이 없으면 분할 필요
            return split(target, key);
        }
    } else {
        // 리프가 아니면 적절한 자식으로 이동
        if (key < target->keyValue[0]) {
            // 왼쪽 자식으로 이동
            target->child[LEFT] = insertNode(target->child[LEFT], key);
        } else if (target->currentKeyCount == 1) {
            // 노드에 하나의 키만 있는 경우 오른쪽으로 이동
            target->child[RIGHT] = insertNode(target->child[RIGHT], key);
        } else if (key < target->keyValue[1]) {
            // 두 개의 키가 있는 경우 가운데로 이동
            target->child[CENTER] = insertNode(target->child[CENTER], key);
        } else {
            // 그 외의 경우 오른쪽으로 이동
            target->child[RIGHT] = insertNode(target->child[RIGHT], key);
        }

        // 자식 노드를 업데이트한 후 현재 노드를 반환
        return target;
    }
}

void printTree(node *root, int level) {
    if (root == NULL) {
        return;
    }

    // 현재 레벨의 키들을 출력
    printf("Level %d: ", level);
    for (int i = 0; i < root->currentKeyCount; i++) {
        printf("%d ", root->keyValue[i]);
    }
    printf("\n");

    // 리프 노드가 아닌 경우, 자식 노드들을 출력
    if (!root->leafStatus) {
        for (int i = 0; i <= root->currentKeyCount; i++) {
            if (root->child[i] != NULL) {
                printTree(root->child[i], level + 1);
            }
        }
    }
}
void testInsert() {
    printf("Inserting keys into B-Tree:\n");


    insertNode(root, 10);
    insertNode(root, 20);
    insertNode(root, 30);
    insertNode(root, 50);
    insertNode(root, 70);


    printf("Tree structure after inserts:\n");
    printTree(root, 0);
}

void testSearch(node *root,int key) {
    node *result = searchNode(root, key);
    if (result != NULL) {
        printf("Key %d found in the tree!\n", key);
    } else {
        printf("Key %d not found in the tree.\n", key);
    }
}

int main(void) {
    // Root 노드 생성
    root = createNode(40);

    // 삽입 테스트
    testInsert();

    // 검색 테스트
    printf("\nSearching for keys in B-Tree:\n");
    testSearch(root,20); // 존재하는 키
    testSearch(root,50); // 존재하는 키
    testSearch(root,70); // 존재하지 않는 키

    return 0;
}
