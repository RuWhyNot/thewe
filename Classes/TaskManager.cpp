#include "TaskManager.h"

#include "Log.h"
#include "World.h"
#include "LuaInstance.h"
#include "MessageManager.h"
#include "GameInfo.h"

#include <luabind/luabind.hpp>

TaskManager::TaskManager()
	: _isTasksFilled(false)
{
}

void TaskManager::RunTask(Cell::WeakPtr cell, const Task::Info* info, Utils::GameTime startTime)
{
	RunnedTaskInfo runnedTaskInfo;
	runnedTaskInfo.cell = cell;
	runnedTaskInfo.task = Task::Create(info, startTime);
	cell.lock()->AddCurrentTask(runnedTaskInfo.task);

	_runnedTasks.push_back(runnedTaskInfo);

	MessageManager::Instance().PutMessage(Message("SaveGame"));
}

void TaskManager::RunTask(Cell::WeakPtr cell, const std::string& id, Utils::GameTime startTime)
{
	const Task::Info* taskInfo = GetTaskInfoById(id);
	if (taskInfo)
	{
		RunTask(cell, taskInfo, startTime);
	}
	else
	{
		WRITE_WARN(std::string("Wrong task id ").append(id));
	}
}

void TaskManager::CallCuccessfulCompletition(Cell::WeakPtr cell, const Task::Info *info)
{
	if (!info || cell.expired()) {
		WRITE_WARN("TaskManager::CallCuccessfulCompletition wrong params");
		return;
	}

	luabind::call_function<bool>(World::Instance().GetLuaInst()->GetLuaState()
								, info->successFn.c_str()
								, cell.lock().get()
								, info
								, 0);
}

Task::Info TaskManager::FindTaskById(const std::string &id) const
{
	Task::Info def;

	std::map<const std::string, const Task::Info>::const_iterator it;
	it = _allTasks.find(id);
	return it != _allTasks.end() ? (*it).second : def;
}

void TaskManager::UpdateToTime(Utils::GameTime worldTime)
{
	std::vector<RunnedTaskInfo>::iterator iterator = _runnedTasks.begin();
	while (iterator != _runnedTasks.end())
	{
		Task* task = iterator->task.get();

		bool isEnded = iterator->task->CheckCompleteness(worldTime);

		if (isEnded)
		{
			Cell::Ptr cell = iterator->cell.lock();
			// if Cell wasn't removed yet
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
					// call lua function that calculate status of the task
					bool isSuccess = luabind::call_function<bool>(World::Instance().GetLuaInst()->GetLuaState()
																  , "CheckTaskStatus"
																  , cellInfo
																  , taskInfo
																  , 0);

					if (isSuccess)
					{
						Utils::GameTime waitTime;
						if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("WaitingForFinishFirstTask") ||
							World::Instance().GetTutorialManager().IsTutorialStateAvailable("ReadyToFinishFirstRealWork")) {
							waitTime = GameInfo::Instance().GetTime("TASK_REWARD_TUTORIAL_WAIT_TIME");
						} else {
							waitTime = GameInfo::Instance().GetTime("TASK_REWARD_WAIT_TIME");
						}

						Message message("PushTaskRewardOnMap");
						message.variables.SetInt("CELL_UID", cell->GetUid());
						message.variables.SetString("TASK_ID", taskInfo->id);
						message.variables.SetTime("WAIT_TIME", waitTime);
						MessageManager::Instance().PutMessage(message);
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

				if (!funcName.empty())
				{
					// call task end function (success, fail, abort)
					luabind::call_function<bool>(World::Instance().GetLuaInst()->GetLuaState()
												 , funcName.c_str()
												 , cell.get()
												 , taskInfo
												 , 0);
				}

				// adds information of the completed task to the cell
				cell->AddCompletedTask(info);
			}

			// release smart ptr
			iterator = _runnedTasks.erase(iterator);

			MessageManager::Instance().PutMessage(Message("SaveGame"));
		}
		else
		{
			++iterator;
		}
	}
}

void TaskManager::FillTasks(const std::vector<Task::Info>& tasks)
{
	if (_isTasksFilled)
	{
		WRITE_WARN("Trying to fill tasks info twice");
	}

	for (const Task::Info& info : tasks)
	{
		_CheckTask(info);
		_allTasks.insert(std::pair<const std::string, const Task::Info>(info.id, info));
	}

	_isTasksFilled = true;
}

float TaskManager::CalcTaskSuccessChance(const Cell::Info& cellInfo, const Task::Info* taskInfo)
{
	return luabind::call_function<float>(World::Instance().GetLuaInst()->GetLuaState()
												  , "CalcTaskSuccessChance"
												  , cellInfo
												  , taskInfo
												  , 0);
}

TaskManager::Tasks TaskManager::GetAvailableTasks(Cell::WeakPtr cell) const
{
	if (!_isTasksFilled)
	{
		WRITE_ERR("Trying to acces to not initialized TaskManager");
	}

	std::vector<const Task::Info*> availableTasks;

	for (const auto& pair : _allTasks)
	{
		bool add = luabind::call_function<bool>(World::Instance().GetLuaInst()->GetLuaState()
													  , "IsShowTaskInList"
													  , cell.lock().get()
													  , pair.second
													  , 0);
		if (add)
		{
			availableTasks.push_back(&pair.second);
		}
	}

	return availableTasks;
}

void TaskManager::_CheckTask(const Task::Info& taskInfo) const
{
	if (taskInfo.id == "")
	{
		WRITE_WARN("Empty task id");
	}

	if (taskInfo.successFn == "")
	{
		WRITE_WARN("Empty success function name");
	}

	if (taskInfo.failFn == "")
	{
		WRITE_WARN("Empty fail function name");
	}

	if (taskInfo.abortFn == "")
	{
		WRITE_WARN("Empty abort function name");
	}

	if (taskInfo.moralLevel < 0.0f || 1.0f < taskInfo.moralLevel)
	{
		WRITE_WARN("Wrong task moral level");
	}

	if (taskInfo.fameImpact < 0.0f || 1.0f < taskInfo.fameImpact)
	{
		WRITE_WARN("Wrong fameImpact level");
	}

	if (taskInfo.chanceToLooseMembers < 0.0f || 1.0f < taskInfo.chanceToLooseMembers)
	{
		WRITE_WARN("Wrong chanceToLooseMembers value");
	}

	if (taskInfo.heartPoundingLevel < 0.0f || 1.0f < taskInfo.heartPoundingLevel)
	{
		WRITE_WARN("Wrong heartPoundingLevel value");
	}

	if (taskInfo.duration < 0.0f)
	{
		WRITE_WARN("Negative task duration");
	}

	if (taskInfo.level < 0.0f)
	{
		WRITE_WARN("Negative task level");
	}

	if (taskInfo.needCash < 0)
	{
		WRITE_WARN("Negative needCash value");
	}

	if (taskInfo.needMembers < 0)
	{
		WRITE_WARN("Negative needMembers value");
	}

	if (taskInfo.needTech < 0)
	{
		WRITE_WARN("Negative needTech value");
	}
}

const Task::Info* TaskManager::GetTaskInfoById(const std::string& id)
{
	auto taskInfoIterator = _allTasks.find(id);
	if (taskInfoIterator != _allTasks.end())
	{
		return &(taskInfoIterator->second);
	}
	else
	{
		return nullptr;
	}
}
