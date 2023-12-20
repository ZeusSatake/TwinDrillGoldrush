#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�̌@�ꃊ�U���g
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"

#include "../Scene/Task_Map.h"
#include "../Scene/Task_JewelryMap.h"

class PriceTagComponent;
class Scene;

namespace MiningResult
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("UI");	//�O���[�v��
	const  string  defName("MiningResult");	//�^�X�N��
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

	private:
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

		map<Map::Object::ChipKind, shared_ptr<PriceTagComponent>> orePriceTags_;
		map<JewelryMap::Object::ChipKind, shared_ptr<PriceTagComponent>> jewelryPriceTags_;

		map<Map::Object::ChipKind, int> getOreCount_;
		map<JewelryMap::Object::ChipKind, int> getJewelryCount_;

		Scene* nowScene_;
		Map::Object::ChipKind targetOreKind_;
		int needTargetDestroyAmount_;

		static const Map::Object::ChipKind sellableOres_[];
		static string SellableOreName(const Map::Object::ChipKind kind);
		static const JewelryMap::Object::ChipKind sellableJewelrys_[];
		static string SellableJewelryName(const JewelryMap::Object::ChipKind kind);

		static bool IsSellableOre(const Map::Object::ChipKind oreKind);
		static bool IsSellableJewelry(const JewelryMap::Object::ChipKind oreKind);

		int CalcTotalSellingPrice() const;
	public:
		void SetTargetOre(const Map::Object::ChipKind oreKind, const int needDestroyAmount);
		void SetNowSecene(Scene* scene);
		void CountUpOre(const Map::Object::ChipKind oreKind);
		void CountUpJewelry(const JewelryMap::Object::ChipKind jewelryKind);
	};
}