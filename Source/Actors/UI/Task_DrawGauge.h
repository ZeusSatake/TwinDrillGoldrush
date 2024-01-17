#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[�W
//-------------------------------------------------------------------
#include "../../../GameEngine_Ver3_83.h"
#include "../../../GameObject.h"

namespace DrawGauge
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("UI");	//�O���[�v��
	const  string  defName("HPBar");	//�^�X�N��
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
	private:
		ML::Percentage gaugeValue_;

		bool isMaxCharge;
		bool isSupportScroll_;
		bool isVisible_;

		ML::Vec2 pos_;
		ML::Point size_;
	public:

		void Set(const int max, const string& path);
		void Set(const ML::Percentage& value);

		void SetMax(const int max);
		void SetMin(const int min);
		int Getmax() const;

		void SetImg(const string& path);
		void SetDrawSize(const int width, const int height);
		void SetPos(const ML::Vec2& pos);
		void SetPos(const float x, const float y);

		void SetSupportScroll(const bool isSupportScroll);
		void SetVisible(const bool visible);

		bool IsMax() const;
		ML::Point GetSize() const;
	};
}