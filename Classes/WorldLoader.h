#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

namespace WorldLoader
{
	/* ��������� �� ��������� �� ������ ������ �����*/
	bool LoadGameInfo(void);
	
	/* ��������������� ��������� ���� �� ����� */
	bool LoadGameState(void);

	/* ��������� ��������� ���� � ������� �������� ������, ���� ����� ���������� */
	bool SaveGameState(void);

	/* ���������� ��������� ���� (����� ����\������ ������) */
	void FlushGameState(void);
};

#endif // WORLD_LOADER_H