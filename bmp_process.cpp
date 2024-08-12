#include "bmp_process.h"

BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;


int main()
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
	if (BitsPerPixel != 24 && BitsPerPixel != 32) {
		printf("此程序不支持非24位且非32位的BMP图片");
		return 0;
	}

	if (BitsPerPixel == 32) {
		//Read Bitmp
		RGBA* img = new RGBA[PixelCnt];
		for (int i = 0; i < ImageHeight; i++) {
			for (int j = 0; j < ImageWidth; j++) {
				int index = i * ImageWidth + j;
				fread(&img[index], sizeof(RGBA), 1, pfin);
				img[index] = imgOpr32(img[index]); //反色运算
			}
			fread(pad, 1, PaddingSize, pfin);
		}

		//将修改后的图片保存到文件   
		fwrite(&fileHeader, sizeof(fileHeader), 1, pfout);
		fwrite(&infoHeader, sizeof(infoHeader), 1, pfout);
		for (int i = 0; i < ImageHeight; i++) {
			for (int j = 0; j < ImageWidth; j++)
				fwrite(&img[i * ImageWidth + j], sizeof(RGBA), 1, pfout);
			fwrite(pad, 1, PaddingSize, pfout);
		}
	}
	else if (BitsPerPixel == 24) {
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