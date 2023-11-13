//-------------------------------------------------------------------
//数字を画像で描画
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_DrawNumFont.h"

namespace DrawNumFont
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
		this->render2D_Priority[1] = 0.1f;
		this->img = nullptr;

		//this->valueStr = nullptr;

		drawDigit = 0;
		charSrc = ML::Box2D(0, 0, 0, 0);
		charDraw = ML::Box2D(0, 0, 0, 0);

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		this->img.reset();
		//ポインタ解放
		//free(this->valueStr);
		//this->valueStr = nullptr;

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
		//書式指定
		char digitStr[2];
		sprintf(digitStr, "%d", drawDigit);
		string format = "%0";
		format.append(digitStr);
		format.append("d");

		//数値を文字列に変換
		char valueStr[10];
		sprintf(valueStr, format.c_str(), drawValue);

		string param = "";

		//数値表示
		for (int i = 0; i < drawDigit; ++i) {
			ML::Box2D draw = this->charDraw;
			draw.x = i * draw.w;
			ML::Box2D src = this->charSrc;
			src.x = (valueStr[i] - '0') * src.w;
			
			img->Draw(draw.OffsetCopy(drawPos), src);
		}
	}

	void Object::SetDrawValue(const int drawValue)
	{
		//同じ値だったら何もしない
		if (drawValue == this->drawValue)
			return;

		//値更新
		this->drawValue = drawValue;
		////メモリの確保
		//if (valueStr == nullptr)
		//	valueStr = (char*)malloc(drawValue * sizeof(char));
		//else
		//	valueStr = (char*)realloc(valueStr, drawValue * sizeof(char));
	}
	void Object::SetImg(const string& imgPath)
	{
		if (img == nullptr)
			img = DG::Image::Create(imgPath);
		else
			img->ReLoad(imgPath);
	}
	void Object::SetCharDraw(const ML::Box2D& charDraw)
	{
		this->charDraw = charDraw;
	}
	ML::Box2D Object::GetCharDraw() const
	{
		return this->charDraw;
	}
	void Object::SetCharSrc(const ML::Box2D& charSrc)
	{
		this->charSrc = charSrc;
	}
	//9桁まで
	void Object::SetDrawDigit(const int drawDigit)
	{
		if (drawDigit < 0) {
			assert(!"表示する桁数が0未満です");
		}

		this->drawDigit = drawDigit;
		if (this->drawDigit > 9)
			this->drawDigit = 9;
	}
	void Object::SetDrawPos(const ML::Vec2& drawPos)
	{
		this->drawPos = drawPos;
	}
	void Object::SetUp(const string& imgPath, const ML::Box2D& charSrc, const ML::Box2D& charDraw, const int drawDigit, const ML::Vec2& drawPos)
	{
		SetImg(imgPath);
		SetCharSrc(charSrc);
		SetCharDraw(charDraw);
		SetDrawDigit(drawDigit);
		SetDrawPos(drawPos);
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