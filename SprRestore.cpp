#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <cmath>
//#include <RestoreOtherSprite.cpp>

typedef unsigned int Word;

using namespace std;

extern void RestoreSprite(Word Config);
extern void RestoreIMP(Word Config);
extern void LoadSprites(FILE* fp);

extern char Delimiters[] = " \t\n";        /* Token delimiters */
extern char NumDelimiters[] = " ,\t\n";    /* Value delimiters */
Word LineNum;               /* Line being executed from the script */
static char InputLine[256];        /* Input line from script */
static char *Commands[] = {
	"TYPE","LOAD","ENDIAN","LOADNEXT","LOADFIXED","LOADHANDLE"
};
#define CommandCount 6      /* Number of commands */

#define Swap2_2Bytes(val) val = ( (((val) >> 8) & 0x00FF) | (((val) << 8) & 0xFF00) )
#define Swap4Bytes(val) val = ( (((val) >> 24) & 0x000000FF) | (((val) >> 8) & 0x0000FF00) | (((val) <<  8) & 0x00FF0000) | (((val) << 24) & 0xFF000000) )
//#define SwapULong(val) val = ( (((val) >> 24) & 0x000000FF) | (((val) >> 8) & 0x0000FF00) | (((val) <<  8) & 0x00FF0000) | (((val) << 24) & 0xFF000000) )
//#define SwapULong(val) (val << 24 | (val << 8 & 0xFF0000) | (val >> 8 & 0xFF00) | val >> 24 & 0xFF)
#define Swap8Bytes(val) ( (((val) >> 56) & 0x00000000000000FF) | (((val) >> 40) & 0x000000000000FF00) | (((val) >> 24) & 0x0000000000FF0000) | (((val) >>  8) & 0x00000000FF000000) | (((val) <<  8) & 0x000000FF00000000) | (((val) << 24) & 0x0000FF0000000000) | (((val) << 40) & 0x00FF000000000000) | (((val) << 56) & 0xFF00000000000000) )
#define Swap2Bytes(val) ( (((val) >> 8) & 0x00FF) | (((val) << 8) & 0xFF00) )

int Count;
char *SpriteName;
char *TextPtr;  /* Pointer to string token */


static void PrintError(char *Error)
{
	printf("# Error in Line %d, %s\n",LineNum,Error);
}





extern void LoadSprites(FILE* fp)
{
//	char *TextPtr;  /* Pointer to string token */
	Word i;         /* Index */

	while (fgets(InputLine,sizeof(InputLine),fp)) { /* Get a string */
		++LineNum;          /* Adjust the line # */
		TextPtr = strtok(InputLine,Delimiters); /* Get the first token */
		if (!TextPtr) {
			continue;
		}
		i = 0;      /* Check for the first command */
		if (isalnum(TextPtr[0])) {  /* Comment? */
			do {
				if (!strcmp(TextPtr,Commands[i])) { /* Match? */
					switch (i) {        /* Execute the command */
					case 0:
//						SetType();   /* Target machine */
						break;
					case 1:
						RestoreIMP(0);
//						RestoreSprite(0); 
						break;
					}
					break;      /* Don't parse anymore! */
				}
			} while (++i<CommandCount); /* Keep checking */
		}
		if (i==CommandCount) {      /* Didn't find it? */
			printf("# Command %s not implemented\n",TextPtr);
		}
	}
}




