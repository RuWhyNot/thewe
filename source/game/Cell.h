#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Region.h"
#include "Point.h"

class Cell
{
public:
	typedef std::shared_ptr<Cell> Ptr;
public:
	Cell(Point location);

	int GetMembersCount() const;
	float GetCash() const;
	float GetMoralValue() const;
	float GetContentment() const;

	Point GetLocation();

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

	/** �������������� ������������ ������ �� ����� */
	Point _worldLocation;
};

#endif // CELL_H