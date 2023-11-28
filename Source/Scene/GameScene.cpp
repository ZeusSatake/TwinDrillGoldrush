//-------------------------------------------------------------------
//Q[{า
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "GameScene.h"

#include  "../../randomLib.h"
#include  "../../sound.h"

#include  "EndingScene.h"

#include  "../System/Task_BackGround.h"
#include  "Task_Map.h"
#include  "Task_EnemyMap.h"

#include  "../Actors/UI/SceneChangeButton.h"

namespace  GameScene
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//u๚ปv^XNถฌษP๑พฏsค
	bool  Object::Initialize()
	{
		//X[p[NX๚ป
		__super::Initialize(defGroupName, defName, true);
		//\[XNXถฌor\[XคL
		this->res = Resource::Create();

		//f[^๚ป
		this->render2D_Priority[1] = 0.0f;
		ge->debugRectLoad();

		ge->GameOverFlag = false;
		ge->GameClearFlag = false;
		ge->gameScreenWidth = ge->screenWidth;
		
		fontImg.img = DG::Image::Create("./data/image/font_number.png");
		fontImg.size = ML::Point{ 20, 32 };
		ge->score = 0;
		
		//fobOptHgฬ๕
		this->TestFont = DG::Font::Create("lr SVbN", 30, 30);

		//^XNฬถฌ


		{//wi^XNถฌ
			ML::Point imgSize = { 960, 500 };
			ML::Point drawSize = { (int)ge->screenWidth, (int)ge->screenHeight };
			int sprit = 1;
			auto back = BackGround::Object::Create(true);
			back->SetUp("./data/image/gameback.png",
						imgSize,
						drawSize,
						BackGround::Object::RenderSize::FullScreen,
						sprit);
		}
		
		{//ฮ zฮ
			auto map = Map::Object::Create(true);
			map->Load("MapStone");
		}
		{//๓ฮ
			auto mapore = Map::Object::Create(true);
			mapore->Load("MapJewelry");
			mapore->render2D_Priority[1] = 0.85f;
		}
		//{//G
		//	auto enemymap = EnemyMap::Object::Create(true);
		//	enemymap->Load("MapEnemy");
		//	enemymap->SetEnemy();
		//}

		{//_ษ฿้{^(fobOp
			auto gotoBaseButton = SceneChangeButton::Object::Create(true);
			gotoBaseButton->SetEnterButton(XI::VGP::ST);
			gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
			gotoBaseButton->SetScene(this, Scene::Kind::Base);
			gotoBaseButton->SetText("_ึ");
			AddSceneChangeButton(gotoBaseButton);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uIนv^XNมลษP๑พฏsค
	bool  Object::Finalize()
	{
		//f[^^XN๐๚

		ge->KillAll_G("{า");
		ge->KillAll_G("VXe");
		ge->KillAll_G(SceneChangeButton::defGroupName);

		ge->debugRectReset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//๘ซpฌ^XNฬถฌ
			CreateNextScene();
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[ษsค
	void  Object::UpDate()
	{
		Scene::UpDate();

		auto inp = ge->in1->GetState();
		if (inp.SE.down) {
			this->Kill();
		}
		if (ge->GameOverFlag) {
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
	}

	//
	//ศบอ๎{IษฯXsvศ\bh
	//
	//-------------------------------------------------------------------
	//^XNถฌ๛
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWษo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYษธsตฝ็Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//\[XNXฬถฌ
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}