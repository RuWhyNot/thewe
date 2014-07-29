#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Region.h"

class Cell
{
public:
	typedef std::shared_ptr<Cell> Ptr;
public:
	int GetMembersCount() const;
	float GetCash() const;
	float GetMoralValue() const;
	float GetContentment() const;
private:
	/** ���������� ������ ������ */
	int _membersCount;
	/** �������� �������� */
	float _cash;
	/** ������� ������ */
	float _moralValue;
	/** ���������������� */
	float _contentment;

	/** ������, � ������� ��������� ������ */
	Region::Ptr _region;
};

#endif // CELL_H