//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "SceneChangeButton.h"

#include  "../../Scene/TitleScene.h"
#include  "../../Scene/GameScene.h"
#include  "../../Scene/ShopScene.h"
#include  "../../Scene/EndingScene.h"

namespace SceneChangeButton
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
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
		box_->setHitBase(ML::Box2D(-100, -50, 200, 100));
		
		SetEnterButton(XI::VGP::B1);
		SetRecieveInputEnable(true);
		SetSelected(false);
		SetMouse(ge->mouse);
		
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
		ToggleButton::UpDate();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		Drawtext(ge->debugFont, false);
	}
	void Object::OnEvent()
	{
		nowScene_->SetNextScene(nextScene_);
	}
	void Object::OffEvent()
	{
	}
	void Object::Set(const string& text, Scene* nowScene, const Scene::Kind nextScene, const XI::Mouse::MB mouseEnterButton, const XI::VGP gamePadEnterButton, const Actor* selectorPtr)
	{
		SetText(text);
		SetScene(nowScene, nextScene);
		SetEnterButton(mouseEnterButton);
		SetEnterButton(gamePadEnterButton);
		if (selectorPtr != nullptr)
			SetSelector(selectorPtr);
	}
	void Object::Set(const SetInfo& setInfo)
	{
		Set(
			setInfo.text,
			setInfo.nowScene,
			setInfo.nextScene,
			setInfo.mouseEnterButton,
			setInfo.gamePadEnterButton,
			setInfo.selectorPtr
		);
	}
	void Object::SetScene(Scene* nowScene, const Scene::Kind& nextScene)
	{
		nowScene_ = nowScene;
		nextScene_ = nextScene;
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