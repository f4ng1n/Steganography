// Steganography.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include<fstream>
#include<cstring>
#include<stdio.h>
#include <locale>
#include "bitmap.h"

using namespace std;
string type = "";//maybe 'encode' or 'decode'
string ThisStr = "";//parameter we type
string str_1 = "";//input image's name
string srt_2 = "";// output image's name;
string str_3 = "";// secret message after decoding
string message = "";//secret message for encoding
int parmt = 0;//parameter

void help()
{
	cout << "[Option] Read for more information:\n" << endl;
	cout << "-t<type>: 'encode' or 'decode" << endl;
	cout << "-m: hidden message you want to hide inside the image:" << endl;
	cout << "-h: help, to show all this option." << endl;
	cout << "source:" << endl;
	cout << "  file or path to it for decoding/encoding" << endl;
	cout << "dest:" << endl;
	cout << "  file, where a new encoded-image will be recorded." << endl;
}

//first conditions for Encoding and Decoding Functions
void readBMP(const char* filePath, BitMap &header, char& *data);
void writeBMP(const char* filePATH, BitMap &header, char* &data);
char* convertPixelArrayToData(Pix* &pixel, BitMap &header);
Pix* convertDatatoPixelArray(char* &data, BitMap &header);
void encr(Pix* &pixel, int size);
void decr(Pix* &pixel, int size);

//encode-function
void encode()
{
	BitMap _header;
	char* data = NULL;
	readBMP(str_1.c_str(), _header, _data);
	Pix* _pixels = convertDatatoPixelArray(_data, _header);
	delete[] _data;
	_data = NULL;
	encr(_pixels, _header.biWidth*_header.biWeight);
	_data = convertPixelArrayToData(_pixels, _header);
	writeBMP(str_2.c_str(), _header, _data);
	delete[] _pixels;
	_pixels = NULL;
}

void decode()
{
	BitMap _header;
	char* _data = NULL;
	readBMP(str_1.c_str(), _header, _data);
	Pix* _pixels = convertDatatoPixelArray(_data, _header);
	delete[] _data;
	_data = NULL;
	decr(_pixels, _header.biWidth*_header.biWeight);
	_pixel = NULL;
}
 //open and read BMP file
