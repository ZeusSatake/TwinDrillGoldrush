//-------------------------------------------------------------------
//セーブ
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_Save.h"
#include  <array>

namespace Save
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

	string Object::saveDataPath = "./data/save.txt";//../../data/
		
	const std::string Object::valueKindStrings[] =
	{
				"ステージ番号",	 //MiningStageNo
				"ドリルレベル",		 //DrillLevel
				"防御レベル",		 //DefenceLevel
				"速さレベル",		 //SpeedLevel
				"所持金",			 //HaveMoney
	};

	const std::string Object::stringKindStrings[] =
	{
		"名前",			//Name
		"会社名",		//Company
	};

	std::array<std::string, (int)Object::ValueKind::End + (int)Object::StringKind::End> Object::dataFormat;

	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		for (int i = 0; i < (int)ValueKind::End; ++i)
		{
			dataFormat.at(i) = DataKindToString((ValueKind)i);
		}
		for (int i = 0; i < (int)StringKind::End; ++i)
		{
			dataFormat.at(i) = DataKindToString((StringKind)i);
		}

		for (int i = 0; i < dataFormat.size(); ++i)
		{
			if (i < (int)ValueKind::End)
			{
				AddValueDatum((ValueKind)i, 0.0f);
			}
			else
			{
				AddStringDatum((StringKind)(i - (int)ValueKind::End), DataKindToString<StringKind>((StringKind)(i - (int)ValueKind::End)));
			}
		}

		ifstream fin(saveDataPath);
		if (!fin)
		{
			WriteData();
		}
		else
		{
			ReadData();

			//足りない項目があれば書き出す
			if (!HaveIntegrity())
			{
				WriteData();
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

	template<typename KindType>
	string Object::DataKindToString(const KindType kind)
	{
		string targetName = typeid(KindType).name();

		if (targetName == typeid(ValueKind).name())
			return valueKindStrings[(int)kind];
		if (targetName == typeid(StringKind).name())
			return stringKindStrings[(int)kind];

		assert(!"設定されていません。");
	}
	template<typename KindType>
	KindType Object::StringToDataKind(const string& name)
	{
		int num = 0;
		for (int i = 0; i < (int)KindType::End; ++i)
		{
			if (DataKindToString<KindType>((KindType)i) == name)
			{
				num = i;
				break;
			}
		}

		if (num == (int)KindType::End)
		{
			assert(!string(name + "は登録されていないです。 Saveクラスを確認してください。").c_str());
		}

		return (KindType)num;
	}

	void Object::AddValueDatum(const ValueKind kind, const float value)
	{
		valueData_.insert(ValueDatum(kind, value));
	}
	void Object::AddStringDatum(const StringKind kind, const string& str)
	{
		stringData_.insert(StringDatum(kind, str));
	}

	void Object::WriteData()
	{
		ofstream fout(saveDataPath);
		if (!fout)
		{
			assert(!"ファイル書き込みに失敗しました。");
		}

		for (int i = 0; i < dataFormat.size(); ++i)
		{
			if (i < (int)ValueKind::End)
			{
				ValueKind kind = (ValueKind)i;
				fout << DataKindToString(kind) << " " << valueData_.at(kind) << endl;
			}
			else
			{
				StringKind kind = (StringKind)(i - (int)ValueKind::End);
				fout << DataKindToString(kind) << " " << stringData_.at(kind) << endl;
			}
		}
	}
	bool Object::HaveIntegrity() const
	{
		ifstream fin(saveDataPath);
		if (!fin)
			assert(!"セーブデータのファイルが正常に開けませんでした。");

		string line;
		string name;

		while (!fin.eof())
		{
			fin >> name;
			if (fin.eof())
				break;
			for (int i = 0; i < dataFormat.size(); ++i)
			{
				if (dataFormat.at(i) != name)
					return false;
			}
		}
		return true;
	}

	void Object::ReadData()
	{
		ifstream fin(saveDataPath);
		if (!fin)
		{
			assert(!"セーブデータのファイルが正常に開けませんでした。");
		}

		string line;
		for (int i = 0; i < dataFormat.size(); ++i)
		{
			string name;
			if (i < (int)ValueKind::End)
			{
				float value;
				fin >> name >> value;
				if (fin.eof())
					break;
				SetValue(StringToDataKind<ValueKind>(name), value);
			}
			else
			{
				string str;
				fin >> name >> str;
				if (fin.eof())
					break;
				SetString(StringToDataKind<StringKind>(name), str);
			}
		}
	}

	void Object::SetValue(const ValueKind kind, const float value)
	{
		if (kind == ValueKind::End)
		{
			assert(!"SaveData 範囲外の種類です。");
		}

		if (valueData_.count(kind) == 0)
		{
			std::string msg = DataKindToString(kind) + "の項目が見つかりません。" + __FILE__ + "で登録してください。";
			assert(!msg.c_str());
		}

		valueData_.at(kind) = value;

		WriteData();
	}

	void Object::SetString(const StringKind kind, const std::string& str)
	{
		if (kind == StringKind::End)
		{
			assert(!"SaveData 範囲外の種類です。");
		}

		if (stringData_.count(kind) == 0)
		{
			std::string msg = DataKindToString(kind) + "の項目が見つかりません。" + __FILE__ + "で登録してください。";
			assert(!msg.c_str());
		}

		stringData_.at(kind) = str;

		WriteData();
	}

	string Object::GetString(const StringKind kind) const
	{
		return stringData_.at(kind);
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