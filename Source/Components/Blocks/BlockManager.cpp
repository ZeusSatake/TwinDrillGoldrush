//-------------------------------------------------------------------
//�u���b�N�}�l�[�W���[
//-------------------------------------------------------------------
#include	"../../../MyPG.h"
#include	"BlockManager.h"
#include	"../../../sound.h"

#include	"../../Scene/Task_Map.h"
#include    "../../Scene/Task_JewelryMap.h"
#include	"../../Actors/Task_Player.h"
#include	"../../Components/StateComponent.h"


namespace	BlockManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/effect/debugrect.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->render2D_Priority[1] = 0.7f;

		//���f�[�^������

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
		//�L�΁@���~
		/*for (auto it = DamegeBlockPos.begin(); it != DamegeBlockPos.end(); ++it)
		{
			int Dx = (*it).x;
			int Dy = (*it).y;
			DamegeBlock(Dx, Dy);
		}*/
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
		auto map = ge->GetTask<Map::Object>("�{��", MapName);
		for (int y = 0; y < map->sizeY; y++)
		{
			for (int x = 0; x < map->sizeX; x++)
			{
				int id = map->GetMapChip(y, x);
				switch (id) {
				case (int)Map::Object::ChipKind::Empty:					//��C�u���b�N
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					break;
				case (int)Map::Object::ChipKind::Soil:					//�y
					this->arr[y][x].MaxHP = 1;
					this->arr[y][x].HP = 1;
					this->arr[y][x].event = 1;
					break;
				case (int)Map::Object::ChipKind::HardSoil:				//�d���y
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 7;
					break;
				case (int)Map::Object::ChipKind::MossySoil:				//�ۂނ����y
					this->arr[y][x].MaxHP = 3;
					this->arr[y][x].HP = 3;
					break;
				case (int)Map::Object::ChipKind::Stone:					//��
					this->arr[y][x].MaxHP = 3;
					this->arr[y][x].HP = 3;
					this->arr[y][x].event = 6;
					break;
				case (int)Map::Object::ChipKind::BedRock:				//���
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 7;
					break;	
				case (int)Map::Object::ChipKind::MossyStone:			//�L��
					//DamegeBlockPos.push_back(ML::Vec2(x,y));
					break;
				case (int)Map::Object::ChipKind::CollapseStone:			//������
					this->arr[y][x].MaxHP = 1;
					this->arr[y][x].HP = 1;
					this->arr[y][x].event = 10;
					break;
				//====================�z��====================
				case (int)Map::Object::ChipKind::Coal:					//�ΒY
					this->arr[y][x].MaxHP = 3;
					this->arr[y][x].HP = 3;
					this->arr[y][x].event = 9;
					break;
				case (int)Map::Object::ChipKind::Iron:					//�S�z��
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 11;
					break;
				case (int)Map::Object::ChipKind::Gold:					//���z�΂ۂ����
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 12;
					break;
				case (int)Map::Object::ChipKind::HihiIroKane:			//�q�q�C���J�l�ۂ���@��
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 13;
					break;
				case (int)Map::Object::ChipKind::Damascus:				//�_�}�X�J�X�ۂ���� ��
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 14;
					break;
				case (int)Map::Object::ChipKind::Orichalcum:			//�I���n���R���ۂ���@��
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 15;
					break;
				case (int)Map::Object::ChipKind::Palladium:				//�p���W�E���ۂ���@��
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 16;
					break;
				case (int)Map::Object::ChipKind::Adamantite:			//�A�_�}���^�C�g�ۂ���@�Ԏ�
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 17;
					break;
				//====================���====================
				case (int)JewelryMap::Object::ChipKind::Diamond:		//�_�C�������h
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 18;
					break;
				case (int)JewelryMap::Object::ChipKind::BlackDiamond:	//�u���b�N�_�C�������h
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 19;
					break;
				case (int)JewelryMap::Object::ChipKind::PinkDiamond:	//�s���N�_�C�������h
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 20;
					break;
				case (int)JewelryMap::Object::ChipKind::Emerald:		//�G�������h
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 21;
					break;
				case (int)JewelryMap::Object::ChipKind::Sapphire:		//�T�t�@�C�A
					this->arr[y][x].MaxHP = 6;
					this->arr[y][x].HP = 6;
					this->arr[y][x].event = 22;
					break;
				case (int)JewelryMap::Object::ChipKind::Garnet:			//�K�[�l�b�g
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 23;
					break;
				case (int)JewelryMap::Object::ChipKind::Topaz:			//�g�p�[�Y
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 24;
					break;
				case (int)JewelryMap::Object::ChipKind::Amethyst:		//�A���W�X�g
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					this->arr[y][x].event = 25;
					break;
				case (int)Map::Object::ChipKind::Brick:
					this->arr[y][x].MaxHP = -1;
					this->arr[y][x].HP = -1;
					break;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//���l����u���b�N�j�󏈗�
	bool Object::DestroyBlock(const ML::Point pos_, int power)
	{
		int x = pos_.x;
		int y = pos_.y;
		//����یn�̃`�b�v�͉���炷����
		if (this->arr[y][x].HP < 0)
		{
			this->eventSearch(y, x);
			return false;
		}

		this->arr[y][x].HP -= power;

		if (this->arr[y][x].HP <= 0)
		{
			auto map = ge->GetTask<Map::Object>("�{��", MapName);
			this->eventSearch(y, x);
			map->SetMapChip(y, x, 0);
			return true;
		}
		else
		{
			return false;
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
		case (int)Map::Object::ChipKind::HardSoil:
			SoundOnly(pos);
			break;
		case (int)Map::Object::ChipKind::BedRock:
			SoundOnly(pos);
			break;
		case (int)Map::Object::ChipKind::CollapseStone:
			collapseBlock(x_, y_);
			break;
		//======================
		case (int)Map::Object::ChipKind::DiagonalBrick:
			EffectOnly(pos, 6);
			break;

		//============�G�t�F�N�g�̂�======
		default:
			EffectOnly(pos, eventNum);
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u���b�N�����E�hTask_�u���b�N���h�͎g��Ȃ��悤�ɂ��邩��
	//=========�g���܂킵�p==========
	void Object::EffectOnly(ML::Vec2 pos, int n)
	{
		ge->CreateEffect(11, pos, n);
		se::Play("crush");
	}

	void Object::SoundOnly(ML::Vec2 pos)
	{
		se::Play("repelled");
	}

	//==========================================
	//�A������u���b�N
	void Object::collapseBlock(int x,int y)
	{
		auto map = ge->GetTask<Map::Object>("�{��", MapName);
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
		ge->CreateEffect(11, ep, 6);
		se::Play("crush");
	}
	
	//�L�� �������~
	//void Object::DamegeBlock(int x, int y)
	//{
	//	auto map = ge->GetTask<Map::Object>("�{��", MapName);

	//	ML::Point m[8] = {
	//		{-1,-1},{ 0,-1},{+1,-1},
	//		{-1, 0},		{+1, 0},
	//		{-1,+1},{ 0,+1},{+1,+1},
	//	};
	//	for (int i = 0; i < 8; i++) {
	//		m[i].x += x;
	//		m[i].y += y;
	//	}
	//	for (int i = 0; i < 8; i++) {
	//		//�͈͊O�`�F�b�N
	//		if (m[i].x < 0) { continue; }
	//		if (m[i].y < 0) { continue; }
	//		if (m[i].x >= map->sizeX) { continue; }
	//		if (m[i].y >= map->sizeY) { continue; }

	//		int id = map->GetMapChip(m[i].y, m[i].x);
	//		if (id >= 0) { return; }

	//		auto me = ML::Box2D(m[i].x * 16, m[i].y * 16, 16, 16);
	//		auto target = ge->GetTask<player::Object>("�L�����N", "�v���C���[");
	//		if (target->CheckHit(me) && this->interval < 1)
	//		{
	//			this->interval = 60;
	//		}
	//		this->interval--;
	//		auto src = ML::Box2D(0, 0, 32, 32);
	//		this->res->img->Draw(me, src);
	//	}
	//}
	

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