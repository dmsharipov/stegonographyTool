#define _CRT_SECURE_NO_WARNINGS
#define LEN 8
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <malloc.h>
#include <math.h>

const int g_FirstPixel = 54;
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

    int degree, BuffSize, i = 0, j = 0, textsize = 0, max;
    int rank = 0, FirstTextPixel, stop, bits = 0, count = 0;
    int *RankBuff = NULL;
    unsigned char PicElem, StegoPixel, *buff;
    FILE *picture_in, *picture_out, *text;
    picture_in = fopen("stegocontainer.bmp", "rb");
    if (picture_in == NULL) { printf("Error: .bmp file is not founed."); _getch(); return 0; }
    picture_out = fopen("outpic.bmp", "wb");

    while (i < g_FirstPixel) 
    {
        fread(&PicElem, sizeof(char), 1, picture_in);
        fwrite(&PicElem, sizeof(char), 1, picture_out);
        i++;
    }
    
    text = fopen("text.txt", "rb");
    if (text == NULL) { printf("Error: .txt file is not founed."); _getch(); return 0; }
    fseek(text, 0, SEEK_END);
    BuffSize = ftell(text);
    buff = (unsigned char*)malloc(BuffSize);
    fseek(text, 0, SEEK_SET);
    while (fscanf(text, "%c", &PicElem) != EOF)
    {
        buff[textsize] = PicElem;
        textsize++;
    }
    fclose(text);
    
    printf("Enter the degree of packing from 1 to 7: ");
    scanf("%d", &degree);
    if ((degree < 1) || (degree > 7)) { 
        printf("Error: Degree of packing is incorrect.");
        _getch();
        return 0;
    }

    int plus_one = 0;
    if ((degree > 4) || (degree == 3)) plus_one = 1 ;
    fseek(picture_in, 0, SEEK_END);
    max = ftell(picture_in) - g_FirstPixel;
    if ((textsize * (LEN / degree + plus_one)) > max) { printf("\nError: Not enough memory to write.");
    _getch();
    return 0;
    }

    fseek(picture_in, g_FirstPixel, SEEK_SET);
    fseek(picture_out, g_FirstPixel, SEEK_SET);

    i = 0;
    FromDec_ToBin(degree, g_BitLetter);
    while (i < 3)
    {
        fread(&PicElem, sizeof(char), 1, picture_in);
        FromDec_ToBin(PicElem, g_BitPixel);
        g_BitPixel[0] = g_BitLetter[i];
        StegoPixel = FromBin_ToDec(g_BitPixel);
        fwrite(&StegoPixel, sizeof(char), 1, picture_out);
        i++;
        Null(g_BitPixel);
    }
    i = 0;
    Null(g_BitLetter);

    while (textsize > 0)
    {
        RankBuff = (int*)realloc(RankBuff, (rank+1)*sizeof(int));
        RankBuff[rank] = textsize % 10;
        textsize = textsize / 10;
        rank++;
    }

    fseek(picture_in, g_FirstPixel + 3, SEEK_SET);
    fseek(picture_out, g_FirstPixel + 3, SEEK_SET);
    FromDec_ToBin(rank, g_BitLetter);
    while (i < 3)
    {
        fread(&PicElem, sizeof(char), 1, picture_in);
        FromDec_ToBin(PicElem, g_BitPixel);
        g_BitPixel[0] = g_BitLetter[i];
        StegoPixel = FromBin_ToDec(g_BitPixel);
        fwrite(&StegoPixel, sizeof(char), 1, picture_out);
        i++;
        Null(g_BitPixel);
    }
    i = 0;
    Null(g_BitLetter);

    fseek(picture_in, g_FirstPixel + 6, SEEK_SET);
    fseek(picture_out, g_FirstPixel + 6, SEEK_SET);
    FromDec_ToBin(degree, g_BitLetter);

    for (int j = 0; j < rank; j++)
    {
        FromDec_ToBin(RankBuff[j], g_BitLetter);
        while (i < 4)
        {
            fread(&PicElem, sizeof(char), 1, picture_in);
            FromDec_ToBin(PicElem, g_BitPixel);
            g_BitPixel[0] = g_BitLetter[i];
            StegoPixel = FromBin_ToDec(g_BitPixel);
            fwrite(&StegoPixel, sizeof(char), 1, picture_out);
            i++;
            Null(g_BitPixel);
        }
        i = 0;
        Null(g_BitLetter);
    }
    free(RankBuff);
    FirstTextPixel = g_FirstPixel + 6 + rank * 4;

    fseek(picture_in, FirstTextPixel, SEEK_SET);
    fseek(picture_out, FirstTextPixel, SEEK_SET);

    for (i = 0; i < BuffSize; i++)
    {
        FromDec_ToBin(buff[i], g_BitLetter);
        while ((j < LEN) && ((PicElem = fgetc(picture_in)) != EOF))
        {
            FromDec_ToBin(PicElem, g_BitPixel);
            for (int k = j; (k < j + degree) && (bits < degree); k++, bits++)
            {
                if (k == LEN) break;
                g_BitPixel[bits] = g_BitLetter[k];
            }
            StegoPixel = FromBin_ToDec(g_BitPixel);
            fputc(StegoPixel, picture_out);
            j += degree;
            count++;
            bits = 0;
            PicElem = NULL;
            Null(g_BitPixel);
        }
        Null(g_BitLetter);
        j = 0;
    }
    free(buff);

    fseek(picture_in, 0, SEEK_END);
    stop = ftell(picture_in);

    fseek(picture_in, count + FirstTextPixel, SEEK_SET);
    fseek(picture_out, count + FirstTextPixel, SEEK_SET);


    i = count + FirstTextPixel;
    while (fread(&PicElem, sizeof(char), 1, picture_in))
    {
        if (i == stop) break;
        fwrite(&PicElem, sizeof(char), 1, picture_out);
        i++;
    }

    printf("\nEncoding completed successfully.");
    fclose(picture_in);
    fclose(picture_out);
    _getch();
    return 0;
}
