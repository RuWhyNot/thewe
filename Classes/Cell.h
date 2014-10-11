#ifndef CELL_H
#define CELL_H

#include <memory>

#include "Town.h"
#include "Vector2.h"
#include "Task.h"

class Cell
{
public:
	enum State
	{
		CONSTRUCTION,
		ARRESTED,
		READY
	};

	struct Info
	{
		Cell *parent;
		Town::WeakPtr town;
		Vector2 location;
		State state;

		float morale;
		float contentment;
		Utils::GameTime constructionBegin;
		Utils::GameTime constructionDuration;
		
		int cash;
		int membersCount;
	};

	typedef std::shared_ptr<Cell> Ptr;
	typedef std::weak_ptr<Cell> WeakPtr;

public:
	Cell(const Info &info);

	/**
	 * Create Cell and return a smart ptr
	 */
	static Ptr Create(const Info &info);
	
	/**
	 * Adds new child to the cell
	 * This cell automatically adds as parent
	 */
	void AddChild(Cell::Ptr cell);

	/**
	 * Removes child cell
	 * Set child's parent to null
	 */
	void RemoveChild(Cell::Ptr cell);

	/** Returns all child cells */
	const std::vector<Cell::Ptr>& GetChildren() const;

	const Cell* GetParent() const;

	/**
	* Returns information about the cell
	*/
	Info& GetInfo(void);

	void AddCurrentTask(Task::WeakPtr currentTask);
	Task::WeakPtr getCurrentTask() const;

	bool IsCurrentTaskExists(void) const;

	/** Add completed task */
	void AddCompletedTask(const Task::CompletedTaskInfo& completedTask);

	unsigned int GetUid(void) const;

	void UpdateToTime(Utils::GameTime time);

	float GetConstructionProgress(Utils::GameTime time) const;

private:

	/** Set parent for the cell */
	void _SetParent(Cell *cell);

	/** Recalculate cash */
	void _UpdateCash(float deltatime);
	/** Recalculate morale value */
	void _UpdateMorale(float deltatime);
	/** Recalculate contentement level */
	void _UpdateContentment(float deltatime);

	/** Check all values and write warnings to the log */
	void _CheckValues() const;

	/** All usual info about the cell */
	Info _info;

	State _state;

	unsigned int _uid;

	/** Child cells */
	std::vector<Cell::Ptr> _childCells;

	/** Task that runned now */
	Task::WeakPtr _currentTask;

	/** List of recently completed task */
	std::vector<Task::CompletedTaskInfo> _completedTasks;

	/** Geographic position on the map */
	Vector2 _worldLocation;
};

#endif // CELL_H
