#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// token
enum {
  TK_NUM = 256, // 整数トークン
  TK_EOF,       // 入力の終了トークン
  TK_EQ,        // 比較演算子等しい
  TK_NE,        // 比較演算子等しくない
  TK_LE,        // 比較演算子小さい
  TK_GE         // 比較演算子大きい
};

typedef struct {
  int ty;       // 型
  int val;      // 整数トークンの場合の値
  char *input;
} Token;

typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

extern Vector *new_vector();
extern void vec_push(Vector *vec, void *elem);

// node definition
typedef struct Node {
  int ty;             // 演算子かND_NUM
  struct Node *lhs;   // 左辺
  struct Node *rhs;   // 右辺
  int val;
} Node;

// node
enum {
  ND_NUM = 256, // 整数ノードの型
};

// トークナイズした配列
// 可変長配列として定義されている
extern Vector *tokens;

void error_at(char *loc, char *msg);
void runtest();
void tokenize(char *user_input);
void gen(Node *node);
Node *expr();
