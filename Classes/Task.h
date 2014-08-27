#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>

struct Task
{
public:
	typedef std::shared_ptr<Task> Ptr;
	typedef std::weak_ptr<Task> WeakPtr;
	
	/** ������ ������ */
	enum class Status
	{
		Runned
		, Successed
		, Failed
		, Aborted
	};

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
		Status status;
	};

public:
	Task(const Task::Info* info, float startTime);
	
	static Ptr Create(const Info* info, float startTime);

	/**
	 * ���������, �� ����������� �� �������
	 *
	 * @return true ���� ������� ��� ���������
	 */
	bool CheckCompleteness(float worldTime);

	const Info* GetInfo() const;

	float GetStartTime() const;
	float GetEndTime() const;
	float CalculateProgress(float worldTime) const;
	bool IsAborted() const;
	bool IsFastFinished() const;
	/** �������� ���������� ������� */
	void Abort();
	/** ��������� ������� ������ (�������� ����������) */
	void FastFinish();

private:
	/** ����� ������ ���������� ������� */
	float _startTime;
	/** ����� ��������� ���������� ������� */
	float _endTime;

	bool _isAborted;
	bool _isFastFinished;

	const Task::Info* _info;
};

#endif // TASK_H