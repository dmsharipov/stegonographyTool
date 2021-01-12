#define _CRT_SECURE_NO_WARNINGS
#define LEN 8
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <malloc.h>
#include <math.h>

int g_BitPixel[LEN], g_BitLetter[LEN];

void FromDec_ToBin(unsigned char symbol, int *bit)
{
    for (int i = 0; i < LEN; i++)
    {
        bit[i] = symbol % 2;
        symbol /= 2;
    }
}

char FromBin_ToDec(int *bit)
{
    char dec = 0;
    for (int i = 0; i < LEN; i++)
    {
        dec += bit[i] * pow(2, i);
    }
    return dec;
}

void Null(int *array)
{
    for (int i = 0; i < LEN; i++)
    {
        array[i] = 0;
    }
}

int main(void)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    unsigned char PicElem, *buff;
    const int FirstPixel = 54;
    int degree = 0, i = 0, j = 0, rank = 0, TextSize = 0;
    int BitDegree, BitRank, BitSize;
    FILE *picture, *text;
    picture = fopen("outpic.bmp", "rb");
    if (picture == NULL) { printf("Error: .bmp File is not founded.");
    _getch();
    return 0;
    }
    text = fopen("text.txt", "w");

    fseek(picture, FirstPixel, SEEK_SET);
    while (i < 3) {
        fread(&PicElem, sizeof(char), 1, picture);
        FromDec_ToBin(PicElem, g_BitPixel);
        BitDegree = g_BitPixel[0];
        degree += BitDegree * pow(2, i);
        i++;
        Null(g_BitPixel);
    }
    i = 0;

    while (i < 3) {
        fread(&PicElem, sizeof(char), 1, picture);
        FromDec_ToBin(PicElem, g_BitPixel);
        BitRank = g_BitPixel[0];
        rank += BitRank * pow(2, i);
        i++;
        Null(g_BitPixel);
    }

    i = 0;
    int temp = 0;
    while (i < rank)
    {
        while (j < 4)
        {
            fread(&PicElem, sizeof(char), 1, picture);
            FromDec_ToBin(PicElem, g_BitPixel);
            BitSize = g_BitPixel[0];
            temp += BitSize * pow(2, j);
            Null(g_BitPixel);
            j++;
        }
        TextSize += temp * pow(10, i);
        i++;
        temp = 0;
        j = 0;
    }

    buff = (unsigned char*)malloc(TextSize*sizeof(unsigned char)+1);
    buff[TextSize] = { 0 };
    int bits = 0;
    for (i = 0; i < TextSize; i++)
    {
        FromDec_ToBin(buff[i], g_BitLetter);
        while ((j < LEN) && (PicElem = fgetc(picture)) != EOF)
        {
            FromDec_ToBin(PicElem, g_BitPixel);
            for (int k = j; (k < j + degree) && (bits < degree); k++, bits++) 
            {
                if (k == LEN) break;
                g_BitLetter[k] = g_BitPixel[bits];
            }
            j += degree;
            bits = 0;
            PicElem = NULL;
            Null(g_BitPixel);
        }
        buff[i] = FromBin_ToDec(g_BitLetter);
        Null(g_BitLetter);
        j = 0;
    }

    buff[TextSize] = '\0';
    fputs((const char*)buff, text);
    printf("Decoding is successfully completed.");
    printf("\n%s", buff);
    fclose(text);
    fclose(picture);
    _getch();
    return 0;
}
