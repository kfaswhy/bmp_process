#include <iostream>
#include <windows.h> 

typedef struct
{
	BYTE b;
	BYTE g;
	BYTE r;
}RGB;

typedef struct
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
}RGBA;

RGBA imgOpr32(RGBA a)
{
	RGBA b;
	b.b = 255 - a.b;
	b.g = 255 - a.g;
	b.r = 255 - a.r;
	b.a = a.a;
	return b;
}

RGB imgOpr24(RGB a)
{
	RGB b;
	b.b = 255 - a.b;
	b.g = 255 - a.g;
	b.r = 255 - a.r;
	return b;
}