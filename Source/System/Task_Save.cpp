//-------------------------------------------------------------------
//�Z�[�u
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_Save.h"
#include  <array>

namespace Save
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}

	string Object::saveDataPath = "./data/save.txt";//../../data/
		
	const std::string Object::valueKindStrings[] =
	{
				"�X�e�[�W�ԍ�",	 //MiningStageNo
				"�h�������x��",		 //DrillLevel
				"�h�䃌�x��",		 //DefenceLevel
				"�������x��",		 //SpeedLevel
				"������",			 //HaveMoney
	};

	const std::string Object::stringKindStrings[] =
	{
		"���O",			//Name
		"��Ж�",		//Company
	};

	std::array<std::string, (int)Object::ValueKind::End + (int)Object::StringKind::End> Object::dataFormat;

	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
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

			//����Ȃ����ڂ�����Ώ����o��
			if (!HaveIntegrity())
			{
				WriteData();
			}
		}

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
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

		assert(!"�ݒ肳��Ă��܂���B");
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
			assert(!string(name + "�͓o�^����Ă��Ȃ��ł��B Save�N���X���m�F���Ă��������B").c_str());
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
			assert(!"�t�@�C���������݂Ɏ��s���܂����B");
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
			assert(!"�Z�[�u�f�[�^�̃t�@�C��������ɊJ���܂���ł����B");

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
			assert(!"�Z�[�u�f�[�^�̃t�@�C��������ɊJ���܂���ł����B");
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
			assert(!"SaveData �͈͊O�̎�ނł��B");
		}

		if (valueData_.count(kind) == 0)
		{
			std::string msg = DataKindToString(kind) + "�̍��ڂ�������܂���B" + __FILE__ + "�œo�^���Ă��������B";
			assert(!msg.c_str());
		}

		valueData_.at(kind) = value;

		WriteData();
	}

	void Object::SetString(const StringKind kind, const std::string& str)
	{
		if (kind == StringKind::End)
		{
			assert(!"SaveData �͈͊O�̎�ނł��B");
		}

		if (stringData_.count(kind) == 0)
		{
			std::string msg = DataKindToString(kind) + "�̍��ڂ�������܂���B" + __FILE__ + "�œo�^���Ă��������B";
			assert(!msg.c_str());
		}

		stringData_.at(kind) = str;

		WriteData();
	}

	string Object::GetString(const StringKind kind) const
	{
		return stringData_.at(kind);
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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