//-------------------------------------------------------------------
//背景表示
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_BackGround.h"

namespace BackGround
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
		this->render2D_Priority[1] = 1.0f;
		this->scrollSpeed = 0;
		this->pos = ML::Vec2(0, 0);
		this->isScroll = false;
		this->isSubScreen = false;
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		this->img.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (!isScroll)
			return;

		pos += scrollDirTable[(int)scrollDirection] * (float)scrollSpeed;

		int outPos;
		switch (scrollDirection) {
		case ScrollDirection::Up:
			outPos = (int)(resetPos.y - drawSize_Full.y);
			if (pos.y <= outPos) {
				pos.y = resetPos.y + drawSize_Full.y;
			}
			break;

		case ScrollDirection::Down:
			outPos = (int)(resetPos.y + drawSize_Full.y);
			if (pos.y >= outPos) {
				pos.y = resetPos.y - drawSize_Full.y;
			}
			break;

		case ScrollDirection::Left:
			outPos = (int)(resetPos.x - drawSize_Full.x);
			if (pos.x <= outPos) {
				pos.x = resetPos.x + drawSize_Full.x;
			}

		case ScrollDirection::Right:
			outPos = (int)(resetPos.x + drawSize_Full.x);
			if (pos.x >= outPos) {
				pos.x = resetPos.x - drawSize_Full.x;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, this->imgSize.x, this->imgSize.y);

		for (int i = 0; i < this->spritNum; ++i) {
			ML::Box2D draw(0, 0, drawSize_Sprit.x, drawSize_Sprit.y);

			switch (renderSize) {
			case RenderSize::Sprit_Horizontal:
				draw.x = drawSize_Sprit.x * i;
				break;
				
			case RenderSize::Sprit_Vertical:
				draw.y = drawSize_Sprit.y * i;
			
			default:
				break;
			}

			draw.Offset(pos);
			
			this->img->Draw(draw, src);
		}
	}
	//-------------------------------------------------------------------
	//セットアップ パス、サイズ、ループ回数
	void Object::SetUp(string imgPath_, ML::Point imgSize_, ML::Point drawSize_, RenderSize renderSize_, int spritNum_)
	{
		this->imgPath = imgPath_;
		this->img = DG::Image::Create(imgPath);
		this->imgSize = imgSize_;
		this->renderSize = renderSize_;
		
		if (spritNum_ < 1)
			this->spritNum = 1;
		else
			this->spritNum = spritNum_;

		SetDrawSize(drawSize_, this->renderSize, this->spritNum);
	}
	//描画サイズ設定
	void Object::SetDrawSize(ML::Point drawSize_, RenderSize renderSize_, int spritNum_)
	{
		this->drawSize_Full = drawSize_;
		this->drawSize_Sprit = this->drawSize_Full;

		if (spritNum_ == 0)
			return;

		//サイズ調整
		switch (renderSize_)
		{
		case RenderSize::Sprit_Horizontal:
			this->drawSize_Sprit.x /= spritNum_;
			break;

		case RenderSize::Sprit_Vertical:
			this->drawSize_Sprit.y /= spritNum_;
			break;

		case RenderSize::FullScreen:
			break;
		}
	}

	void Object::SetScroll(ML::Vec2 pos_, int speed_, ScrollDirection scrollDirection_)
	{
		this->isScroll = true;
		this->pos = pos_;
		this->resetPos = pos_;
		this->scrollSpeed = speed_;
		this->scrollDirection = scrollDirection_;

		if (!this->isSubScreen)
			CreateSubScreen();
	}
	ML::Point Object::GetDrawSize()
	{
		return drawSize_Sprit;
	}
	void Object::CreateSubScreen()
	{
		if (!isScroll)
			return;

		auto subScreen = BackGround::Object::Create(true);
		subScreen->isSubScreen = true;
		subScreen->SetUp(this->imgPath, this->imgSize, this->drawSize_Full, this->renderSize, this->spritNum);

		ML::Vec2 subPos = this->resetPos;
		switch (this->scrollDirection) {
		case ScrollDirection::Up:
			subPos.y += this->drawSize_Full.y;
			break;

		case ScrollDirection::Down:
			subPos.y -= this->drawSize_Full.y;
			break;

		case ScrollDirection::Left:
			subPos.x += this->drawSize_Full.x;
			break;

		case ScrollDirection::Right:
			subPos.x -= this->drawSize_Full.x;
			break;

		default:
				break;
		}

		subScreen->SetScroll(subPos, this->scrollSpeed, this->scrollDirection);
		subScreen->resetPos = resetPos;
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