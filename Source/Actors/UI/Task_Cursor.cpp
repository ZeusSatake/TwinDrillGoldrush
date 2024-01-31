//-------------------------------------------------------------------
//UI�J�[�\��
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_Cursor.h"
#include  "../../Components/Movement.h"
#include  "../../UIBase/ToggleButton.h"

namespace Cursor
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		image_ = DG::Image::Create("./data/image/ui/cursor/preDrill.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		image_.reset();
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
		this->SetPos(ge->screenCenterPos);

		AddComponent(movement_ = make_shared<Movement>(this));
		//�p�����[�^�̐ݒ�
		movement_->SetSpeed(8.0f, 15.0f, 0.5f);
		movement_->SetDecelerationRate(ML::Percentage(30.0f));
		movement_->SetAcceleration(20.0f);

		box_->setHitBase(ML::Box2D(-28, -28, 56, 56));
		
		auto buttons = ge->GetTasks<ToggleButton>("UI", "Button");
		for (auto& button : *buttons)
		{
			button->SetSelector(this);
		}
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		movement_.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		movement_->LStickInputToMove(ge->in1);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D src(24, 24, 64, 64);
		res->image_->Draw(box_->getHitBase().OffsetCopy(this->GetPos()), src);
	}

	void Object::SetEnterButton(const XI::VGP enterButton)
	{
		enterButton_ = enterButton;
	}
	XI::VGP Object::GetEnterButton() const
	{
		return enterButton_;
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