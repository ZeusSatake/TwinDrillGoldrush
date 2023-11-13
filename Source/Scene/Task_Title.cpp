//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "../../randomLib.h"
#include  "../../Task_Effect00.h"
#include  "../../sound.h"
#include  "../../easing.h"

#include  "Task_Title.h"
#include  "Task_Game.h"

#include  "../System/Task_BackGround.h"
#include  "../System/Task_FlashDraw.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		TitleLogo01 = DG::Image::Create("./data/image/title.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		TitleLogo01.reset();
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
		this->controller = ge->in1;
		this->render2D_Priority[1] = 0.5f;
		int RenderTime = 0;

		ge->stage = 1;

		//デバッグ用フォントの準備
		this->TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);

		//音楽ファイルの読み込み
		//BGM
		// BGMの利用するファイルは曲の長さによってサイズが変わります。
		// 一般的にWavは中身が波形データと呼ばれるサイズが大きなデータにります。
		// 一方mp3はネットなどでの扱いを想定した圧縮形式です。
		// BGMを用いる場合はmp3形式のファイルを利用しましょう。
		// ちなみにこのサンプルのファイルは
		// 再生時間1：30ほぼの曲で
		// mp3 = 4.3MB wav = 19MBです。
		//bgm::LoadFile("bgm1", "./data/sound/TitleBGM.mp3");
		//bgm::Play("bgm1");

		//se
		// seは効果音です。
		// bgmとの最大の違いはひとつの音楽ファイルの同時再生に対応していることです。
		// seはwavファイルしか扱うことが出来ません。mp3はエラーになります。
		// 同時再生の必要がないものはBGM
		// 同時再生が必要なものはseと使い分けてください。
	  // またこのサンプルのような日本語ファイル名はやめた方が良いです。
		// 読み込みエラーの元になります。
		//se::LoadFile("se1","./data/sound/se/「すごいすごい」.wav");

//		se::LoadFile("se2", "./data/sound/se/「頑張ったね」.mp3");これはエラー wavじゃないとダメ
		//se::LoadFile("se2", "./data/sound/se/「頑張ったね」.wav");

		//se::LoadFile("ok", "./data/sound/ok.wav");

		easing::Set("titleX", easing::CIRCOUT, -200, (float)ge->screenCenterPos.x, 120);
		easing::Start("titleX");
		easing::Set("titleY", easing::BOUNCEOUT, -200, (float)ge->screenCenterPos.y, 120);
		easing::Start("titleY");

		frameCnt = 0;

		//★タスクの生成
		auto backGound = BackGround::Object::Create(true);
		backGound->SetUp(
			"./data/image/titleback.png",
			ML::Point{ 1920, 1200 },
			ML::Point{ (int)ge->screenWidth, (int)ge->screenHeight });
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("背景");
		//bgm::Stop("bgm1");
		ge->KillAll_G("システム");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		++frameCnt;
		auto inp = ge->in1->GetState();

		if (inp.SE.down) {
			ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
			ge->CreateEffect(98, ML::Vec2(0, 0));
			//se::Play("ok");
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		easing::UpDate();
		this->logoY = easing::GetPos("titleY");
		this->logoX = easing::GetPos("titleX");

		if (frameCnt == 150) {
			auto PressStartKey = FlashDraw::Object::Create(true);
			MyPG::MyGameEngine::DrawInfo drawInfo{
				DG::Image::Create("./data/image/click.png"),
				ML::Box2D(-143, -32, 286, 64),
				ML::Box2D(0, 0, 286, 64),
				ML::Vec2(ge->screenCenterPos.x, ge->screenHeight - 160.0f)
			};
			PressStartKey->SetUp(drawInfo, 30, 0.3f);
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, 792, 89);
		ML::Box2D draw((int)(-src.w * 0.5f), 0, src.w, src.h);
		draw.Offset(ML::Vec2(logoX, logoY));
		res->TitleLogo01->Draw(draw, src);
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