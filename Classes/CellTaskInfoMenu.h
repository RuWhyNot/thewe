#ifndef CELL_TASK_INFO_MENU
#define CELL_TASK_INFO_MENU

#include "CellGameInterface.h"

class CellTaskInfoMenu : public cocos2d::Layer
{
public:
	CellTaskInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector);

	virtual bool init(void) override;
	virtual void update(float dt) override;

private:
	void _OnCloseCallback(cocos2d::Ref *sender);
	void _CloseMenu(void);
	void _OnCancel(cocos2d::Ref *sender);
	void KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);

private:
	cocos2d::Label *_taskProgressLabel;

	SquareProgressBar *_taskProgressBar;
	cocos2d::MenuItemImage *_cancelButton;

	Cell::WeakPtr _cell;
	Task::WeakPtr _cellCurrentTask;
	CellMenuSelector *_cellMenuSelector;
};

#endif // CELL_TASK_INFO_MENU
