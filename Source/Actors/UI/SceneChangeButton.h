#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//シーンを切り替えるボタン
//-------------------------------------------------------------------
#include "../../../ToggleButton.h"
#include "../../../Scene.h"

namespace SceneChangeButton
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");					//グループ名
	const  string  defName("Button");					//タスク名
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
	};
	//-------------------------------------------------------------------
	class  Object : public  ToggleButton
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		typedef  shared_ptr<vector<Object::SP>> SPV;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;

	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()		override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()	override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理

	private:

		Scene* nowScene_;
		Scene::Kind nextScene_;

	public:
		struct SetInfo
		{
			string		  text;
			Scene*		  nowScene;
			Scene::Kind	  nextScene;
			XI::Mouse::MB mouseEnterButton;
			XI::VGP		  gamePadEnterButton;
			const Actor* selectorPtr;
		};

		void Set(const string& text,Scene* nowScene, const Scene::Kind nextScene, const XI::Mouse::MB mouseEnterButton, const XI::VGP gamePadEnterButton, const Actor* selectorPtr);
		void Set(const SetInfo& setInfo);

		void SetScene(Scene* nowScene, const Scene::Kind& nextScene);

		void OnEvent() override;
		void OffEvent() override;

		string debugText = "NON";
	};
}