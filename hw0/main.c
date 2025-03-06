#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data;
    int count;
    struct Node* next;
} Node;

Node* find(Node* head, char ch) {
    while (head) {
        if (head->data == ch) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void append(Node** head, char ch) {
    Node* existingNode = find(*head, ch);
    if (existingNode) {
        existingNode->count++;
    } else {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = ch;
        newNode->count = 1;
        newNode->next = *head;
        *head = newNode;
    }
}

int main() {
    FILE* file = fopen(__FILE__, "r");

    Node* head = NULL;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        append(&head, ch);
    }
    fclose(file);

    Node* temp = head;
    while (temp) {
        printf("%c %d\n", temp->data, temp->count);
        temp = temp->next;
    }

    return 0;
}
