RigidChips
====

## Description

## How to build

VisualStudio2013を使用してビルドを行います．

1. `Rigid8.slnを`開きます．すると以下のダイアログが表示されるのでOKを選択します．
![dev01](https://github.com/rigidchips-lib/rigidchips/blob/master/doc/images/dev01.png)

2. プロジェクトのプロパティページの`構成プロパティ`->`全般`から，`プラットフォームツールセット`を`Visual Studio2013(v120)`に指定します．
![dev02](https://github.com/rigidchips-lib/rigidchips/blob/master/doc/images/dev02.png)

3. 次に`構成プロパティ`->`リンカ`->`詳細設定`の，`安全な例外ハンドラーを含むイメージ`を`いいえ(/SAFESEH:NO)`に指定します．
![dev03](https://github.com/rigidchips-lib/rigidchips/blob/master/doc/images/dev03.png)

4. ビルドを行います．

## How to debug build

デバッグビルドを行うにはHow to buildの内容に加えて以下の設定が必要です．

`プロジェクトのプロパティ`->`VC++プロパティ`の`インクルードディレクトリ`と`ライブラリディレクトリ`をReleaseからDebugにコピーします．



## Contribution
1. Fork it
2. Create your feature branch (git checkout -b my-new-feature)
3. Commit your changes (git commit -am 'Add some feature')
4. Push to the branch (git push origin my-new-feature)
5. Create new Pull Request
