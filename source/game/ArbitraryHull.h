#ifndef ARBITRARY_HULL_H
#define ARBITRARY_HULL_H

#include <vector>
#include "Color.h"
#include "Point.h"

// ����������� �� ������������� �������� ��� ��������� cocos2d
namespace cocos2d
{
	class CCpoint;
}

class ArbitraryHull
{
public:
	void PushPoint(const Point &point);
	void PopPoint(void);
	void Draw(const Color &color) const;
	void Draw(void) const;
	void Clear(void);
	
	int GetPointsNum(void) const;

	bool Contain(const Point &point) const;

private:
	std::vector<cocos2d::CCPoint> _pointsArray;

	friend class MapProjector;
	friend class EditorLayer;
};

#endif