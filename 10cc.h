#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TK_RESERVED,  // 記号
    TK_IDENT,     // 識別子
    TK_NUM,       // 整数
    TK_EOF,       // EOF
} TokenKind;

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_EQ,      // ==
    ND_NE,      // !=
    ND_LE,      // <=
    ND_LT,      // <
    ND_ASSIGN,  // =
    ND_LVAR,    // ローカル変数
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;   // left-hand side
    Node *rhs;   // right-hand side
    int val;     // used iff kind is ND_NUM.
    int offset;  // used iff kind is ND_LVAR.
};

typedef struct Token Token;

struct Token {
    TokenKind kind;  // トークンの型
    Token *next;     // 次の入力トークン
    int val;         // kindがTK_NUMの場合、その数値
    char *str;       // トークン文字列
    int len;         // token length
};

Token *tokenize(char *p);
void program();
void gen(Node *node);

void error_at(char *loc, char *fmt, ...);
void error(char *fmt, ...);

extern Token *token;
extern Node *code[];

// 入力プログラム
extern char *user_input;
