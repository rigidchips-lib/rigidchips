#ifndef G_CONSTS
#define G_CONSTS
//最大チップ数
const int GCHIPMAX = 512;

//最大オブジェクト数(e.g. ball)
const int GOBJMAX = 2;
//生やせる子チップの最大数
const int GCHILDMAX = 12;

const float GDESTROY_K = 0.3f;

const int GTYPE_FACE = 0;
const int GTYPE_DISK = 1;
const int GTYPE_BOX = 2;
const int GTYPE_BALL = 3;
const int GTYPE_CAN = 4;
const int GTYPE_CONE = 5;
const int GTYPE_DUMMY = 6;

const int GMODELMAX = 38;
const int  GTEXMAX = 23;
const int GCHECKPOINTMAX = 100;

//最大Recフレーム数？
const int GRECMAX = 30 * 60 * 3;
const int GCOURSEMAX = 10;

//Valの最大数
const int GVALMAX = 256;
//Keyの最大数（０－１６）
const int GKEYMAX = 17;
//SKEYの最大数
const int GSYSKEYMAX = 4;
//?
const int GREFMAX = 1024;
//outの最大行数？
const int GOUTPUTMAX = 25;
//おけるRingの最大数
const int GRINGMAX = 100;
//CCDの解像度
const int CCD_RESOLUTION = 64;

//最大パーティクル数（表示？存在？）
const int GPARTMAX = 10000;
//円周率
const double M_PI = 3.14159265358979323846;

//dt?
const int GDTSTEP = 10;
//位置演算辺りのループ数？
const int GLOOP = 10;

//ネットワークの送信間隔[ms]。実際にはコレにメッセージごとの比率をかけて使う
const int GNETSPAN = 200;
//Armのたまの速度
const float ARMSPEED = 20.0f;

//燃料からの変換効率(大きいほど効率がよい)
const double ARM_EFF = 1.0;
const double JET_EFF = 10.0;
const double WHL_EFF = 30.0;

typedef float GFloat;
const GFloat WATER_LINE = -0.45f;

const int MESSAGEMAX = 64;

const int GPLAYERMAX = 30;
const int MAX_PLAYER_NAME = 128;
#endif