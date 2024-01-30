#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../../UIBase/PushButton.h"

class PriceTagComponent;
class WalletComponent;
class Player;

namespace BuyButton
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("UI");	//�O���[�v��
	const  string  defName("Button");		//�^�X�N��
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
	class  Object : public  PushButton
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

		DG::Image::SP image_;
		ML::Box2D	  imageSize_;

		shared_ptr<PriceTagComponent> priceTag_;
		weak_ptr<WalletComponent> buyerWallet_;
		std::function<void()> buyEffect_;

		int buyAmount_;

		void OnEvent() override;
	public:
		void SetImage(const string& path);
		void SetDrawSize(const ML::Point& size);
		void SetImageSize(const ML::Point& size);

		void SetPriceTag(const string& name, const int price);
		void SetBuyerWallet(const shared_ptr<WalletComponent>& wallet);
		void SetBuyEffect(const std::function<void()>& buyEffect);
		void SetProduct(const string& name, const int price, const std::function<void()>& buyEffect);
		void SetBuyAmount(const int amount);
		const shared_ptr<const WalletComponent> GetBuyerWallet() const;
		const shared_ptr<const PriceTagComponent> GetPriceTag() const;
	};
}