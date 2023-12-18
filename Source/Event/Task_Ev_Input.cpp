//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_Ev_Input.h"
#include "Task_InputMap.h"
#include "../../Character.h"
#include "../Actors/Task_Player.h"
#include <iostream>

namespace Ev_Input
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/preDrill.png");
		this->font= DG::Font::Create("HG丸ゴシックM-PRO", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->font.reset();
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
		this->render2D_Priority[1] = 0.2f;
		this->maxName = 8;
		this->nameText = "なまえ　";
		this->name = "";
		x1 = 0;
		y1 = 0;
		this->flag = false;
		
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
		auto Map = ge->GetTask<InputMap::Object>(InputMap::defGroupName, InputMap::defName);
		
		if (inp.LStick.BD.down)
		{
			if (y1 < 9)
			{
				y1++;
				if (Map->arr[y1][x1] == "")
				{
					y1--;
				}
			}
		}
		if (inp.LStick.BU.down)
		{
			if (y1 >0)
			{
				y1--;
				if (Map->arr[y1][x1] == "")
				{
					y1++;
				}
			}
		}
		if (inp.LStick.BL.down)
		{
			if (x1 > 0)
			{
				x1--;
				if (Map->arr[y1][x1] == "")
				{
					x1++;
				}
			}
		}
		if (inp.LStick.BR.down)
		{
			if (x1 < 9)
			{
				x1++;
				if (Map->arr[y1][x1] == "")
				{
					x1--;
				}
			}
		}
		if (inp.B1.down && flag == false)
		{//Aボタンで一文字追加(予定)
			if (this->name.size() / 2 < maxName)
			{
				this->name += Map->arr[y1][x1];
			}
		}
		if (inp.B2.down && flag == false)
		{//Bボタンで一文字削除(予定)
			if (this->name.size() != 0)
			{
				this->name.pop_back();
				this->name.pop_back();
			}
		}
		if (inp.B3.down&& this->name.size()!=0)
		{//決定
			this->flag = true;
		}
		if (inp.B2.down&&flag == true)
		{//確認後キャンセル
			flag = false;
		}
		if (inp.B1.down&&flag == true)
		{//確認後決定
			ge->playerPtr->SetPersonalName(this->name);
			Map->Kill();
			this->Kill();
			ge->StopAll_GN("イベント", "実行エンジン", false);
		}
		this->now = Map->arr[y1][x1];
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (flag == false)
		{
			//なまえ：表示
			ML::Box2D draw(ge->screen2DWidth/4+50, 20, 160, 32);
			this->res->font->Draw(draw, nameText, ML::Color(1, 1, 1, 1));
			//なまえ表示
			ML::Box2D draw2(ge->screen2DWidth / 4+175, 20, 320, 32);
			this->res->font->Draw(draw2, this->name, ML::Color(1, 1, 1, 1));
			//カーソル表示
			ML::Box2D src1(0, 0, 64, 64);
			auto map = ge->GetTask<InputMap::Object>(InputMap::defGroupName, InputMap::defName);
			for (int y = 0; y < map->sizeY; ++y)
			{
				for (int x = 0; x < map->sizeX; ++x)
				{
					if (map->arr[y][x] != "")
					{
						ML::Box2D draw1(ge->screen2DWidth/4-15, 106, 16, 16);
						if (this->now == map->arr[y][x])
						{
							draw1.Offset(x * (map->chipSize+5), y * (map->chipSize + 5));
							this->res->img->Draw(draw1, src1);
						}
					}
				}
			}
			{//キー表示
				ML::Box2D draw(0, 0, 400, 32);
				ML::Box2D draw1(0, 50, 400, 32);
				ML::Box2D draw2(0, 100, 400, 32);
				string text = "一文字追加Z(A)";
				string text1 = "一文字削除X(B)";
				string text2 = "決定C(X)";
				this->res->font->Draw(draw, text, ML::Color(1, 1, 1, 1));
				this->res->font->Draw(draw1, text1, ML::Color(1, 1, 1, 1));
				this->res->font->Draw(draw2, text2, ML::Color(1, 1, 1, 1));
			}
		}
		else
		{//確認メッセージ　キー表示
			ML::Box2D draw(ge->screen2DWidth/4, ge->screen2DHeight/3, 800, 32);
			ML::Box2D draw1(100, ge->screen2DHeight / 3+100, 800, 32);
			ML::Box2D draw2(100, ge->screen2DHeight / 3 + 150, 800, 32);
			string text = "「" + this->name + "」でよろしいですか？";
			string text1 = "はい　Zキー(A)";
			string text2 ="いいえ　Xキー(B)";
			this->res->font->Draw(draw, text, ML::Color(1, 1, 1, 1));
			this->res->font->Draw(draw1, text1, ML::Color(1, 1, 1, 1));
			this->res->font->Draw(draw2, text2, ML::Color(1, 1, 1, 1));
		}
	}

	//-------------------------------------------------------------------
	void  Object::Set(const string& taskName_, stringstream& ss_)
	{
		////パラメータを分解する
		//string msg;
		//ss_ >> msg;
		////タスク名を設定
		//this->name = taskName_;

		auto map = InputMap::Object::Create(true);
		map->Load("Book2");
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
		auto p = ge->GetTask<Object>("入力", taskName);
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