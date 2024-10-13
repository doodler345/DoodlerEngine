#include "ResourceManager.h"
#include "Debug.h"

void ResourceManager::StartUp()
{
}

void ResourceManager::ShutDown()
{
}

sf::Texture ResourceManager::RegisterTexture(std::string path)
{
	if (m_registeredTextures.count(path) == 0)
	{
		Resource resource;
		if (!resource.m_texture.loadFromFile(path))
		{
			DebugPrint("not found, cannot load from " + path, TextColor::Red, DebugChannel::ResourceManager, __FILE__, __LINE__, 1);
		}
		else
		{
			DebugPrint("loaded sucessfully from " + path, TextColor::LightYellow, DebugChannel::ResourceManager, __FILE__, __LINE__);
			resource.m_id = m_idCounter++;
			m_registeredTextures[path] = resource;
		}
		return resource.m_texture;
	}
	else
	{
		DebugPrint("already exists from " + path, TextColor::LightYellow, DebugChannel::ResourceManager, __FILE__, __LINE__);
		return m_registeredTextures[path].m_texture;
	}
}

sf::Texture ResourceManager::UpdateTexture(std::string path)
{
	if (m_registeredTextures.count(path) == 0)
	{
		DebugPrint("Texture at " + path + " doesn't exist yet, trying to register...", TextColor::Red, DebugChannel::ResourceManager, __FILE__, __LINE__, 1);
		RegisterTexture(path);
	}
	else
	{
		DebugPrint("Texture at " + path + " updated", TextColor::LightYellow, DebugChannel::ResourceManager, __FILE__, __LINE__);
		m_registeredTextures[path].m_texture.loadFromFile(path);
	}
	return m_registeredTextures[path].m_texture;
}

sf::Font ResourceManager::RegisterFont(std::string path)
{
	if (m_registeredFonts.count(path) == 0)
	{
		Resource resource;
		if (!resource.m_font.loadFromFile(path))
		{
			DebugPrint("not found, cannot load from " + path, TextColor::Red, DebugChannel::ResourceManager, __FILE__, __LINE__, 1);
		}
		else
		{
			DebugPrint("loaded sucessfully from " + path, TextColor::LightYellow, DebugChannel::ResourceManager, __FILE__, __LINE__);
			resource.m_id = m_idCounter++;
			m_registeredFonts[path] = resource;
		}
		return resource.m_font;
	}
	else
	{
		DebugPrint("already exists from " + path, TextColor::LightYellow, DebugChannel::ResourceManager, __FILE__, __LINE__);
		return m_registeredFonts[path].m_font;
	}
}
