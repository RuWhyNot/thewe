#include "WorldMapLayer.h"

#include "WorldMap.h"
#include "GameScene.h"
#include "MapGuiLayer.h"

WorldMapLayer::WorldMapLayer(MapProjector* projector)
	: _mapProjector(projector)
{
	init();
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}

	addChild(_mapProjector->GetSprite());
	setTouchEnabled(true);
    setKeypadEnabled(true);

	cocos2d::CCPoint origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();
	cocos2d::CCSize screen = cocos2d::CCDirector::sharedDirector()->getVisibleSize();

	_mapGui = new MapGuiLayer();
	addChild(_mapGui);
	_mapGui->autorelease();

	// �������� ��� ��������� ����� ���� ������
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// ������ ������ ����� ����� � ����� ������
	_mapProjector->SetShift(origin + screen / 2.0f);

	return true;
}

void WorldMapLayer::menuCloseCallback(cocos2d::CCObject *Sender)
{
}

void WorldMapLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());
	_touchLastPoint = touch->getLocation();
	_tappedRegion = GetRegionUnderPoint(touch->getLocation());
}

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

	if (GetRegionUnderPoint(touch->getLocation()) == _tappedRegion && _tappedRegion != nullptr)
	{
		dynamic_cast<GameScene*>(this->getParent())->ShowRegionInfo("Italy");
	}
}

void WorldMapLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

	_mapProjector->SetShift(_mapProjector->GetShift() - _touchLastPoint + touch->getLocation());
	_touchLastPoint = touch->getLocation();
}

void WorldMapLayer::visit()
{
	CCLayer::visit();

	for (auto regionIterator : WorldMap::Instance().GetRegions())
	{
		const Region::HullsArray &array = regionIterator.second->GetHullsArray();

		for (const ArbitraryHull &hull : array)
		{
			ArbitraryHull projectedHull;
			for (auto &point : hull.GetPoints())
			{
				projectedHull.PushPoint(_mapProjector->ProjectOnScreen(point));
			}
			
			projectedHull.Draw();
		}
	}
}

Region::Ptr WorldMapLayer::GetRegionUnderPoint(const cocos2d::CCPoint& point)
{
	for (auto regionIterator : WorldMap::Instance().GetRegions())
	{
		const Region::HullsArray &array = regionIterator.second->GetHullsArray();

		for (const ArbitraryHull &hull : array)
		{
			if (hull.Contain(_mapProjector->ProjectOnMap(point)))
			{
				return regionIterator.second;
			}
		}
	}

	return Region::Ptr();
}


void WorldMapLayer::ModifyZoom(float multiplier)
{
	_mapProjector->SetScale(_mapProjector->GetScale() * multiplier);
}