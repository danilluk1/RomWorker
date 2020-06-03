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
		@Name Имя файла лежащего в образе
		@Length Длина файла лежащего в образе
		@Offset Смещение от начала файла, до начала данных файла
	*/
	typedef struct {
		char* Name;
		size_t Length;
		size_t Offset;

	}ROMFile;
	/*
		Поиск файла в образе
		@param pathToRom Путь к файлу .rom
		@param filename Имя файла, который необходимо найти
		@retval Ссылка на структуру или NULL
	*/
	ROMFile* ROMFind(char* pathToRom, char* filename);
	/*
		Чтение файла
		@param pathToRom Путь к файлу .rom
		@param romFile Структура со свойствами файла, лежащего в .rom
		@param buffer Буфер, куда будут прочитаны данные из файла
		@param Length Длина буфера
		@retval Кол-во прчитанных байт, или -1
	*/
	int ROMLoad(char* pathToRom, ROMFile* romFile, char* buffer, size_t Length);

#ifdef __cplusplus
}
#endif
#endif

