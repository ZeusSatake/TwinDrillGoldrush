#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�V�[����؂�ւ���{�^��
//-------------------------------------------------------------------
#include "../../../ToggleButton.h"
#include "../../../Scene.h"

namespace SceneChangeButton
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("UI");					//�O���[�v��
	const  string  defName("Button");					//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()		override;
		bool  Finalize()		override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	};
	//-------------------------------------------------------------------
	class  Object : public  ToggleButton
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		typedef  shared_ptr<vector<Object::SP>> SPV;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;

	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()		override;	//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()	override;	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������

	private:

		Scene* nowScene_;
		Scene::Kind nextScene_;

	public:
		struct SetInfo
		{
			string		  text;
			Scene*		  nowScene;
			Scene::Kind	  nextScene;
			XI::Mouse::MB mouseEnterButton;
			XI::VGP		  gamePadEnterButton;
			const Actor* selectorPtr;
		};

		void Set(const string& text,Scene* nowScene, const Scene::Kind nextScene, const XI::Mouse::MB mouseEnterButton, const XI::VGP gamePadEnterButton, const Actor* selectorPtr);
		void Set(const SetInfo& setInfo);

		void SetScene(Scene* nowScene, const Scene::Kind& nextScene);

		void OnEvent() override;
		void OffEvent() override;

		string debugText = "NON";
	};
}