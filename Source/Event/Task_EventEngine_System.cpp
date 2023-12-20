//----------------------------------------------------
//�C�x���g���s�G���W���̃V�X�e������n���\�b�h
//----------------------------------------------------
#include "../../MyPG.h"
#include "Task_EventEngine.h"
//#include "AppearObject.h"
#include "../../Source/Scene/Task_Map.h"

namespace EventEngine {
	//�C�x���g�t���O�̃f�[�^��������
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
	//��������
	bool Object::If(stringstream& ss_)
	{
		//�p�����[�^����Ώۂ𓾂�
		string flagKind;
		ss_ >> flagKind;
		bool flag = false;
		string labelName;
		//�C�x���g�t���O��ΏۂƂ���ꍇ
		if (flagKind == "ev_flag")
		{
			string flagName;
			string sign;
			float value;
			ss_ >> flagName >> sign >> value>>labelName;
			//��r
			if (sign == "==") { flag=ge->evFlags[flagName] == value; }
			else if (sign == ">") { flag=ge->evFlags[flagName] > value; }
			else if (sign == "<") { flag=ge->evFlags[flagName] < value; }
			else { return false; }
		}
		//�L�����N�^�̃p�����[�^��Ώۂ�����ꍇ
		else if (flagKind == "charaParam")
		{
			//������
		}
		//���ϐ���Ώۂ�����ꍇ
		else if (flagKind == "System")
		{
			//������
		}
		//�ړI�̃��x���܂Ői��
		if (true == flag)
		{
			//�ǂݍ��݈ʒu���t�@�C���̐擪�֖߂�
			this->evFile.seekg(0, ios_base::beg);
			//��s�ǂݍ��݂��J��Ԃ�
			string lt;
			while (this->ReadLine(lt))
			{
				//�s�����
				string::size_type t = lt.find(">");
				string headerStr = lt.substr(0, t);
				string dataStr = lt.substr(t + 1);
				if ("label" == headerStr)
				{
					//�ǂݍ��񂾍s�����x��
					stringstream ss;
					ss << dataStr;
					string labelName2;
					ss >> labelName2;
					if (labelName == labelName2)
					{
						//���x��������v
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
		if (nullptr != img)//�Ώۂ̉摜�����݂���ꍇ
		{
			img->ReLoad(filePath);//�摜�����ւ���
		}
		//�C�x���g�����ォ��A�摜��V�K�ǂݍ��݂�����ꍇ
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
	//	//�p�����[�^����Ώۂ𓾂�
	//	string kind;
	//	float posX, posY;
	//	ss_ >> kind >> posX >> posY;
	//	//�I�u�W�F�N�g����(�G���W���ɓo�^)
	//	BChara::SP obj = AppearObject_BChara(kind);
	//	if (obj)
	//	{
	//		obj->pos.x = posX;
	//		obj->pos.y = posY;
	//		obj->Stop();
	//	}
	//	//�C�ӂ̃p�����[�^�ւ̑Ή�
	//	while (false == ss_.eof())
	//	{
	//		string paramName, eq;
	//		ss_ >> paramName >> eq;
	//		//�C�Ӄp�����[�^�łȂ��Ȃ�
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
		auto map = ge->GetTask<Map::Object>("�{��","�}�b�v");
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