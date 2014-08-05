#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Region.h"
#include "Point.h"

class Cell
{
public:
	struct Info
	{
		Cell *parent;
		Region::Ptr region;
		Point location;

		float cach;
		float morale;
		float contentment;

		int membersNum;
	};

	typedef std::shared_ptr<Cell> Ptr;

public:
	Cell(Point location);
	Cell(const Info &info);

	int GetMembersCount() const;
	float GetCash() const;
	float GetMoralValue() const;
	float GetContentment() const;

	Point GetLocation();
	
	Cell* GetParent() const;
	
	/**
	 * ��������� ������ ����� �������� ������
	 * ������ ����� ������������� ��� ������������ � �������� ������
	 */
	void AddChild(Cell::Ptr cell);

	/**
	 * ������� �������� ������
	 * ������ � nullptr �������� ������������ ������ � ��������
	 */
	void RemoveChild(Cell::Ptr cell);

	/** ���������� ��� �������� ������ */
	const std::vector<Cell::Ptr>& GetChildren();

	/**
	* ���������� ���������� � ������
	*/
	Info GetInfo(void) const;

private:
	/** ���������� ������ �������� */
	void _SetParent(Cell* cell);
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

	/** ������������ ������ (����� ������������� ��������) */
	Cell *_parentCell;

	/** �������� ������ */
	std::vector<Cell::Ptr> _childCells;

	/** �������������� ������������ ������ �� ����� */
	Point _worldLocation;
};

#endif // CELL_H