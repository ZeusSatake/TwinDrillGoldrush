#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�u���b�N�}�l�[�W���[
//-------------------------------------------------------------------
#include	"../../../BBlocks.h"

namespace	Manager
{
	class BlockState
	{
	public:
		int HP;
		int MaxHP;
		int event;
	};
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName(	"Blocks");	//�O���[�v��
	const  string  defName(			"BManager");		//�^�X�N��
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
		//�ύX������������������������������������������������������
			//���L����ϐ��͂����ɒǉ�����
	};
	//-------------------------------------------------------------------
	class  Object : public	BBlocks
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
	public:
		//�ύX������������������������������������������������������
			//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
			//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����
		void Damage(const ML::Point pos, int power);
		void InitArray();					//�u���b�NHP�̏������Ɏg�p

	private:
		BlockState  arr[160][160];//yx

		void eventSearch(int y_, int x_);	//�u���b�N�̃C�x���g(����)���Ăяo��		
		//��			num		�j��
		void Stone(ML::Vec2 pos);			//��			.6		.��
		void Bedrock(ML::Vec2 pos);			//���		.7		.�s��
		void collapseBlock(int x, int y);	//�A������	.10		.��
		void IronOre(ML::Vec2 pos);			//�S�z��		.11		.��
	};
}