pop rdi ; スタックから値をポップ
pop rax ; スタックから値をポップ
cmp rax, rdi ; 上記ふたつを比較→結果はフラグレジスタへ。
sete al ; フラグレジスタからraxにコピーしたいが、通常どおりは操作できないのでstate命令で取り出す。同じであれば1を、異なる場合は0がセットされる。
        ; alはraxの下位8ビットを指すレジスタ→alに値をセットすると、raxに自動的に更新される。
movzb rax, al ; が、上位56ビットはリセットされないのでリセットされない。されないのでmobzb命令でリセットする
              ; そもそもstateが64ビットすべてをraxに書き込めればよいが、stateが8ビットレジスタしか引数に取れない。
; < はstale, <=はstale、!=はsetneを使う
