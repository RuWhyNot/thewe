#include "CellInfoMenu.h"

CellInfoMenu::CellInfoMenu(Cell::WeakPtr cell, CellMenuSelector *selector)
	: _cell(cell)
	, _cellMenuSelector(selector)
	, _taskProgressBar(nullptr)
{
	init();
}

cocos2d::Label *CreateTTFLabel(const std::string text,
							Vector2 position,
							cocos2d::Node *parent,
							cocos2d::TextHAlignment aligment = cocos2d::TextHAlignment::LEFT,
							Vector2 anchorPoint = cocos2d::Vec2(0.0f, 0.0f))
{
	cocos2d::Label *label = cocos2d::Label::createWithBMFont("arial-26-en-ru.fnt", text, aligment);
	label->setAnchorPoint(anchorPoint);
	label->setPosition(position);
	parent->addChild(label, 1);
	return label;
}

bool CellInfoMenu::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::EventListenerKeyboard *keyboard = cocos2d::EventListenerKeyboard::create();
	keyboard->onKeyReleased = CC_CALLBACK_2(CellInfoMenu::KeyReleased, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard, this);
	
	cocos2d::Point screen = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	cocos2d::Point center = origin + screen / 2.0f;

	cocos2d::MenuItemImage *closeButton;
	{
		using namespace cocos2d;
		closeButton = MenuItemImage::create("cell-tasks-menu-close-normal.png",
			"cell-tasks-menu-close-pressed.png", CC_CALLBACK_1(CellInfoMenu::_OnCloseCallback, this));
	}

	cocos2d::Menu *menu = cocos2d::Menu::create(closeButton, nullptr);
	menu->setPosition(center);
	cocos2d::Sprite *background = cocos2d::Sprite::create("cell-tasks-menu.png");
	background->setPosition(center);
	cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
	cocos2d::Label *labelTitle = cocos2d::Label::createWithTTF(ttfConfig, "Cell Info", cocos2d::TextHAlignment::CENTER);

	float close_x = background->getContentSize().width  / 2 - closeButton->getContentSize().width  + 5.0f;
	float close_y = background->getContentSize().height / 2 - closeButton->getContentSize().height + 5.0f;
	closeButton->setPosition(close_x, close_y);

	float title_x = center.x;
	float title_y = center.y + background->getContentSize().height / 2 - 16.0f;
	labelTitle->setPosition(title_x, title_y);
	labelTitle->setColor(cocos2d::Color3B(255, 255, 255));
	
	float info_x = center.x - background->getContentSize().width  / 2.0f + 10.0f;
	float info_y = center.y + background->getContentSize().height / 2.0f - 70.0f;

	_labelCashInfo = CreateTTFLabel("", Vector2(info_x, info_y), this);
	
	info_y -= 30.0f;
	
	_labelMembersInfo = CreateTTFLabel("", Vector2(info_x, info_y), this);
		
	info_y -= 30.0f;
	
	_labelContentmentInfo = CreateTTFLabel("", Vector2(info_x, info_y), this);

	info_y -= 30.0f;
	
	_labelMoraleInfo = CreateTTFLabel("", Vector2(info_x, info_y), this);

	info_y -= 30.0f;
	
	_labelChildrensInfo = CreateTTFLabel("", Vector2(info_x, info_y), this);

	Cell::Ptr cell = _cell.lock();
	_cellCurrentTask = cell->getCurrentTask().lock();

	UpdateInfoBy(cell);

	if (_cellCurrentTask != nullptr)
	{
		float w = background->getContentSize().width - 50.0f;
		float x = center.x - background->getContentSize().width / 2.0f + 25.0f;
		float y = center.y - background->getContentSize().height / 2.0f + 25.0f;
		
		Utils::GameTime time = Utils::GetGameTime();
		float progress = _cellCurrentTask->CalculateProgress(time);

		_taskProgressBar = new SquareProgressBar(w, 10.0f, cocos2d::Color4F(1.0f, 0.5f, 0, 1.0f));
		_taskProgressBar->setPosition(x, y);
		_taskProgressBar->autorelease();
		_taskProgressBar->SetProgressPercentage(progress);

		std::string strTaskLabel = cocos2d::StringUtils::format("Current task: %s", _cellCurrentTask->GetInfo()->title.c_str());
		_currentTaskLabel = cocos2d::Label::createWithTTF(ttfConfig, strTaskLabel, cocos2d::TextHAlignment::CENTER);
		_currentTaskLabel->setPosition(center.x, y+25.0f);

		addChild(_taskProgressBar, 1);
		addChild(_currentTaskLabel, 1);
	}

	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);
	cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);

	scheduleUpdate();
	setScale(0.01f);
	setOpacity(0);	
	runAction(elastic_scale);
	runAction(fade);
	addChild(background, 0);
	addChild(menu, 1);
	addChild(labelTitle, 1);

	return true;
}

void CellInfoMenu::update(float dt)
{
	if (_cellCurrentTask)
	{
		if (_taskProgressBar->IsFinished())
		{
			removeChild(_taskProgressBar, true);
			removeChild(_currentTaskLabel, true);
			_cellCurrentTask = nullptr;
		}
		else
		{
			Utils::GameTime time = Utils::GetGameTime();
			float progress = _cellCurrentTask->CalculateProgress(time);
			_taskProgressBar->SetProgressPercentage(progress * 100.0f);
		}
	}
}

void CellInfoMenu::UpdateInfoBy(Cell::Ptr cell)
{
	Cell::Info info = cell->GetInfo();
	
	std::string strCashInfo = cocos2d::StringUtils::format("Cash: %d $", info.cash);
	_labelCashInfo->setString(strCashInfo);

	std::string strMembersInfo = cocos2d::StringUtils::format("Members: %d", info.membersCount);
	_labelMembersInfo->setString(strMembersInfo);

	std::string strDevotion = cocos2d::StringUtils::format("Devotion: %.1f %%", info.devotion);
	_labelContentmentInfo->setString(strDevotion);

	std::string strMoraleInfo = cocos2d::StringUtils::format("Morale: %.1f %%", info.morale);
	_labelMoraleInfo->setString(strMoraleInfo);

	std::string strChildrensInfo = cocos2d::StringUtils::format("Childrens: %s",
	cocos2d::StringUtils::toString<int>(cell->GetChildren().size()).c_str());
	_labelChildrensInfo->setString(strChildrensInfo);
}

void CellInfoMenu::_OnCloseCallback(cocos2d::Ref *sender)
{
	_CloseMenu();
}

void CellInfoMenu::_CloseMenu()
{
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
	cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
	cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnCellMenuClosed, _cellMenuSelector));

	runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
}

void CellInfoMenu::KeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		_CloseMenu();
	}
}
