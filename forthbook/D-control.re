= 制御構造

通常「条件分岐」や「繰り返し」はコンパイル状態でのスタックを活用して実現される。
この章では、コンパイル状態で利用できる制御フロー関連の機能に触れていく。

== 条件分岐

条件分岐を作り出すためには @<hidx>{IF}@<code>{IF}, @<hidx>{ELSE}@<code>{ELSE}, @<hidx>{THEN}@<code>{THEN} ワードを用いる。
他のプログラミング言語の具象構文と見比べて語順が異なっている。
この 3 つのワードはコンパイル状態でしか呼び出すことができず、解釈時意味論は未定義である。
その代わり、それぞれ実行時意味論が定義されている。

@<list>{branch}を見てほしい。これは、スタックに積まれた 1 つの数が 2 の倍数の場合に
@<code>{Multiple of 2} と出力するワード @<code>{word3-1} の定義だ。

//list[branch][IF THEN による条件分岐][forth]{
: word3-1  ( n -- )  2 MOD 0 = IF ." Multiple of 2" THEN ;
//}

@<code>{IF}, @<code>{THEN} ワードの組が特別扱いされているわけではなく、
それぞれ独立したワードとして定義されている。
@<code>{IF} ワードは「先にスタックに積まれた 1 つの数を条件として、
それが偽だった場合のジャンプ先を定めないまま、真だった場合のコンパイルを開始する」という役割を持つ。
そして @<code>{THEN} ワードは「未解決だったジャンプ先を解決する」という役割を持つ。
@<list>{branch-test}では、実際に @<code>{word3-1} ワードを解釈させて挙動を確認している。
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
@<hidx>{CASE}@<code>{CASE}, @<hidx>{OF}@<code>{OF}, @<hidx>{ENDOF}@<code>{ENDOF}, @<hidx>{ENDCASE}@<code>{ENDCASE} ワードを用いる。
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

@<code>{CASE} ワードの呼び出しまでにスタックのトップに積まれた値 (以下、「入力値」) が、
@<code>{OF} ワードの前に積んだ値 (以下、「基準値」) と一致した場合には、
その後の @<code>{ENDOF} ワードまでを実行して @<code>{ENDCASE} ワードまでジャンプする。
@<code>{OF}, @<code>{ENDOF} ワードを用いないことで、
それまで挙げたどの基準値にも一致しなかった場合の処理を記述できる。

@<code>{OF} ワードの走行時意味論は「直前に積まれた入力値と基準値のうち、基準値だけをポップすること」であり、
@<code>{ENDCASE} ワードの走行時意味論は「入力値をポップして実行を続けること」である。
そのため、スタック上の入力値を消費する操作をする場合には、入力値を複製しておくことが必要となる。
@<code>{word3-3} ワードでも、@<code>{Other number:} を出力した後に入力値を出力するために、
予め @<code>{DUP} ワードを呼び出している。

== 繰り返し

=== ループ

ここでは回数を指定してループを発生させる方法についてのみ述べる。
確定ループを作り出すには @<hidx>{DO}@<code>{DO}, @<hidx>{LOOP}@<code>{LOOP} ワードを用いる。
@<code>{DO ( n1 n2 -- )} ワードの走行時意味論としては、スタックから 2 数を取り出し、
順番にループの「境界値 @<code>{n1}」「初期値 @<code>{n2}」とする。
ループカウンタの値は @<hidx>{I}@<code>{I} ワードを用いて取得する。
ネストする場合は @<hidx>{J}@<code>{J} ワードを用いて内側のループでのカウンタの値を取得できるが、
そもそも 1 つのワード定義のなかでネストした制御構造をまとめて記述することは推奨しない。
可読性に欠けるからだ。

これから、確定ループを扱う練習としてインクリメンタルに
「スタックに積まれた数の階乗を求める」ワードを定義していく。
まずは「@<code>{1} から @<code>{10} までの数を出力する」
@<code>{1to10} ワードと、「@<code>{0} から @<code>{9} までの数を出力する」
@<code>{0to9} ワードを @<list>{1to10} で定義してみる。

//list[1to10][1 から 10 までの数を出力する][forth]{
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
最初に積んだ @<code>{1} に対して毎回ループカウンタを掛けている。
先にスタックのトップに積まれた数を求めるには、それがここでの @<code>{n} として扱われるようにすればいい。
それを実装したものが @<list>{fact1} の @<code>{fact-n} ワードである。
@<code>{DO} ワードを呼び出す直前に、スタックに「ループカウンタを掛けられる数」
「境界値」「初期値」を順に積んでおく必要があるので、@<code>{SWAP} ワードを用いて順序を直している。

