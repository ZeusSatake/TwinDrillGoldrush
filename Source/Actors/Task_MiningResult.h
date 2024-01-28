#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�̌@�ꃊ�U���g
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"

#include "../Scene/Task_Map.h"
#include "../Scene/Task_JewelryMap.h"

class Scene;
class PriceTagComponent;
class SecondsTimerComponent;
namespace EventEngine { class Object; }

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

		using OreKind = Map::Object::ChipKind;
		using JewelryKind = JewelryMap::Object::ChipKind;


		Scene* nowScene_;
		int nowStage_;

		bool clear_;
		shared_ptr<SecondsTimerComponent> transitionTimer_;
		weak_ptr<EventEngine::Object> clearEvent_;

		int initialHaveMoney_;

		OreKind targetOreKind_;
		int needTargetDestroyAmount_;

		map<OreKind, int> getOreCount_;
		map<JewelryKind, int> getJewelryCount_;

		map<OreKind, shared_ptr<PriceTagComponent>> orePriceTags_;
		map<JewelryKind, shared_ptr<PriceTagComponent>> jewelryPriceTags_;

		int totalSellingPrice_;
		void CalcTotalSellingPrice();

		static const OreKind sellableOres_[];
		static string SellableOreName(const OreKind kind);
		static const JewelryKind sellableJewelrys_[];
		static string SellableJewelryName(const JewelryKind kind);

		static bool IsSellableOre(const OreKind oreKind);
		static bool IsSellableJewelry(const JewelryKind oreKind);
	public:
		void SetTargetOre(const OreKind oreKind, const int needDestroyAmount);
		void SetNowSecene(Scene* scene);
		void CountUpOre(const OreKind oreKind);
		void CountUpJewelry(const JewelryKind jewelryKind);
	};
}