//-------------------------------------------------------------------
//エンディング
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "EndingScene.h"
#include  "TitleScene.h"
#include  "../../randomLib.h"
#include  "../../sound.h"

#include  "../System/Task_FlashDraw.h"
#include  "../System/Task_BackGround.h"
#include  "../Event/Task_EventEngine.h"

namespace  EndingScene
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		resultImg = DG::Image::Create("./data/image/result.png");
		rankImg = DG::Image::Create("./data/image/rank.png");
		font = DG::Font::Create("ＭＳ ゴシック", 32, 64);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		resultImg.reset();
		rankImg.reset();
		font.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		frameCount = 0;
		bgm::LoadFile("ending", "./data/sound/ending.mp3");
		//★タスクの生成
		{//画像フォント(スコア
			//int digit = 4;
			//ML::Box2D draw(0, 0, 48, 64);
			//ML::Box2D src(0, 0, 24, 32);
			//ML::Vec2 pos = ge->screenCenterPos;
			//pos.x -= draw.w * 0.5f * digit;
			//pos.y -= draw.h * 0.5f - 50;
			//drawScore = DrawNumFont::Object::Create(true);
			//drawScore->SetUp("./data/image/notext.png", src, draw, digit, pos);
		}

		{//背景タスク生成
			ML::Point imgSize;
			imgSize.x = 800;
			imgSize.y = 600;

			ML::Point drawSize;
			drawSize.x = (int)ge->screenWidth;
			drawSize.y = (int)ge->screenHeight;

			auto backGround = BackGround::Object::Create(true);
			backGround->SetUp("./data/image/back.png", imgSize, drawSize);
		}

		{
			endingEvent_ = EventEngine::Object::Create_Mutex();
			endingEvent_.lock()->Set("./data/event/Ending.txt");
		}
		
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("背景");
		ge->KillAll_G("システム");
		bgm::Stop("ending");
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto  nextTask = TitleScene::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		bgm::Play("ending");
		auto inp = ge->in1->GetState();

		if (inp.SE.down) {
			ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
			ge->CreateEffect(99, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		if (endingEvent_.lock())
			Kill();

		++frameCount;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "End");

		{//リザルト
			//ML::Box2D draw(0, 0, 512, 124);
			//draw.x = (int)ge->screenCenterPos.x - 140;
			//draw.y = (int)ge->screenCenterPos.y - 150;
			//ML::Box2D src(0, 0, 256, 64);
			//this->res->resultImg->Draw(draw, src);
		}
		
		//drawScore->SetDrawValue(ge->score);

		//PressStartKey
		//if (frameCount == 60) {
		//	auto PressStartKey = FlashDraw::Object::Create(true);
		//	MyPG::MyGameEngine::DrawInfo drawInfo{
		//		DG::Image::Create("./data/image/click.png"),
		//		ML::Box2D(-143, -32, 286, 64),
		//		ML::Box2D(0, 0, 286, 64),
		//		ML::Vec2(ge->screenCenterPos.x, ge->screenHeight - 160.0f)
		//	};
		//	PressStartKey->SetUp(drawInfo, 30, 0.3f);
		//}
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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