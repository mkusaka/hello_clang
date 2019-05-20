#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *user_input; // 入力プログラム

// error関数群
void error(char *fmt, ...)
{
  // 可変長引数のリスト http://kwski.net/cplusplus/209/
  va_list ap;
  // 可変長引数の初期化 http://www.c-tipsref.com/reference/stdarg/va_start.html
  va_start(ap, fmt);
  // 可変長のリストデータを標準出力に出す http://www.c-tipsref.com/reference/stdio/vfprintf.html
  vfprintf(stderr, fmt, ap);
  // 第2引数に渡されるフォーマットにしたがって標準出力する http://www.c-tipsref.com/reference/stdio/fprintf.html
  fprintf(stderr, "\n");
  exit(1);
}

// エラー報告のための関数
void error_at(char *loc, char *msg)
{
  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, ""); // pos個の空白表示
  fprintf(stderr, "^ %s\n", msg);
  exit(1);
}

// token
enum {
  TK_NUM = 256, // 整数トークン
  TK_EOF,       // 入力の終了トークン
};

typedef struct {
  int ty;       // 型
  int val;      // 整数トークンの場合の値
} Token;

// トークナイズした配列
// 100しかトークナイズできないのでそれ以上長いと扱えない
Token tokens[100];

void tokenize() {
  char *p = user_input;

  int i = 0;
  while (*p) {
    // 空白はスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    // `+` または `-` であればtokensに格納する
    if (*p == '+' || *p == '-') {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }

    error_at(p, "トークナイズできません");
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

// node
enum {
  ND_NUM = 256, // 整数ノードの型
};

typedef struct Node {
  int ty;             // 演算子かND_NUM
  struct Node *lhs;   // 左辺
  struct Node *rhs;   // 右辺
  int val;
} Node;

// nodeを新規作成する
Node *new_node(int ty, Node *lhs,Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

// numのnodeを新規作成する
Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

int main(int argc, char **argv) {
  if (argc !=2) {
    fprintf(stderr, "引数の個数が正しくありません。");
    return 1;
  }

  user_input = argv[1];
  tokenize();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  if (tokens[0].ty != TK_NUM)
    error_at(tokens[0].input, "数ではありません");
  printf("  mov rax, %d\n", tokens[0].val);

  int i = 1;

  while (tokens[i].ty != TK_EOF) {
    if (tokens[i].ty == '+') {
      i++;
      if (tokens[i].ty != TK_NUM)
        error_at(tokens[i].input, "数ではありません");
      printf("  add rax, %d\n", tokens[i].val);
      i++;
      continue;
    }

    if (tokens[i].ty == '-') {
      i++;
      if (tokens[i].ty != TK_NUM)
        error_at(tokens[i].input, "数ではありません");
      printf("  sub rax, %d\n", tokens[i].val);
      i++;
      continue;
    }

  }

  printf("  ret\n");
  return 0;
}

// gcc -o tim tim.c
