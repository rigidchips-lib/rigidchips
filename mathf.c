#include "mathf.h"



#define ASMCALL __declspec(naked) __fastcall


int ASMCALL FloatToInt(float f)
{
	__asm {
		FLD dword ptr [esp+4]   // push x
		FRNDINT
		FISTP dword ptr [esp-4]
		MOV eax, dword ptr [esp-4]
		RET 4
	}
}

float ASMCALL _fm_sinf(float x)
{
	__asm {
		FLD dword ptr [esp+4]   // push x
		FSIN                    // sin x
		RET 4
	}
}

float ASMCALL _fm_cosf(float x)
{
	__asm {
		FLD dword ptr [esp+4]   // push x
		FCOS                    // cos x
		RET 4
	}
}

float ASMCALL _fm_tanf(float x)
{
	__asm {
		FLD dword ptr [esp+4]   // push x
		FPTAN                   // tan x
		FSTP st(0)              // pop 1.0
		RET 4
	}
}

float __fastcall _fm_asinf(float x)
{
	return atan2f(x, sqrtf(1.0f - x*x));
}

float __fastcall _fm_acosf(float x)
{
	return atan2f(sqrtf(1.0f - x*x), x);
}

float __fastcall _fm_atanf(float x)
{
	return atan2f(x, 1.0f);
}

float ASMCALL _fm_atan2f(float y, float x)
{
	__asm {
		FLD dword ptr [esp+8]   // push y
		FLD dword ptr [esp+4]   // push x
		FPATAN                  // atan2
		RET 8
	}
}

float ASMCALL _fm_sqrtf(float x)
{
	__asm {
		FLD dword ptr [esp+4]   // push x
		FSQRT                   // sqrt x
		RET 4
	}
}

float ASMCALL _fm_expf(float x)
{
	__asm {
		FLD dword ptr [esp+4]   // push x
		FLDL2E                  // push log2(e)
		FMUL
		FLD st(0)
		FRNDINT
		FSUB st(1), st
		FXCH st(1)
		F2XM1
		FLD1
		FADD
		FSCALE
		FSTP st(1)
		RET 4
	}
}

float __declspec(naked) __stdcall _fm_ldexpf(float x, int p)
{
	__asm {
		FILD dword ptr [esp+8]  // push p
		FLD dword ptr [esp+4]   // push x
		FSCALE                  // ST = ST * 2**ST(1)
		FSTP st(1)              // ST -> ST(1), pop ST
		RET 8
	}
}

float ASMCALL _fm_logf(float x)
{
	__asm {
		FLD dword ptr [esp+4]   // push x
		FLDLN2                  // push log(2)
		FXCH                    // swap st(0), st(1)
		FYL2X                   // log x
		RET 4
	}
}

float ASMCALL _fm_log10f(float x)
{
	__asm {
		FLD dword ptr [esp+4]   // push x
		FLDLG2                  // push log10(2)
		FXCH                    // swap st(0), st(1)
		FYL2X                   // log x
		RET 4
	}
}

float ASMCALL _fm_fabsf(float x)
{
	__asm {
		FLD dword ptr [esp+4]   // push x
		FABS
		RET 4
	}
}

float ASMCALL _fnorm2nf(float x, float y)
{
	__asm {
		FLD dword ptr [esp+8]   // push x
		FMUL st(0),st(0)        // st0 = x*x
		FLD dword ptr [esp+4]   // push y
		FMUL st(0),st(0)        // st0 = y*y
		FADDP                   // st0 = x*x + y*y
		RET 8
	}
}

float ASMCALL _fnorm3nf(float x, float y, float z)
{
	__asm {
		FLD dword ptr [esp+12]  // push x
		FMUL st(0),st(0)        // st0 = x*x
		FLD dword ptr [esp+8]   // push y
		FMUL st(0),st(0)        // st0 = y*y
		FADDP                   // st0 = y*y + z+z
		FLD dword ptr [esp+4]   // push z
		FMUL st(0),st(0)        // st0 = z*z
		FADDP                   // st0 = x*x + y*y + z*z
		RET 12
	}
}

float ASMCALL _fabs2nf(float x, float y)
{
	__asm {
		FLD dword ptr [esp+8]   // push x
		FMUL st(0),st(0)        // st0 = x*x
		FLD dword ptr [esp+4]   // push y
		FMUL st(0),st(0)        // st0 = y*y
		FADDP
		FSQRT                   // sqrt x*x + y*y
		RET 8
	}
}

float ASMCALL _fabs3nf(float x, float y, float z)
{
	__asm {
		FLD dword ptr [esp+12]  // push x
		FMUL st(0),st(0)        // st0 = x*x
		FLD dword ptr [esp+8]   // push y
		FMUL st(0),st(0)        // st0 = y*y
		FADDP
		FLD dword ptr [esp+4]   // push z
		FMUL st(0),st(0)        // st0 = z*z
		FADDP
		FSQRT                   // sqrt x*x + y*y + z*z
		RET 12
	}
}


#pragma option pop



