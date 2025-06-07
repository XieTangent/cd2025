#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 128

typedef enum {
    TOKEN_TYPE,
    TOKEN_MAIN,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_ASSIGN,
    TOKEN_EQUAL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_SEMICOLON,
    TOKEN_UNKNOWN,
    TOKEN_COMMENT
} TokenType;

const char* token_type_str(TokenType t) {
    switch (t) {
        case TOKEN_TYPE: return "TYPE";
        case TOKEN_MAIN: return "MAIN";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMENT: return "COMMENT";
        default: return "UNKNOWN";
    }
}

int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_num(char c) {
    return c >= '0' && c <= '9';
}

int is_valid_char(char c) {
    return is_letter(c) || is_num(c) || c == '_';
}

int is_blank(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

TokenType check_reserved(const char* word) {
    if (strcmp(word, "int") == 0) return TOKEN_TYPE;
    if (strcmp(word, "main") == 0) return TOKEN_MAIN;
    if (strcmp(word, "if") == 0) return TOKEN_IF;
    if (strcmp(word, "else") == 0) return TOKEN_ELSE;
    if (strcmp(word, "while") == 0) return TOKEN_WHILE;
    return TOKEN_IDENTIFIER;
}

void process_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("File error");
        return;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (is_blank(ch)) continue;

        // skip line comment
        if (ch == '/') {
            int next = fgetc(file);
            if (next == '/') {
                while ((ch = fgetc(file)) != EOF && ch != '\n');
                continue;
            } else {
                ungetc(next, file);
            }
        }

        if (is_letter(ch) || ch == '_') {
            char word[BUF_SIZE] = {0};
            int idx = 0;
            word[idx++] = ch;
            while ((ch = fgetc(file)) != EOF && is_valid_char(ch)) {
                if (idx < BUF_SIZE - 1)
                    word[idx++] = ch;
            }
            word[idx] = '\0';
            if (ch != EOF) ungetc(ch, file);
            TokenType t = check_reserved(word);
            printf("[%-15s] -> %s\n", token_type_str(t), word);
        } 
        else if (is_num(ch)) {
            char num[BUF_SIZE] = {0};
            int idx = 0;
            num[idx++] = ch;
            while ((ch = fgetc(file)) != EOF && is_num(ch)) {
                if (idx < BUF_SIZE - 1)
                    num[idx++] = ch;
            }
            num[idx] = '\0';
            if (ch != EOF) ungetc(ch, file);
            printf("[%-15s] -> %s\n", token_type_str(TOKEN_NUMBER), num);
        } 
        else {
            TokenType t;
            char sym[3] = {ch, '\0', '\0'};
            switch (ch) {
                case '=':
                    ch = fgetc(file);
                    if (ch == '=') {
                        sym[1] = '=';
                        t = TOKEN_EQUAL;
                    } else {
                        if (ch != EOF) ungetc(ch, file);
                        t = TOKEN_ASSIGN;
                    }
                    break;
                case '+': t = TOKEN_PLUS; break;
                case '-': t = TOKEN_MINUS; break;
                case '(': t = TOKEN_LPAREN; break;
                case ')': t = TOKEN_RPAREN; break;
                case '{': t = TOKEN_LBRACE; break;
                case '}': t = TOKEN_RBRACE; break;
                case ';': t = TOKEN_SEMICOLON; break;
                default: t = TOKEN_UNKNOWN;
            }
            printf("[%-15s] -> %s\n", token_type_str(t), sym);
        }
    }

    fclose(file);
}

int main() {
    const char* target = "hw1/main.c";
    process_file(target);
    return 0;
}