//list[fact1][1 以上の数の階乗を求める][forth]{
: fact-n  ( n1 -- n2 )
  1 SWAP 1 + 1 DO I * LOOP
;
//}

しかし @<code>{fact-n} ワードには欠陥がある。@<code>{0} 以下の数をスタックのトップに積んだ状態で
呼び出すと無限ループが発生する。この問題は、@<code>{DO} ループのカウンタがずっと境界値に一致しないことに起因する。

これを解決するには @<code>{DO} ワードの代わりに @<code>{?DO} ワードを使用する。
@<hidx>{?DO}@<code>{?DO} ワードは基本的には @<code>{DO} ワードと同じ挙動だが、初期値が境界値以下だった場合 1 度も
ループしない、という特徴がある。これを使って、 @<code>{0} 以下の数値の入力に対しては @<code>{1} を
返すように定義すればいい。最終的に、「スタックに積まれた数の階乗を求める」ワード @<code>{fact} の定義は
@<list>{fact2} のようになった。

//list[fact2][階乗を求める][forth]{
: fact  ( n1 -- n2 )
  1 SWAP 1 + 1 ?DO I * LOOP
;
0 fact .
1 fact .
2 fact .
3 fact .
4 fact .
//}

//emlist[実行結果]{
1 1 2 6 24 
//}

=== ループからの脱出

#@# @<code>{LOOP} ワードの代わりに @<code>{+LOOP} ワードを用いると、
#@# ループカウンタのステップ数を動的に設定することができる。
#@# @<code>{+LOOP} ワードが呼び出される前に、ステップ数として扱われる数値をスタックに積んでおけばいい。

#@# //list[loop-step][][forth]{
#@# DO +LOOP
#@# //}

「ワードの呼び出しを終了する」という走行時意味論を持った @<hidx>{EXIT}@<code>{EXIT} ワードを用いると、
どこからでも定義内容の最後までジャンプできる。@<code>{IF} ワードと @<code>{EXIT} ワードを
組み合わせることで、特定の条件を満たさない場合に呼び出しを終了し、満たしていた場合は続行するという
挙動を定義できる。これは可読性を向上させる上で有効な手段であり、一般的には@<hidx>{早期リターン}@<b>{早期リターン}と呼ばれている。
早期リターンを用いた一例を @<list>{early-return} に示す。

//list[early-return][早期リターン][forth]{
: word-for-pos  ( n -- )
  DUP 0 <
  IF
    DROP EXIT
  THEN
  ." positive number: " . CR
;

-2 word-for-pos
-1 word-for-pos
1 word-for-pos
2 word-for-pos
//}

//emlist[実行結果]{
positive number: 1
positive number: 2
//}

@<code>{EXIT} ワードの注意点として、「@<code>{DO}, @<code>{?DO}, @<code>{LOOP} ワードによる
ループからの脱出のために @<code>{EXIT} ワード単体で使用してはいけない」ということが挙げられる。
その理由については 3.4 節「リターンスタック」で述べる。
代替策として @<hidx>{UNLOOP}@<code>{UNLOOP} ワードが用意されており、@<code>{EXIT} ワードを
呼び出す直前に呼び出すことでループから正常に脱出できる。
ループがネストしている場合は、その個数分だけ @<code>{UNLOOP} ワードを呼び出す必要がある。

//list[unloop][UNLOOP ワードの使用例][forth]{
: less-than-4  ( n -- )
  CR 1 + 1
  DO
    I DUP ." Checked: " . CR 4 =
    IF
      ." Equal to or greater than 4" CR UNLOOP EXIT
    THEN
  LOOP
  ." Less than 4" CR
;

2 less-than-4
10 less-than-4
//}

//emlist[実行結果]{
Checked: 1
Checked: 2
Less than 4

Checked: 1
Checked: 2
Checked: 3
Checked: 4
Equal to or greater than 4
//}

@<list>{unloop} で定義している @<code>{less-than-4} ワードでは、
スタックから取り出した数に応じて @<code>{DO} ループを発生させ、
ループカウンタが 4 まで増加するとその時点で @<code>{UNLOOP EXIT} によってループを脱出する。

== 再帰

@<hidx>{RECURSE}@<code>{RECURSE} ワードを用いると、呼び出されている最中に自らの呼び出しを行うようなワードを定義できる。
@<list>{fact-rec} では、@<code>{RECURSE} ワードを用いて先述した階乗関数を再定義している。
当然 @<code>{RECURSE} ワードは @<code>{:} によるワード定義の最中にしか使うことができない。

//list[fact-rec][RECURSE ワードを用いて階乗関数を定義する][forth]{
: fact  ( n1 -- n2 )
  DUP 0 <=
  IF
    DROP 1
  ELSE
    DUP 1 - RECURSE *
  THEN
