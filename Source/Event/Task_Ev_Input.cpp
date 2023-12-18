//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_Ev_Input.h"
#include "Task_InputMap.h"
#include "../../Character.h"
#include "../Actors/Task_Player.h"
#include <iostream>

namespace Ev_Input
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/preDrill.png");
		this->font= DG::Font::Create("HG�ۃS�V�b�NM-PRO", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->font.reset();
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
		this->Stop();
		this->render2D_Priority[1] = 0.2f;
		this->maxName = 8;
		this->nameText = "�Ȃ܂��@";
		this->name = "";
		x1 = 0;
		y1 = 0;
		this->flag = false;
		
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
		auto inp = ge->in1->GetState();
		auto Map = ge->GetTask<InputMap::Object>(InputMap::defGroupName, InputMap::defName);
		
		if (inp.LStick.BD.down)
		{
			if (y1 < 9)
			{
				y1++;
				if (Map->arr[y1][x1] == "")
				{
					y1--;
				}
			}
		}
		if (inp.LStick.BU.down)
		{
			if (y1 >0)
			{
				y1--;
				if (Map->arr[y1][x1] == "")
				{
					y1++;
				}
			}
		}
		if (inp.LStick.BL.down)
		{
			if (x1 > 0)
			{
				x1--;
				if (Map->arr[y1][x1] == "")
				{
					x1++;
				}
			}
		}
		if (inp.LStick.BR.down)
		{
			if (x1 < 9)
			{
				x1++;
				if (Map->arr[y1][x1] == "")
				{
					x1--;
				}
			}
		}
		if (inp.B1.down && flag == false)
		{//A�{�^���ňꕶ���ǉ�(�\��)
			if (this->name.size() / 2 < maxName)
			{
				this->name += Map->arr[y1][x1];
			}
		}
		if (inp.B2.down && flag == false)
		{//B�{�^���ňꕶ���폜(�\��)
			if (this->name.size() != 0)
			{
				this->name.pop_back();
				this->name.pop_back();
			}
		}
		if (inp.B3.down&& this->name.size()!=0)
		{//����
			this->flag = true;
		}
		if (inp.B2.down&&flag == true)
		{//�m�F��L�����Z��
			flag = false;
		}
		if (inp.B1.down&&flag == true)
		{//�m�F�㌈��
			ge->playerPtr->SetPersonalName(this->name);
			Map->Kill();
			this->Kill();
			ge->StopAll_GN("�C�x���g", "���s�G���W��", false);
		}
		this->now = Map->arr[y1][x1];
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (flag == false)
		{
			//�Ȃ܂��F�\��
			ML::Box2D draw(ge->screen2DWidth/4+50, 20, 160, 32);
			this->res->font->Draw(draw, nameText, ML::Color(1, 1, 1, 1));
			//�Ȃ܂��\��
			ML::Box2D draw2(ge->screen2DWidth / 4+175, 20, 320, 32);
			this->res->font->Draw(draw2, this->name, ML::Color(1, 1, 1, 1));
			//�J�[�\���\��
			ML::Box2D src1(0, 0, 64, 64);
			auto map = ge->GetTask<InputMap::Object>(InputMap::defGroupName, InputMap::defName);
			for (int y = 0; y < map->sizeY; ++y)
			{
				for (int x = 0; x < map->sizeX; ++x)
				{
					if (map->arr[y][x] != "")
					{
						ML::Box2D draw1(ge->screen2DWidth/4-15, 106, 16, 16);
						if (this->now == map->arr[y][x])
						{
							draw1.Offset(x * (map->chipSize+5), y * (map->chipSize + 5));
							this->res->img->Draw(draw1, src1);
						}
					}
				}
			}
			{//�L�[�\��
				ML::Box2D draw(0, 0, 400, 32);
				ML::Box2D draw1(0, 50, 400, 32);
				ML::Box2D draw2(0, 100, 400, 32);
				string text = "�ꕶ���ǉ�Z(A)";
				string text1 = "�ꕶ���폜X(B)";
				string text2 = "����C(X)";
				this->res->font->Draw(draw, text, ML::Color(1, 1, 1, 1));
				this->res->font->Draw(draw1, text1, ML::Color(1, 1, 1, 1));
				this->res->font->Draw(draw2, text2, ML::Color(1, 1, 1, 1));
			}
		}
		else
		{//�m�F���b�Z�[�W�@�L�[�\��
			ML::Box2D draw(ge->screen2DWidth/4, ge->screen2DHeight/3, 800, 32);
			ML::Box2D draw1(100, ge->screen2DHeight / 3+100, 800, 32);
			ML::Box2D draw2(100, ge->screen2DHeight / 3 + 150, 800, 32);
			string text = "�u" + this->name + "�v�ł�낵���ł����H";
			string text1 = "�͂��@Z�L�[(A)";
			string text2 ="�������@X�L�[(B)";
			this->res->font->Draw(draw, text, ML::Color(1, 1, 1, 1));
			this->res->font->Draw(draw1, text1, ML::Color(1, 1, 1, 1));
			this->res->font->Draw(draw2, text2, ML::Color(1, 1, 1, 1));
		}
	}

	//-------------------------------------------------------------------
	void  Object::Set(const string& taskName_, stringstream& ss_)
	{
		////�p�����[�^�𕪉�����
		//string msg;
		//ss_ >> msg;
		////�^�X�N����ݒ�
		//this->name = taskName_;

		auto map = InputMap::Object::Create(true);
		map->Load("Book2");
		//�C�x���g�G���W�����ꎞ��~������
		ge->StopAll_GN("�C�x���g", "���s�G���W��");
		//���g�̒�~��Ԃ���������
		this->Stop(false);
	}
	
	//-------------------------------------------------------------------
	void Object::CreateOrReset(stringstream& ss_)
	{
		//�p�����[�^�𕪉�����
		string taskName;
		ss_ >> taskName;

		//�V�K�쐬���X�V�̔��f
		auto p = ge->GetTask<Object>("����", taskName);
		//�V�K�쐬�̏ꍇ
		if (nullptr == p)
		{
			p = Object::Create(true);
			p->Set(taskName, ss_);
		}
		//�X�V�̏ꍇ
		else
		{
			p->Set(taskName, ss_);
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