extern void RestoreSprite(Word Config) {
// Restore other sprite (non-IMP)
cout << "\nNON-Imp file!\n";

///char *TextPtr;
FILE *infile, *outfile;
char *FinName, *tmpName, *tmpName2, *FinName1;
Word temp, temp2;
int FileSize, i, Off2Entry, Count;
int start_offset, RealStartOffset, offset, p;
int Width, Height, PRE0, PRE1, PDAT_length, FrameCount, NextFr;

int Prefix40 = 0x40;
int Prefix80 = 0x80;

struct {
		Word CCB;
		Word Head_50;
		Word Blank;
	} CCB;


struct {
		Word temp;
		Word PLUT;
		Word PLUT_size;
		Word PLUT_value;
	} PLUT;


struct {
		Word temp;
		Word PDAT;
	} PDAT;


if (!TextPtr) {
	cout << "\nTextPtr is not recognized!\n";
	return;
}

FrameCount =0;

//Открываем исходный файл спрайта для чтения
infile = fopen(TextPtr, "rb");
cout << "\nFile "<< TextPtr << " opened.";


// Определяем количество спрайтов внутри.
fseek(infile,0,SEEK_END);
FileSize = ftell(infile);
rewind(infile);

// Ищем сигнатуру 
// Это будут флаги 47 E6 42 10. Читаем как 10 42 - E6 47
Count = 0;
for (i=0; i<FileSize/4; i++) {
			fread(&PDAT.temp,4,1,infile);
			temp = PDAT.temp;
				if (temp == 0x1042E647) {
					Count++;
					}
	}
cout << "\nFrames Count = "<< Count;
rewind(infile);


// Определим смещение к первой CCB

fseek(infile,0,SEEK_END);
FileSize = ftell(infile);
rewind(infile);


// Делаем цикл чтения кадров. // Чтобы включить отображение только первого кадра, 
// просто убрать эту строку и соответствующую скобку.
/* Начало цикла чтения и записи кадра *//////////////////////////////////////////////////////////////////////
for (p=0; p<Count; p++) {
	cout << "\n";


// Устанавливаем позицию на чтение кадра. Если кадр первый, то FrameCount==0, 
// если кадр не первый, то позиция смещается на 4*FrameCount. 

	// Ставим позицию на начало следующей записи. Количество записей мы знаем (Count)
	// Текущая позиция будет равняться количеству обработанных кадров * 4. 
fseek(infile,FrameCount*4,SEEK_SET);
// Сначала читаем 2 байта и определяем префикс. В зависимости от этого по-разному определяется смещение
// к урезанному заголовку кадра.
fread(&start_offset, 2, 1, infile);
// Если префикс = 0x40, то к прочитанному далее указателю плюсуем то смещение, куда оно указывает.
// Если префикса нет (!=40), то просто переходим по данному указателю. Off2Entry = RealStartOffset
	if (Swap2Bytes(start_offset) != 0x4000) {
			cout << "\nPrefix = "<< "0x" << hex << Swap2Bytes(start_offset);
///			cout << "\nPrefix = "<< Swap2Bytes(start_offset);
			temp2 = 0;
			fread(&start_offset, 2, 1, infile);
			RealStartOffset = Swap2Bytes(start_offset);
			goto NonPrefix;
		} 

///cout << "\nPrefix = "<< Swap2Bytes(start_offset);
	cout << "\nPrefix = "<< "0x" << hex << Swap2Bytes(start_offset);
fread(&start_offset, 2, 1, infile);
RealStartOffset = Swap2Bytes(start_offset);
cout << "\nRealStartOffset = " << dec << RealStartOffset;

// Прочитаем смещение, которое нужно будет приплюсовать к RealStartOffset
fseek(infile, RealStartOffset+2, SEEK_SET);
	fread(&offset, 2, 1, infile);
	temp2 = Swap2Bytes(offset);
	cout << "\ntemp2= "<< "0x" << hex << temp2;


NonPrefix:
// Получаем смещение к началу урезанной шапки CCB.
Off2Entry = RealStartOffset+temp2;
cout << "\nShape CCB offset= "<< dec << Off2Entry;
///cout << "\nShape CCB offset= " << Off2Entry;

// Найдено смещение к CCB. Плюсуем FrameCount;
FrameCount++;
cout << "\nFrame: "<< FrameCount;

// Установим позицию на начало PLUT.
rewind(infile);
fseek(infile,Off2Entry+4,SEEK_SET); // +4, потому что нужно еще пропустить 4 байта.
// Эти 4 байта, видимо, попали из PC версии, где означали размещение спрайта по X,Y.


// Делаем имя файла для экспорта кадра спрайта.
char begin[30] = "Recover_";
char Number[10] = "";
FinName1 = itoa(FrameCount, Number, 10);
tmpName = strcat(begin, FinName1);
FinName = strcat(tmpName, TextPtr);
//FinName = strcat(tmpName, itoa(Count, tmpName2, 10));
cout << "\nNew filename = "<< FinName;

// Открываем выходной файл для записи очередного кадра
outfile = fopen(FinName, "wb");

// Пишем сначала CCB заголовок.
CCB.CCB = 0x43434220;
temp = Swap4Bytes(CCB.CCB);
fwrite(&CCB.CCB, 4, 1, outfile);

CCB.Head_50 = 0x000050;
temp = Swap4Bytes(CCB.Head_50);
fwrite(&CCB.Head_50, 4, 1, outfile);

CCB.Blank = 0;
temp = Swap4Bytes(CCB.Blank);
fwrite(&CCB.Blank, 4, 1, outfile);


// Читаем и пишем урезанный заголовок CCB в outfile
	for (i=0; i<60/4; i++) {
			fread(&PLUT.temp,4,1,infile);
			temp = PLUT.temp;
			fwrite(&temp, 4, 1, outfile);
		}

// Определим размеры кадра и поменяем их местами, потому что спрайт повернут на 90 градусов
	// против часовой стрелки.
rewind(infile);
fseek(infile,Off2Entry+56,SEEK_SET);

Height = Width = 0;

fread(&PLUT.temp,4,1,infile);
PRE0 = Swap4Bytes(PLUT.temp);
//GetShapeHeight
Height = ((PRE0 >> 6) & 0x3FF) + 1;
cout << "\nHeight = " << Height;

fread(&PLUT.temp,4,1,infile);
PRE1 = Swap4Bytes(PLUT.temp);
//GetShapeWidth	
Width = (PRE1 & 0x7FF) + 1;
cout << "\nWidth = " << Width;

// Запишем ширину
temp = Swap4Bytes(Width);
fwrite(&Width, 4, 1, outfile);

// Запишем высоту
temp = Swap4Bytes(Height);
fwrite(&Height, 4, 1, outfile);


// Теперь запишем PLUT. Она в исходном файле спрайта идет без заголовка.
// Ориентируемся на размер 4C. Пока так.
// Заголовок:
PLUT.PLUT = 0x504C5554;
temp = Swap4Bytes(PLUT.PLUT);
fwrite(&temp, 4, 1, outfile);

PLUT.PLUT_size = 0x00004C;
temp = Swap4Bytes(PLUT.PLUT_size);
fwrite(&temp, 4, 1, outfile);

PLUT.PLUT_value = 0x000020;
temp = Swap4Bytes(PLUT.PLUT_value);
fwrite(&temp, 4, 1, outfile);

// Теперь пишем содержимое PLUT
rewind(infile);
fseek(infile,Off2Entry+64,SEEK_SET);

	for (i=0; i<16; i++) {		// Размер PLUT читаем в 64 байта (16*4)
			fread(&PLUT.temp,4,1,infile);
			temp = PLUT.temp;
			fwrite(&temp, 4, 1, outfile);
		}

// Пишем заголовок PDAT.
PDAT.PDAT = 0x50444154;
temp = Swap4Bytes(PDAT.PDAT);
fwrite(&temp, 4, 1, outfile);

// Вычисляем длину PDAT.
PDAT_length = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1
for (i=0; i<FileSize/4; i++) {
			fread(&PDAT.temp,4,1,infile);
			temp = PDAT.temp;
			PDAT_length += 4; 
			if (feof(infile)) {
					PDAT_length += 4;
					goto OnePDAT; // Если достигнут конец файла, то вычисляем заголовок и пишем PDAT
					}
				//Находим следующий PDAT. Это будут флаги 47 E6 42 10 как 10 42 - E6 47
			if (temp == 0x1042E647) {
					// Следующий PDAT найден.
					cout << "\nNext CCB flags found";
OnePDAT:
					cout << "\nPDAT_length= " << PDAT_length;
					temp = Swap4Bytes(PDAT_length);
					fwrite(&temp, 4, 1, outfile);  // Пишем размер PDAT в выходной файл.
					temp = Swap4Bytes(PDAT_length);
						// Теперь запишем весь PDAT в выходной файл.
						rewind(infile);
						fseek(infile,Off2Entry+64+64,SEEK_SET); // 64 - длина обрезанного заголовка
						// второе 64 - длина PLUT в исходном файле без заголовка (4C)
						cout << "\nOFFSET to PDAT = " << Off2Entry+64+64;
							for (i=0; i<PDAT_length/4-2; i++) { // -2, потому что заголовок PDAT и ее и размер мы уже записали
									fread(&PDAT.temp,4,1,infile);
									temp = PDAT.temp;
									fwrite(&temp, 4, 1, outfile);
							}
		//// ЗДЕСЬ ПРОПИСАТЬ, ЕСЛИ КАДР ОДИН И ДОСТИГНУТ КОНЕЦ ФАЙЛА! ////////////
///							cout << "\nPress Enter to Continue";
///							getchar(); 
					goto Finish;
//					break;
				}
///			else goto OnePDAT;
///			cout << "\nPDAT SIZE = " << PDAT_length;
	}
#endif;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Finish:
temp = 0;
temp2 = 0;
Off2Entry = 0;
RealStartOffset = 0;
start_offset = 0;
offset = 0;

fclose(outfile);
} /* Конец цикла чтения и записи кадра */////////////////////////////////////////////////////////////////////
fclose(infile);

cout << "\nFile "<< TextPtr << " closed.\n\n";

}













