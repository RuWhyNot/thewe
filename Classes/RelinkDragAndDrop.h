#ifndef RELINK_DRAG_AND_DROP_H
#define RELINK_DRAG_AND_DROP_H

#include "CellGameInterface.h"

class WorldMapLayer;

class RelinkDragAndDrop : public cocos2d::Node
{
public:
	RelinkDragAndDrop(WorldMapLayer *worldMapLayer, MapProjector *projector, Cell::WeakPtr cell);

	bool init(void) override;
	void update(float dt) override;

	void TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	void TouchesCancelled(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

private:
	enum class State
	{
		IDLE,
		WAIT,
		DRAG,
		DROP
	};

private:
	WorldMapLayer *_worldMapLayer;
	MapProjector *_projector;
	Cell::WeakPtr _cell;
	MultipleImageSprite *_sprite;
	StripEffect *_strip;
	State _state;
	cocos2d::Sprite *_mark;

	Vector2 _touchFirst;
	Vector2 _touchWorld;
	Vector2 _touchLocal;

	float _timeLocal;
	float _markAngle;
	bool _isAttracting;

	static const float DRAG_WAIT_DELAY;
	static const float DRAG_WAIT_DISTANCE;
	static const float ATTRACTION_DISTANCE;
	static const Vector2 MARK_POSITION;
};

#endif