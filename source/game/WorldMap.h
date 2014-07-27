#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <cocos2d.h>
#include <string>

#include "Region.h"

class WorldMap
{
public:
	/**
	 * ���������� ��������� �����
	 */
	static WorldMap& Instance();

	typedef std::map<std::string, Region::Ptr> RegionsMap;

	Region::Ptr GetRegion(std::string regionName) const;
	// ������� ��� �������
	void CleanupRegions(void);
	void AddRegion(std::string regionName, Region::Ptr region);
	void CreateRegion(std::string regionName);

	const RegionsMap& GetRegions() const;

private:
	std::map<std::string, Region::Ptr> _regions;

	static WorldMap* _singleInstance;

	/*
	 * ��������� �������� ��������
	 */
	WorldMap();
	~WorldMap();
	WorldMap(const WorldMap&);
	void operator=(const WorldMap&);
};

#endif // WORLD_MAP_H
