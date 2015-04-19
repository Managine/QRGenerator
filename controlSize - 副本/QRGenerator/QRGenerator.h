
#include "stdafx.h"
#include <string.h>
#include <errno.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

#include "qrencode.h"
//#include "maskGenerate.h"
//#include "readBMP.h"
#include "pictureTreating.h"

extern "C" __declspec(dllexport) extern void __stdcall generate(char* path, char* input, float x, float y, int width, float minpro, float adjust, float sacrificeRate);
