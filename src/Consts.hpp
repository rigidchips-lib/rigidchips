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

const float MAX_VELOCITY = 140.0f;//モトがfなので
const float MAX_VELOCITY_UNCORE_RATIO = 10.0 / 8.0;//

const double GDEFCD = 1.0;//デフォルトの空気抵抗値

const float CHIPSIZE = 0.6f;//チップのサイズ（ｍ）

enum ChipType
{
GT_CORE,
GT_CHIP,	
GT_RUDDER,	
GT_DUMMY,	
GT_WHEEL,	
GT_RLW,		
GT_TRIM,		
GT_JET,	
GT_CHIPH,//ChipH=Weight
GT_COWL,		
GT_ARM,	
GT_BALLOBJ=31,
GT_CHIP2=33,//Frame
GT_RUDDER2,
GT_TRIM2,	
GT_WHEEL2,	
GT_RLW2,		
};
enum NetworkCode
{
	NC_ERR = -1,
	NC_MODEL = 0,
	NC_CHAT = 1,
	NC_B20_BULLET = 11,
	NC_B20_EXPLOSION = 12,	
	NC_CORE_ONLY = 20,
	NC_UNKNOWN_BULLET = 21,
	NC_UNKNOWN_EXPLOSION = 22,
	NC_SCENARIO_MESSAGE = 30,
	NC_B26_BULLET = 31,
	NC_B26_EXPLOSION = 32,
	NC_UPDATE_PLAYER_DATA= 100,	
	NC_LAND = 50,//Land問い合わせ
	NC_SCENARIO = 51, //Scenario問い合わせ
	NC_VERSION = 52, //Version問い合わせ
	NC_POSITION = 53, //座標問い合わせ
	NC_NAME = 54,//名前問い合わせ
	NC_POST_KOKUTI = 55,//告知伝える
	NC_ACQUIRE_KOKUTI = 56,//告知取得
	NC_FPS = 57, //FPSを取得
	NC_SYSTEM = 58, //OSその他の情報を取得
	NC_CHIP = 59,//チップ数を取得
	NC_PING_1 = 62,
	NC_PING_2 = 63,
	NC_PING_3 = 64,
	NC_PING_4 = 65, //pingのやり取り終了、表示する奴
	
};
const int VALNAMEMAX = 256;
const GFloat MAX_TORELANCE = 10000.0;//耐久度の初期値

const int MESH_CORE = 0;
const int MESH_CHIP = 1;
const int MESH_WHEEL = 2;
const int MESH_RLW = 3;
const int MESH_RUDDER = 4;
const int MESH_TRIM = 5;
const int MESH_DUMMY = 6;
const int MESH_FRAME = 7;
const int MESH_WHEEL2 = 8;
const int MESH_RLW2 = 9;
const int MESH_JET = 10;
const int MESH_FIRE = 11;
const int MESH_COMPUS = 12;
const int MESH_COMPUS2 = 13;
const int MESH_WATER = 14;

const int MESH_CHECK_POINT = 15;
const int MESH_RUDDER_F = 16;
const int MESH_TRIM_F = 17;
const int MESH_DUST = 18;
const int MESH_SPLASH = 19;
const int MESH_BALL = 20;
const int MESH_WEIGHT = 21;
const int MESH_WHEEL_T = 22;
const int MESH_COWL = 23;//カウルのMesh番号
const int MESH_COWL_1 = 24;
const int MESH_COWL_2= 25;
const int MESH_COWL_3 = 26;
const int MESH_COWL_4 = 27;
const int MESH_COWL_5 = 28;
const int MESH_JET_EFFECT = 29;
const int MESH_ARM = 30;
const int MESH_FIRE_2 = 31;
const int MESH_BULLET = 32;
const int MESH_JET_2 = 33;
const int MESH_USER_NO_ARM = 34;
const int MESH_EXPLOSION = 35;
const int MESH_EXPLOSION_2 = 36;
const int MESH_USER_ARM = 37;
#endif