#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>

struct Task
{
public:
	typedef std::shared_ptr<Task> Ptr;
	typedef std::weak_ptr<Task> WeakPtr;
	
	/** ����������� ���������� � ������ */
	struct Info
	{
		/** ������������� */
		std::string id;
		/** ��������� */
		float severity;
		/** ����������������� */
		float duration;
		float moralLevel;

		/** ��� Lua-�������, ������� ����� ��������� ��� �������� ������ */
		std::string successFn;
		/** ��� Lua-�������, ������� ����� ��������� ��� ������� ������� */
		std::string failFn;
		/** ��� Lua-�������, ������� ����� ��������� ��� ���������� ������� */
		std::string abortFn;
	};

	struct CompletedTaskInfo
	{
		const Info* taskInfo;
		float startTime;
		float endTime;
	};

public:
	Task(const Task::Info* info, float startTime);
	
	static Task::Ptr Create(const Task::Info* info, float startTime);

	/**
	 * ���������, �� ����������� �� �������
	 *
	 * @return true ���� ������� ��� ���������
	 */
	bool CheckComleteness(float worldTime);

	const Info* GetInfo() const;

	float GetStartTime() const;
	float GetEndTime() const;

private:
	/** ����� ������ ���������� ������� */
	float _startTime;
	/** ����� ��������� ���������� ������� */
	float _endTime;

	const Task::Info* _info;
};

#endif // TASK_H