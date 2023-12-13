//----------------------------------------------------
//イベント実行エンジンのシステム制御系メソッド
//----------------------------------------------------
#include "../../MyPG.h"
#include "Task_EventEngine.h"
//#include "AppearObject.h"
#include "../../Source/Scene/Task_Map.h"

namespace EventEngine {
	//イベントフラグのデータ書き換え
	bool Object::EventFlag(stringstream& ss_)
	{
		string flagName;
		string sign;
		float value;
		ss_ >> flagName >> sign >> value;
		if (sign == "=") { ge->evFlags[flagName] = value; }
		else if (sign == "+") { ge->evFlags[flagName] += value; }
		else if (sign == "-") { ge->evFlags[flagName] -= value; }
		else { return false; }
		return true;
	}
	//条件分岐
	bool Object::If(stringstream& ss_)
	{
		//パラメータから対象を得る
		string flagKind;
		ss_ >> flagKind;
		bool flag = false;
		string labelName;
		//イベントフラグを対象とする場合
		if (flagKind == "ev_flag")
		{
			string flagName;
			string sign;
			float value;
			ss_ >> flagName >> sign >> value>>labelName;
			//比較
			if (sign == "==") { flag=ge->evFlags[flagName] == value; }
			else if (sign == ">") { flag=ge->evFlags[flagName] > value; }
			else if (sign == "<") { flag=ge->evFlags[flagName] < value; }
			else { return false; }
		}
		//キャラクタのパラメータを対象をする場合
		else if (flagKind == "charaParam")
		{
			//未実装
		}
		//環境変数を対象をする場合
		else if (flagKind == "System")
		{
			//未実装
		}
		//目的のラベルまで進む
		if (true == flag)
		{
			//読み込み位置をファイルの先頭へ戻す
			this->evFile.seekg(0, ios_base::beg);
			//一行読み込みを繰り返す
			string lt;
			while (this->ReadLine(lt))
			{
				//行を解読
				string::size_type t = lt.find(">");
				string headerStr = lt.substr(0, t);
				string dataStr = lt.substr(t + 1);
				if ("label" == headerStr)
				{
					//読み込んだ行がラベル
					stringstream ss;
					ss << dataStr;
					string labelName2;
					ss >> labelName2;
					if (labelName == labelName2)
					{
						//ラベル名が一致
						break;
					}
				}
			}
		}
		return true;
	}
	bool Object::Image(stringstream& ss_)
	{
		string OffScreenName;
		string filePath;
		ss_ >> OffScreenName >> filePath;
		DG::Image::SP img = DG::ImageManager::Get(OffScreenName);
		if (nullptr != img)//対象の画像が存在する場合
		{
			img->ReLoad(filePath);//画像を入れ替える
		}
		//イベント処理上から、画像を新規読み込みさせる場合
		else
		{
			img = DG::Image::Create(filePath);
			DG::ImageManager::Set(OffScreenName, img);
		}
		return true;
	}
	bool Object::FileSet(stringstream& ss_)
	{
		string filePath;
		ss_ >> filePath;
		return this->Set(filePath);
	}
	//bool Object::AppearObject(stringstream& ss_)
	//{
	//	//パラメータから対象を得る
	//	string kind;
	//	float posX, posY;
	//	ss_ >> kind >> posX >> posY;
	//	//オブジェクト生成(エンジンに登録)
	//	BChara::SP obj = AppearObject_BChara(kind);
	//	if (obj)
	//	{
	//		obj->pos.x = posX;
	//		obj->pos.y = posY;
	//		obj->Stop();
	//	}
	//	//任意のパラメータへの対応
	//	while (false == ss_.eof())
	//	{
	//		string paramName, eq;
	//		ss_ >> paramName >> eq;
	//		//任意パラメータでないなら
	//		if (eq != "=") { break; }
	//		if (paramName == "name") { ss_ >> obj->name; }
	//		if (paramName == "jumpPow") { ss_ >> obj->jumpPow; }
	//		if (paramName == "maxFallSpeed") { ss_ >> obj->maxFallSpeed; }
	//		if(paramName == "maxSpeed") { ss_ >> obj->maxSpeed; }
	//		if (paramName == "eventFileName") { ss_ >> obj->eventFileName; }
	//	}
	//	return nullptr != obj;
	//}
	bool Object::MapLoad(stringstream& ss_)
	{
		auto map = ge->GetTask<Map::Object>("本編","マップ");
		if (nullptr == map) {
			map = Map::Object::Create(true);
		}
		string mapFilePath;
		ss_ >> mapFilePath;
		return map->Load(mapFilePath);
	}
	/*bool Object::ModifyChara(stringstream& ss_)
	{
		string gname, name;
		ss_ >> gname >> name;

		if (auto mc = ge->GetTask<BChara>(gname, name))
		{
			while (false == ss_.eof())
			{
				string paramName, eq;
				ss_ >> paramName >> eq;

				if (eq == "=")
				{
					if (paramName == "posX") { ss_ >> mc->pos.x; }
					if (paramName == "posY") { ss_ >> mc->pos.y; }
					if (paramName == "jumpPow") { ss_ >> mc->jumpPow; }
					if (paramName == "maxFallSpeed") { ss_ >> mc->maxFallSpeed; }
					if (paramName == "maxSpeed") { ss_ >> mc->maxSpeed; }
					if (paramName == "event") { ss_ >> mc->eventFileName; }
				}
			}
			return true;
		}
		return false;
	}*/
	bool Object::KillObject(stringstream& ss_)
	{
		string gname;
		ss_ >> gname;

		if (false == ss_.eof())
		{
			string name;
			ss_ >> name;
			ge->KillAll_GN(gname,name);
		}
		else
		{
			ge->KillAll_G(gname);
		}
		return true;
	}

}