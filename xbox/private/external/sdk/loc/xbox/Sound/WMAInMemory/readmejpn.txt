//-----------------------------------------------------------------------------
// 名前 : WMAInMemory Xbox サンプル
// 
// Copyright (c) 2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


説明
====
WMAInMemory サンプルは、同期 WMA デコーダ XMO を使用して、ファイルではなくメモリから WMA データをデコードする方法を紹介します。タイトルでは、このデコーダが WMA データへのポインタを取得するときに呼び出すコールバック関数を実装します。
これ以降は、AsyncXMO や WMAStream などの WMA ストリーミング サンプルと同様です。


必要なファイルとメディア
========================
このサンプルを実行する前に、メディア ツリーをターゲット マシンにコピーします。


プログラミング上の注意
======================
メモリ内 WMA デコーダは、Process() の呼び出しに同期して動作します。
タイトル側のメイン スレッドではデコードが完了しないと処理が再開されないので、この処理によるタイトルへの影響を考慮する必要があります。AsyncXMO サンプルでは非同期 WMA デコーダを作成しているので、このような配慮は必要ありません。WMAStream サンプルでは、WMA デコーダの Process() を呼び出すスレッドを作成しているので、上記のような配慮は必要ありません。ファイルからの読み込みが行われないので、同期した後にメモリからデコードすることに大きな問題はありませんが、実際にゲームをするときには WMAStream のようにワーカー スレッドで同期させることが最良の方法です。
