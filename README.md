# DARK FORTH

[![Build Status](https://travis-ci.org/0918nobita/forth-book.svg?branch=master)](https://travis-ci.org/0918nobita/forth-book)

- FORTH 言語の入門書
- GitHub Pages で内容を無料公開
- A5 サイズ無線綴じ製本冊子にして、技術同人誌として販売したい

## 内容

- 基本的なスタック操作ワードの使い方・インタプリタモードでの動作原理
- Interpretation / Compilation / Execution / Runtime / Initiation Semantics の違い
- ``IMMEDIATE`` / ``POSTPONE`` ワードについて
- メモリ操作ワード・変数・配列・固定長文字列について
- Compilation Words を用いたマクロの作成
- データ構造とその実装

(できたらいいかも)

- Forth とデザインパターン
- Forth リアクティブプログラミング

## 環境構築

yarn を事前にインストールしてください。

```bash
yarn install
```

### Web 版のサーバ起動

```bash
yarn serve
```

### PDF 版の出力

```bash
yarn pdf
```

## EPUB 版の出力

```bash
yarn epub
```
