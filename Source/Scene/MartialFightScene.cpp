//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "MartialFightScene.h"

#include "../../Camera.h"
#include "../EnemyClasses/BossLady.h"

#include "../System/Task_Save.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Enemys/Task_BlondeLady.h"
#include  "Task_Map.h"
#include  "Task_EnemyMap.h"
#include  "../Actors/UI/SceneChangeButton.h"

#include "../Components/SecondsTimerComponent.h"
#include "../Components/HPBarComponent.h"

#include "../Actors/Enemys/Task_LadyKumagai.h"
#include "../Actors/Enemys/Task_LadyHaraguchi.h"
#include "../Actors/Enemys/Task_LadyKiyohara.h"
#include "../Actors/Enemys/Task_LadyNegishi.h"
#include "../Actors/Enemys/Task_LadySatake.h"

#include "../Event/Task_EventEngine.h"

namespace MartialFightScene
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

		AddComponent(transitionTimer_ = make_shared<SecondsTimerComponent>(this));
		transitionTimer_->SetCountSeconds(3.0f);

		clear_ = false;

		//���f�[�^������
		ge->camera2D = ML::Box2D(0, 0, (int)ge->screenWidth, (int)ge->screenHeight);
		//ge->playerPtr->ResetState();
		auto hpBar = ge->playerPtr->GetHPBar();
		hpBar->SetPos(ML::Vec2(hpBar->GetSize().x * 0.5f, hpBar->GetSize().y * 0.5f));
		spawnableBoss_ = false;

		//���^�X�N�̐���
		{
			if (auto ev = EventEngine::Object::Create_Mutex())
			{
				ev->Set("./data/event/eventmartialfightstart.txt");
			}
		}

		auto save = Save::Object::Create(true);
		nowStage_ = save->GetValue<int>(Save::Object::ValueKind::StageNo);

		{//������
			auto map = Map::Object::Create(true);
			map->Load("MartialFight");
		}
		{//�G
			auto enemymap = EnemyMap::Object::Create(true);
			enemymap->Load("MartialFight1Enemy");
			enemymap->SetEnemy();
			enemyCount_ = ge->GetTasks<BlondeLady::Object>(BlondeLady::defGroupName, BlondeLady::defName)->size();
			//boss_ = ge->GetTask<Satake::Object>(Satake::defGroupName, Satake::defName);
		}
		{
			ge->playerPtr->SetPos(ML::Vec2{ 50,450 });
			auto camera = Camera::Object::Create(true);
			camera->horizontalScroll=true;
			camera->SetPos(ge->playerPtr->GetPos());
			camera->target = ge->playerPtr;
		}
		{//���_�ɖ߂�{�^��(�f�o�b�O�p
			auto gotoBaseButton = SceneChangeButton::Object::Create(true);
			gotoBaseButton->SetEnterButton(XI::VGP::ST);
			gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
			gotoBaseButton->SetScene(this, Scene::Kind::Base);
			gotoBaseButton->SetText("���_��");
			AddSceneChangeButton(gotoBaseButton);
		}

		//AddComponent(debugTimer = make_shared<SecondsTimerComponent>(this));
		//debugTimer->SetCountSeconds(0.3f);
		//debugTimer->Start();
		//debugMsg = "";

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		if (clear_)
		{
			auto save = Save::Object::Create(true);
			const int nowStage = save->GetValue<int>(Save::Object::ValueKind::StageNo);
			//���̃X�e�[�W��
			if (nowStage < 3)
				save->SetValue(Save::Object::ValueKind::StageNo, nowStage + 1);
			save->Kill();
		}

		ge->KillAll_G("�{��");
		ge->KillAll_GN(SceneChangeButton::defGroupName, SceneChangeButton::defName);
		ge->KillAll_G("�L�����N�^");
		ge->KillAll_G("�G");
		ge->KillAll_G("�I�u�W�F�N�g");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			CreateNextScene();
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		Scene::UpDate();
		UpdateComponents();

		//{//�f�o�b�O�p
		//	debugMsg = spawnableBoss_ ?
		//		"�{�X�C�x���g�J�n�t���O�@ON" :
		//		"�{�X�C�x���g�J�n�t���O�@OFF";

		//	if (debugTimer->IsCountEndFrame() && enemyCount_ > 0)
		//	{
		//		auto enemy = ge->GetTask<BlondeLady::Object>(BlondeLady::defGroupName);
		//		enemy->Kill();
		//		debugTimer->Start();
		//	}

		//	auto inp = ge->in1->GetState();
		//	if (inp.ST.down)
		//		boss_.lock()->Kill();
		//}
		
		SpawnBoss();

		if (enemyCount_ <= 0 &&
			!boss_.lock()	 &&
			!clear_)
		{
			clearEvent_ = EventEngine::Object::Create_Mutex();
			clearEvent_.lock()->Set("./data/event/eventmartialfightclear.txt");
			clear_ = true;
		}

		if (clear_ && 
			!clearEvent_.lock())
		{
			CreateNextScene();
		}

		/*if (transitionTimer_->IsCountEndFrame())
		{
			this->Kill();
			return;
		}*/


	}

	void Object::SetBoss(const shared_ptr<BossLady>& boss)
	{
		boss_ = boss;
	}

	void Object::DecreaseEnemyCount()
	{
		if (enemyCount_ <= 0)
			return;

		--enemyCount_;

		//�G���G���Ȃ��Ȃ�����C�x���g�J�n
		if (enemyCount_ <= 0)
		{
			bossEvent_ = EventEngine::Object::Create_Mutex();
			//ev->Set("./data/event/");
			spawnableBoss_ = true;
		}
	}

	void Object::SpawnBoss()
	{
		if (!spawnableBoss_)
			return;

		//�C�x���g�Đ���
		if (!EndOfSpawnBossEvent())
			return;

		shared_ptr<BossLady> boss;
		switch (nowStage_)
		{
		case 0:
			boss = Kumagai::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		case 1:
			boss = Haraguchi::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		case 2:
			boss = Kiyohara::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		case 3:
			boss = Negishi::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		case 4:
			boss = Satake::Object::Create(true);
			boss->SetPos(ML::Vec2(300, 300));
			break;

		default:
			assert(!"�ݒ肳��Ă��Ȃ��l�ł�");
			break;
		}
		SetBoss(boss);
		spawnableBoss_ = false;
	}

	bool Object::EndOfSpawnBossEvent()
	{
		return enemyCount_ <= 0 &&
			   !bossEvent_.lock();
	}

	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ge->debugFont->Draw(ML::Box2D(500, 500, 500, 500), "������");

		if (clear_)
		{
			ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x - 50, 30, 500, 500), "�{�X��|������ˁB", ML::Color(1, 1, 0, 0));
		}

		{//�f�o�b�O�p
			//ge->debugFont->Draw(ML::Box2D(100, 100, 500, 500), to_string(enemyCount_) + "\n" + debugMsg, ML::Color(1, 1, 0, 0));
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