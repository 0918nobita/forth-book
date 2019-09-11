= 制御構造

FORTH では、通常「条件分岐」や「繰り返し」はコンパイル状態でのスタックを活用して実現される。
この章では、コンパイル状態で利用できる制御フロー関連の機能に触れていく。

== 条件分岐

条件分岐を作り出すためには @<code>{IF}, @<code>{ELSE}, @<code>{THEN} ワードを用いる。
他のプログラミング言語の具象構文と見比べて語順が異なっている。
この 3 つのワードはコンパイル状態でしか呼び出すことができず、解釈時意味論は未定義である。
その代わり、それぞれ実行時意味論が定義されている。

@<list>{branch}を見てほしい。これは、スタックに積まれた 1 つの数が 2 の倍数の場合に @<code>{Multiple of 2} と
出力するワード @<code>{word3-1} の定義だ。

//list[branch][IF THEN による条件分岐][forth]{
: word3-1  ( n -- )  2 MOD 0 = IF ." Multiple of 2" THEN ;
//}

@<code>{IF}, @<code>{THEN} ワードの組が特別扱いされているわけではなく、それぞれ独立したワードとして定義されている。
@<code>{IF} ワードは「先にスタックに積まれた 1 つの数を条件として、それが偽だった場合のジャンプ先を定めないまま、
真だった場合のコンパイルを開始する」という役割を持つ。そして @<code>{THEN} ワードは「未解決だったジャンプ先を解決する」
という役割を持つ。@<list>{branch-test}では、実際に @<code>{word3-1} ワードを解釈させて挙動を確認している。
1 行目を解釈させても何も主力されないが、2 行目を解釈させると @<code>{Multiple of 2} と出力されたはずだ。

//list[branch-test][word3-1 の動作確認][forth]{
3 word3-1
4 word3-1
//}

@<code>{IF}, @<code>{THEN} ワードは、「コンパイル状態で呼び出されるとコンパイラに働きかけて、
実際に定義されるワードの挙動に特別な影響を与える」ワードである。
このようなワードによって実現される意味論を@<hidx>{走行時意味論}@<b>{走行時意味論} (runtime semantics) という。

@<code>{ELSE} ワードは「未解決だった『条件が偽の場合のジャンプ先』を解決し、
『条件が真の状態で @<code>{ELSE} ワードに達したときのジャンプ先』を未解決にしたままコンパイルを続行する」という役割を持つ。
@<list>{branch2} で定義している @<code>{word3-2} ワードは、スタック上の 2 数のうち小さい方をスタックに残して、
もう一方をスタックから取り除く。

//list[branch2][ELSE を用いた条件分岐][forth]{
: word3-2  ( n1 n2 -- n3 )  2DUP < IF DROP ELSE NIP THEN ;
7 10 word3-2 .
10 7 word3-2 .
//}

スタック上の値の違いに応じた多方向への分岐を実現するには、
@<code>{CASE}, @<code>{OF}, @<code>{ENDOF}, @<code>{ENDCASE} ワードを用いる。
@<list>{branch3} で定義している @<code>{word3-3} ワードでは、先にスタックに積まれていた数が
@<code>{1}, @<code>{2}, @<code>{3} の場合に、それぞれ異なる文字列を出力する。
それ以外の値の場合には、@<code>{Other number:[数値]} の形式で出力する。

//list[branch3][CASE による多方向への分岐][forth]{
  : word3-3  ( n1 n2 -- n3 )
    CASE
      1 OF ." It is one." ENDOF
      2 OF ." It is two." ENDOF
      3 OF ." It is three." ENDOF
      ." Other number: " DUP .
    ENDCASE
  ;
  1 word3-3
  2 word3-3
  3 word3-3
  4 word3-3
//}

@<code>{CASE} ワードまでにスタックの一番上に積まれた値 (以下、「入力値」) が、@<code>{OF} ワードの前に積んだ値 (以下、「基準値」) と一致した場合には、
その後の @<code>{ENDOF} ワードまでを実行して @<code>{ENDCASE} ワードまでジャンプする。
@<code>{OF}, @<code>{ENDOF} ワードを用いないことで、それまで挙げたどの基準値にも一致しなかった場合の処理を記述できる。

@<code>{OF} ワードの走行時意味論は「直前に積まれた入力値と基準値のうち、基準値だけをポップすること」であり、
@<code>{ENDCASE} ワードの走行時意味論は「入力値をポップして実行を続けること」である。
そのため、スタック上の入力値を消費する操作をする場合には入力値を複製しておくことが必要となる。
@<code>{word3-3} ワードでも、@<code>{Other number:} を出力した後に入力値を出力するために、
予め @<code>{DUP} ワードを呼び出している。

== 繰り返し

=== 確定ループと脱出

