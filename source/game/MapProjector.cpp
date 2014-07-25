#include "MapProjector.h"


static const cocos2d::CCPoint MAP_INITIAL_SIZE = cocos2d::CCPoint(1390.0f, 1003.0f);

MapProjector::MapProjector(cocos2d::CCPoint shift, float scale)
{
	// ToDo: ������� ������������� �������� � ��������� �����
	_mapSprite = cocos2d::CCSprite::create("../_gamedata/WorldMap.png");

	SetShift(shift);
	SetScale(scale);
}

void MapProjector::SetShift(cocos2d::CCPoint shift)
{
	cocos2d::CCPoint spriteSize = GetSprite()->getContentSize();
	
	_mapShift = shift;
	if (shift.y < -spriteSize.y/2)
	{
		_mapShift.y = -spriteSize.y/2;
	}

	if (shift.y > _screenCenter.y*2 + spriteSize.y/2)
	{
		_mapShift.y = _screenCenter.y*2 + spriteSize.y/2;
	}

	GetSprite()->setPosition(_mapShift);
}

void MapProjector::SetScale(float scale)
{
	SetShift(_screenCenter + (_mapShift - _screenCenter) * (scale / _mapScale));
	_mapScale = scale;
	GetSprite()->setScale(_mapScale);
}

cocos2d::CCPoint MapProjector::GetShift()
{
	return _mapShift;
}

float MapProjector::GetScale()
{
	return _mapScale;
}

cocos2d::CCPoint MapProjector::ProjectOnMap(cocos2d::CCPoint screenPoint) const
{
	return (screenPoint - _mapShift) / _mapScale;
}

cocos2d::CCPoint MapProjector::ProjectOnScreen(cocos2d::CCPoint mapPoint) const
{
	return (mapPoint * _mapScale) + _mapShift;
}

cocos2d::CCSprite* MapProjector::GetSprite() const
{
	return _mapSprite;
}

void MapProjector::SetScreenCenter(cocos2d::CCPoint centerPos)
{
	_screenCenter = centerPos;	
}