#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�������摜�ŕ`��
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"

namespace DrawNumFont
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�V�X�e��");	//�O���[�v��
	const  string  defName("DrawNumFont");	//�^�X�N��
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
		DG::Image::SP img;
		ML::Box2D charDraw;
		ML::Box2D charSrc;
		int drawDigit;
		int drawValue;
		ML::Vec2 drawPos;
	public:
		void SetDrawValue(const int value);
		void SetImg(const string& imgPath);
		void SetCharDraw(const ML::Box2D& charDraw);
		ML::Box2D GetCharDraw() const;
		void SetCharSrc(const ML::Box2D& charSrc);
		void SetDrawDigit(const int drawDigit);
		void SetDrawPos(const ML::Vec2& drawPos);
		void SetUp(const string& imgPath, const ML::Box2D& charSrc, const ML::Box2D& charDraw, const int drawDigit, const ML::Vec2& drawPos);
	};
}