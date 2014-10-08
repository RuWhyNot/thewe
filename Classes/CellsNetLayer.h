#ifndef CELLS_NET_LAYER_H
#define CELLS_NET_LAYER_H

#include <map>
#include <cocos2d.h>
#include "NetCellWidget.h"

class CellsNetLayer : public cocos2d::Layer
{
public:
	typedef std::multimap<int, NetCellWidget*> CellsTree;

public:
	CellsNetLayer();

	virtual bool init(void) override;

private:
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event) override;
	void ShowLevel(int level);
	void HideAllLevels();
	NetCellWidget* RecursivelyCreateCellsNetwork(Cell::Ptr rootCell, cocos2d::Vector<cocos2d::MenuItem*>* menuItems, int deepness);
	void _CellTouchInputListener(cocos2d::Ref *sender);

private:
	CellsTree _cells;
};

#endif // CELLS_NET_LAYER_H
