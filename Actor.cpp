//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "Actor.h"
#include  "MyPG.h"
#include "Source/Scene/Task_Map.h"

Actor::Actor()
	:GameObject()
{
	AddComponent(box_ = shared_ptr<BoxCollisionComponent>(new BoxCollisionComponent(this)));
}

//��ʊO�ɏo�Ă�����ture��Ԃ�
bool Actor::OutOfScreen() const
{
	ML::Box2D halfHitBase;
	halfHitBase.w = this->box_->getHitBase().w * 0.5f;
	halfHitBase.h = this->box_->getHitBase().h * 0.5f;

	ML::Rect bodyEdge
	{
		int(this->pos_.x - halfHitBase.w),//��
		int(this->pos_.y - halfHitBase.h),//��
		int(this->pos_.x + halfHitBase.w),//�E
		int(this->pos_.y + halfHitBase.h) //��
	};

	//��ʊO�Ȃ�true
	//��
	if (bodyEdge.bottom < 0.0f)
		return true;

	//��
	if (bodyEdge.right < 0.0f)
		return true;

	//�E
	if (bodyEdge.left > ge->gameScreenWidth)
		return true;

	//��
	if (bodyEdge.top > (int)ge->screenHeight)
		return true;


	//��ʊO�ɏo�Ă��Ȃ�
	return false;


}



bool Actor::CheckHit(const ML::Box2D& target) const
{
	ML::Box2D me = this->box_->getHitBase().OffsetCopy(this->pos_);
	return me.Hit(target);
}

void Actor::CheckMove(ML::Vec2& est)
{
	//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	auto   map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
	if (nullptr == map) return;//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j

	//�����ɑ΂���ړ�
	while (est.x != 0) 
	{
		float  preX = this->pos_.x;
		if (est.x >= 1) 
		{ 
			this->pos_.x += 1;		
			est.x -= 1; 
		}
		else if (est.x <= -1) 
		{ 
			this->pos_.x -= 1;		
			est.x += 1; 
		}
		else 
		{ 
			this->pos_.x += est.x;		
			est.x = 0; 
		}
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->pos_);
		if (true == map->CheckHit(hit)) 
		{
			this->pos_.x = preX;		//�ړ����L�����Z��
			break;
		}
	}
	//�c���ɑ΂���ړ�
	while (est.y != 0) 
	{
		float  preY = this->pos_.y;
		if (est.y >= 1) 
		{ 
			this->pos_.y += 1;
			est.y -= 1; 
		}
		else if (est.y <= -1) 
		{ 
			this->pos_.y -= 1;		
			est.y += 1; 
		}
		else 
		{ 
			this->pos_.y += est.y;		
			est.y = 0; 
		}
		
		ML::Box2D  hit = this->box_->getHitBase().OffsetCopy(this->pos_);
		if (true == map->CheckHit(hit)) 
		{
			this->pos_.y = preY;		//�ړ����L�����Z��
			break;
		}
	}
}



