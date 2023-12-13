//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_Drill.h"
#include "Task_Player.h"
#include "../../Source/Scene/Task_Map.h"

namespace  drill
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/preDrill.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->box_->setHitBase(ML::Box2D{ -4,-4,8,8 });
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
		this->SetAngle(this->UpdateDrillAngle());
		//this->SetPosX(this->GetPos().x+ cos(GetNowAngle()) * 15.f);
		//this->SetPosY(this->GetPos().y + sin(GetNowAngle()) * 15.f);
		this->SetMoveVec(ML::Vec2{ (cos(GetNowAngle()) * 15.f), (sin(GetNowAngle()) * 15.f) });
		this->DrillCheckMove(this->GetMoveVec());
		this->dState = this->state_->GetNowState();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->box_->getHitBase().OffsetCopy(this->GetPos());
		ML::Box2D src = ML::Box2D{ 0,0,64,64 };
		this->res->img->Rotation(this->UpdateDrillAngle(), ML::Vec2{4, 4});
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		this->res->img->Draw(draw, src);
		//----------------------------------------------------
		ge->debugFont->Draw(ML::Box2D(1000, 100, 500, 500), "�h�����̊p�x:"+to_string(ML::ToDegree(GetNowAngle())));
		ML::Vec2 mapPoint{ (this->GetPos().x+16 + ge->camera2D.x),(this->GetPos().y+16 + ge->camera2D.x )};
		//ML::Box2D mapPoint{this->box_->getHitBase().OffsetCopy(this->GetPos()) };
		ge->debugFont->Draw(ML::Box2D(1000, 200, 500, 500), "�h�����̃}�X:" + to_string((int)mapPoint.x/16)+" "+to_string((int)mapPoint.y/16));
		
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