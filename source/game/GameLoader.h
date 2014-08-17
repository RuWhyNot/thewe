#ifndef GAME_LOADER_H
#define GAME_LOADER_H

namespace GameLoader
{
	/* ��������� �� ��������� �� ������ ������ ���� */
	bool LoadGameInfo(void);
	
	/* ��������������� ��������� ���� �� ����� */
	bool LoadGameState(void);

	/* ��������� ��������� ���� � ������� �������� ������, ���� ����� ���������� */
	bool SaveGameState(void);

	/* ���������� ��������� ���� (����� ����\������ ������) */
	void FlushGameState(void);
};

#endif // GAME_LOADER_H