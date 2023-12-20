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
		this->target = DG::Image::Create("./data/image/target.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->target.reset();
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
		this->box_->setHitBase(ML::Box2D{ -2,-2,4,4 });
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
		this->plPos = ge->playerPtr->GetPos();
		this->SetAngle(this->UpdateDrillAngle());
		ML::Vec2 preMoveVec{ (cos(GetNowAngle()) * 16.f), (sin(GetNowAngle()) * 16.f) };
		this->SetDrawPos( this->GetPos() + preMoveVec);//���̎��_�Ō�������̃h�����̕`������肷��
		
		
		this->SetMoveVec(preMoveVec*x);
		this->DrillCheckMove(this->GetMoveVec());//�^�[�Q�b�g�p�̋�`����

		this->dState = this->state_->GetNowState();
		this->UpdateTargetPos(this->ChangeBrockPos());
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = ML::Box2D{-4,-4,8,8}.OffsetCopy(this->GetDrawPos());
		ML::Box2D src = ML::Box2D{ 0,0,64,64 };
		this->res->img->Rotation(this->UpdateDrillAngle(), ML::Vec2{4, 4});
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		this->res->img->Draw(draw, src);
		//----------------------------------------------------
		ML::Box2D tDraw = ML::Box2D{ (int)this->GetTargetPos().x*16,(int)this->GetTargetPos().y*16,16,16};
		tDraw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D tSrc = ML::Box2D{ 0,0,128,128 };
		this->res->target->Draw(tDraw, tSrc);
		//----------------------------------------------------
		ge->debugFont->Draw(ML::Box2D(1000, 100, 500, 500), "�h�����̊p�x:"+to_string(ML::ToDegree(GetNowAngle())));
		ML::Vec2 mapPoint{ (this->GetPos().x + 16 + ge->camera2D.x),(this->GetPos().y + 16 + ge->camera2D.x) };
		//ML::Box2D mapPoint{this->box_->getHitBase().OffsetCopy(this->GetPos()) };
		ge->debugFont->Draw(ML::Box2D(1000, 200, 500, 500), "�h�����̃}�X:" + to_string((int)mapPoint.x/16)+" "+to_string((int)mapPoint.y/16));
		ge->debugFont->Draw(ML::Box2D(900, 200, 500, 500), to_string((int)GetTargetPos().x)+" "+ to_string((int)GetTargetPos().y));
		ge->debugFont->Draw(ML::Box2D(850, 600, 500, 500), to_string(ge->playerPtr->GetPos().x) +" " + to_string(this->GetTargetPos().x*16) + " " + to_string(fabsf(this->GetTargetPos().x*16 - ge->playerPtr->GetPos().x)));
		{
			ML::Vec2 diff = ML::Vec2{
	fabsf(this->GetTargetPos().x * 16 - plPos.x),
	fabsf(this->GetTargetPos().y * 16 - plPos.y)
			};
			float hypo = ((diff.x * diff.x) + (diff.y * diff.y));
			ge->debugFont->Draw(ML::Box2D(1000, 500, 500, 500), "���� :" + to_string(hypo)+"Limit :"+to_string((3 * 16 * 3 * 16)));
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