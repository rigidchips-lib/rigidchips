#ifndef G_CONSTS
#define G_CONSTS
//�ő�`�b�v��
const int GCHIPMAX = 512;

//�ő�I�u�W�F�N�g��(e.g. ball)
const int GOBJMAX = 20;
//���₹��q�`�b�v�̍ő吔
const int GCHILDMAX = 12;

const float GDESTROY_K = 0.3f;
//���̂̃^�C�v
const int GTYPE_FACE = 0;//����
const int GTYPE_DISK = 1;//�~��
const int GTYPE_BOX = 2;//��
const int GTYPE_BALL = 3;//�{�[��
const int GTYPE_CAN = 4;//�~��
const int GTYPE_CONE = 5;//�~��
const int GTYPE_DUMMY = 6;

const int GMODELMAX = 38;//�ǂݍ���X�t�@�C���i�|���S�����f���j�̍ő吔
const int  GTEXMAX = 23;
//�ݒu�\�`�F�b�N�|�C���g�̍ő吔
const int GCHECKPOINTMAX = 100;

//�ő�Rec�t���[�����H
const int GRECMAX = 30 * 60 * 3;
const int GCOURSEMAX = 10;

//Val�̍ő吔
const int GVALMAX = 256;
//Key�̍ő吔�i�O�|�P�U�j
const int GKEYMAX = 17;
//SKEY�̍ő吔
const int GSYSKEYMAX = 4;
//?
const int GREFMAX = 1024;
//out�̍ő�s���H
const int GOUTPUTMAX = 25;
//������Ring�̍ő吔
const int GRINGMAX = 100;
//CCD�̉𑜓x
const int CCD_RESOLUTION = 64;

//�ő�p�[�e�B�N�����i�\���H���݁H�j
const int GPARTMAX = 10000;
//�~����
const double M_PI = 3.14159265358979323846;

//dt?
const int GDTSTEP = 10;
//�ʒu���Z�ӂ�̃��[�v���H
const int GLOOP = 10;

//�l�b�g���[�N�̑��M�Ԋu[ms]�B���ۂɂ̓R���Ƀ��b�Z�[�W���Ƃ̔䗦�������Ďg��
const int GNETSPAN = 200;
//Arm�̂��܂̑��x
const float ARMSPEED = 20.0f;

//�R������̕ϊ�����(�傫���قǌ������悢)
const double ARM_EFF = 1.0;
const double JET_EFF = 10.0;
const double WHL_EFF = 30.0;

typedef float GFloat;//���u��
//���ʂ̍���
const GFloat WATER_LINE = -0.45f;

const int MESSAGEMAX = 64;
//�ő�ڑ��\�v���C���[��
const int GPLAYERMAX = 30;
//�v���C���[���̍Œ�
const int MAX_PLAYER_NAME = 128;
//�R�A�̍ō����x[m/s]
const float MAX_VELOCITY = 140.0f;//���g��f�Ȃ̂�
//�R�A�ƃR�A�ȊO�̍ō����x�̔䗦�B
const float MAX_VELOCITY_UNCORE_RATIO = 10.0 / 8.0;//

const double GDEFCD = 1.0;//�f�t�H���g�̋�C��R�l

const float CHIPSIZE = 0.6f;//�`�b�v�̃T�C�Y�i���j

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
	NC_UPDATE_PLAYER_DATA= 100,	//I/R/U/Y�̐��Ƃ��𑗂�z
	NC_LAND = 50,//Land�₢���킹
	NC_SCENARIO = 51, //Scenario�₢���킹
	NC_VERSION = 52, //Version�₢���킹
	NC_POSITION = 53, //���W�₢���킹
	NC_NAME = 54,//���O�₢���킹
	NC_ACQUIRE_KOKUTI = 55,//���m�`����
	NC_POST_KOKUTI = 56,//���m�擾
	NC_FPS = 57, //FPS���擾
	NC_SYSTEM = 58, //OS���̑��̏����擾
	NC_CHIP = 59,//�`�b�v�����擾
	NC_PING_1 = 62,
	NC_PING_2 = 63,
	NC_PING_3 = 64,
	NC_PING_4 = 65, //ping�̂����I���A�\������z
	
};
//VAL���̍Œ�
const int VALNAMEMAX = 256;
const GFloat MAX_TORELANCE = 10000.0;//�ϋv�x�̏����l

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
const int MESH_COWL = 23;//�J�E����Mesh�ԍ�
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