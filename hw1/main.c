#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100

typedef struct TokenNode {
    char tokenStr[MAX_TOKEN_LENGTH];
    int count;
    struct TokenNode* next;
} TokenNode;

TokenNode* find_token(TokenNode* head, const char* tokenStr) {
    while (head) {
        if (strcmp(head->tokenStr, tokenStr) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void add_token(TokenNode** head, const char* tokenStr) {
    TokenNode* node = find_token(*head, tokenStr);
    if (node) {
        node->count++;
    } else {
        TokenNode* newNode = (TokenNode*)malloc(sizeof(TokenNode));
        strncpy(newNode->tokenStr, tokenStr, MAX_TOKEN_LENGTH);
        newNode->count = 1;
        newNode->next = *head;
        *head = newNode;
    }
}

int is_keyword(const char* str) {
    const char* keywords[] = {"int", "main", "if", "else", "while", NULL};
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

void scan(const char* filename, TokenNode** tokenList) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open %s\n", filename);
        return;
    }

    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (isspace(ch)) continue;

        if (isalpha(ch) || ch == '_') {
            char buffer[MAX_TOKEN_LENGTH] = {0};
            int i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
                if (i < MAX_TOKEN_LENGTH - 1) buffer[i++] = ch;
            }
            if (ch != EOF) ungetc(ch, fp);
            buffer[i] = '\0';

            if (is_keyword(buffer)) {
                char kwToken[120];
                snprintf(kwToken, sizeof(kwToken), "KEYWORD(%s)", buffer);
                add_token(tokenList, kwToken);
            } else {
                char idToken[120];
                snprintf(idToken, sizeof(idToken), "IDENTIFIER(%s)", buffer);
                add_token(tokenList, idToken);
            }
        } else if (isdigit(ch)) {
            char buffer[MAX_TOKEN_LENGTH] = {0};
            int i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != EOF && isdigit(ch)) {
                if (i < MAX_TOKEN_LENGTH - 1) buffer[i++] = ch;
            }
            if (ch != EOF) ungetc(ch, fp);
            buffer[i] = '\0';

            char litToken[120];
            snprintf(litToken, sizeof(litToken), "LITERAL(%s)", buffer);
            add_token(tokenList, litToken);
        } else {
            char symToken[3] = {ch, 0, 0};
            if (ch == '=') {
                int next = fgetc(fp);
                if (next == '=') {
                    strcpy(symToken, "==");
                } else {
                    if (next != EOF) ungetc(next, fp);
                }
            }
            char finalToken[120];
            snprintf(finalToken, sizeof(finalToken), "SYMBOL(%s)", symToken);
            add_token(tokenList, finalToken);
        }
    }

    fclose(fp);
}

void print_tokens(TokenNode* head) {
    TokenNode* curr = head;
    while (curr) {
        printf("%s : %d\n", curr->tokenStr, curr->count);
        curr = curr->next;
    }
}

void free_tokens(TokenNode* head) {
    while (head) {
        TokenNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    TokenNode* tokenList = NULL;

    const char* filename = "hw1/test.c";
    scan(filename, &tokenList);

    print_tokens(tokenList);

    free_tokens(tokenList);
    return 0;
}