;
//}



== リターンスタック

今まで「スタックは 1 つだけ」のように説明してきたが、実はもう 1 つある。
@<hidx>{リターンスタック}@<b>{リターンスタック}という、
「ワードの呼び出しが完了した後にジャンプする先」を記録するためのスタックである。
今まで扱ってきたスタックは、今後リターンスタックと区別して@<hidx>{データスタック}@<b>{データスタック}と呼ぶことにする。

実は FORTH では、このリターンスタックの内容を操作することが可能である。
以下ではリターンスタックの操作に関する組み込みワードである @<code>{>R}, @<code>{R>}, @<code>{R@} ワードを紹介しているが、
データスタックのスタック表記に加えて、 @<code>{( R: before -- after )} の形式でリターンスタックのスタック表記も記述している。

 : @<idx>{>R}
    解釈時: 未定義動作@<br>{}実行時: @<code>{( x -- ) ( R: -- x )}@<br>{}データスタックから 1 つ要素をポップし、それをリターンスタックにプッシュする。
 : @<idx>{R>}
    解釈時: 未定義動作@<br>{}実行時: @<code>{( -- x ) ( R: x -- )}@<br>{}リターンスタックから 1 つ要素をポップし、それをデータスタックにプッシュする。
 : @<idx>{R@}
    解釈時: 未定義動作@<br>{}実行時: @<code>{( -- x ) ( R: x -- x )}@<br>{}リターンスタックの一番上の要素をコピーして、データスタックにプッシュする。

リターンスタックは「1 ワード定義内に限って利用可能な、値の仮置場」として使われることを想定して、アクセス可能になっている。
裏を返せば、それ以外の使い方をした場合の挙動は保証されていない。@<list>{bad-ret-manu} では、FORTH における禁忌を犯している。

//list[bad-ret-manu][2 重になったワード呼び出しの順序を変更する][forth]{
: don't-do  ( -- ) ( R: n1 n2 -- n2 n1 )
  R> R> SWAP >R >R
;

: foo  ( -- )  CR ." FOO-BEGIN" don't-do CR ." FOO-END" ;
: bar  ( -- )  CR ." BAR-BEGIN" foo CR ." BAR-END" ;

bar
//}

//emlist[実行結果]{
BAR-BEGIN
FOO-BEGIN
BAR-END
FOO-END
//}

呼び出し順序が変わっていることがわかる。
まず @<code>{bar} ワードの呼び出しが始まった直後に、リターンスタックには
「@<code>{bar} ワードの呼び出し後のジャンプ先アドレス」が積まれる。
その後 @<code>{foo} ワードの呼び出しが始まり、「@<code>{foo} ワードの呼び出し後のジャンプ先アドレス」が積まれる。
同様に @<code>{don't-do} ワードの呼び出し後のジャンプ先アドレスが積まれるわけだが、リターンスタックが操作されて
トップの 2 つの要素が入れ替わる。つまり @<code>{don't-do} ワードの呼び出しが終わると、
もともと @<code>{foo} ワードのために積まれていたアドレスにジャンプしてしまう。
その結果 @<code>{CR ." BAR-END"} が実行され、もともと @<code>{don't-do} ワードのために
積まれていたアドレスにジャンプし、@<code>{CR ." FOO-END"} が実行される。

リターンスタックを「値の仮置場」として安全に扱うには、「1 ワード定義中での @<code>{>R} ワードの使用回数と
@<code>{R>} ワードの使用回数を一致させること」を意識する必要がある。@<list>{2swap} では、この考え方に従って
@<code>{2SWAP} ワードを実装している。

//list[2swap][リターンスタックの安全な使い方]{
: 2swap'  ( n1 n2 n3 n4 -- n3 n4 n1 n2 )
  ROT >R ROT R>
;
//}

3.2 節「繰り返し」で挙げた「@<code>{DO} ループから抜け出すときに
@<code>{EXIT} ワードを単体で使ってはいけない」という問題は、まさにこの節で扱っているリターンスタックに起因する。
Forth Standard での @<code>{DO} ワードの解説@<fn>{do-link}にある通り、
@<code>{DO} ワードは走行時意味論としてリターンスタックに値をひとつプッシュする。
そして @<code>{LOOP} ワードの走行時意味論では、リターンスタックからひとつ値をポップする。
@<code>{DO} ワードがすでに呼び出されていて @<code>{LOOP} ワードがまだ呼び出されていないという状況で
@<code>{EXIT} すると、リターンスタックに (元々は @<code>{DO} ワードがプッシュした) 余分な要素が
残ってしまい、処理系がクラッシュしてしまう。

//footnote[do-link][https://forth-standard.org/standard/core/DO]
