#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "../../Engine/BMPImage.h"
#include "../Engine/Engine.h"
#include "../Engine/Entity/Entity.h"
#include "../Engine/EntityComponents/Drawables/ShaderComponent.h"
#include "../Engine/EntityComponents/Drawables/RectangleComponent.h"

class World : public Entity
{
public:
	void EntityInit() override;
	void Scale(float factor);
	void Setup(unsigned int width, unsigned int height);
	void Setup(const char* bmpPath);
	void UpdateTexture();
	void Colorize(bool useColor);

	int* GetPixelValue(const sf::Vector2u& worldPositon);
	sf::Vector2u ScreenToWorldPosition(sf::Vector2u screenPosition);
	sf::Vector2i WorldToScreenPosition(sf::Vector2u worldPosition);

	bool* GetNeighbours(int index, bool onlyLeftandRight = false); //TODO: add onlyUpAndDown (e.g. for gras)
	bool* GetNeighbours2(int index, bool onlyLeftandRight = false);
	bool* GetNeighbours3(int index, bool onlyLeftandRight = false);
	bool* GetNeighbours4(int index, bool onlyLeftandRight = false);

	// During gameplay
	void TryDestroyPixel(int index);

	std::vector<int> m_pixelValues;
	std::unique_ptr<sf::Uint8[]> m_pixelColors;

	int m_worldWidth;
	int m_worldHeight;
	int m_worldImageBorders[4];


private:
	void UpdateBorders();
	void DestroyDerived() override;

	int m_numOfPixels;
	const int m_grasDepth = 2;
	const int m_caveStoneDepth = 3;
	
	bool m_neighbours[9];
	bool m_neighbours2[9];
	bool m_neighbours3[9];
	bool m_neighbours4[9];

	BMPImage m_bmpGroundTile = BMPImage(0, 0);
	bmp::Vector2 m_groundTileSize;

	const char* m_pathBmpGroundTile = "../Resources/bmp/DontDelete/GroundTile.bmp";
	std::string vertPath = "../Resources/Shaders/World_Image.vert";
	std::string fragPath = "../Resources/Shaders/World_Image.frag";

	sf::Texture m_texture;
	sf::VertexArray m_shaderSpaceLocal;
	RectangleComponent* m_rectangleComponent = nullptr;
	ShaderComponent* m_shaderComponent = nullptr;
	sf::Vector2u m_shaderSpaceSize;
};


// TODO: make type-sizes fixed (e.g. sf::Int32 instead int)
inline sf::Packet& operator <<(sf::Packet& packet, const std::vector<int>& vector)
{
	int size = vector.size();
	packet << size;
	
	for (int i = 0; i < vector.size(); i++)
	{
		packet << vector[i];
	}
	return packet;
}

inline sf::Packet& operator >>(sf::Packet& packet, std::vector<int>& vector)
{
	int value;
	int size;
	packet >> size;
	vector.clear();
	for (int i = 0; i < size; i++)
	{
		packet >> value;
		vector.push_back(value);
	}
	return packet;
}

