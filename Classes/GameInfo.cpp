#include "GameInfo.h"

#include "Log.h"

#include <pugixml.hpp>
#include <cocos2d.h>

GameInfo& GameInfo::Instance(void)
{
	static GameInfo singleGameInfo;
	return singleGameInfo;
}

bool GameInfo::ParseXml(const std::string &filename)
{
	std::string fullPath;
	unsigned char* buffer = NULL;
	ssize_t bufferSize = 0;

	fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);
	buffer = cocos2d::FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &bufferSize);

	if (!buffer)
	{
		Log::Instance().writeWarning("Failed to get file data: " + fullPath);
		return false;
	}

	pugi::xml_document gameInfo;

	if (gameInfo.load_buffer(buffer, bufferSize))
	{
		pugi::xml_node root = gameInfo.first_child();
		pugi::xml_node node = root.first_child();

		while (node)
		{
			std::string name = node.attribute("name").as_string();
			std::string type = node.attribute("type").as_string();

			if (type == "int" || type == "bool")
			{
				int value = node.attribute("value").as_int();
				_intMap.insert(std::pair<std::string, int>(name, value));
			}
			else if (type == "float")
			{
				float value = node.attribute("value").as_float();
				_floatMap.insert(std::pair<std::string, float>(name, value));
			}
			else if (type == "string")
			{
				std::string value = node.attribute("value").as_string();
				_strMap.insert(std::pair<std::string, std::string>(name, value));
			}
			else
			{
				std::string message = "Failed to read global constant of name: " + name + ", type: " + type;
				Log::Instance().writeWarning(message);
			}

			node = node.next_sibling();
		}

		return true;
	}
	else
	{
		Log::Instance().writeWarning("Failed to parse file: " + fullPath);
		return false;
	}
}

int GameInfo::GetInt(const std::string &name, int def) const
{
	std::map<std::string, int>::const_iterator it;
	it = _intMap.find(name);
	return it != _intMap.end() ? (*it).second : def;
}

bool GameInfo::GetBool(std::string &name, bool def) const
{
	std::map<std::string, int>::const_iterator it;
	it = _intMap.find(name);
	return it != _intMap.end() ? (*it).second != 0 : def;
}

float GameInfo::GetFloat(const std::string &name, float def) const
{
	std::map<std::string, float>::const_iterator it;
	it = _floatMap.find(name);
	return it != _floatMap.end() ? (*it).second : def;
}

std::string GameInfo::GetString(const std::string &name, std::string def) const
{
	std::map<std::string, std::string>::const_iterator it;
	it = _strMap.find(name);
	return it != _strMap.end() ? (*it).second : def;
}