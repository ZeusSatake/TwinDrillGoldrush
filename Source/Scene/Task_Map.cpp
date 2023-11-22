//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_Map.h"
#include  "../Components/Blocks/Task_Stone.h"

#include  "../Components/Blocks/BlockManager.h"

namespace  Map
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
		this->img = DG::Image::Create("./data/image/mapchip.png");
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
		for (int i = 0; i < 31; ++i)
		{
			int x = i % 8;
			int y = i / 8;
			this->chip[i] = ML::Box2D(x * 32, y * 32, 32, 32);
		}
		//���^�X�N�̐���
		auto Bm = Manager::Object::Create(true);
		Bm->LoadSe();

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
		auto ms = ge->mouse->GetState();
		if (ms.RB.down) {
			this->Search(ms.pos);
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		////�J���������S�Ƀ}�b�v�O���w���Ă��邩���ׂ�
		//if (false == this->hitBase.Hit(ge->camera2D)) {
		//	return; //���S�ɊO�ɏo�Ă����炻�̎��_�ŕ`�揈��������߂�
		//}
		////�J�����ƃ}�b�v���d�Ȃ��Ă���͈͂����̋�`�����
		//ML::Rect c = {
		//ge->camera2D.x,
		//ge->camera2D.y,
		//ge->camera2D.x + ge->camera2D.w,
		//ge->camera2D.y + ge->camera2D.h };
		//ML::Rect m = {
		//this->hitBase.x,
		//this->hitBase.y,
		//this->hitBase.x + this->hitBase.w,
		//this->hitBase.y + this->hitBase.h };
		////�Q�̋�`�̏d�Ȃ��Ă���͈͂����̋�`�����߂�
		//ML::Rect isr;
		//isr.left = max(c.left, m.left);
		//isr.top = max(c.top, m.top);
		//isr.right = min(c.right, m.right);
		//isr.bottom = min(c.bottom, m.bottom);
		////���[�v�͈͂�����
		//int sx, sy, ex, ey;
		//sx = isr.left / chipSize;
		//sy = isr.top / chipSize;
		//ex = (isr.right - 1) / chipSize;
		//ey = (isr.bottom - 1) / chipSize;

		for (int y = 0; y < this->sizeY; ++y)//int y = sy; y <= ey; ++y �X�N���[��
		{
			for (int x = 0; x < this->sizeX; ++x)//int x = sx; x <=ex; ++x�@�X�N���[��
			{
				if (this->arr[y][x] != -1)
				{
					ML::Box2D draw(0, 0, chipSize, chipSize);
					draw.Offset(x * chipSize, y * chipSize);
					//�X�N���[���Ή�
					draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
					this->img->Draw(draw, this->chip[this->arr[y][x]]);
				}
			}
		}
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
	//-----------------------------------------------------------
	//�����蔻��
	bool  Object::CheckHit(const  ML::Box2D& hit)
	{
		ML::Rect  r = { hit.x, hit.y, hit.x + hit.w, hit.y + hit.h };
		//��`���}�b�v�O�ɏo�Ă�����T�C�Y��ύX����
		ML::Rect  m =
		{
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h
		};
		if (r.left < m.left) { r.left = m.left; }
		if (r.top < m.top) { r.top = m.top; }
		if (r.right > m.right) { r.right = m.right; }
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }

		//���[�v�͈͒���
		int sx, sy, ex, ey;
		sx = r.left / chipSize;
		sy = r.top / chipSize;
		ex = (r.right - 1) / chipSize;
		ey = (r.bottom - 1) / chipSize;

		//�͈͓��̏�Q����T��
		for (int y = sy; y <= ey; ++y)
		{
			for (int x = sx; x <= ex; ++x)
			{
				if (1 <= this->arr[y][x])
				{
					return true;
				}
			}
		}
		return false;
	}

	//------------------------------------------------------------------- 
//�}�b�v�O�������Ȃ��悤�ɃJ�������ʒu��������
	void Object::AdjustCameraPos()
	{
		//�J�����ƃ}�b�v�͈̔͂�p��
		ML::Rect c = {
		ge->camera2D.x,
		ge->camera2D.y,
		ge->camera2D.x + ge->camera2D.w,
		ge->camera2D.y + ge->camera2D.h };
		ML::Rect m = {
		this->hitBase.x,
		this->hitBase.y,
		this->hitBase.x + this->hitBase.w,
		this->hitBase.y + this->hitBase.h };
		//�J�����̈ʒu�𒲐�
		if (c.right > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top < m.top) { ge->camera2D.y = m.top; }
		//�}�b�v���J������菬�����ꍇ
		if (this->hitBase.w < ge->camera2D.w) { ge->camera2D.x = m.left; }
		if (this->hitBase.h < ge->camera2D.h) { ge->camera2D.y = m.top; }
	}
	//-------------------------------------------------------------------
	void Object::Search(const ML::Point& pos_)
	{
		ML::Point pos = pos_;
		if (pos.x >= 0 && pos.x < 16 * this->sizeX &&
			pos.y >= 0 && pos.y < 16 * this->sizeY) {
			//
			ML::Point masu = { pos.x / 16,pos.y / 16 };
			if (this->arr[masu.y][masu.x] != 0) {
				int inp = Manager::Object::CreatBlocks(arr[masu.y][masu.x], pos_);
				this->arr[masu.y][masu.x] = inp;
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