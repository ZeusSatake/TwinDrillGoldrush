#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�w�i�\��
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"

namespace BackGround
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�V�X�e��");	//�O���[�v��
	const  string  defName("�w�i");	//�^�X�N��
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
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
		//�`�惂�[�h
		enum class RenderSize {
			FullScreen,			//�S���
			Sprit_Horizontal,	//���� �c
			Sprit_Vertical,		//���� ��
		};
		enum class ScrollDirection {
			Up	 ,
			Down ,
			Left ,
			Right,
		};
	private:
		DG::Image::SP img;
		string imgPath;
		ML::Point imgSize;
		int spritNum;
		RenderSize renderSize;
		ML::Point drawSize_Sprit;
		ML::Point drawSize_Full;

		//�X�N���[���p�ϐ�
		bool isScroll;
		ML::Vec2 pos;
		ML::Vec2 resetPos;
		ML::Vec2 scrollDirTable[4] = {
			ML::Vec2( 0, -1),
			ML::Vec2( 0,  1),
			ML::Vec2(-1,  0),
			ML::Vec2( 1,  0),
		};
		ScrollDirection scrollDirection;
		int scrollSpeed;
		void CreateSubScreen();
	protected:
		bool isSubScreen;
	public:	
		//�Z�b�g�A�b�v �p�X�A�T�C�Y
		void SetUp(string imgPath_, ML::Point imgSize_, ML::Point drawSize_, RenderSize renderSize_ = RenderSize::FullScreen, int spritNum_ = 1);
		//�`��T�C�Y�ݒ�
		void SetDrawSize(ML::Point drawSize_, RenderSize renderMode_, int spritNum_);
		void SetScroll(ML::Vec2 pos_, int speed, ScrollDirection scrollDirection_);
		ML::Point GetDrawSize();
	};
}