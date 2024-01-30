//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_UIBanner.h"

#include  "../../Components/HPBarComponent.h"
#include  "../../Actors/Task_Player.h"

namespace UIBanner
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		backImage = DG::Image::Create("./data/image/backGround/uiBanner/Newstuff 1.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		backImage.reset();
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
		render2D_Priority[1] = 0.7f;
		SetBackGroundAlpha(1.0f);
		SetMargin(37);
		
		//HP�o�[�\���ʒu
		NormalizeHPBarPos();
		ge->playerPtr->GetHPBar()->SetSupportScroll(false);
		
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

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D src(40, 112, 192, 192);
		res->backImage->Draw(drawSize_.OffsetCopy(GetPos()), src, ML::Color(alpha_, 1, 1, 1));
	}

	void Object::NormalizeHPBarPos()
	{
		auto hpBar = ge->playerPtr->GetHPBar();
		hpBar->SetPos(ML::Vec2(hpBar->GetSize().x * 0.5f + GetPos().x + margin_, hpBar->GetSize().y * 0.5f + GetPos().y + margin_));
	}

	void Object::SetDrawSize(const ML::Box2D& drawSize)
	{
		drawSize_ = drawSize;
		ge->playerPtr->GetHPBar()->SetDrawSize(drawSize_.w * 0.8f, drawSize_.h * 0.2f);
	}

	void Object::SetBackGroundAlpha(const float alpha)
	{
		alpha_ = alpha;
	}

	void Object::SetMargin(const float margin)
	{
		margin_ = margin;
		NormalizeHPBarPos();
	}
	
	void Object::SetDrawArea(DrawArea drawArea)
	{
		switch (drawArea)
		{
		case DrawArea::LeftTop:
			SetPos(ML::Vec2(0, 0));
			break;

		case DrawArea::RightTop:
			SetPos(ML::Vec2(ge->screenWidth - drawSize_.w, 0));
			break;

		case DrawArea::RightBottom:
			SetPos(ML::Vec2(ge->screenWidth - drawSize_.w, ge->screenHeight - drawSize_.h));
			break;

		case DrawArea::LeftBottom:
			SetPos(ML::Vec2(0, ge->screenHeight - drawSize_.h));
			break;
		}

		NormalizeHPBarPos();
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