//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_DrawGauge.h"

namespace DrawGauge
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/bar.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		img.reset();
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
		this->render2D_Priority[1] = 0.05f;
		count_ = 0;
		pos = ML::Vec2(0, 0);

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
		if (res->img == nullptr)
			assert(!"�Q�[�W��img��nullptr�ł�");
		
		//�w�i
		ML::Box2D backDraw(-48, -16, 96, 32);
		ML::Box2D backSrc(0, 0, 96, 32);

		res->img->Draw(backDraw.OffsetCopy(pos), backSrc);

		//���g
		ML::Box2D insideDraw(backDraw);
		insideDraw.w *= Normalized();
		
		ML::Box2D insideSrc = ML::Box2D(0, 32, 96, 32);
		insideSrc.w *= Normalized();
		//�ő嗭�߂͐�
		if (isMaxCharge)
			insideSrc.y = 64;

		res->img->Draw(insideDraw.OffsetCopy(pos), insideSrc);
	}

	//===================================================================
	//�Z�b�g�A�b�v
	//===================================================================
	void Object::SetUp(const int max, const string& path)
	{
		SetMax(max);
		SetImg(path);
	}
	//===================================================================
	//�J�E���g
	//===================================================================
	void Object::CountUp(int add)
	{
		count_ += add;

		if (count_ > max_) {
			count_ = max_;
			isMaxCharge = true;
		}
	}
	int  Object::GetCount() const
	{
		return count_;
	}
	void Object::ResetCount()
	{
		count_ = 0;
		isMaxCharge = false;
	}
	//�i�s�x�� 0.0~1.0 �ŕԂ�
	float Object::Normalized() const
	{
		return (float)count_ / (float)max_;
	}
	//===================================================================
	//�ő�J�E���g
	//===================================================================
	void  Object::SetMax(const int max)
	{
		if (max <= 0) {
			assert(!"�ݒ肵�悤�Ƃ��Ă���Q�[�W�̍ő�l��0�ȉ��ł�");
		}
		this->max_ = max;
	}
	int  Object::Getmax() const
	{
		return max_;
	}
	bool Object::IsMax() const
	{
		return isMaxCharge;
	}
	//===================================================================
	//�摜
	//===================================================================
	void Object::SetImg(const string& path)
	{
		if (res->img != nullptr) {
			res->img->ReLoad(path);
		}
		else {
			res->img = DG::Image::Create(path);
		}
	}
	//===================================================================

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