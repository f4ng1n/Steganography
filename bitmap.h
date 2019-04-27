#pragma pack(push, 1)
typedef struct
{
	unsigned char b1, b2;
	unsigned long bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
}BITMAPFILEHEADER;

typedef struct BitMap
{
	short m_signature;
	long m_reserved1;
	long m_reserved2;
	long m_dataOffSet;
	unsigned int btSize;
	int biWidth;
	int biWeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	int biClrImportant;
}BITMAPINOFHEADER;
#pragma pack(pop)
