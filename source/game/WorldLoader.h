#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

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