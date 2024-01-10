//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_LadySatake.h"
#include "../../Actors/Task_Player.h"

namespace Satake
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/LadySatake.png");
		fanImg = DG::Image::Create("./data/image/Slash.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		status_->HP.Initialize(200);
		status_->attack.Initialize(15, 100);
		status_->defence.Initialize(0, 100);
		status_->speed.Initialize(2.5f, 100.f, 10.f);
		box_->setHitBase(ML::Box2D{ -8,-16,16,32 });
		gravity_->SetDirection(ML::Vec2::Down());
		gravity_->SetSpeed(0.0f, status_->speed.GetFallSpeed(), 0.5f);
		gravity_->SetAcceleration(ML::Gravity(32) * 10);

		angle_LR_ = Angle_LR::Right;

		SetPreState(AIState::Idle);
		SetNowState(AIState::Idle);

		SetFov(200.f);
		SetRange(30.f);

		moveCnt_->SetCountFrame(0);
		unHitTimer_->SetCountFrame(90);
		fanEdge_->setHitBase(ML::Box2D{ -4,-16,8,32 });

		SetTarget(ge->playerPtr.get());
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		moveCnt_->Update();
		unHitTimer_->Update();
		Think();
		Move();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = box_->getHitBase().OffsetCopy(GetPos());
			ML::Box2D src = ML::Box2D(0, 0, 500, 615);
			//�X�N���[���Ή�
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

			res->img->Draw(draw, src);
		}
		if (IsAttacking())
		{
			ML::Box2D draw;
			if (angle_LR_ == Angle_LR::Left)
			{
				draw = fanEdge_->getHitBase().OffsetCopy(ML::Vec2(GetPos().x, GetPos().y));
			}
			else
			{
				draw = fanEdge_->getHitBase().OffsetCopy(ML::Vec2(GetPos().x, GetPos().y));
			}
			ML::Box2D src = ML::Box2D(0, 0, 16, 64);

			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			res->fanImg->Draw(draw, src);
		}
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}