回数を指定してループを発生させる場合は、@<code>{DO}, @<code>{LOOP} ワードを用いる。
@<code>{DO ( n1 n2 -- )} ワードの走行時意味論としては、スタックから 2 数を取り出し、
順番にループの「境界値 @<code>{n1}」「初期値 @<code>{n2}」とする。
ループカウンタの値は @<code>{I} ワードを用いて取得する。
ネストする場合は @<code>{J} ワードを用いて内側のループでのカウンタの値を取得できるが、
そもそも一つのワード定義のなかでネストした制御構造をまとめて記述することは推奨しない。
可読性に欠けるからだ。

これから、確定ループを扱う練習としてインクリメンタルに
「スタックに積まれた数の階乗を求める」ワードを定義していく。
まずは「@<code>{1} から @<code>{10} までの数を出力する」
@<code>{1to10} ワードと、「@<code>{0} から @<code>{9} までの数を出力する」
@<code>{0to9} ワードを @<list>{1to10} で定義してみる。

//list[1to10][1から10までの数を出力する][forth]{
: 1to10  ( -- )
  11 1 DO I . LOOP
;

: 0to9 ( -- )
  10 0 DO I . LOOP
;
//}

@<code>{LOOP} ワードに達してループカウンタを更新したときに、
境界値と初期値が一致していればループは終了する。
そのため @<code>{10} 回確定でループさせたい場合は
「境界値 @<code>{-} 初期値 @<code>{= 10}」となるように境界値と初期値を設定すればいい。

次は @<code>{1}, @<code>{2}, @<code>{3} の階乗を求める @<code>{fact-1}, @<code>{fact-2}, 
@<code>{fact-3} ワードを定義する。

//list[fact][1, 2, 3 の階乗を求める][forth]{
: fact-1  ( -- n )
  1 2 1 DO I * LOOP
;
: fact-2  ( -- n )
  1 3 1 DO I * LOOP
;
: fact-3  ( -- n )
  1 4 1 DO I * LOOP
;
fact-1 .
fact-2 .
fact-3 .
//}

//emlist[実行結果]{
1 2 6
//}

@<code>{n} の階乗を求めるために、@<code>{1} から @<code>{n + 1} までの確定ループのなかで、
最初に積んだ @<code>{1} に対して毎回ループカウンタを掛けていっている。
先にスタックのトップに積まれた数を求めるには、それがここでの @<code>{n} として扱われるようにすればいい。

//list[fact1][][forth]{
: fact-n  ( n1 -- n2 )
  1 SWAP 1 + 1 DO I * LOOP
;
//}

//list[fact2][][forth]{
: fact  ( n1 -- n2 )
  1 SWAP 1 + 1 ?DO I * LOOP
;
//}

@<code>{LOOP} ワードの代わりに @<code>{+LOOP} ワードを用いると、
ループカウンタのステップ数を動的に設定することができる。
@<code>{+LOOP} ワードが呼び出される前に、ステップ数として扱われる数値をスタックに積んでおけばいい。

//list[loop-step][][forth]{
DO +LOOP
//}

「ワードの呼び出しを終了する」という走行時意味論を持った @<code>{EXIT} ワードを用いると、
どこからでも定義内容の最後までジャンプすることができる。

=== 不定ループ

条件に応じてループ発生させる場合は、いくつかの用法が存在している。

==== BEGIN UNTIL 用法

//list[begin-until][][forth]{
BEGIN UNTIL
//}

==== BEGIN WHILE REPEAT 用法

//list[begin-while-repeat][][forth]{
BEGIN WHILE REPEAT
//}

==== BEGIN AGAIN 用法

//list[begin-again][][forth]{
BEGIN AGAIN
//}

//list[loop][][forth]{
LEAVE UNLOOP
//}

== 再帰

//list[rec][][forth]{
RECURSE
//}

== リターンスタック

今まで「スタックは 1 つだけ」のように説明してきたが、実はもう 1 つある。
@<hidx>{リターンスタック}@<b>{リターンスタック}という、
「ワードの呼び出しが完了した後にジャンプする先」を記録するためのスタックである。
今まで扱ってきたスタックは、今後リターンスタックと区別して@<hidx>{データスタック}@<b>{データスタック}と呼ぶことにする。

実は FORTH ではこのリターンスタックの内容を操作することが可能である。
以下ではリターンスタックの操作に関する組み込みワードである @<code>{>R}, @<code>{R>}, @<code>{R@} ワードを紹介しているが、
データスタックのスタック表記に加えて、 @<code>{( R: before -- after )} の形式でリターンスタックのスタック表記も記述している。

 : @<idx>{>R}
    解釈時: 未定義動作@<br>{}実行時: @<code>{( x -- ) ( R: -- x )}@<br>{}データスタックから 1 つ要素をポップし、それをリターンスタックにプッシュする。
 : @<idx>{R>}
    解釈時: 未定義動作@<br>{}実行時: @<code>{( -- x ) ( R: x -- )}@<br>{}リターンスタックから 1 つ要素をポップし、それをデータスタックにプッシュする。
 : @<idx>{R@}
    解釈時: 未定義動作@<br>{}実行時: @<code>{( -- x ) ( R: x -- x )}@<br>{}リターンスタックの一番上の要素をコピーして、データスタックにプッシュする。

== 練習問題
