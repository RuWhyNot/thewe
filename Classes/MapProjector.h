#ifndef MAP_PROJECTOR_H
#define MAP_PROJECTOR_H

#include <memory>
#include <string>
#include "Vector2.h"
#include "ArbitraryHull.h"
#include "Drawable.h"

class MapProjector
{
public:
	MapProjector(Vector2 mapSize);

	/** ������������� ����� �� ��������� ������ � ���������� ����� */
	Vector2 ProjectOnMap(Vector2 screenPoint) const;
	/** ������������� ����� �� ��������� ����� � ���������� ������ */
	Vector2 ProjectOnScreen(Vector2 mapPoint) const;

	/** ������������� ������� �� ��������� ������ � ���������� ����� */
	ArbitraryHull ProjectOnMap(const ArbitraryHull& screenHull) const;
	/** ������������� ������� �� ��������� ����� � ���������� ������ */
	ArbitraryHull ProjectOnScreen(const ArbitraryHull& mapHull) const;

	/** ���������� ����� ��������� ������ �� ����� */
	void SetLocation(Vector2 mapLocation);
	/** ���������� ����������� ���� */
	void SetScale(float scale);

	/** �������� ��� � ����������� ������ */
	void ShiftView(Vector2 screenDelta);

	/** �������� ��������� ������ � ���� */
	Vector2 GetLocation() const;
	/** �������� ������� �������� ����������� ���� */
	float GetScale() const;

	/** ������� � ����� ����� ������ ��������� ������ ������� ������ */
	void SetScreenCenter(Vector2 centerPos);
	
	/** �������� ����� �������, ������� ����� �������������� �� ����� */
	void AddMapPart(Drawable::Ptr node, Vector2 location, Vector2 shift, float scale, bool dontScale = false);
	
	/** ������� � ��������� ������� ��� ������������� */
	void RemoveMapPart(const Drawable::Ptr node);

	/** �������� ��������� ���� ��������� (�������� ����� ����������) */
	void Update();

private:
	struct MapPart
	{
		/** ��������� ������� � ���� */
		Vector2 location;
		/** ����� ������ ������� */
		Vector2 shift;
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
	Vector2 _viewLocation;

	Vector2 _screenCenter;

	Vector2 _mapSize;

	MapParts _mapParts;
};

#endif // MAP_PROJECTOR_H