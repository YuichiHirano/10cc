#include "10cc.h"

// 抽象構文木

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

bool consume(char *op) {
    if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len)) {
        return false;
    }

    token = token->next;
    return true;
}

Token *consume_ident() {
    if (token->kind != TK_IDENT) {
        return NULL;
    }

    Token *tok = token;
    token = token->next;
    return tok;
}

void expect(char *op) {
    if (!consume(op)) {
        error_at(op, "'%c'ではありません", op);
    }
}

int expect_number() {
    if (token->kind != TK_NUM) {
        error_at(token->str, "数ではありません");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();

Node *code[100];

Node *assign() {
    Node *node = equality();
    if (consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

Node *expr() {
    return assign();
}

Node *stmt() {
    Node *node = expr();

    // error("stmt", (int)token->kind);
    // expect(";");
    return node;
}

void program() {
    int i = 0;
    while (!at_eof()) {
        code[i++] = stmt();
        // error("at_eof");
    }
    code[i] = NULL;
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("==")) {
            node = new_node(ND_EQ, node, relational());
            continue;
        }
        if (consume("!=")) {
            node = new_node(ND_NE, node, relational());
            continue;
        }
        return node;
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<")) {
            node = new_node(ND_LT, node, add());
            continue;
        }
        if (consume(">")) {
            node = new_node(ND_LT, add(), node);
            continue;
        }
        if (consume("<=")) {
            node = new_node(ND_LE, node, add());
            continue;
        }
        if (consume(">=")) {
            node = new_node(ND_LE, add(), node);
            continue;
        }
        return node;
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+")) {
            node = new_node(ND_ADD, node, mul());
            continue;
        }
        if (consume("-")) {
            node = new_node(ND_SUB, node, mul());
            continue;
        }
        return node;
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*")) {
            node = new_node(ND_MUL, node, unary());
            continue;
        }
        if (consume("/")) {
            node = new_node(ND_DIV, node, unary());
            continue;
        }
        return node;
    }
}

Node *unary() {
    if (consume("+"))
        return primary();
    if (consume("-"))
        return new_node(ND_SUB, new_node_num(0), primary());
    return primary();
}

Node *primary() {
    // 次のトークンが"("なら、"(" expr ")"のはず
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    // 記号であるか判定
    Token *tok = consume_ident();
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;
        node->offset = (tok->str[0] - 'a' + 1) * 8;
        return node;
    }

    // そうでなければ数値のはず
    return new_node_num(expect_number());
}