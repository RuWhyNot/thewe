#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Town.h"
#include "Vector2.h"
#include "Task.h"

class Cell
{
public:
	struct Info
	{
		Cell *parent;
		Town::WeakPtr town;
		Vector2 location;

		float cash;
		float morale;
		float contentment;

		int membersCount;
	};

	typedef std::shared_ptr<Cell> Ptr;
	typedef std::weak_ptr<Cell> WeakPtr;

public:
	Cell(const Info &info);

	/**
	 * ������� Cell � ������� ����� ���������
	 */
	static Ptr Create(const Info &info);
	
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

	void AddCurrentTask(Task::WeakPtr currentTask);
	Task::WeakPtr getCurrentTask() const;

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

	/** ��������� �������� � ������� � ������ ��� �������������� */
	void _CheckValues() const;

	/** ��� ������ ���������� � ������ */
	Info _info;

	/** �������� ������ */
	std::vector<Cell::Ptr> _childCells;

	/** �������, ������� ������ ����������� */
	Task::WeakPtr _currentTask;

	/** ������ ����������� ������� */
	std::vector<Task::CompletedTaskInfo> _completedTasks;

	/** �������������� ������������ ������ �� ����� */
	Vector2 _worldLocation;
};

#endif // CELL_H