#pragma warning(disable : 4996)
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "readSub.hpp"

//���������[�N���o�p
#include <crtdbg.h>  
#ifdef _DEBUG 
#ifdef __MEMLEAKCHECK 
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__) 
#define malloc(p1) _malloc_dbg((p1),_NORMAL_BLOCK,__FILE__,__LINE__) 
#define __MEMLEAKCHECK
#endif
#endif 
//--���������[�N���o�p

extern int DataCheck;

int  getCharData(FILE *fp)
{
	int ch;
	do {
		ch = toupper(getc(fp));
	} while (isspace(ch));
	return ch;
}

int  getStrData(FILE *fp, char *str)
{
	int i = 0, ch;
	ch = getCharData(fp);
	while (isalnum(ch) || ch == '.' || ch == '-' || ch == '=') {
		str[i] = ch;
		i++;
		ch = toupper(getc(fp));
	}
	str[i] = '\0';
	return ch;
}

int TokenCh = '\0';

void cancelSpace(FILE *fp)
{
	while (isspace(TokenCh)) {
		if (TokenCh == '\n') DataCheck++;
		TokenCh = getc(fp);
	}
}
int GetTokenCh(void)
{
	int ch = TokenCh;
	TokenCh = '\0';
	return ch;
}

int  getToken(FILE *fp, char *str)
{
	int k = 0;
	int ret;
restart:
	str[0] = '\0';
	if (TokenCh == '\0') TokenCh = getc(fp);
	cancelSpace(fp);
	if (TokenCh == '/') {	//�R�����g���͊���Z
		ret = TokenCh;
		str[0] = TokenCh;
		str[1] = '\0';
		TokenCh = getc(fp);
		if (TokenCh == '/') {
			do {
				TokenCh = getc(fp);
			} while (TokenCh != '\n');
			DataCheck++;
			TokenCh = '\0';
			goto restart;
		}
		else return ret;
	}
	if (isalpha(TokenCh) || TokenCh == '_') {	//���O
		ret = 1;
		while (isalnum(TokenCh) || TokenCh == '_') {
			if (k < 255)str[k++] = toupper(TokenCh);
			TokenCh = getc(fp);
		}
		str[k] = '\0';
	}
	else if (TokenCh == '#') {	//16�i��
		TokenCh = getc(fp);
		ret = 2;
		while (isdigit(TokenCh) || (toupper(TokenCh) >= 'A' && toupper(TokenCh) <= 'F')) {
			if (k < 255)str[k++] = TokenCh;
			TokenCh = getc(fp);
		}
		str[k] = '\0';
		int v;
		sscanf(str, "%x", &v);
		sprintf(str, "%d", v);
	}
	else if (isdigit(TokenCh) || TokenCh == '.' || TokenCh == '-') {	//���l
		if (k < 255)str[k++] = TokenCh;
		TokenCh = getc(fp);
		ret = 2;
		while (isdigit(TokenCh) || TokenCh == '.') {
			if (k < 255)str[k++] = TokenCh;
			TokenCh = getc(fp);
		}
		str[k] = '\0';
	}
	else if (TokenCh == '"') {	//������
		ret = 3;
		TokenCh = getc(fp);
		while (TokenCh != '"' && TokenCh >= ' ') {
			if (k < 255) {
				str[k++] = TokenCh;
			}
			TokenCh = getc(fp);
		}
		str[k] = '\0';
		TokenCh = getc(fp);
	}
	else {	//�L��
		ret = TokenCh;
		str[0] = TokenCh;
		str[1] = '\0';
		TokenCh = getc(fp);
	}
	return ret;
}
void  resetToken()
{
	TokenCh = '\0';
}
int  getToken2(FILE *fp, char *str)
{
	int k = 0;
	int ret;
restart:
	str[0] = '\0';
	if (TokenCh == '\0') TokenCh = getc(fp);
	if (isspace(TokenCh)) {
		ret = 0;
		str[k++] = ' ';
		TokenCh = getc(fp);
		str[k] = '\0';
	}
	else if (TokenCh == '/') {	//�R�����g���͊���Z
		ret = 0;
		str[0] = TokenCh;
		str[1] = '\0';
		TokenCh = getc(fp);
		if (TokenCh == '/') {
			do {
				TokenCh = getc(fp);
			} while (TokenCh != '\n');
			TokenCh = '\0';
			goto restart;
		}
		else return TokenCh;
	}
	else if (isalpha(TokenCh) || TokenCh == '_') {	//���O
		ret = 1;
		while (isalnum(TokenCh) || TokenCh == '_') {
			if (k < 255)str[k++] = toupper(TokenCh);
			TokenCh = getc(fp);
		}
		str[k] = '\0';
	}
	else if (TokenCh == '#') {	//16�i��
		str[k++] = TokenCh;
		TokenCh = getc(fp);
		ret = 2;
		while (isdigit(TokenCh) || (toupper(TokenCh) >= 'A' && toupper(TokenCh) <= 'F')) {
			if (k < 255)str[k++] = TokenCh;
			TokenCh = getc(fp);
		}
		str[k] = '\0';
	}
	else if (isdigit(TokenCh) || TokenCh == '.' || TokenCh == '-') {	//���l
		if (k < 255)str[k++] = TokenCh;
		TokenCh = getc(fp);
		ret = 2;
		while (isdigit(TokenCh) || TokenCh == '.') {
			if (k < 255)str[k++] = TokenCh;
			TokenCh = getc(fp);
		}
		str[k] = '\0';
	}
	else if (TokenCh == '"') {	//������
		ret = 3;
		str[k++] = TokenCh;
		TokenCh = getc(fp);
		while (TokenCh != '"' && TokenCh >= ' ') {
			if (k < 255) {
				str[k++] = TokenCh;
			}
			TokenCh = getc(fp);
		}
		str[k++] = '"';
		str[k] = '\0';
		TokenCh = getc(fp);
	}
	else {	//�L��
		ret = TokenCh;
		str[0] = TokenCh;
		str[1] = '\0';
		TokenCh = getc(fp);
	}
	return ret;
}
int  getStrData2(FILE *fp, char *str)
{
	int i = 0;
	char ch;
	ch = getCharData(fp);
	while (!isspace(ch) && ch != EOF) {
		if (ch == '"') {
			str[i] = ch;
			i++;
			ch = getc(fp);
			while (ch != '"' && ch != EOF  && ch != '\n') {
				str[i] = ch;
				i++;
				ch = getc(fp);
			}
			str[i++] = '"';
			ch = getc(fp);
		}
		else if (ch == '/') {
			ch = getc(fp);
			if (ch == '/') {
				while (ch != EOF  && ch != '\n') {
					ch = getc(fp);
				}
				ch = getc(fp);
			}
			else {
				str[i++] = '/';
			}
			continue;
		}
		else {
			str[i] = ch;
			i++;
			ch = getc(fp);
		}
	}
	str[i] = '\0';
	return ch;
}

void searchData(FILE *fp, int c)
{
	int ch;
	do {
		ch = getc(fp);
	} while (ch != c && ch != EOF);
}