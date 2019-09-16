= コンパイラ変形

ここまで扱ってきた内容を総合すると以下のようになる。

 * ワードの意味論は処理系の状態に依存して切り替わる
 * 制御構造を作り出すワードがコンパイル状態で利用できる
 * メモリへのデータ書き込み・読み出しのための手段が用意されている

@<code>{:} や @<code>{;} 、@<code>{DOES>} ワードを除くと原則「処理系の状態に直接影響を与えることのない」機能である。
処理系がコンパイラ・インタプリタのどちらとして動作しているかに従って、
事前に定義されたワードの定義内容を実行する、ということに変わりはない。

コンパイラ「変形」と題した本章では、コンパイル状態の処理系に対して直接的に影響を与えるいくつかの手法について述べる。
ここで述べられる内容はどれもアドホックなコンパイルをするための断片的な機能だが、それぞれの特性を知り上手く組み合わせることで、
いわゆるメタプログラミングが可能になる。

== メタプログラミングとは

ロジックを直接記述するのではなく、あるパターンを持ったロジックを生成する高位ロジックを用いて実装していく手法である。
FORTH においては「コンパイル状態にある処理系に対して影響を与えるワードを用いて、パターンに従ったワードを定義させる手法」と対応する。

== IMMEDIATE / POSTPONE

//list[ip][][forth]{
IMMEDIATE POSTPONE
//}

== EVALUATE とパーサ

文字列をそのままソースコードとして解釈実行できてしまうのが @<code>{EVALUATE} ワードだ。

//list[eval][][forth]{
S\" .\" evaluated\"" EVALUATE
//}

//emlist[実行結果]{
evaluated
//}

//list[parser][][forth]{
PARSE WORD
//}

== コンパイル中断

//list[interrupt][][forth]{
[ ] LITERAL SLITERAL STATE
//}
