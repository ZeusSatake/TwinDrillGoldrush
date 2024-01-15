//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_EnemyMap.h"
#include  "../Components/Blocks/BlockManager.h"
#include "../Actors/Enemys/Task_EtoHaiji.h"
#include "../Actors/Enemys/Task_BlondeLady.h"
#include "../Actors/Enemys/Task_LadySatake.h"
#include "../Actors/Enemys/Task_LadyKumagai.h"
#include "../Actors/Enemys/Task_LadyHaraguchi.h"

namespace  EnemyMap
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
		this->render2D_Priority[1] = 0.8f;
		//arrの要素数分(*32)のマップサイズ
		this->sizeX = sizeof(this->arr[0]) / sizeof(this->arr[0][0]);
		this->sizeY = sizeof(this->arr) / sizeof(this->arr[0]);
		this->chipSize = 16;//1マスの大きさ
		for (int y = 0; y < this->sizeY; ++y)
		{
			for (int x = 0; x < this->sizeX; ++x)
			{
				this->arr[y][x] = 0;
			}
		}
		
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
	bool Object::Load(const  string& fileName)
	{
		//ファイル名の作成
		string filePath = "./data/Map/" + fileName + ".csv";
		//ファイルの読み込み
		ifstream ifs(filePath);
		if (!ifs) { return false; }
		this->hitBase = ML::Box2D(0, 0, this->sizeX * chipSize, this->sizeY * chipSize);
		for (int y = 0; y < this->sizeY; ++y)
		{
			//改行までの文字列を取得
			string lineText;
			getline(ifs, lineText);

			istringstream ss_lt(lineText);
			for (int x = 0; x < this->sizeX; ++x)
			{
				//カンマまでの文字列を取得
				string text;
				getline(ss_lt, text, ',');
				stringstream ss;
				ss << text;
				ss >> this->arr[y][x];
			}
		}
		ifs.close();
		return true;
	}
	//-------------------------------------------------------------------
	void Object::SetEnemy()
	{
		for (int y = 0; y < this->sizeY; ++y) {
			for (int x = 0; x < this->sizeX; ++x) {
				// チップの番号によって生成する敵を変える
				switch (this->arr[y][x])
				{
				case 1:
				{
					auto enemy = EtoHaiji::Object::Create(true);
					enemy->SetPosX(x * chipSize);
					enemy->SetPosY(y * chipSize);
				}
				break;
				case 6:
				{
					auto enemy = BlondeLady::Object::Create(true);
					enemy->SetPosX(x * chipSize);
					enemy->SetPosY(y * chipSize);
				}
				break;
				case 10:
				{
					auto enemy = Satake::Object::Create(true);
					enemy->SetPosX(x * chipSize);
					enemy->SetPosY(y * chipSize);
				}
				break;
				case 15:
				{
					auto enemy = Kumagai::Object::Create(true);
					enemy->SetPosX(x * chipSize);
					enemy->SetPosY(y * chipSize);
				}
				break;
				case 20:
				{
					auto enemy = Haraguchi::Object::Create(true);
					enemy->SetPosX(x * chipSize);
					enemy->SetPosY(y * chipSize);
				}
				}
			}
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