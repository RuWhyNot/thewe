#include "TaskRewardMapWidget.h"

#include "MessageManager.h"
#include "GameInfo.h"
#include "Log.h"

TaskRewardMapWidget::TaskRewardMapWidget(Cell::WeakPtr cell, const Task::Info &task,
	Utils::GameTime waitTime, cocos2d::Vec2 pos, float scale)
	: _cell(cell)
	, _task(task)
	, _pos(pos)
	, _scale(scale)
	, _waitTime(waitTime)
	, _startTime(Utils::GetGameTime())
	, _accentEffTime(0.0f)
	, _isDestroyMsgSended(false)
	, _isPickEnabled(true)
	, _isWaving(false)
{
	init();
}

bool TaskRewardMapWidget::init(void)
{
	cocos2d::EventListenerTouchAllAtOnce *touch = cocos2d::EventListenerTouchAllAtOnce::create();
	touch->onTouchesBegan = CC_CALLBACK_2(TaskRewardMapWidget::TouchBegan, this);
	touch->onTouchesEnded = CC_CALLBACK_2(TaskRewardMapWidget::TouchEnded, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);
	
	cocos2d::CallFunc *func_begin = cocos2d::CallFunc::create([&]() {SetPickEnabled(false);});
	cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&]() {SetPickEnabled(true);});

	_texture = cocos2d::Sprite::create("task_reward.png");
	_texture->setPosition(0.0f, 0.0f);
	_texture->setScale(0.01f);
	_texture->setOpacity(0);
	_texture->runAction(cocos2d::Sequence::create(func_begin, GetJumpAnimation(), func_end, nullptr));
	
	SetPickEnabled(false);
	
	addChild(_texture);
	scheduleUpdate();

	return true;
}

void TaskRewardMapWidget::update(float dt)
{
	if (_isPickEnabled && !_isWaving) {
		_isWaving = true;

		// create _waving again because stopAction was delete them
		cocos2d::MoveTo *move_up = cocos2d::MoveTo::create(2.0f, _pos + _pos/20.0f);
		cocos2d::MoveTo *move_down = cocos2d::MoveTo::create(2.0f, _pos - _pos/20.0f);
		cocos2d::EaseSineOut *move_up_ease = cocos2d::EaseSineOut::create(move_up);
		cocos2d::EaseSineOut *move_down_ease = cocos2d::EaseSineOut::create(move_down);
		cocos2d::Sequence *seqence = cocos2d::Sequence::create(move_up_ease, move_down_ease, nullptr);
		_waving = cocos2d::RepeatForever::create(seqence);
		_texture->runAction(_waving);
	}
	if (!_isPickEnabled && _isWaving) {
		_isWaving = false;
		_texture->stopAction(_waving);
	}

	_accentEffTime += dt;
	if (_accentEffTime >= GameInfo::Instance().GetFloat("TASK_REWARD_ACCENT_PERIOD"))
	{
		_accentEffTime = 0.0f;

		cocos2d::CallFunc *func_begin = cocos2d::CallFunc::create([&]() {SetPickEnabled(false);});
		cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&]() {SetPickEnabled(true);});

		cocos2d::MoveTo *move_up = cocos2d::MoveTo::create(0.4f, _pos*2.0f);
		cocos2d::EaseCubicActionOut *move_up_ease = cocos2d::EaseCubicActionOut::create(move_up);
		cocos2d::MoveTo *move_down = cocos2d::MoveTo::create(0.8f, _pos);
		cocos2d::EaseBounceOut *move_bounce = cocos2d::EaseBounceOut::create(move_down);

		_texture->runAction(cocos2d::Sequence::create(func_begin, move_up_ease, move_bounce, func_end, nullptr));
	}

	if ((Utils::GetGameTime() - _startTime) >= _waitTime && !_isDestroyMsgSended)
	{
		_isDestroyMsgSended = true;

		cocos2d::CallFunc *func_begin = cocos2d::CallFunc::create([&]() {SetPickEnabled(false);});
		cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&]() {SendMessageAboutDelete();});
		_texture->runAction(cocos2d::Sequence::create(func_begin, GetLateAnimation(), func_end, nullptr));
	}
}

