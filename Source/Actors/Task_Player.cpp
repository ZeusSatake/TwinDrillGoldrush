//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../Player.h"
#include  "Task_Player.h"

namespace player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->playerImg = DG::Image::Create("./data/image/oyasiro.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->playerImg.reset();
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
		this->box_->setHitBase(ML::Box2D{ -150,-210,300,420 });
		this->pos_ = ML::Vec2{ 0,0 };
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
		this->Move();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�v���C���L�����̕`��
		{
			ML::Box2D draw = this->box_->getHitBase().OffsetCopy(this->pos_);
			ML::Box2D src{ 0,0,300,420 };
			this->res->playerImg->Draw(draw, src);
		}
	}

	void Object::Move()
	{
		this->PState = this->state_->GetNowState();
		switch (PState)
		{
		case StateComponent::State::Non:
			break;
		case StateComponent::State::Idle:
			break;
		case StateComponent::State::Walk:
			break;
		case StateComponent::State::Attack:
			break;
		case StateComponent::State::SpinAttack:
			break;
		case StateComponent::State::Damage:
			break;
		case StateComponent::State::KnockBack:
			break;
		case StateComponent::State::Dead:
			break;
		case StateComponent::State::Jump:
			break;
		case StateComponent::State::Fall:
			break;
		case StateComponent::State::Dash:
			break;
		case StateComponent::State::DrillDash:
			break;
		case StateComponent::State::Mining:
			break;
		case StateComponent::State::Appeal:
			break;

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
