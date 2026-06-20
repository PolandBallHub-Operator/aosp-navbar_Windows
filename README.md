# Android NavBar for Windows
<p align="center">
  <img src="https://raw.githubusercontent.com/PolandBallHub-Operator/aosp-navbar_Windows/refs/heads/main/icon.png" alt="WBM2" width="500">
</p>
Windowsデスクトップ上でAndroidスタイルの操作感を提供するネイティブEXEツールです。

## 機能
- **ステータスバー**: 画面上部に時計とWiFi情報を表示（左揃え）。
- **ナビゲーションバー**: 画面下部に「戻る」「ホーム」「タスク」ボタンを表示（最前面）。
- **ホームボタン**: クリックですべてのウィンドウを最小化し、デスクトップを表示します。
- **戻るボタン**: アクティブなアプリに対して戻る操作（Alt+Left/Esc）を送信します。
- **タスクボタン**: Windowsのタスクビューを表示します。
- **自動最大化**: ツール動作中、アクティブなウィンドウを自動的に最大化し、Androidアプリのようなフルスクリーン体験を提供します。
- **設定UI**: ボタンの位置、バーの高さ、アイコンスタイル（Fill/Outline）をWinGUIから設定可能。
- **長押しアクション**: ナビゲーションボタンの長押しで設定画面を開きます。

## ビルド環境
- Mingw-w64 (x86_64-w64-mingw32-g++)
- DLL依存なしのスタティックリンクビルド

## 使い方
1. `android_navbar.exe` を実行します。
2. 画面上下にバーが表示されます。
3. いずれかのナビゲーションボタンを長押しすると設定画面が開きます。