void TaskRewardMapWidget::PickReward(void)
{
	cocos2d::CallFunc *func_begin = cocos2d::CallFunc::create([&]() {SetPickEnabled(false);});
	cocos2d::CallFunc *func_end = cocos2d::CallFunc::create(
		[&]() {
			World::Instance().GetTaskManager().CallCuccessfulCompletition(_cell, &_task);
			SendMessageAboutDelete();
		});

	_texture->runAction(cocos2d::Sequence::create(func_begin, GetPickAnimation(), func_end, nullptr));
}

void TaskRewardMapWidget::SetPickEnabled(bool flag)
{
	_isPickEnabled = flag;
}

bool TaskRewardMapWidget::IsTaskId(const std::string &id) const
{
	return _task.id == id;
}

void TaskRewardMapWidget::TouchBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (!_isPickEnabled) {
		return;
	}

	Vector2 location = convertTouchToNodeSpace(touches.at(0));
	if (_texture) {
		if (_texture->getBoundingBox().containsPoint(location)) {
			cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.1f, _scale*1.25f, _scale*0.75f, 1.0f);
			_texture->runAction(scale);
		}
	}
}

void TaskRewardMapWidget::TouchEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (!_isPickEnabled) {
		return;
	}

	Vector2 location = convertTouchToNodeSpace(touches.at(0));
	if (_texture->getBoundingBox().containsPoint(location))
	{
		PickReward();
		event->stopPropagation();
	}
	else
	{
		cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.1f, _scale, _scale, 1.0f);
		_texture->runAction(scale);
	}
}

void TaskRewardMapWidget::SendMessageAboutDelete(void) const
{
	Message message("DeleteTaskRewardWidget");
	message.variables.SetInt("CELL_UID", _cell.lock()->GetUid());
	message.variables.SetString("TASK_ID", _task.id);
	MessageManager::Instance().PutMessage(message);
}

cocos2d::FiniteTimeAction* TaskRewardMapWidget::GetJumpAnimation(void)
{
	cocos2d::FadeIn *fadeout = cocos2d::FadeIn::create(0.4f);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.4f, _scale, _scale, 1.0f);
	cocos2d::MoveTo *moveup = cocos2d::MoveTo::create(0.5f, _pos * 2.0f);
	cocos2d::EaseCubicActionInOut* moveup_ease = cocos2d::EaseCubicActionInOut::create(moveup);
	cocos2d::Spawn *stage1 = cocos2d::Spawn::create(fadeout, scale, moveup_ease, nullptr);

	cocos2d::MoveTo *movedown = cocos2d::MoveTo::create(0.5f, _pos);
	cocos2d::EaseBounceOut *movedown_ease = cocos2d::EaseBounceOut::create(movedown);

	return cocos2d::Sequence::create(stage1, movedown_ease, nullptr);
}

cocos2d::FiniteTimeAction* TaskRewardMapWidget::GetPickAnimation(void)
{
	cocos2d::MoveTo *move_up = cocos2d::MoveTo::create(0.5f, _pos*2.0f);
	cocos2d::ScaleTo *scale_up = cocos2d::ScaleTo::create(0.1f, _scale, _scale, 1.0f);
	cocos2d::EaseCubicActionInOut* move_up_ease = cocos2d::EaseCubicActionInOut::create(move_up);
	cocos2d::Spawn *stage1 = cocos2d::Spawn::create(move_up_ease, scale_up, nullptr);
	
	cocos2d::MoveTo *move_down = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(0.0f, 0.0f));
	cocos2d::ScaleTo *scale_down = cocos2d::ScaleTo::create(0.4f, 0.1f, 0.1f, 1.0f);
	cocos2d::FadeOut *fade_out = cocos2d::FadeOut::create(0.4f);
	cocos2d::EaseCubicActionOut *move_down_ease = cocos2d::EaseCubicActionOut::create(move_down);
	cocos2d::Spawn *stage2 = cocos2d::Spawn::create(move_down_ease, scale_down, fade_out, nullptr);

	return cocos2d::Sequence::create(stage1, stage2, nullptr);
}

cocos2d::FiniteTimeAction* TaskRewardMapWidget::GetLateAnimation(void)
{
	cocos2d::FadeOut *fade_out = cocos2d::FadeOut::create(1.0f);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(1.1f, _scale/1.5f, _scale/1.5f, 1.0f);

	return cocos2d::Spawn::create(fade_out, scale, nullptr);
}