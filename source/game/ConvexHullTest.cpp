#include "ConvexHullTest.h"

void ConvexHullTest::AddPoint(const cocos2d::CCPoint &point)
{
	_pointsArray.push_back(point);
}

void ConvexHullTest::Draw(const cocos2d::ccColor4F &color) const
{
	//cocos2d::ccDrawSolidPoly(&(*_pointsArray.begin()), _pointsArray.size(), color);

	if (_pointsArray.empty())
	{
		return;
	}

	cocos2d::ccDrawColor4F(color.r, color.g, color.b, color.a);
	cocos2d::ccDrawPoly(&(*_pointsArray.begin()), _pointsArray.size(), false);
}

void ConvexHullTest::Draw() const
{
	if (_pointsArray.empty())
	{
		return;
	}

	cocos2d::ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
	cocos2d::ccDrawPoly(&(*_pointsArray.begin()), _pointsArray.size(), false);
}

void ConvexHullTest::Clear(void)
{
	_pointsArray.clear();
}

int ConvexHullTest::GetPointsNum(void) const
{
	return _pointsArray.size();
}

bool ConvexHullTest::Contain(const cocos2d::CCPoint &point)
{
	unsigned int i, j;
	bool result = false;

	for (i = 0, j = _pointsArray.size() - 1; i < _pointsArray.size(); j = i++)
	{
		// ...����, � ��� � ���� �� ����� ����.
		if ((_pointsArray[i].y > point.y) != (_pointsArray[j].y > point.y) &&
			(point.x < (_pointsArray[j].x - _pointsArray[i].x) * (point.y - _pointsArray[i].y) 
			/ (_pointsArray[j].y-_pointsArray[i].y) + _pointsArray[i].x))
		{
			result = !result;
		}
	}

	return result;
}

const std::vector<cocos2d::CCPoint>& ConvexHullTest::GetPoints() const
{
	return _pointsArray;
}