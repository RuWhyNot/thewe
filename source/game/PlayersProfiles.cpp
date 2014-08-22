#include "PlayersProfiles.h"

#include <pugixml.hpp>
#include <cocos2d.h>
#include "Point.h"
#include "ArbitraryHull.h"
#include "Log.h"

static const std::string profilesSettingsPath = "../_gamedata/saves/profiles.xml"; 

ProfilesManager::ProfilesManager()
	: _currentProfile(NULL)
{
}

ProfilesManager::~ProfilesManager()
{
}

void ProfilesManager::LoadProfiles(void)
{
	pugi::xml_document doc;
	pugi::xml_parse_result successfulLoaded = doc.load_file(profilesSettingsPath.c_str());

	if (!successfulLoaded)
	{
		Log::Instance().writeError(std::string("Can't find file '").append(profilesSettingsPath).append("'"));
	}

	pugi::xml_node root = doc.first_child();
	pugi::xml_node node = root.first_child();

	while (node)
	{
		PlayerProfile profile;
		profile.playerName = node.attribute("name").as_string();
		profile.gameStateFilename = node.attribute("path").as_string();
		profile.totalPlayingTime = node.attribute("time").as_float();

		_profilesMap.insert(ProfileMapPair(profile.playerName, profile));

		node = node.next_sibling();
	}

	std::string key = (root.attribute("current").as_string());
	ProfileMap::iterator it = _profilesMap.find(key);
	
	if (it != _profilesMap.end())
	{
		_currentProfile = &it->second;
	}
}

void ProfilesManager::SaveProfiles(void)
{
}

ProfilesManager& ProfilesManager::Instance(void)
{
	static ProfilesManager instance;
	return instance;
}

void ProfilesManager::AddNewProfile(const std::string &name)
{
	// TODO: create and make as current.
}

void ProfilesManager::RemoveProfile(const std::string &name)
{
}