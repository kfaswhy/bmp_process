﻿#include "bmp_process.h"

BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;

int ImageHeight = 0;
int ImageWidth = 0;
int PaddingSize = 0;
BYTE* pad = NULL;

RGB* load_bmp(const char* filename)
{
	FILE* f_in = fopen(filename, "rb");
	
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, f_in);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, f_in);

	ImageHeight = infoHeader.biHeight;
	ImageWidth = infoHeader.biWidth;
	int LineByteCnt = (((ImageWidth * infoHeader.biBitCount) + 31) >> 5) << 2;
	//int ImageDataSize = LineByteCnt * ImageHeight;
	PaddingSize = 4 - ((ImageWidth * infoHeader.biBitCount) >> 3) & 3;
	pad = (BYTE*)malloc(sizeof(BYTE) * PaddingSize);
	RGB* img = (RGB*)malloc(sizeof(RGB) * ImageHeight * ImageWidth);

	if (infoHeader.biBitCount == 24) {
		//Read Bitmp
		for (int i = 0; i < ImageHeight; i++) {
			for (int j = 0; j < ImageWidth; j++) {
				int index = i * ImageWidth + j;
				fread(&img[index], sizeof(RGB), 1, f_in);
				//img[index] = imgOpr24(img[index]); //反色运算
			}
			if (PaddingSize != 0)
			{
				fread(pad, 1, PaddingSize, f_in);
			}
		}
	}
	else 
	{
		printf("此程序不支持非24位图片");
		return NULL;
	}

	fclose(f_in);
	return img;
}

int img_process(RGB* img)
{
	for (int i = 0; i < ImageHeight; i++) {
		for (int j = 0; j < ImageWidth; j++) {
			int index = i * ImageWidth + j;
			/* 像素运算 */
			img[index].r = img[index].r >> 1;
			img[index].g = img[index].g >> 1;
			img[index].b = img[index].b >> 1;


			/* 像素运算结束 */
		}
	}
	return 0;
}

void save_bmp(const char* filename, RGB* img)
{
	FILE* f_out = fopen(filename, "wb");
	fwrite(&fileHeader, sizeof(fileHeader), 1, f_out);
	fwrite(&infoHeader, sizeof(infoHeader), 1, f_out);
	for (int i = 0; i < ImageHeight; i++) {
		for (int j = 0; j < ImageWidth; j++)
			fwrite(&img[i * ImageWidth + j], sizeof(RGB), 1, f_out);
		if (PaddingSize != 0)
		{
			fwrite(pad, 1, PaddingSize, f_out);
		}
	}
	fclose(f_out);
	return;
}

int main()
{
	char bmp_in[] = "C:/Work/Desktop/1.bmp";
	char bmp_out[] = "C:/Work/Desktop/2.bmp";
	RGB* img = NULL;
	img = load_bmp(bmp_in);
	img_process(img);
	save_bmp(bmp_out, img);
	return 0;
}

int record()
{
	FILE* pfin = fopen("C:/Work/Desktop/1.bmp", "rb");
	FILE* pfout = fopen("C:/Work/Desktop/2.bmp", "wb");
	//Read the Bitmap file header;  
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pfin);
	//Read the Bitmap info header;  
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pfin);
	//Define variable of Bitmap;
	int ImageHeight = infoHeader.biHeight;
	int ImageWidth = infoHeader.biWidth;
	int BitsPerPixel = infoHeader.biBitCount;
	//Calculate 
	int LineByteCnt = (((ImageWidth * infoHeader.biBitCount) + 31) >> 5) << 2;
	int ImageDataSize = LineByteCnt * ImageHeight;
	int PaddingSize = 4 - ((ImageWidth * BitsPerPixel) >> 3) & 3;
	BYTE* pad = new BYTE[PaddingSize];
	int PixelCnt = ImageHeight * ImageWidth;
	//Check BitsPerPixel
	if (BitsPerPixel != 24) {
		printf("此程序不支持非24位图片");
		return 0;
	}
	if (BitsPerPixel == 24) {
		//Read Bitmp
		RGB* img = new RGB[PixelCnt];
		for (int i = 0; i < ImageHeight; i++) {
			for (int j = 0; j < ImageWidth; j++) {
				int index = i * ImageWidth + j;
				fread(&img[index], sizeof(RGB), 1, pfin);
				img[index] = imgOpr24(img[index]); //反色运算
			}
			fread(pad, 1, PaddingSize, pfin);
		}

		//将修改后的图片保存到文件   
		fwrite(&fileHeader, sizeof(fileHeader), 1, pfout);
		fwrite(&infoHeader, sizeof(infoHeader), 1, pfout);
		for (int i = 0; i < ImageHeight; i++) {
			for (int j = 0; j < ImageWidth; j++)
				fwrite(&img[i * ImageWidth + j], sizeof(RGB), 1, pfout);
			fwrite(pad, 1, PaddingSize, pfout);
		}
	}

	//End
	fclose(pfin);
	fclose(pfout);
	return 0;
}