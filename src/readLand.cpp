#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Globals.hpp"
#include "GVector.hpp"
#include "GRigid.hpp"
#include "readLand.hpp"
#include "readSub.hpp"


void readLand(char *name)
{
	FILE *fp;
	char str[256];
	int i, cw, n, vn;
	if ((fp = fopen(name, "r")) == NULL) return;
}
