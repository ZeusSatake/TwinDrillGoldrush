#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//数字を画像で描画
//-------------------------------------------------------------------
#include "../../GameEngine_Ver3_83.h"

namespace DrawNumFont
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("システム");	//グループ名
	const  string  defName("DrawNumFont");	//タスク名
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
		//共有する変数はここに追加する
		
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
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