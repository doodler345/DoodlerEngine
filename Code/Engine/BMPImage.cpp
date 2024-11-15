#include "BMPImage.h"

#include <iostream>
#include <string>
#include <fstream>

#include "Debug.h"

namespace bmp
{

	Color::Color()
		: r(0), g(0), b(0)
	{
	}

	Color::Color(bool isWhite)
		: r(isWhite), g(isWhite), b(isWhite)
	{
	}

	Color::Color(float r, float g, float b)
		: r(r), g(g), b(b)
	{
	}

	Color::~Color()
	{
	}
}


BMPImage::BMPImage()
{
	m_size.x = 0;
	m_size.y = 0;
}

BMPImage::BMPImage(int width, int height)
	: m_colors(std::vector<bmp::Color>(width* height))
{
	m_size.x = width;
	m_size.y = height;
}

BMPImage::~BMPImage()
{
}

bmp::Color BMPImage::GetColor(int x, int y) const
{
	return m_colors[(y) * m_size.x + x];
}

void BMPImage::SetColor(const bmp::Color& color, int x, int y)
{
	int i = (y)* m_size.x + x;
	m_colors[i].r = color.r;
	m_colors[i].g = color.g;
	m_colors[i].b = color.b;
}

void BMPImage::SetColor(const bool isWhite, int x, int y)
{
	int i = (y)* m_size.x + x;
	m_colors[i].r = isWhite;
	m_colors[i].g = isWhite;
	m_colors[i].b = isWhite;
}

void BMPImage::Read(const char* path)
{
	std::ifstream f;
	f.open(path, std::ios::in | std::ios::binary);

	if (!f.is_open())
	{
		DebugPrint("Can not read BMP Image", TextColor::Red, DebugChannel::ResourceManager, __FILE__, __LINE__, true);
		return;
	}

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;

	unsigned char fileHeader[fileHeaderSize];
	f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

	if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
	{
		DebugPrint("File you try to read is not a BMP", TextColor::Red, DebugChannel::ResourceManager, __FILE__, __LINE__, true);
		return;
	}

	unsigned char informationHeader[informationHeaderSize];
	f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
	m_size.x = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
	m_size.y = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

	m_colors.resize(m_size.x * m_size.y);

	const int paddingAmount = (4 - ( m_size.x * 3) % 4) % 4;

	for (int y = 0; y < m_size.y; y++)
	{
		for (int x = 0; x < m_size.x; x++)
		{
			unsigned char color[3];
			f.read(reinterpret_cast<char*>(color), 3);

			m_colors[(m_size.y - 1 - y) * m_size.x + x].r = static_cast<float>(color[2]) / 255.0f;
			m_colors[(m_size.y - 1 - y) * m_size.x + x].g = static_cast<float>(color[1]) / 255.0f;
			m_colors[(m_size.y - 1 - y) * m_size.x + x].b = static_cast<float>(color[0]) / 255.0f;
		}

		f.ignore(paddingAmount);
	}

	f.close();

	DebugPrint("BMP File read", TextColor::Green, DebugChannel::ResourceManager, __FILE__, __LINE__, false);
}

void BMPImage::Export(const char* path) const
{
	std::string path_string(path);
	if (path_string.substr(path_string.length() - 4, 4) != ".bmp")
	{
		DebugPrint("Please save as .bmp file", TextColor::Red, DebugChannel::ResourceManager, __FILE__, __LINE__, true);
		return;
	}

	std::ofstream f;
	f.open(path, std::ios::out | std::ios::binary);

	if (!f.is_open())
	{
		DebugPrint("Can not open file", TextColor::Red, DebugChannel::ResourceManager, __FILE__, __LINE__, true);
		return;
	}

	unsigned char bmpPaddings[3] = { 0,0,0 };
	const int paddingAmount = (4 - ( m_size.x * 3) % 4) % 4;

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int fileSize = fileHeaderSize + informationHeaderSize + m_size.x * m_size.y * 3 + paddingAmount * m_size.y;

	unsigned char fileHeader[fileHeaderSize];

	// File type
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	// File size
	fileHeader[2] = fileSize;
	fileHeader[3] = fileSize >> 8;
	fileHeader[4] = fileSize >> 16;
	fileHeader[5] = fileSize >> 24;
	// Reserved 1 (Not Used)
	fileHeader[6] = 0;
	fileHeader[7] = 0;
	// Reserved 2 (Not Used)
	fileHeader[8] = 0;
	fileHeader[9] = 0;
	// Pixel data offset
	fileHeader[10] = fileHeaderSize + informationHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];

	// Header Size
	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;
	// Image width
	informationHeader[4] = m_size.x;
	informationHeader[5] = m_size.x >> 8;
	informationHeader[6] = m_size.x >> 16;
	informationHeader[7] = m_size.x >> 24;
	// Image Height
	informationHeader[8] = m_size.y;
	informationHeader[9] = m_size.y >> 8;
	informationHeader[10] = m_size.y >> 16;
	informationHeader[11] = m_size.y >> 24;
	// Planes
	informationHeader[12] = 1;
	informationHeader[13] = 0;
	// Bits per Pixel (RGB)
	informationHeader[14] = 24;
	informationHeader[15] = 0;
	// Compression (No compression)
	informationHeader[16] = 0;
	informationHeader[17] = 0;
	informationHeader[18] = 0;
	informationHeader[19] = 0;
	// Image size (No compression)
	informationHeader[20] = 0;
	informationHeader[21] = 0;
	informationHeader[22] = 0;
	informationHeader[23] = 0;
	// X pixels per meter (Not specified)
	informationHeader[24] = 0;
	informationHeader[25] = 0;
	informationHeader[26] = 0;
	informationHeader[27] = 0;
	// Y pixels per meter (Not specified)
	informationHeader[28] = 0;
	informationHeader[29] = 0;
	informationHeader[30] = 0;
	informationHeader[31] = 0;
	// Total colors (Color palette not used)
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;
	// Important colors (Generally ignored)
	informationHeader[36] = 0;
	informationHeader[37] = 0;
	informationHeader[38] = 0;
	informationHeader[39] = 0;

	f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	for (int y = 0; y < m_size.y; y++)
	{
		for (int x = 0; x < m_size.x; x++)
		{
			unsigned char r = static_cast<unsigned char>(GetColor(x, (m_size.y - 1 - y)).r * 255.0f);
			unsigned char g = static_cast<unsigned char>(GetColor(x, (m_size.y - 1 - y)).g * 255.0f);
			unsigned char b = static_cast<unsigned char>(GetColor(x, (m_size.y - 1 - y)).b * 255.0f);

			unsigned char color[] = { b,g,r };

			f.write(reinterpret_cast<char*>(color), 3);
		}

		f.write(reinterpret_cast<char*>(bmpPaddings), paddingAmount);
	}

	f.close();

	DebugPrint("BMP File created", TextColor::Green, DebugChannel::ResourceManager, __FILE__, __LINE__, false);

}
