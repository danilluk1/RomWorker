#define _CRT_SECURE_NO_WARNINGS
#include "rom.h"

//Функция читает из файла строку произвольного размера
char* readLineFromStream(FILE* file) {
	static char* str = NULL;
	static size_t size = 0;
	int ch = fgetc(file);
	if ((ch == '\n') || (ch == EOF)) {
		str = malloc(size + 1);
		str[size] = 0;
	}
	else {
		size++;
		readLineFromStream(file);
		str[--size] = ch;
	}
	return str;
}

//Функция проверяет, является ли строка заголовком файла в образе
int IsStringIsROM(char* str) {
	if (str == NULL) return 0;

	//Делаем копию строки
	char* buffer = calloc(sizeof(char), strlen(str));
	strcpy(buffer, str);

	//Если первые четыре символа равны ROM:, то делим строку сепаратором  
	if (strncmp(buffer, "ROM:", 4) == 0) {
		char* istr = strtok(buffer, ROM_SEPARATOR);
		//Если строка не NULL и её длина меньше разрешенной в образе
		if ((istr != NULL) && (strlen(istr) < ROM_FILENAME_MAX_LENGTH)) {
			//Если первая часть строки до сепаратора является строкой
			if (atol(istr) == 0) {
				while (istr != NULL) {
					//Если вторая часть строки после сепаратора является строкой
					if (atol(istr) != 0) {
						return 1;
					}
					//Продолжаем разбиение
					istr = strtok(NULL, ROM_SEPARATOR);
				}
			}
		}
	}
	return 0;
}

//Функция передвигает указатель потока
void fseekulli(FILE* file, unsigned long long int val) {
	int times = val / INT_MAX;
	long remains = val % INT_MAX;
	for (int i = 0; i < times; i++) {
		fseek(file, INT_MAX, SEEK_CUR);
	}
	fseek(file, remains, SEEK_CUR);
}

//Находит файл в образе по его имени
ROMFile* ROMFind(char* pathToRom, char* filename) {
	ROMFile* rom = malloc(sizeof(ROMFile));
	FILE* file = fopen(pathToRom, "rb");

	char* b = NULL;
	char* name = NULL;

	//Читаем файл до тех пор, пока не получим значение EOF
	while (!feof(file)) {
		b = readLineFromStream(file);
		if (IsStringIsROM(b) == 1) {

			char* istr;
			name = istr = strtok(b, ROM_SEPARATOR) + 4;
			while (istr != NULL) {
				if (atol(istr) != 0) {
					rom->Length = strtoul(istr, NULL, 10);
				}
				istr = strtok(NULL, ROM_SEPARATOR);
			}
			if (strcmp(filename, name) == 0) {
				rom->Name = calloc(sizeof(char), strlen(name));
				rom->Offset = _ftelli64(file);
				strcpy(rom->Name, name);

				break;
			}
			else {
				fseekulli(file, rom->Length);
			}
		}
	}
	if (strcmp(filename, name) != 0) {
		return NULL;
	}
	fclose(file);
	return rom;
}

//Считывает данные файла из образа.
int ROMLoad(char* pathToRom, ROMFile* romFile, char* buffer, size_t Length) {
	if (buffer == NULL) return -1;
	//Открываем поток
	FILE* file = fopen(pathToRom, "rb");

	//Ставим указатель потока на начало данных файла.
	fseekulli(file, romFile->Offset);

	//Считываем данные в буффер.
	size_t numread = fread_s(buffer, Length, sizeof(char), romFile->Length, file);

	//Если данные успешно получены возращаем кол-во байт. Иначе возращаем -1
	if (numread == Length) {
		return numread;
	}
	else {
		return -1;
	}
}