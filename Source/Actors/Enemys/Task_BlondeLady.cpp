//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_BlondeLady.h"
#include  "../../Actors/Task_Player.h"

namespace BlondeLady
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/BlondeLady.png");
		fanImg = DG::Image::Create("./data/image/Slash.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		img.reset();
		fanImg.reset();
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
		box_->setHitBase(ML::Box2D{ -8,-16,16,32 });
		gravity_->SetDirection(ML::Vec2::Down());
		gravity_->SetSpeed(0.0f, 10, 0.5f);
		gravity_->SetAcceleration(ML::Gravity(32)*10);

		angle_LR_ = Angle_LR::Right;

		SetPreState(AIState::Approach);
		SetNowState(AIState::Approach);

		SetFov(200.f);
		SetRange(10.f);

		moveCnt_->SetCountFrame(90);
		fanEdge_->setHitBase(ML::Box2D{ -4,-8,8,32 });

		SetTarget(ge->playerPtr.get());
		//���^�X�N�̐���
		return  true;
	}

	void BeginPlay()
	{

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
		/*auto pl = ge->GetTask<player::Object>(player::defGroupName, player::defName);
		SetTarget(pl.get());*/

		moveCnt_->Update();
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
		{
			ML::Box2D draw = fanEdge_->getHitBase().OffsetCopy(ML::Vec2(GetPos().x + 16, GetPos().y));
			ML::Box2D src = ML::Box2D(0, 0, 16, 64);

			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			res->fanImg->Draw(draw, src);
		}

		if (IsAttacking())
		{
			ge->debugFont->Draw(ML::Box2D(500, 700, 500, 500), "�U����");
		}
		

		string stateName;
		switch (GetNowState())
		{
		case AIState::Idle:
			stateName = "�ҋ@";
			break;
		case AIState::Approach:
			stateName = "�ڋ�";
			break;
		case AIState::Attack:
			stateName = "�U��";
			break;
		}
		ge->debugFont->Draw(ML::Box2D(1000, 700, 500, 500), stateName);
		auto pl = ge->GetTask<player::Object>(player::defGroupName, player::defName);
		SetTarget(pl.get());

		if(WithinRange(GetTarget()))
		ge->debugFont->Draw(ML::Box2D(1000, 300, 500, 500), "�U���͈�");

		

		ge->debugFont->Draw(ML::Box2D(1000, 300, 500, 500), to_string(moveCnt_->GetCount()));
	}
	//-------------------------------------------------------------------
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