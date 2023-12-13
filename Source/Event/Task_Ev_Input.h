#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//���b�Z�[�W�\��
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"
#include "../Actors/Task_Player.h"
#include "../../GameObject.h"

namespace  Ev_Input
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName(		"����");	//�O���[�v��
	const  string  defName(				"NoName");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		DG::Image::SP img;
		DG::Font::SP font;
	};
	//-------------------------------------------------------------------
	class  Object : public  GameObject
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		string nameText;
		string table[5][10];
		int maxName;
		string name;
		bool flag;
		string now;
		int x1, y1;
		//�^�X�N�𐶐����邩�A�����̃^�X�N�̏����X�V����
		static void CreateOrReset(stringstream& ss_);
		//�\�����郁�b�Z�[�W��ݒ肷��
		void Set(const string& taskName_, stringstream& ss_);

		Player player;
	};
}