#include "WorldMapLayer.h"

#include "WorldMap.h"
#include "GameScene.h"
#include "MapGuiLayer.h"

WorldMapLayer::WorldMapLayer(MapProjector* projector)
	: _mapProjector(projector)
	, _isInputEnabled(true)
	, _mapGui(nullptr)
{
	init();
}

bool WorldMapLayer::init(void)
{
	if (!cocos2d::CCLayer::init())
	{
		return false;
	}

	cocos2d::CCSprite *worldSprite = cocos2d::CCSprite::create("WorldMap.png");
	_mapProjector->AddNode(Point(0.0f, 0.0f), Point(0.0f, 0.0f), worldSprite);

	addChild(worldSprite);
	setTouchEnabled(true);
    setKeypadEnabled(true);

	Point origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();
	Point screen = cocos2d::CCDirector::sharedDirector()->getVisibleSize();

	SetGuiEnabled(true);

	WorldMap::Instance().AddCell(std::make_shared<Cell>(Cell(Point(100.0f, 100.0f))));

	for (const Cell::Ptr cell : WorldMap::Instance().GetCells())
	{
		cocos2d::CCSprite* cellSprite = cocos2d::CCSprite::create("pin.png");
		_mapProjector->AddNode(cell->GetLocation(), Point(100.0f, -300.0f), cellSprite);
		addChild(cellSprite);
	}
	
	// �������� ��� ��������� ����� ���� ������
	_mapProjector->SetScreenCenter(origin + screen / 2.0f);
	// ������ ������ ����� ����� � ����� ������
	_mapProjector->SetLocation(Point(0.0f, 0.0f));
	// ������ �����, ����� ����� ��������� �����������������
	_mapProjector->SetScale(1.0f);

	return true;
}

void WorldMapLayer::SetMapInputEnabled(bool isEnabled)
{
	_isInputEnabled = isEnabled;
}

void WorldMapLayer::SetGuiEnabled(bool isEnabled)
{
	if (_mapGui && !isEnabled)
	{
		removeChild(_mapGui);
		_mapGui = nullptr;
	}
	else if (!_mapGui && isEnabled)
	{
		_mapGui = new MapGuiLayer();
		addChild(_mapGui);
		_mapGui->autorelease();
	}
}

void WorldMapLayer::menuCloseCallback(cocos2d::CCObject *Sender)
{
}

void WorldMapLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (_isInputEnabled)
	{
		cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());
		_touchLastPoint = touch->getLocation();
		_touchFirstPos = touch->getLocation();
	}
}

void WorldMapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (_isInputEnabled)
	{
		cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch *>(touches->anyObject());
		Point point = touch->getLocation();
		Point v = _touchFirstPos - point;
		
		const float size = v.Size();
		const float tolerance = 5.0f;

		if (size <= tolerance)
		{
			Region::Ptr region = GetRegionUnderPoint(point);

			if (region)
			{
				dynamic_cast<GameScene*>(this->getParent())->ShowRegionInfo(region);
			}
		}
	}
}

void WorldMapLayer::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (_isInputEnabled)
	{
		cocos2d::CCTouch *touch = dynamic_cast<cocos2d::CCTouch*>(touches->anyObject());

		_mapProjector->ShiftView(-_touchLastPoint + touch->getLocation());
		_touchLastPoint = touch->getLocation();
	}
}

void WorldMapLayer::visit()
{
	CCLayer::visit();

	for (Region::Ptr region : WorldMap::Instance().GetRegions())
	{
		const Region::HullsArray &array = region->GetHullsArray();

		for (const ArbitraryHull &hull : array)
		{
			// ��������� ��������
			_mapProjector->ProjectOnScreen(hull).Draw();
		}
	}
}

Region::Ptr WorldMapLayer::GetRegionUnderPoint(const Point& point) const
{
	for (Region::Ptr region : WorldMap::Instance().GetRegions())
	{
		const Region::HullsArray &array = region->GetHullsArray();

		for (const ArbitraryHull &hull : array)
		{
			if (hull.Contain(_mapProjector->ProjectOnMap(point)))
			{
				return region;
			}
		}
	}

	return Region::Ptr();
}


void WorldMapLayer::ModifyZoom(float multiplier)
{
	_mapProjector->SetScale(_mapProjector->GetScale() * multiplier);
}