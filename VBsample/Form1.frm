VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   615
      Left            =   1680
      TabIndex        =   0
      Top             =   1800
      Width           =   1815
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
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
