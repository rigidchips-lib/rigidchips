RigidChipsに別アプリケーションからある特定のデータファイルを開かせたい場合

①RigidChips15B19以降を起動しておきます（あとから起動でも可）

②WinAPIのRegisterWindowMessageを使って
　ユーザーメッセージ"WM_RIGHTCHIP_LOAD"を登録します
　これはRigidChipsにも登録してあるので、RigidChipsはこのメッセージを認識します
　
③RegisterWindowMessageから得られるユーザーメッセージIDと
　別に定めたメッセージ種別定数を使ってロードさせたいファイル名を送信します
　ファイル名はASCIIまたはシフトJISに限ります(Unicodeは駄目)
　送信先はHWND_BROADCASTにしておきます
　
④送信が終わるとRigidChipsは指定したデータファイルをロードします
　エラーがある場合にはダイアログが表示され、ロードされません
　'The file doesn't open it.'が出る場合は
　送られたファイル名のファイルが存在しないということです

---------------------以下VB6サンプル(Form + Button(Command1) の構成)

'RigidChipsにデータファイルを開くメッセージを送る(VB6サンプル)

'WinAPI登録
Private Declare Function RegisterWindowMessageA Lib "user32" (ByVal msgid As String) As Long
Private Declare Function PostMessageA Lib "user32" (ByVal hwd As Long, ByVal msg As Long, ByVal wpara As Long, ByVal lpara As Long) As Long
Const HWND_BROADCAST = -1 '0xFFFF

'メッセージ種別定数
Const UMSG_RCLOAD_START = 0   '文字列送信開始
Const UMSG_RCLOAD_CHAR = 1    '一文字ずつ送る
Const UMSG_RCLOAD_END = 2     '文字列送信終了

Dim WmRCLoad As Long

Private Sub Form_Load()
' 共用ウィンドウメッセージID(WM_RIGHTCHIP_LOAD)登録
  WmRCLoad = RegisterWindowMessageA("WM_RIGHTCHIP_LOAD")

End Sub

Private Sub Command1_Click()
    
    '送りたい文字列（パス付きファイル名）注:VBのStringはUnicode
    filenameW = "C:\Rigid15B19\Data\BigFoot.txt"
    'UnicodeからシフトJISへの変換
    filenameA = StrConv(filenameW, vbFromUnicode)
  
    'ウィンドウメッセージ送信
    '文字列送信開始
    PostMessageA HWND_BROADCAST, WmRCLoad, UMSG_RCLOAD_START, 0
    '一文字ずつ送る
    For i = 1 To LenB(filenameA)
      PostMessageA HWND_BROADCAST, WmRCLoad, UMSG_RCLOAD_CHAR, AscB(MidB(filenameA, i, 1))
    Next
    '文字列送信終了
    PostMessageA HWND_BROADCAST, WmRCLoad, UMSG_RCLOAD_END, 0

End Sub

