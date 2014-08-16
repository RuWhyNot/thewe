#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Town.h"
#include "Point.h"
#include "Task.h"

class Cell
{
public:
	struct Info
	{
		Cell *parent;
		Town::Ptr town;
		Point location;

		float cash;
		float morale;
		float contentment;

		int membersNum;
	};

	typedef std::shared_ptr<Cell> Ptr;

public:
	Cell(const Info &info);
	
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
	const std::vector<Cell::Ptr>& GetChildrens() const;

	/**
	* ���������� ���������� � ������
	*/
	const Info& GetInfo(void) const;
	
	/** ��������� ��������� ������ */
	void Update(float deltatime);

	/** ��������� ������� ��� ����������� */
	void AddCompletedTask(const Task::CompletedTaskInfo& completedTask);
private:

	/** ���������� ������ �������� */
	void _SetParent(Cell* cell);

	/** ����������� �������� �� �������� */
	void _UpdateCash(float deltatime);
	/** ����������� ���������������� �� �������� */
	void _UpdateMorale(float deltatime);
	/** ����������� ���������������� �� �������� */
	void _UpdateContentment(float deltatime);

	/** ��� ������ ���������� � ������ */
	Info _info;

	/** �������� ������ */
	std::vector<Cell::Ptr> _childCells;

	/** ������ ����������� ������� */
	std::vector<Task::CompletedTaskInfo> _completedTasks;

	/** �������������� ������������ ������ �� ����� */
	Point _worldLocation;
};

#endif // CELL_H