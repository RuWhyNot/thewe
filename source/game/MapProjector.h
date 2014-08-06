#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include <memory>
#include <string>
#include "Point.h"
#include "ArbitraryHull.h"
#include "Drawable.h"

class MapProjector
{
public:
	MapProjector(Point mapSize);

	/** ������������� ����� �� ��������� ������ � ���������� ����� */
	Point ProjectOnMap(Point screenPoint) const;
	/** ������������� ����� �� ��������� ����� � ���������� ������ */
	Point ProjectOnScreen(Point mapPoint) const;

	/** ������������� ������� �� ��������� ������ � ���������� ����� */
	ArbitraryHull ProjectOnMap(const ArbitraryHull& screenHull) const;
	/** ������������� ������� �� ��������� ����� � ���������� ������ */
	ArbitraryHull ProjectOnScreen(const ArbitraryHull& mapHull) const;

	/** ���������� ����� ��������� ������ �� ����� */
	void SetLocation(Point mapLocation);
	/** ���������� ����������� ���� */
	void SetScale(float scale);

	/** �������� ��� � ����������� ������ */
	void ShiftView(Point screenDelta);

	/** �������� ��������� ������ � ���� */
	Point GetLocation() const;
	/** �������� ������� �������� ����������� ���� */
	float GetScale() const;

	/** ������� � ����� ����� ������ ��������� ������ ������� ������ */
	void SetScreenCenter(Point centerPos);
	
	/** �������� ����� �������, ������� ����� �������������� �� ����� */
	void AddMapPart(Drawable::Ptr node, Point location, Point shift, float scale, bool dontScale = false);
	
	/** ������� � ��������� ������� ��� ������������� */
	void RemoveMapPart(const Drawable::Ptr node);

private:
	struct MapPart
	{
		/** ��������� ������� � ���� */
		Point location;
		/** ����� ������ ������� */
		Point shift;
		/** ������� ������� �� ��������� */
		float initialScale;
		/** ����� �� �������������� ������ ������ � ������ */
		bool isScalable;
		/** ��������� �� ����������� ������ */
		Drawable::Ptr node;
	};

	/** ������������ �������� */
	typedef std::vector<MapPart> MapParts;

private:
	/** ��������� ����� ������ ���� �� ������� ����� � ��������� ��������� ���� ��� ������������� */
	void _CheckBoundings();

	/** �������� ��������� � ������� ���� ������������ ��������� */
	void _UpdateNodes();

	float _viewScale;
	Point _viewLocation;

	Point _screenCenter;

	Point _mapSize;

	MapParts _mapParts;
};

#endif // MAP_PROJECTOR_H