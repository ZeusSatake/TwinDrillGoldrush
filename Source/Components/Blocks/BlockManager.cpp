//-------------------------------------------------------------------
//�u���b�N�}�l�[�W���[
//-------------------------------------------------------------------
#include	"../../../MyPG.h"
#include	"BlockManager.h"
#include	"../../Scene/Task_Map.h"
#include	"../../../sound.h"

namespace	Manager
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
		this->InitArray();

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
	//�u���b�NHP�E�����ݒ�
	void Object::InitArray()
	{
		auto map = ge->GetTask<Map::Object>("�{��", "�}�b�v");
		for (int y = 0; y < map->sizeY; y++)
		{
			for (int x = 0; x < map->sizeX; x++)
			{
				int id = map->GetMapChip(y, x);
				switch (id) {
				case 0:
					this->arr[y][x].MaxHP = 0;
					this->arr[y][x].HP = 0;
					break;
				case 1:
					this->arr[y][x].MaxHP = 1;
					this->arr[y][x].HP = 1;
					break;
				case 2:
					this->arr[y][x].MaxHP = '�s';
					this->arr[y][x].HP = '�s';
					this->arr[y][x].event = 7;
					break;
				case 3:
					this->arr[y][x].MaxHP = 3;
					this->arr[y][x].HP = 3;
					break;
				case 6:
					this->arr[y][x].MaxHP = 3;
					this->arr[y][x].HP = 3;
					this->arr[y][x].event = 6;
					break;
				case 7:
					this->arr[y][x].MaxHP = '�s';
					this->arr[y][x].HP = '�s';
					this->arr[y][x].event = 7;
					break;
				case 10:
					this->arr[y][x].MaxHP = 1;
					this->arr[y][x].HP = 1;
					this->arr[y][x].event = 10;
					break;
				case 11:
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					break;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//���l����u���b�N�j�󏈗�
	void Object::Damage(const ML::Point pos_, int power)
	{
		int x = pos_.x;
		int y = pos_.y;
		if (this->arr[y][x].HP == '�s')
		{
			this->eventSearch(y, x);
			return;
		}

		if (this->arr[y][x].HP - power > 0) { this->arr[y][x].HP -= power; }
		else if (this->arr[y][x].HP >= 0)
		{
			this->arr[y][x].HP -= power;

			auto map = ge->GetTask<Map::Object>("�{��", "�}�b�v");
			this->eventSearch(y, x);
			map->SetMapChip(y, x, 0);
		}
	}
	//-------------------------------------------------------------------
	//
	void Object::eventSearch(int y_, int x_)
	{
		//�擾�ԍ�����Y���������Ăяo��
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
		default:
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u���b�N�����E�hTask_�u���b�N���h�͎g��Ȃ��悤�ɂ��邩��
	void Object::Stone(ML::Vec2 pos)
	{
		ge->CreateEffect(11, pos);
		se::Play("crush");
	}
	void Object::Bedrock(ML::Vec2 pos)
	{
		se::Play("repelled");
	}
	void Object::collapseBlock(int x,int y)
	{
		auto map = ge->GetTask<Map::Object>("�{��", "�}�b�v");
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
			//�͈͊O�`�F�b�N
			if (m[i].x < 0) { continue; }
			if (m[i].y < 0) { continue; }
			if (m[i].x >= map->sizeX) { continue; }
			if (m[i].y >= map->sizeY) { continue; }

			//�ċA
			this->collapseBlock(m[i].x, m[i].y);
		}

		auto ep = ML::Vec2(x * 16, y * 16);
		ge->CreateEffect(11, ep);
		se::Play("crush");
	}


	//-------------------------------------------------------------------
	// �ߋ��̈�Y
	//int Object::CreatBlocks(const int inp, const ML::Vec2 pos)
	//{
	//	//�擾�ԍ�����^�X�N����
	//		//�j��s�u���b�N�������ꍇ���̃u���b�N�ԍ��̂܂ܕԂ�
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
	//		return 7;	//�j��s�̂���
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