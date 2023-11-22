//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_Map.h"
#include  "../Components/Blocks/Task_Stone.h"

#include  "../Components/Blocks/BlockManager.h"

namespace  Map
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
		this->img = DG::Image::Create("./data/image/mapchip.png");
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
		for (int i = 0; i < 31; ++i)
		{
			int x = i % 8;
			int y = i / 8;
			this->chip[i] = ML::Box2D(x * 32, y * 32, 32, 32);
		}
		//★タスクの生成
		auto Bm = Manager::Object::Create(true);
		Bm->LoadSe();

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
		auto ms = ge->mouse->GetState();
		if (ms.RB.down) {
			this->Search(ms.pos);
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		////カメラが完全にマップ外を指しているか調べる
		//if (false == this->hitBase.Hit(ge->camera2D)) {
		//	return; //完全に外に出ていたらその時点で描画処理を取りやめる
		//}
		////カメラとマップが重なっている範囲だけの矩形を作る
		//ML::Rect c = {
		//ge->camera2D.x,
		//ge->camera2D.y,
		//ge->camera2D.x + ge->camera2D.w,
		//ge->camera2D.y + ge->camera2D.h };
		//ML::Rect m = {
		//this->hitBase.x,
		//this->hitBase.y,
		//this->hitBase.x + this->hitBase.w,
		//this->hitBase.y + this->hitBase.h };
		////２つの矩形の重なっている範囲だけの矩形を求める
		//ML::Rect isr;
		//isr.left = max(c.left, m.left);
		//isr.top = max(c.top, m.top);
		//isr.right = min(c.right, m.right);
		//isr.bottom = min(c.bottom, m.bottom);
		////ループ範囲を決定
		//int sx, sy, ex, ey;
		//sx = isr.left / chipSize;
		//sy = isr.top / chipSize;
		//ex = (isr.right - 1) / chipSize;
		//ey = (isr.bottom - 1) / chipSize;

		for (int y = 0; y < this->sizeY; ++y)//int y = sy; y <= ey; ++y スクロール
		{
			for (int x = 0; x < this->sizeX; ++x)//int x = sx; x <=ex; ++x　スクロール
			{
				if (this->arr[y][x] != -1)
				{
					ML::Box2D draw(0, 0, chipSize, chipSize);
					draw.Offset(x * chipSize, y * chipSize);
					//スクロール対応
					draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
					this->img->Draw(draw, this->chip[this->arr[y][x]]);
				}
			}
		}
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
	//-----------------------------------------------------------
	//あたり判定
	bool  Object::CheckHit(const  ML::Box2D& hit)
	{
		ML::Rect  r = { hit.x, hit.y, hit.x + hit.w, hit.y + hit.h };
		//矩形がマップ外に出ていたらサイズを変更する
		ML::Rect  m =
		{
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h
		};
		if (r.left < m.left) { r.left = m.left; }
		if (r.top < m.top) { r.top = m.top; }
		if (r.right > m.right) { r.right = m.right; }
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }

		//ループ範囲調整
		int sx, sy, ex, ey;
		sx = r.left / chipSize;
		sy = r.top / chipSize;
		ex = (r.right - 1) / chipSize;
		ey = (r.bottom - 1) / chipSize;

		//範囲内の障害物を探す
		for (int y = sy; y <= ey; ++y)
		{
			for (int x = sx; x <= ex; ++x)
			{
				if (1 <= this->arr[y][x])
				{
					return true;
				}
			}
		}
		return false;
	}

	//------------------------------------------------------------------- 
//マップ外を見せないようにカメラを位置調整する
	void Object::AdjustCameraPos()
	{
		//カメラとマップの範囲を用意
		ML::Rect c = {
		ge->camera2D.x,
		ge->camera2D.y,
		ge->camera2D.x + ge->camera2D.w,
		ge->camera2D.y + ge->camera2D.h };
		ML::Rect m = {
		this->hitBase.x,
		this->hitBase.y,
		this->hitBase.x + this->hitBase.w,
		this->hitBase.y + this->hitBase.h };
		//カメラの位置を調整
		if (c.right > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top < m.top) { ge->camera2D.y = m.top; }
		//マップがカメラより小さい場合
		if (this->hitBase.w < ge->camera2D.w) { ge->camera2D.x = m.left; }
		if (this->hitBase.h < ge->camera2D.h) { ge->camera2D.y = m.top; }
	}
	//-------------------------------------------------------------------
	void Object::Search(const ML::Point& pos_)
	{
		ML::Point pos = pos_;
		if (pos.x >= 0 && pos.x < 16 * this->sizeX &&
			pos.y >= 0 && pos.y < 16 * this->sizeY) {
			//
			ML::Point masu = { pos.x / 16,pos.y / 16 };
			if (this->arr[masu.y][masu.x] != 0) {
				int inp = Manager::Object::CreatBlocks(arr[masu.y][masu.x], pos_);
				this->arr[masu.y][masu.x] = inp;
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