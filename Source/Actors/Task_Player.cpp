//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../Player.h"
#include  "Task_Player.h"
#include "Task_Drill.h"

namespace player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->playerImg = DG::Image::Create("./data/image/prePlayer.png");
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
		this->box_->setHitBase(ML::Box2D{ -4,-8,8,16 });
		this->SetPos( ML::Vec2{ 0,0 });
		this->movement_->SetSpeed(2.f, 5.f, 0.f);
		gravity_->SetDirection(ML::Vec2::Down());
		gravity_->SetSpeed(0.0f, 10, 0.5f);
		gravity_->SetAcceleration(ML::Gravity(32) * 10);

		status_->HP.Initialize(100);
		this->hp_->SetMaxHP(10,HP::MaxLifeSetMode::MaxHeal);
		//���^�X�N�̐���
		auto dl = drill::Object::Create(true);
		this->drill_ = dl;
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		//ge->KillAll_G("�v���C���[");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{

		this->pState = this->state_->GetNowState();
		this->state_->moveCnt_++;
		this->Think();
		this->Move();
		drill_->SetPos(this->GetPos());
		drill_->dState = this->state_->GetNowState();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�v���C���L�����̕`��
		{
			ML::Box2D draw = this->box_->getHitBase().OffsetCopy(this->GetPos());
			ML::Box2D src{ 0,0,32,64};
			//�X�N���[���Ή�
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->res->playerImg->Draw(draw, src);
		}
		this->DebugInfo();
	}

	void Object::DebugInfo()
	{
		ge->debugFont->Draw(ML::Box2D(1000, 0, 500, 500), "���݈ʒu:"+to_string(this->GetPos().x) + " " + to_string(this->GetPos().y));
		ge->debugFont->Draw(ML::Box2D(100, 0, 1500, 500), "~���݂̑�����@~�ړ�:L�X�e�B�b�N�@�p�x�ύX:R�X�e�B�b�N�@�W�����v:R1(W�L�[)�@�̌@(�̌@���[�h��):L1(Q�L�[)�@���[�h�ύX:B2(X�L�[)�@�_�b�V��:B1(Z�L�[)");
		if (this->CheckHead())
			ge->debugFont->Draw(ML::Box2D(1000, 20, 500, 500), "������I");
		if (this->CheckFoot())
			ge->debugFont->Draw(ML::Box2D(1060, 20, 500, 500), "������I");
		ge->debugFont->Draw(ML::Box2D(1000, 60, 500, 500), "�ړ��x�N�g��:"+to_string(this->GetMoveVec().x) + to_string(this->GetMoveVec().y));

		string stateName;
		switch (pState)
		{
		case StateComponent::State::Non:
			stateName = "�Ȃ�";
			break;
		case StateComponent::State::Idle:
			stateName = "�ҋ@";
			break;
		case StateComponent::State::Walk:
			stateName = "����";
			break;
		case StateComponent::State::Attack:
			stateName = "�U��";
			break;
		case StateComponent::State::SpinAttack:
			stateName = "��]�U��";
			break;
		case StateComponent::State::Damage:
			stateName = "�_���[�W";
			break;
		case StateComponent::State::KnockBack:
			stateName = "�m�b�N�o�b�N";
			break;
		case StateComponent::State::Dead:
			stateName = "�˖S";
			break;
		case StateComponent::State::Jump:
			stateName = "�W�����v";
			break;
		case StateComponent::State::Fall:
			stateName = "������";
			break;
		case StateComponent::State::Dash:
			stateName = "�_�b�V��";
			break;
		case StateComponent::State::Drill:
			stateName = "�h�������[�h";
			break;
		case StateComponent::State::DrillDash:
			stateName = "�̌@�_�b�V��";
			break;
		case StateComponent::State::Mining:
			stateName = "�̌@��";
			break;
		case StateComponent::State::Appeal:
			stateName = "�A�s�[��";
			break;
		}
		ge->debugFont->Draw(ML::Box2D(1000, 80, 500, 500), "���݂̃X�e�[�^�X:"+stateName);
		if (StateComponent::State::Drill == this->state_->GetNowState() ||
			StateComponent::State::Mining == this->state_->GetNowState())
		{
			ge->debugFont->Draw(ML::Box2D(1000, 120, 500, 500), "�̌@���[�h");
		}
		else
		{
			ge->debugFont->Draw(ML::Box2D(1000, 120, 500, 500), "�s�����[�h");
		}
		ge->debugFont->Draw(ML::Box2D(1000, 140, 500, 500), "moveCnt:"+to_string(this->state_->moveCnt_));
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
