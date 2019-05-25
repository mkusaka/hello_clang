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

// トークナイズした配列
// 100しかトークナイズできないのでそれ以上長いと扱えない
Token tokens[100];

void tokenize(char *user_input) {
  char *p = user_input;

  int i = 0;
  while (*p) {
    // 空白はスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (strncmp(p, "==", 2) == 0) {
      tokens[i].ty = TK_EQ;
      tokens[i].input = p;
      i++;
      p += 2;
    }

    if (strncmp(p, "!=", 2) == 0) {
      tokens[i].ty = TK_NE;
      tokens[i].input = p;
      i++;
      p += 2;
    }

    if (strncmp(p, "<=", 2) == 0) {
      tokens[i].ty = TK_LE;
      tokens[i].input = p;
      i++;
      p += 2;
    }

    if (strncmp(p, ">=", 2) == 0) {
      tokens[i].ty = TK_GE;
      tokens[i].input = p;
      i++;
      p += 2;
    }

    // `+` または `-` であればtokensに格納する
    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>') {
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

// utils
// 型をチェックして、期待した型であればトークンを一つ進めて真を返す

int pos = 0;

int consume(int ty) {
  if (tokens[pos].ty != ty)
    return 0;
  pos++;
  return 1;
}

// prototype宣言
// 関数は使用する前に宣言されている必要がある

Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *term();

// expr = equality と対応
Node *expr() {
  return equality();
}

// 等号
// equality = relational ("==" relational | "!=" relational)* と対応
Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume(TK_EQ))
      node = new_node(TK_EQ, node, relational());
    else if (consume(TK_NE))
      node = new_node(TK_NE, node, relational());
    else
      return node;
  }
}

// 大小関係
// relational = add ("<" add | "<=" add | ">" add | ">=" add)* と対応
Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume(TK_LE))
      node = new_node(TK_LE, node, add());
    else if (consume(TK_GE))
      // genで TK_LE の逆操作を使用するために順序を変更
      node = new_node(TK_GE, add(), node);
    else if (consume('<'))
      node = new_node('<', node, add());
    else if (consume('>'))
      // genで TK_LE の逆操作を使用するために順序を変更
      node = new_node('>', add(), node);
    else
      return node;
  }
}

// 足し算、引き算
// add = mul ("+" mul | "-" mul)* と対応
Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_node('+', node, mul());
    else if (consume('-'))
      node = new_node('-', node, mul());
    else
      return node;
  }
}

// 乗算、除算
// mul = unary ("*" unary | "/" unary)* と対応
Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume('*'))
      node = new_node('*', node, unary());
    else if (consume('/'))
      node = new_node('/', node, unary());
    else
      return node;
  }
}

// 単項演算子
// unary = ("+" | "-")? term と対応
Node *unary() {
  if (consume('+'))
    return term();
  if (consume('-'))
    return new_node('-', new_node_num(0), term());
  return term();
}

// 括弧、数字
// term = num | "(" expr ")" と対応
Node *term() {
  if (consume('(')) {
    Node *node = expr();
    if (!consume(')'))
      error_at(tokens[pos].input, "閉じカッコに対応するカッコがありません");

    return node;
  }

  // カッコでなければ数値
  if (tokens[pos].ty == TK_NUM)
    return new_node_num(tokens[pos++].val);

  error_at(tokens[pos].input, "数値でもカッコでもないトークン");
}

// アセンブリコード生成
void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->ty) {
  case '+':
    printf("  add rax, rdi\n");
    break;
  case '-':
    printf("  sub rax, rdi\n");
    break;
  case '*':
    printf("  imul rdi\n");
    break;
  case '/':
    printf("  cqo\n");
    printf("  idiv rdi\n");
    break;
  case TK_EQ:
    printf("  cmp rax, rdi\n");
    printf("  sete al\n");
    printf("  movzb rax, al\n");
    break;
  case TK_NE:
    printf("  cmp rax, rdi\n");
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  case TK_LE:
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  case TK_GE:
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  case '<':
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case '>':
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  }

  printf("  push rax\n");
}

int main(int argc, char **argv) {
  if (argc !=2) {
    fprintf(stderr, "引数の個数が正しくありません。");
    return 1;
  }

  tokenize(argv[1]);
  Node *node = expr();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}

// gcc -o tim tim.c
