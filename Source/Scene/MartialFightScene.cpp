//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "MartialFightScene.h"
#include  "../../sound.h"
#include "../../Camera.h"
#include "../EnemyClasses/BossLady.h"

#include "../System/Task_Save.h"
#include "../Actors/Task_Player.h"
#include "../Actors/Enemys/Task_BlondeLady.h"
#include "../Actors/Enemys/Task_RegentLady.h"
#include  "Task_Map.h"
#include  "Task_EnemyMap.h"
#include  "../Actors/UI/SceneChangeButton.h"

#include "../Components/SecondsTimerComponent.h"
#include "../Components/HPBarComponent.h"
#include "../Components/GameOverEventComponent.h"

#include "../Actors/Enemys/Task_LadyKumagai.h"
#include "../Actors/Enemys/Task_LadyHaraguchi.h"
#include "../Actors/Enemys/Task_LadyKiyohara.h"
#include "../Actors/Enemys/Task_LadyNegishi.h"
#include "../Actors/Enemys/Task_LadySatake.h"

#include "../Event/Task_EventEngine.h"

#include  "../System/Task_BackGround.h"

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

		clear_ = false;

		//���f�[�^������
		ge->camera2D = ML::Box2D(0, 0, (int)ge->screenWidth, (int)ge->screenHeight);
		SetNextScene(Scene::Base);
		//ge->playerPtr->ResetState();
		auto hpBar = ge->playerPtr->GetHPBar();
		hpBar->SetPos(ML::Vec2(hpBar->GetSize().x * 0.5f, hpBar->GetSize().y * 0.5f));
		spawnableBoss_ = false;
		bgm::LoadFile("martialfight", "./data/sound/martialfight.mp3");
		bgm::LoadFile("boss", "./data/sound/NormalBoss01.mp3");
		bgm::LoadFile("lastboss", "./data/sound/LastBoss.mp3");
		//���^�X�N�̐���


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
			enemyCount_+=ge->GetTasks<RegentLady0::Object>(RegentLady0::defGroupName, RegentLady0::defName)->size();
		}
		{
			ge->playerPtr->SetPos(ML::Vec2{ 50,450 });
			auto camera = Camera::Object::Create(true);
			camera->horizontalScroll = true;
			camera->SetPos(ge->playerPtr->GetPos());
			camera->target = ge->playerPtr;
		}
		{//���_�ɖ߂�{�^��(�f�o�b�O�p
			//auto gotoBaseButton = SceneChangeButton::Object::Create(true);
			//gotoBaseButton->SetEnterButton(XI::VGP::ST);
			//gotoBaseButton->SetEnterButton(XI::Mouse::MB::LB);
			//gotoBaseButton->SetScene(this, Scene::Kind::Base);
			//gotoBaseButton->SetText("���_��");
			//AddSceneChangeButton(gotoBaseButton);
		}
		
		AddComponent(gameOverEvent_ = make_shared<GameOverEventComponent>(
										this,
										"./data/event/EventGameOverMartialFight.txt",//�����ŃQ�[���I�[�o�[���̃C�x���g��ύX
										0.8f));
		gameOverEvent_->SetPred(
			function<bool(void)>
			(
				[]()
				{
					return !ge->playerPtr->GetStatus()->HP.IsAlive();
				}
			)
		);

		//AddComponent(debugTimer = make_shared<SecondsTimerComponent>(this));
		//debugTimer->SetCountSeconds(0.3f);
		//debugTimer->Start();
		//debugMsg = "";

		{
			if (auto ev = EventEngine::Object::Create_Mutex())
			{
				ev->Set("./data/event/EventMartialFightStart0" + to_string(nowStage_ + 1/*�t�@�C������1����̂���*/) + ".txt");
			}
		}

		auto backGound = BackGround::Object::Create(true);
		backGound->SetUp(
			"./data/image/backGround/MartialFight/������w�i.png",
			ML::Point{ 2048,1152 },
			ML::Point{ (int)ge->screenWidth, (int)ge->screenHeight });

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
			if (nowStage < 4)
				save->SetValue(Save::Object::ValueKind::StageNo, nowStage + 1);
			//�G���f�B���O��
			else
				SetNextScene(Scene::Ending);

			save->Kill();
		}
		bgm::Stop("martialfight");
		bgm::Stop("boss");
		bgm::Stop("lastboss");
		ge->KillAll_G("�{��");
		ge->KillAll_GN(SceneChangeButton::defGroupName, SceneChangeButton::defName);
		ge->KillAll_G("�L�����N�^");
		ge->KillAll_G("�G");
		ge->KillAll_G("�I�u�W�F�N�g");

		RemoveAllComponent();

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
		
		if (enemyCount_ <= 0)
		{
			if (nowStage_ == 4)
			{
				bgm::Stop("martialfight");
				bgm::Play("lastboss");
			}
			else
			{
				bgm::Stop("martialfight");
				bgm::Play("boss");
			}
		}
		else
		{
			bgm::Play("martialfight");
		}
		{//�f�o�b�O�p
		//	debugMsg = spawnableBoss_ ?
		//		"�{�X�C�x���g�J�n�t���O�@ON" :
		//		"�{�X�C�x���g�J�n�t���O�@OFF";

		//	if (debugTimer->IsCountEndFrame() && enemyCount_ > 0)
		//	{
		//		auto enemy = ge->GetTask<BlondeLady::Object>(BlondeLady::defGroupName);
		//		enemy->Kill();
		//		debugTimer->Start();
		//	}

			//auto inp = ge->in1->GetState();
			
			//�v���C����
			//if (inp.ST.down)
			//	ge->playerPtr->GetStatus()->HP.TakeDamage(10000000);

			//�G���G�S��
			//if (inp.ST.down)
			//{
			//	auto enemys = ge->GetTasks<BlondeLady::Object>(BlondeLady::defGroupName);
			//	for (auto& enemy : *enemys)
			//		enemy->Kill();
			//}
			
			//�{�X��
			//if (inp.SE.down)
			//	boss_.lock()->Kill();
		}

		SpawnBoss();

		if (enemyCount_ <= 0 &&
			!boss_.lock() &&
			!clear_)
		{
			clearEvent_ = EventEngine::Object::Create_Mutex();
			clearEvent_.lock()->Set("./data/event/EventMartialFightClear0" + to_string(nowStage_ + 1/*�t�@�C������1����̂���*/) + ".txt");
			clear_ = true;
		}

		if (IsEndOfClearEvent())
			Kill();
	}

	bool Object::IsEndOfClearEvent()
	{
		return 
			clear_ &&
			!clearEvent_.lock();
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
			bossEvent_.lock()->Set("./data/event/EventMartialFightBeginBoss0" + to_string(nowStage_ + 1/*�t�@�C������1����̂���*/) + ".txt");
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
			break;

		case 1:
			boss = Haraguchi::Object::Create(true);
			break;

		case 2:
			boss = Kiyohara::Object::Create(true);
			break;

		case 3:
			boss = Negishi::Object::Create(true);
			break;

		case 4:
			boss = Satake::Object::Create(true);
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
		if (clear_)
		{
			ge->debugFont->Draw(ML::Box2D(ge->screenCenterPos.x - 50, 30, 500, 500), "�{�X��|������ˁB", ML::Color(1, 1, 0, 0));
		}

		{//�f�o�b�O�p
			//string param =
				//to_string(enemyCount_) + "\n" + debugMsg,
				//"����:" + to_string(ge->playerPtr->GetStatus()->HP.IsAlive());

			//ge->debugFont->Draw(ML::Box2D(100, 100, 500, 500), param, ML::Color(1, 1, 0, 0));			
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