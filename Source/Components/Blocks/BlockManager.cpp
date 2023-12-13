//-------------------------------------------------------------------
//ブロックマネージャー
//-------------------------------------------------------------------
#include	"../../../MyPG.h"
#include	"BlockManager.h"
#include	"../../Scene/Task_Map.h"
#include	"../../../sound.h"

namespace	Manager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//ブロックHP・特性設定
	void Object::InitArray()
	{
		auto map = ge->GetTask<Map::Object>("本編", MapName);
		for (int y = 0; y < map->sizeY; y++)
		{
			for (int x = 0; x < map->sizeX; x++)
			{
				int id = map->GetMapChip(y, x);
				switch (id) {
				case 0:		//空気ブロック
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					break;
				case 1:		//土
					this->arr[y][x].MaxHP = 1;
					this->arr[y][x].HP = 1;
					break;
				case 2:		//硬い土
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 7;
					break;
				case 3:		//苔むした土
					this->arr[y][x].MaxHP = 3;
					this->arr[y][x].HP = 3;
					break;
				case 6:		//石
					this->arr[y][x].MaxHP = 3;
					this->arr[y][x].HP = 3;
					this->arr[y][x].event = 6;
					break;
				case 7:		//岩盤
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 7;
					break;
				//====================鉱石====================
				case 9:		//石炭
					this->arr[y][x].MaxHP = 3;
					this->arr[y][x].HP = 3;
					this->arr[y][x].event = 9;
					break;
				case 10:	//崩れる石
					this->arr[y][x].MaxHP = 1;
					this->arr[y][x].HP = 1;
					this->arr[y][x].event = 10;
					break;
				case 11:	//鉄鉱石
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 11;
					break;
				case 12:	//金鉱石ぽいやつ
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 12;
					break;
				case 13:	//白金ぽいやつ
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 13;
					break;
				case 14:	//クロムぽいやつ
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 14;
					break;
				case 15:	//オリハルコンぽいやつ
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 15;
					break;
				case 16:	//パラジウムぽいやつ
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 15;
					break;
				case 17:	//アダマンタイトぽいやつ
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 15;
					break;
				//====================宝石====================
				case 18:	//ダイヤモンド
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 15;
					break;
				case 19:	//ブラックダイヤモンド
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 15;
					break;
				case 20:	//ピンクダイヤモンド
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 15;
					break;
				case 21:	//エメラルド
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 15;
					break;
				case 22:	//サファイア
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 15;
					break;
				case 23:	//ガーネット
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 23;
					break;
				case 24:	//トパーズ
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 23;
					break;
				case 25:	//アメジスト
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 23;
					break;
				case 27:
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					break;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//数値からブロック破壊処理
	void Object::Damage(const ML::Point pos_, int power)
	{
		int x = pos_.x;
		int y = pos_.y;
		if (this->arr[y][x].HP < 0)
		{
			this->eventSearch(y, x);
			return;
		}

		if (this->arr[y][x].HP - power > 0) { this->arr[y][x].HP -= power; }
		else if (this->arr[y][x].HP >= 0)
		{
			this->arr[y][x].HP -= power;

			auto map = ge->GetTask<Map::Object>("本編", MapName);
			this->eventSearch(y, x);
			map->SetMapChip(y, x, 0);
		}
	}
	//-------------------------------------------------------------------
	//
	void Object::eventSearch(int y_, int x_)
	{
		//取得番号から該当処理を呼び出す
		int eventNum = this->arr[y_][x_].event;
		auto pos = ML::Vec2(x_ * 16, y_ * 16);
		switch (eventNum)
		{
		case 0:
			break;
		case 6:
			Stone(pos);
			break;
		case 7:
			Bedrock(pos);
			break;
		case 10:
			collapseBlock(x_, y_);
			break;
		case 11:
			IronOre(pos);
			break;
		case 23:
			Stone(pos);
			break;
		default:
			break;
		}
	}
	//-------------------------------------------------------------------
	//ブロック特性・”Task_ブロック名”は使わないようにするかも
	void Object::Stone(ML::Vec2 pos)
	{
		ge->CreateEffect(11, pos, 6);
		se::Play("crush");
	}
	void Object::Bedrock(ML::Vec2 pos)
	{
		se::Play("repelled");
	}
	void Object::collapseBlock(int x,int y)
	{
		auto map = ge->GetTask<Map::Object>("本編", MapName);
		int id = map->GetMapChip(y, x);


		if (id != 10) { return; }
		if (id <=  0) { return; }
		map->SetMapChip(y, x, 0);

		ML::Point m[8] = {
			{-1,-1},{ 0,-1},{+1,-1},
			{-1, 0},		{+1, 0},
			{-1,+1},{ 0,+1},{+1,+1},
		};
		for (int i = 0; i < 8; i++) {
			m[i].x += x;
			m[i].y += y;
		}
		for (int i = 0; i < 8; i++) {
			//範囲外チェック
			if (m[i].x < 0) { continue; }
			if (m[i].y < 0) { continue; }
			if (m[i].x >= map->sizeX) { continue; }
			if (m[i].y >= map->sizeY) { continue; }

			//再帰
			this->collapseBlock(m[i].x, m[i].y);
		}

		auto ep = ML::Vec2(x * 16, y * 16);
		ge->CreateEffect(11, ep, 6);
		se::Play("crush");
	}
	void Object::IronOre(ML::Vec2 pos)
	{
		ge->CreateEffect(11, pos, 11);
		se::Play("crush");
	}


	//-------------------------------------------------------------------
	// 過去の遺産
	//int Object::CreatBlocks(const int inp, const ML::Vec2 pos)
	//{
	//	//取得番号からタスク生成
	//		//破壊不可ブロックだった場合そのブロック番号のまま返す
	//	switch (inp)
	//	{
	//	case 2:
	//		if (auto b = Bedrock::Object::Create(true)) {
	//			b->pos = pos;
	//		}
	//		return 2;
	//
	//	case 6:
	//		if (auto b = Stone::Object::Create(true)) {
	//			b->pos = pos;
	//		}
	//		return 0;
	//		break;
	//
	//	case 7:
	//		if (auto b = Bedrock::Object::Create(true)) {
	//			b->pos = pos;
	//		}
	//		return 7;	//破壊不可のため
	//		break;
	//
	//	case 10:
	//		if ()
	//			return 0;
	//		break;
	//	case 11:
	//		if (auto b = IronOre::Object::Create(true)){
	//			b->pos = pos;
	//		}
	//		return 0;
	//		break;
	//
	//	default:
	//		return 0;
	//		break;
	//	}
	//}
	
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