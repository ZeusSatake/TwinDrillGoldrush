//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../../MyPG.h"
#include  "Task_Ev_Message.h"
#include "../../Character.h"
#include "../Actors/Task_Player.h"
#include "../System/Task_Save.h"
#include  "../Actors/Task_MiningResult.h"

namespace Ev_Message
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img[0] = DG::Image::Create("./data/image/MessageF.png");
		this->img[1] = DG::Image::Create("./data/image/message.png");//以下仮
		this->img[2] = DG::Image::Create("./data/image/MessageF.png");
		this->img[3] = DG::Image::Create("./data/image/MessageF.png");
		this->img[4] = DG::Image::Create("./data/image/MessageF.png");
		this->img[5] = DG::Image::Create("./data/image/MessageF.png");
		this->img[6] = DG::Image::Create("./data/image/MessageF.png");
		this->img[7] = DG::Image::Create("./data/image/MessageF.png");
		this->img[8] = DG::Image::Create("./data/image/MessageF.png");
		this->img[9] = DG::Image::Create("./data/image/MessageF.png");

		this->font[0] = DG::Font::Create("HG丸ゴシックM-PRO", 8, 16);
		this->font[1] = DG::Font::Create("HG丸ゴシックM-PRO", 12, 24);
		this->font[2] = DG::Font::Create("HG丸ゴシックM-PRO", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		for (int i = 0; i < _countof(this->img); ++i)
		{
			this->img[i].reset();
		}
		for (int i = 0; i < _countof(this->font); ++i)
		{
			this->font[i].reset();
		}
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
		this->Stop();
		this->render2D_Priority[1] = 0.1f;
		this->pos.x = 0;
		this->pos.y = ge->screen2DHeight - 128;//270
		this->msgText = "";
		this->bgNumber = 1;
		this->fontNumber = 1;
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		//制限時間経過もしくはボタンが押されたとき
		if ((this->timeLimit != 0 && this->timeCnt >= this->timeLimit) || inp.B2.down)
		{
			//自身は停止状態にする
			this->Stop();
			//エンジンの停止状態を解除する
			ge->StopAll_GN("イベント", "実行エンジン", false);
		}
		else
		{
			this->timeCnt++;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, ge->screen2DWidth, 128);
		ML::Box2D src(0, 0, 1280, 206);
		draw.Offset(this->pos);
		this->res->img[this->bgNumber]->Draw(draw, src, ML::Color(1, 1, 1, 1));
		ML::Box2D drawF(50, 15, ge->screen2DWidth - 12, 128 - 12);
		drawF.Offset(this->pos);
		this->res->font[this->fontNumber]->Draw(drawF, this->msgText,ML::Color(1,0.1f,0.1f,0.1f));
	}

	//-------------------------------------------------------------------
	void  Object::Set(const string& taskName_, stringstream& ss_)
	{
		//パラメータを分解する
		int timeL;
		string msg;
		ss_ >> timeL >> msg;
		//タスク名を設定
		this->name = taskName_;
		//メッセージ枠消去要求へ対応する
		if (msg == "off")
		{
			this->Kill();
			return;
		}
		this->timeCnt = 0;
		this->timeLimit = timeL;
		this->msgText = msg;
		//*を改行に置き換える
		string::size_type ast;
		while ((ast = msgText.find("*")) != string::npos)
		{
			msgText.at(ast) = '\n';
		}

		//&を名前に置き換える
		string::size_type ast1;

		while ((ast1 = msgText.find("&")) != string::npos)
		{
			msgText.replace(ast1, 1, ge->playerPtr->GetPersonalName());
		}

		//%を目標鉱石に置き換える
		string::size_type ast2;

		while ((ast2 = msgText.find("%")) != string::npos)
		{
			string orename[5] = { "ダマスカス","オリハルコン","ヒヒイロカネ","アダマンタイト","レディアンタイト" };
			auto save = Save::Object::Create(true);
			msgText.replace(ast2, 1, orename[save->GetValue<int>(Save::Object::ValueKind::MiningStageNo)]);
		}

		//イベントエンジンを一時停止させる
		ge->StopAll_GN("イベント", "実行エンジン");
		//自身の停止状態を解除する
		this->Stop(false);
	}

	//-------------------------------------------------------------------
	void Object::CreateOrReset(stringstream& ss_)
	{
		//パラメータを分解する
		string taskName;
		ss_ >> taskName;

		//新規作成か更新の判断
		auto p = ge->GetTask<Object>("メッセージ表示枠", taskName);
		//新規作成の場合
		if (nullptr == p)
		{
			p = Object::Create(true);
			p->Set(taskName, ss_);
		}
		//更新の場合
		else
		{
			p->Set(taskName, ss_);
		}
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