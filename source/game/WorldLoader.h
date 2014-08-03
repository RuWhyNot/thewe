#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

#include <string>
#include <pugixml.hpp>

#include "Region.h"
#include "PlayersProfiles.h"

namespace WorldLoader
{
	/* ��������� �� ��������� �� ������ ������ �����*/
	bool LoadWorld(void);
	
	/* ��������������� ��������� ���� �� ����� */
	bool LoadGameState(void);

	/* ��������� ��������� ���� � ������� �������� ������, ���� ����� ���������� */
	bool SaveGameState(void);

	/* ���������� ��������� ���� (����� ����\������ ������) */
	void FlushGameState(void);
};

#endif