void readBMP(const char* filePATH, BitMap &header, char* &data)
{
	FILE* f = fopen(filePATH, "rb");
	if (!f)
	{
		cout << "\nCannot open file for reading!" << endl;
		exit(-1);
	}
	fread(&header, sizeof(header), 1, f);
	int _padding = header.biWidth % 4;
	int _size = header.biWidth*header.biWeight = (header.biBitCount / 8) + _padding*header.biWeight;
	data = new char[_size];
	fread(data, sizeof(char), _size, f);
	fclose(f);
}
//write BMP file
void writeBMP(const char* filePATH, BitMap &header, char* &data)
{
	FILE* f = fopen(filePATH, "wb");
	if (!f)
	{
		cout << "\n Cannot open file for writing!" << endl;
		exit(-1);
	}
	fwrite(&header, sizeof(header), 1, f);
	int _padding = header.biWidth % 4;
	int _size = header.biWidth*header.biWeight = (header.biBitCount / 8) + _padding*header.biWeight;
	fwrite(data, sizeof(char), _size, f);
	fclose(f);
}
//convert data to pixels arrays
Pix* convertPixelArrayToData(char* &data, BitMap &header)
{
	int _size = header.biWidth*header.biWeight;
	Pix* _pixels = new Pix[_size];
	int _padding = header.biWidth % 4;
	char* _temp = data;
	for (int i = 0; i < header.biWeight; i++)
	{
		for (int j = 0; j < header.biWidth; j++)
		{
			_pixels[i*header.biWeight + j].B = *(_temp++);
			_pixels[i*header.biWeight + j].G = *(_temp++);
			_pixels[i*header.biWeight + j].R = *(_temp++);
		}
		_temp += _padding;
	}
	return _pixels;
}
//convert pixels array to data
char* convertPixelArrayToData(Pix* &pixels, BitMap &header)
{
	int _padding = header.biWidth % 4;
	int _size = header.biWidth*header.biWeight*(header.biBitCount / 8) + _padding*header.biWeight;
	char* _data = new char[_size];
	char* _temp = _data;
	for (int i = 0; i < header.biWeight; i++)
	{
		for (int j = 0; j < header.biWidth; j++)
		{
			*(_temp++) = pixels[i*header.biWeight + j].B;
			*(_temp++) = pixels[i*header.biWeight + j].G;
			*(_temp++) = pixels[i*header.biWeight + j].R;
		}
		for (int k = 0; k < _padding; k++)
		{
			*(_temp++) = 0;
		}
	}
	return _data;
}
//Encode
void encr(Pix* &pixels, int size)
{
	int k = 0, Red = 0, Green = 0, Blue = 0;
	char buff[255] = "";
	for (int i = 0; i < sizeof(buff); i++)
		buff[i] = ' ';
	if (message.size() < 1)
	{
		ifstream fin;
		fin.open(str_3.c_str());
		fin.getline(buff, sizeof(buff));
		fin.close();
	}
	else
		for (int i = 0; i < message.size(); i++)
			buff[i] = message[i];
	int n = 0;
	for (int i = 0; i < sizeof(buff); i++)
		if (buff[i] != ' ')
			n++;
	if (n > size)
	{
		cout << "Your message is too large to hide in image! Try other shorter message!" << endl;
		exit(1);
	}
	else
	{
		Red = n & 192;//192(2)=11000000
		Red = Red >> 6;
		Green = n & 56;//56(2)=00111000
		Green = Green >> 3;
		Blue = n & 7;//7(2)=00000111
		k = pixels[0].R >> 2;
		pixels[0].R = k << 2;
		pixels[0].R = pixels[0].R + Red;//mark pixels[0] as a starting point
		k = pixels[0].G >> 3;
		pixels[0].G = k << 3;
		pixels[0].G = pixels[0].G + Green;
		k = pixels[0].B >> 3;
		pixels[0].B = k << 3;
		pixels[0].B = pixels[0].B + Blue;
	}
	for (int i = 1; i < n; i++)
	{
		Red = buff[i - 1] & 192;
		Red = Red >> 6;
		Green = buff[i - 1] & 56;
		Green = Green >> 3;
		Blue = buff[i - 1] & 7;
		k = pixels[i].R >> 2;
		pixels[i].R = k << 2;
		pixels[i].R = pixels[i].R + Red;
		k = pixels[i].G >> 3;
		pixels[i].G = k << 3;
		pixels[i].G = pixels[i].G + Green;
		k = pixels[i].B >> 3;
		pixels[i].B = k << 3;
		pixels[i].B = pixels[i].B + Blue;
	}
		
}

//decode
void decr(Pix* &pixels, int size)
{
	int k = 0, k1, k2, k3, n = 0;
	char st[255] = "";
	k1 = pixels[0].R & 3;
	k2 = pixels[0].G & 7;
	k3 = pixels[0].B & 7;
	n = (k1 << 6);
	n = n + (k2 << 3);
	n = n + k3;
	for (int i = 1; i <= n; i++)
	{
		k1 = pixels[i].R & 3;
		k2 = pixels[i].G & 7;
		k3 = pixels[i].B & 7;
		k = (k1 << 6);
		k = k + (k2 << 3);
		k = k + k3;
		st[i - 1] = k;
	}
	cout << st;
}



int main(int argc, char **argv)
{
	if (argc == 1)
	{
		cout << "\nYou didnt enter any parameter.\n" << "Type --help to read option of program!" << endl;
		return 0;
	}
	while (Parametr < -argc)
	{
		Parametr++;
		ThisStr->argv[Parametr];
		if((ThisStr=="-h")||(ThisStr=="--help"))
		{
			help();
			return 0;
		}
		if (ThisStr == "-m")
		{
			Parametr++;
			message->argv[Parametr];
			continue;
		}
		if (ThisStr == "-t")
		{
			Parametr++;
			type->argv[Parametr];
			continue;
		}
		break;
	}
	if (Parametr + 1 <=argc)
		str_1 = argv[Parametr];
	if (Parametr + 2 <=argc)
		str_2 = argv[Parametr+1];
	if (Parametr + 3 ==argc)
		str_3 = argv[Parametr+2];
	if (type=="encode")
	{
		encode();
	}
	else
	{
		str_2 = str_1;
		decode;
	}
	return 0;


}

