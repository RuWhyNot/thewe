#include "TaskManager.h"

#include <fstream>
#include <iostream>

#include "Log.h"

#include <cocos2d.h>

TaskManager::TaskManager()
{
	isTasksFilled = false;
}

TaskManager::~TaskManager()
{
}

TaskManager& TaskManager::Instance()
{
	static TaskManager singleInstance;
	return singleInstance;
}

void TaskManager::RunTask(Cell::WeakPtr &cell, const Task::Info* info, float startTime)
{
	RunnedTaskInfo runnedTaskInfo;
	runnedTaskInfo.cell = cell;
	runnedTaskInfo.task = Task::Create(info, startTime);
	cell.lock()->AddCurrentTask(runnedTaskInfo.task);

	_runnedTasks.push_back(runnedTaskInfo);
}

void TaskManager::UpdateToTime(float worldTime)
{
	std::vector<RunnedTaskInfo>::iterator iterator = _runnedTasks.begin();
	while (iterator != _runnedTasks.end())
	{
		Task* task = iterator->task.get();

		bool isEnded = iterator->task->CheckCompleteness(worldTime);

		if (isEnded)
		{
			Cell::Ptr cell = iterator->cell.lock();
			// ���� Cell ��� �� �������
			if (cell)
			{
				const Cell::Info& cellInfo = cell->GetInfo();
				const Task::Info* taskInfo = task->GetInfo();

				Task::CompletedTaskInfo info;
				info.taskInfo = task->GetInfo();
				info.startTime = task->GetStartTime();
				info.endTime = (task->IsAborted() || task->IsFastFinished()) ? worldTime : task->GetEndTime();
				
				std::string funcName;

				if (!task->IsAborted())
				{
					// �������� ��� ������� ����������� ������� �������
					// TODO: ������� ���������� ����������
					bool isSuccess = true;

					if (isSuccess)
					{
						funcName = taskInfo->successFn;
						info.status = Task::Status::Successed;
					}
					else
					{
						funcName = taskInfo->failFn;
						info.status = Task::Status::Failed;
					}
				}
				else
				{
					funcName = taskInfo->abortFn;
					info.status = Task::Status::Aborted;
				}

				// �������� �� ��� ������ �������
				// TODO: ����������� ������� � ����� �������
				
				// ��������� ���������� � ����������� ������� � ������
				cell->AddCompletedTask(info);
			}

			// ����������� ����� ��������� � ������� Task
			iterator = _runnedTasks.erase(iterator);
		}
		else
		{
			++iterator;
		}
	}
}

void TaskManager::FillTasks(const std::vector<Task::Info>& tasks)
{
	if (isTasksFilled)
	{
		Log::Instance().writeWarning("Trying to fill tasks info twice");
	}

	for (const Task::Info& info : tasks)
	{
		_CheckTask(info);
		_allTasks.insert(std::pair<const std::string, const Task::Info>(info.id, info));
	}

	isTasksFilled = true;
}

TaskManager::TasksList TaskManager::GetAvailableTasks(Cell::WeakPtr cell) const
{
	if (!isTasksFilled)
	{
		Log::Instance().writeError("Trying to acces to not initialized TaskManager");
		assert(false);
	}

	std::vector<const Task::Info*> availableTasks;

	for (const auto& pair : _allTasks)
	{
		// ToDo: ��� ���� �������� ����������� ��������
		availableTasks.push_back(&pair.second);
	}

	return availableTasks;
}

void TaskManager::_CheckTask(const Task::Info& taskInfo) const
{
	if (taskInfo.id == "")
	{
		Log::Instance().writeWarning("Empty task Id");
	}

	if (taskInfo.successFn == "")
	{
		Log::Instance().writeWarning("Empty success function name");
	}

	if (taskInfo.failFn == "")
	{
		Log::Instance().writeWarning("Empty fail function name");
	}

	if (taskInfo.abortFn == "")
	{
		Log::Instance().writeWarning("Empty abort function name");
	}

	if (taskInfo.moralLevel < 0.0f || 1.0f < taskInfo.moralLevel)
	{
		Log::Instance().writeWarning("Wrong moral level");
	}

	if (taskInfo.severity < 0.0f || 1.0f < taskInfo.severity)
	{
		Log::Instance().writeWarning("Wrong severity level");
	}

	if (taskInfo.duration < 0.0f)
	{
		Log::Instance().writeWarning("Negative duration");
	}
}