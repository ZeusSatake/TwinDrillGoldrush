#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
<<<<<<<< HEAD:Source/Actors/Task_MiningResult.h
#include "../../GameEngine_Ver3_83.h"

#include "../Scene/Task_Map.h"
#include "../Scene/Task_JewelryMap.h"
========
#include "../../EnemyClasses/LadySatake.h"
>>>>>>>> 8a7d750acb68ea404fe75d5c571012e50c0b5934:Source/Actors/Enemys/Task_LadySatake.h

namespace Satake
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G");	//�O���[�v��
	const  string  defName("���|����l");//�^�X�N��
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
<<<<<<<< HEAD:Source/Actors/Task_MiningResult.h

	private:
========
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
		DG::Image::SP img;
		DG::Image::SP fanImg;
>>>>>>>> 8a7d750acb68ea404fe75d5c571012e50c0b5934:Source/Actors/Enemys/Task_LadySatake.h
	};
	//-------------------------------------------------------------------
	class  Object : public  LadySatake
	{
<<<<<<<< HEAD:Source/Actors/Task_MiningResult.h
		//�ύX�s����������������������������������������������������
========
>>>>>>>> 8a7d750acb68ea404fe75d5c571012e50c0b5934:Source/Actors/Enemys/Task_LadySatake.h
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

		map<Map::Object::ChipKind, shared_ptr<PriceTagComponent>> orePriceTags_;
		map<JewelryMap::Object::ChipKind, shared_ptr<PriceTagComponent>> jewelryPriceTags_;

		map<Map::Object::ChipKind, int> getOreCount_;
		map<JewelryMap::Object::ChipKind, int> getJewelryCount_;

		static const Map::Object::ChipKind sellableOres_[];
		static string SellableOreName(const Map::Object::ChipKind kind);
		static const JewelryMap::Object::ChipKind sellableJewelrys_[];
		static string SellableJewelryName(const JewelryMap::Object::ChipKind kind);

		static bool IsSellableOre(const Map::Object::ChipKind oreKind);
		static bool IsSellableJewelry(const JewelryMap::Object::ChipKind oreKind);

		int CalcTotalPrice() const;
	public:
<<<<<<<< HEAD:Source/Actors/Task_MiningResult.h

		void CountUpOre(const Map::Object::ChipKind oreKind);
		void CountUpJewelry(const JewelryMap::Object::ChipKind jewelryKind);
========
	//�ύX������������������������������������������������������
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����
>>>>>>>> 8a7d750acb68ea404fe75d5c571012e50c0b5934:Source/Actors/Enemys/Task_LadySatake.h
	};
}