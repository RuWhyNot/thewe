#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <memory>
#include "Point.h"

// ����������� �� ������������� �������� ��� ��������� cocos2d
namespace cocos2d
{
	class CCNode;
}

class Drawable
{
public:
	typedef std::shared_ptr<Drawable> Ptr;
public:
	~Drawable();
	/** ������� */
	static Drawable::Ptr CastFromCocos(cocos2d::CCNode *node);

	void SetPosition(Point location);
	void SetScale(float scale);

private:
	typedef std::shared_ptr<cocos2d::CCNode> NodePtr;
private:
	Drawable(cocos2d::CCNode *node);

	NodePtr _node;
};

#endif // DRAWABLE_H