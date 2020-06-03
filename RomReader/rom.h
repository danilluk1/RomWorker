#ifndef __ROM_H
#define ROM_H
#define ROM_FILENAME_MAX_LENGTH 65535
#define ROM_SEPARATOR "||"

#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <limits.h>
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
		@Name ��� ����� �������� � ������
		@Length ����� ����� �������� � ������
		@Offset �������� �� ������ �����, �� ������ ������ �����
	*/
	typedef struct {
		char* Name;
		size_t Length;
		size_t Offset;

	}ROMFile;
	/*
		����� ����� � ������
		@param pathToRom ���� � ����� .rom
		@param filename ��� �����, ������� ���������� �����
		@retval ������ �� ��������� ��� NULL
	*/
	ROMFile* ROMFind(char* pathToRom, char* filename);
	/*
		������ �����
		@param pathToRom ���� � ����� .rom
		@param romFile ��������� �� ���������� �����, �������� � .rom
		@param buffer �����, ���� ����� ��������� ������ �� �����
		@param Length ����� ������
		@retval ���-�� ���������� ����, ��� -1
	*/
	int ROMLoad(char* pathToRom, ROMFile* romFile, char* buffer, size_t Length);

#ifdef __cplusplus
}
#endif
#endif

