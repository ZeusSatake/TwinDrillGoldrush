#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[�W
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"

namespace DrawGauge
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("System");	//�O���[�v��
	const  string  defName("DrawGauge");	//�^�X�N��
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
	private:
		int count_;
		int max_;

		bool isMaxCharge;
	public:
		ML::Vec2 pos;

		void SetUp(const int max, const string& path);

		void CountUp(int add);
		int GetCount() const;
		void ResetCount();

		void SetMax(const int max);
		int Getmax() const;

		void SetImg(const string& path);

		bool IsMax() const;

		float Normalized() const;
	};
}