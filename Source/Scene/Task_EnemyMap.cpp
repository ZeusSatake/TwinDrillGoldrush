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
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.8f;
		//arr�̗v�f����(*32)�̃}�b�v�T�C�Y
		this->sizeX = sizeof(this->arr[0]) / sizeof(this->arr[0][0]);
		this->sizeY = sizeof(this->arr) / sizeof(this->arr[0]);
		this->chipSize = 16;//1�}�X�̑傫��
		for (int y = 0; y < this->sizeY; ++y)
		{
			for (int x = 0; x < this->sizeX; ++x)
			{
				this->arr[y][x] = 0;
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
	//-------------------------------------------------------------------
	bool Object::Load(const  string& fileName)
	{
		//�t�@�C�����̍쐬
		string filePath = "./data/Map/" + fileName + ".csv";
		//�t�@�C���̓ǂݍ���
		ifstream ifs(filePath);
		if (!ifs) { return false; }
		this->hitBase = ML::Box2D(0, 0, this->sizeX * chipSize, this->sizeY * chipSize);
		for (int y = 0; y < this->sizeY; ++y)
		{
			//���s�܂ł̕�������擾
			string lineText;
			getline(ifs, lineText);

			istringstream ss_lt(lineText);
			for (int x = 0; x < this->sizeX; ++x)
			{
				//�J���}�܂ł̕�������擾
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
				// �`�b�v�̔ԍ��ɂ���Đ�������G��ς���
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