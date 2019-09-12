= データ・辞書

ここまではスタック上のデータの操作や制御構造について触れてきたが、
この章では「データを保存する手段」に関して解説していく。

== 変数と辞書

辞書は「すでに定義されたワードのデータとコードの置き場所」である。
実際には辞書はメモリ中にロードされており、FORTH インタプリタ・コンパイラに関わるコアなワードの定義も含めて、
そこに書き込まれている。辞書に名前とコードを対にして登録すれば、当然それ以降は名前をワードとして扱える。
他の利用法もある。名前と値を対にして辞書に登録すればその名前は定数として機能するし、
名前とアドレスを対にして辞書に登録すればその名前は変数として機能する。

ここからは、実際に定数や変数、ワードを辞書に書き込ませて辞書の内容の変化を追いながら、
辞書の扱い方や意義について探っていくことにしよう。

//list[dict][][forth]{
[DEFINED]
FIND
HERE UNUSED
, C,
ALLIGN ALLIGNED
//}

== 匿名ワードと XT

//list[anonymous][][forth]{
:NONAME
DEFER
'
ACTION-OF
IS
//}


== DOES>

//list[does][][forth]{
CREATE DOES>
//}

== 練習問題
