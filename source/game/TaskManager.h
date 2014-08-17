#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include "Cell.h"
#include "Task.h"

class TaskManager
{
public:
	typedef const std::vector<const Task::Info*> TasksList;

public:
	/**
	 * ���������� ��������� ������
	 */
	static TaskManager& Instance();

public:
	/**
	 * ��������� ����� ������� ��� ������
	 * 
	 * @param cell ������ ��� ������� ����������� �������
	 * @param info ���������� � �������
	 * @param startTime ������� �����, ������� ����� �������
	 * �������� ������ ���������� ������
	 */
	void RunTask(Cell::Ptr &cell, const Task::Info* info, float startTime);
	
	/**
	 * ���������� ������ �����, ������� ������� ��� ������ ������
	 */
	TasksList GetAvailableTasks(Cell* cell) const;

	/** �������� ���������� � �������� �������� ������� */
	void UpdateToTime(float worldTime);

	void FillTasks(const std::vector<Task::Info>& tasks);
private:
	/** ���������� � ���������� ������ */
	struct RunnedTaskInfo
	{
		/** ������, ������� ��������� ������� */
		Cell::Ptr cell;
		/** ������� */
		Task::Ptr task;
	};

private:
	/** ���������� � ���� ��������� �������� (����������� ���� ���) */
	std::map<const std::string, const Task::Info> _allTasks;
	/** ����, ������������ �������� �� ������ ���� ��������� ����� */
	bool isTasksFilled;

	/** �������, ������� ������ ����������� */
	std::vector<RunnedTaskInfo> _runnedTasks;

private:
	/*
	 * ��������� �������� ��������
	 */
	TaskManager();
	~TaskManager();
	TaskManager(const TaskManager&);
	void operator=(const TaskManager&);
};

#endif // TASK_MANAGER_H
