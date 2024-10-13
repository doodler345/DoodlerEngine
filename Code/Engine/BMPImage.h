#pragma once

#include <vector>

namespace bmp
{
	struct Color
	{
		float r, g, b;

		Color();
		Color(float r, float g, float b);
		~Color();
	};

	struct Vector2
	{
		int x;
		int y;
	};
}

class BMPImage
{
public:
	BMPImage(int windowWidth, int height);
	~BMPImage();

	bmp::Color GetColor(int x, int y) const;
	void SetColor(const bmp::Color& color, int x, int y);
	void SetColor(const bool isWhite, int x, int y);

	bmp::Vector2 GetSize() { return m_size; }

	void Read(const char* path);
	void Export(const char* path) const;

private:
	bmp::Vector2 m_size;
	std::vector<bmp::Color> m_colors;

};

