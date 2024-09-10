#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


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
    if (target == NULL) {
        return NULL;
    }

    // 새 키와 기존 키들을 함께 정렬된 상태로 저장
    int tempKeys[M];
    for (int i = 0; i < target->currentKeyCount; i++) {
        tempKeys[i] = target->keyValue[i];
    }
    tempKeys[target->currentKeyCount] = key;

    // 키들을 정렬 (단순 삽입 정렬)
    for (int i = 0; i < target->currentKeyCount; i++) {
        for (int j = i + 1; j <= target->currentKeyCount; j++) {
            if (tempKeys[i] > tempKeys[j]) {
                int temp = tempKeys[i];
                tempKeys[i] = tempKeys[j];
                tempKeys[j] = temp;
            }
        }
    }

    // 중간 키 선택
    int midIndex = (M - 1) / 2;
    int midKey = tempKeys[midIndex];

    // 새로운 좌우 노드 생성
    node *leftNode = createNode(tempKeys[0]);
    node *rightNode = createNode(tempKeys[midIndex + 1]);

    for (int i = 1; i < midIndex; i++) {
        leftNode->keyValue[leftNode->currentKeyCount++] = tempKeys[i];
    }
    for (int i = midIndex + 2; i < M; i++) {
        rightNode->keyValue[rightNode->currentKeyCount++] = tempKeys[i];
    }

    // 자식이 있으면 자식 노드를 좌우로 분배
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

    // 새로운 부모 노드를 반환하거나, 부모에게 중간 키를 삽입
    if (target == root) {
        // 루트 분할
        node *newRoot = createNode(midKey);
        newRoot->child[LEFT] = leftNode;
        newRoot->child[RIGHT] = rightNode;
        newRoot->leafStatus = false;
        root = newRoot;
        return newRoot;
    } else {
        // 부모 노드로 중간 키를 올림 (추가 작업 필요)
        return insertNode(root, midKey);
    }
}

bool duplicatedKey(node *target, int key) {
    if (target == NULL) {
        return false;
    }
    if (searchNode(target, key) == NULL) {
        return false;
    }
    if (searchNode(target, key) != NULL) {
        return true;
    }
    return false;
}

node *insertNode(node *target, int key) {
    if (duplicatedKey(target,key)) {
        printf("duplicated key(%d) found and deleted\n",key);
        return target;
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
            target->child[LEFT] = insertNode(target->child[LEFT], key);
        } else if (target->currentKeyCount == 1 || (key > target->keyValue[0] && key < target->keyValue[1])) {
            target->child[CENTER] = insertNode(target->child[CENTER], key);
        } else {
            target->child[RIGHT] = insertNode(target->child[RIGHT], key);
        }
        return target;
    }
}

void printTree(node *root, int level) {
    if (root == NULL) {
        return;
    }

    printf("Level %d: ", level);
    for (int i = 0; i < root->currentKeyCount; i++) {
        printf("%d ", root->keyValue[i]);
    }
    printf("\n");

    if (!root->leafStatus) {
        for (int i = 0; i <= root->currentKeyCount; i++) {
            printTree(root->child[i], level + 1);
        }
    }
}

void testInsert() {
    printf("Inserting keys into B-Tree:\n");

    // insertNode(root, 30);
    // insertNode(root, 20);
    // insertNode(root, 10);
    // insertNode(root, 15);
    // insertNode(root, 5);
    // insertNode(root, 50);
    // insertNode(root, 50);
    // insertNode(root, 50);
    // insertNode(root, 60);
    // insertNode(root, 10);
    insertNode(root, 10);
    insertNode(root, 10);

    insertNode(root, 20);
    insertNode(root, 30);


    printf("Tree structure after inserts:\n");
    printTree(root, 0);
}

void testSearch(int key) {
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
    testSearch(20); // 존재하는 키
    testSearch(50); // 존재하는 키
    testSearch(70); // 존재하지 않는 키

    return 0;
}
