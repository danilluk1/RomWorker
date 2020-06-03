#define _CRT_SECURE_NO_WARNINGS
#include "rom.h"

/*
This file needs only for education purpose, to use library you need to copy rom.c and rom.h files into your project
*/

static void MakeFileFromBytes(char* path, char* data, size_t dataLength) {
	FILE* file = fopen(path, "wb");
	fwrite(data, sizeof(char), dataLength, file);
	fclose(file);
}

int main(void) {
	char* pathtoRom = "D:\\ROM\\eeprom.rom";
	ROMFile* file = ROMFind(pathtoRom, "sme.txt");
	if (file != NULL) {
		char* buffer = calloc(file->Length, sizeof(char));
		int byteRead = ROMLoad(pathtoRom, file, buffer, file->Length);
		if (byteRead != -1) {
			MakeFileFromBytes("D:\\ROM\\s.txt", buffer, file->Length);
		}
		else {
			//Ошибка чтения
		}
	}
}