extern void RestoreIMP(Word Config) {
FILE *infile, *outfile;
//char *TextPtr;
//Count++;
char *FinName, *tmpName, *tmpName2, *tmpName3, *FinName1, *FinName2;
Word temp, temp2;
int FileSize, i, Off2Entry, Count;
int start_offset, RealStartOffset, offset, p, z, z1, SubFrame, SubFrameOff;
int Width, Height, PRE0, PRE1, PDAT_length, FrameCount, NextFr;

int Prefix40 = 0x40;
int Prefix80 = 0x80;

struct {
		Word CCB;
		Word Head_50;
		Word Blank;
	} CCB;


struct {
		Word temp;
		Word PLUT;
		Word PLUT_size;
		Word PLUT_value;
	} PLUT;


struct {
		Word temp;
		Word PDAT;
	} PDAT;


		TextPtr = strtok(0,NumDelimiters);
	if (!TextPtr) {
		PrintError("\nNot enough parms for LOAD");
		return;
	}

	TextPtr = strtok(0,NumDelimiters);
	if (!TextPtr) {
		PrintError("\nNot enough parms for LOAD");
		return;
	}
FrameCount =1;


// Если открытый спрайт не Имп, используем другой алгоритм распаковки.
if (strcmp(TextPtr, "rSPR_IMP")) {
//if (strcmp(TextPtr, "rSPR_DEMON")) { 
	RestoreSprite(0);
	return;
} 

//Открываем исходный файл спрайта для чтения
infile = fopen(TextPtr, "rb");
cout << "\nFile "<< TextPtr << " opened.";


// Определяем количество спрайтов внутри.
fseek(infile,0,SEEK_END);
FileSize = ftell(infile);
rewind(infile);

// Ищем сигнатуру 
// Это будут флаги 47 E6 42 10. Читаем как 10 42 - E6 47
Count = 0;
for (i=0; i<FileSize/4; i++) {
			fread(&PDAT.temp,4,1,infile);
			temp = PDAT.temp;
				if (temp == 0x1042E647) {
					Count++;
					}
	}
cout << "\nFrames Count = "<< Count;
rewind(infile);



// Определим смещение к первой CCB

fseek(infile,0,SEEK_END);
FileSize = ftell(infile);
rewind(infile);



// Прочитать смещение 1 (с префиксом 40). Перейти туда, куда оно указывает. }
// 8 раз читать последующие смещения и суммировать их со смещением 1.		} сделать так 8 раз.
// Сохранить кадры.															}

// Перейти на смещение 20. 
// 18 раз прочитать смещения без префиксов.
// Сохранить кадры.




// Делаем цикл чтения кадров. 
// Для импа установить 26!
// Для общего спрайта установить Count
/* Начало цикла чтения и записи кадра *//////////////////////////////////////////////////////////////////////
for (p=0; p<26; p++) {
	cout << "\n";

// Устанавливаем позицию на чтение кадра. Если кадр первый, то FrameCount==0, 
// если кадр не первый, то позиция смещается на 4*FrameCount. 

	// Ставим позицию на начало следующей записи. Количество записей мы знаем (Count)
	// Текущая позиция будет равняться количеству обработанных кадров * 4. 
fseek(infile,(FrameCount-1)*4,SEEK_SET);
// Сначала читаем 2 байта и определяем префикс. В зависимости от этого по-разному определяется смещение
// к урезанному заголовку кадра.
fread(&start_offset, 2, 1, infile);
// Если префикс = 0x40, то к прочитанному далее указателю плюсуем то смещение, куда оно указывает.
// Если префикса нет (!=40), то просто переходим по данному указателю. Off2Entry = RealStartOffset
	if (Swap2Bytes(start_offset) != 0x4000) {
			cout << "\nPrefix = "<< "0x" << hex << Swap2Bytes(start_offset);
///			cout << "\nPrefix = "<< Swap2Bytes(start_offset);
			temp2 = 0;
			fread(&start_offset, 2, 1, infile);
			RealStartOffset = Swap2Bytes(start_offset);
			goto NonPrefix;
		} 

///cout << "\nPrefix = "<< Swap2Bytes(start_offset);
	cout << "\nPrefix = "<< "0x" << hex << Swap2Bytes(start_offset);
fread(&start_offset, 2, 1, infile);
RealStartOffset = Swap2Bytes(start_offset);
cout << "\nRealStartOffset = 0x" << hex << RealStartOffset;

// Прочитаем смещение, которое нужно будет приплюсовать к RealStartOffset
fseek(infile, RealStartOffset+2, SEEK_SET);
	fread(&offset, 2, 1, infile);
	temp2 = Swap2Bytes(offset);
	cout << "\ntemp2= "<< "0x" << hex << temp2;

//SubFrameOff=temp2;
SubFrame=0;
SubFrameFinish:
	for (z=SubFrame; z<7; z++) {
		SubFrame++;
		cout << "\nSubFrame #" << SubFrame;
		// Установим насильно позинию на начало чтения SubFrame
		rewind(infile);
		fseek(infile, RealStartOffset+6+z*4, SEEK_SET);
		cout << "\nFSEEK: " << "0x" << RealStartOffset+6+z*4;

///		fread(&offset, 2, 1, infile);
		fread(&offset, 2, 1, infile);
//		cout << "\nSubFrame Offset: " << "0x" << RealStartOffset+SubFrameOff;
		SubFrameOff = Swap2Bytes(offset);
		Off2Entry = SubFrameOff+RealStartOffset;
		cout << "\nSubFrame CCB Offset: " << "0x" << Off2Entry;
		// Здесь нужно прописать запись кадра по узнанному смещению (Off2Entry) CCB.
		goto SubFrameStart;
		}


NonPrefix:
// Получаем смещение к началу урезанной шапки CCB.
///Off2Entry = RealStartOffset+temp2+SubFrame*4;
Off2Entry = RealStartOffset+temp2;
// Найдено смещение к CCB. 
cout << "\nMain shape CCB offset= 0x"<< hex << Off2Entry;
// Это будет SubFrame 8, который по сути основной исходный стартовый Frame.
SubFrame++;



SubFrameStart:
// Плюсуем FrameCount при условии, что все SubFrames завершены.
//if (z=7) {
//	z=8;
//	FrameCount++;
//	}

//if (z>6) {
//	z++;
//	FrameCount++;
//	}
//cout << "\nFrame: "<< FrameCount;


// Установим позицию на начало PLUT.
rewind(infile);
fseek(infile,Off2Entry+4,SEEK_SET); // +4, потому что нужно еще пропустить 4 байта.
// Эти 4 байта, видимо, попали из PC версии, где означали размещение спрайта по X,Y.




// Делаем имя файла для экспорта кадра спрайта.
char begin[30] = "Recover_";
char Number[10] = "";
char Sub[10] = "";
char Space[10] = "_";
FinName1 = itoa(FrameCount, Number, 10);
tmpName = strcat(begin, FinName1);					  // Recover_12

tmpName2 = strcat(tmpName, Space);					  // Recover_12_
tmpName3 = strcat(tmpName2, itoa(SubFrame, Sub, 10)); // Recover_12_4
FinName = strcat(tmpName3, TextPtr);				  // Recover_12_4rSPR_IMP
//FinName = strcat(tmpName, itoa(Count, tmpName2, 10));
cout << "\nNew filename = "<< FinName;

// Открываем выходной файл для записи очередного кадра
outfile = fopen(FinName, "wb");

// Пишем сначала CCB заголовок.
CCB.CCB = 0x43434220;
temp = Swap4Bytes(CCB.CCB);
fwrite(&CCB.CCB, 4, 1, outfile);

CCB.Head_50 = 0x000050;
temp = Swap4Bytes(CCB.Head_50);
fwrite(&CCB.Head_50, 4, 1, outfile);

CCB.Blank = 0;
temp = Swap4Bytes(CCB.Blank);
fwrite(&CCB.Blank, 4, 1, outfile);



	// Читаем и пишем урезанный заголовок CCB в outfile
	for (i=0; i<60/4; i++) {
			fread(&PLUT.temp,4,1,infile);
			temp = PLUT.temp;
			fwrite(&temp, 4, 1, outfile);
		}

// Определим размеры кадра и поменяем их местами, потому что спрайт повернут на 90 градусов
	// против часовой стрелки.
rewind(infile);
fseek(infile,Off2Entry+56,SEEK_SET);

Height = Width = 0;

fread(&PLUT.temp,4,1,infile);
PRE0 = Swap4Bytes(PLUT.temp);
//GetShapeHeight
Height = ((PRE0 >> 6) & 0x3FF) + 1;
cout << "\nHeight = " << dec << Height;

fread(&PLUT.temp,4,1,infile);
PRE1 = Swap4Bytes(PLUT.temp);
//GetShapeWidth	
Width = (PRE1 & 0x7FF) + 1;
cout << "\nWidth = " << dec << Width;

// Запишем ширину
temp = Swap4Bytes(Width);
fwrite(&Width, 4, 1, outfile);

// Запишем высоту
temp = Swap4Bytes(Height);
fwrite(&Height, 4, 1, outfile);


// Теперь запишем PLUT. Она в исходном файле спрайта идет без заголовка.
// Ориентируемся на размер 4C. Пока так.
// Заголовок:
PLUT.PLUT = 0x504C5554;
temp = Swap4Bytes(PLUT.PLUT);
fwrite(&temp, 4, 1, outfile);

PLUT.PLUT_size = 0x00004C;
temp = Swap4Bytes(PLUT.PLUT_size);
fwrite(&temp, 4, 1, outfile);

PLUT.PLUT_value = 0x000020;
temp = Swap4Bytes(PLUT.PLUT_value);
fwrite(&temp, 4, 1, outfile);

// Теперь пишем содержимое PLUT
rewind(infile);
fseek(infile,Off2Entry+64,SEEK_SET);

	for (i=0; i<16; i++) {		// Размер PLUT читаем в 64 байта (16*4)
			fread(&PLUT.temp,4,1,infile);
			temp = PLUT.temp;
			fwrite(&temp, 4, 1, outfile);
		}

// Пишем заголовок PDAT.
PDAT.PDAT = 0x50444154;
temp = Swap4Bytes(PDAT.PDAT);
fwrite(&temp, 4, 1, outfile);

// Вычисляем длину PDAT.
PDAT_length = 0;

#if 0
fseek(infile,Off2Entry+64+64,SEEK_SET);
//	while (!EOF || (temp == 0x1042E647));
	while (!eof(infile) || (temp == 0x1042E647))
		{
		 	fread(&PDAT.temp,4,1,infile);
			temp = PDAT.temp;
			PDAT_length += 4;
			fwrite(&temp, 4, 1, outfile);
			cout << "\nWriting";
		}// while (!EOF || (temp == 0x1042E647)); 
#endif;

#if 0
i=0;
fseek(infile,Off2Entry+64+64,SEEK_SET);  // 64 - длина обрезанного заголовка
///while ((i<FileSize/4) || !EOF) {
while(!feof(infile)) {
	fread(&PDAT.temp,4,1,infile);
	temp = Swap4Bytes(PDAT.temp);
	cout << "\nTEMP= "<< temp << "\n";
	fwrite(&temp, 4, 1, outfile);
	i++;
}
#endif;

#if 0
while (!EOF)) {
	fread(&PDAT.temp,4,1,infile);
	temp = PDAT.temp;
	PDAT_length += 4;
		if (temp == 0x1042E647) { // Значит, найден следующий кадр
			cout << "\nNext CCB flags found";
			cout << "\nPDAT_length= " << PDAT_length;
			temp = Swap4Bytes(PDAT_length);
			fwrite(&temp, 4, 1, outfile);  // Пишем размер PDAT в выходной файл.
			rewind(infile);
			fseek(infile,Off2Entry+64+64,SEEK_SET);  // 64 - длина обрезанного заголовка
			// второе 64 - длина PLUT в исходном файле без заголовка (4C)
			cout << "\nOFFSET to PDAT = " << Off2Entry+64+64;
					for (i=0; i<PDAT_length/4-2; i++) { // -2, потому что заголовок PDAT и ее и размер мы уже записали
						fread(&PDAT.temp,4,1,infile);
						temp = PDAT.temp;
						fwrite(&temp, 4, 1, outfile);
							}
				break;
			}
	}
#endif;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1
for (i=0; i<FileSize/4; i++) {
			fread(&PDAT.temp,4,1,infile);
			temp = PDAT.temp;
			PDAT_length += 4; 
			if (feof(infile)) {
					PDAT_length += 4;
					goto OnePDAT; // Если достигнут конец файла, то вычисляем заголовок и пишем PDAT
					}
				//Находим следующий PDAT. Это будут флаги 47 E6 42 10 как 10 42 - E6 47
			if (temp == 0x1042E647) {
					// Следующий PDAT найден.
					cout << "\nNext CCB flags found";
OnePDAT:
					cout << "\nPDAT_length= " << PDAT_length;
					temp = Swap4Bytes(PDAT_length);
					fwrite(&temp, 4, 1, outfile);  // Пишем размер PDAT в выходной файл.
					temp = Swap4Bytes(PDAT_length);
						// Теперь запишем весь PDAT в выходной файл.
						rewind(infile);
						fseek(infile,Off2Entry+64+64,SEEK_SET); // 64 - длина обрезанного заголовка
						// второе 64 - длина PLUT в исходном файле без заголовка (4C)
						cout << "\nOFFSET to PDAT = " << "0x" << hex << Off2Entry+64+64;
							for (i=0; i<PDAT_length/4-2; i++) { // -2, потому что заголовок PDAT и ее и размер мы уже записали
									fread(&PDAT.temp,4,1,infile);
									temp = PDAT.temp;
									fwrite(&temp, 4, 1, outfile);
							}
		//// ЗДЕСЬ ПРОПИСАТЬ, ЕСЛИ КАДР ОДИН И ДОСТИГНУТ КОНЕЦ ФАЙЛА! ////////////
							if (z<7) {
								cout << "\nSubFrame " << SubFrame << " saved!\n";
								cout << "\nPress Enter to Continue";
								getchar(); 
								goto SubFrameFinish;
								}
					goto Finish;
//					break;
				}
///			else goto OnePDAT;
///			cout << "\nPDAT SIZE = " << PDAT_length;
	}
#endif;
///}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Finish:
temp = 0;
temp2 = 0;
Off2Entry = 0;
RealStartOffset = 0;
start_offset = 0;
offset = 0;
SubFrame=0;

FrameCount++;
cout << "\nFrame: "<< dec << FrameCount << " saved!";

fclose(outfile);
} /* Конец цикла чтения и записи кадра */////////////////////////////////////////////////////////////////////
fclose(infile);

cout << "\nFile "<< TextPtr << " closed.\n\n";

///OneFrame:

}





int _tmain(int argc, _TCHAR* argv[])
{
FILE *fp;

Count = 0;

fp = fopen("SpriteList.txt","r");    /* Read the ASCII script */
	if (!fp) {
		printf("\n# Can't open SpriteList.txt script file %s.\n",argv[1]);    /* Oh oh */
		return 1;
	}

LoadSprites(fp);

fclose(fp);     /* Close the file */

	cout << "\nPress the Enter key to continue ...";
    cin.get();

	return 0;
}

