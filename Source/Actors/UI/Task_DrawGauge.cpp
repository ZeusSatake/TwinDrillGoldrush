//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_DrawGauge.h"

namespace DrawGauge
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
		this->render2D_Priority[1] = 0.01f;
		pos_ = ML::Vec2(0, 0);
		isVisible_ = true;
		isSupportScroll_ = true;

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
		if (isVisible_ == false)
			return;

		if (res->img == nullptr)
			assert(!"�Q�[�W��img��nullptr�ł�");
		
		//�w�i
		ML::Box2D backDraw(-(size_.x / 2), -(size_.y / 2), size_.x, size_.y);
		//ML::Box2D backSrc(0, 0, 96, 32);

		//�X�N���[���Ή�
		if (isSupportScroll_)
			backDraw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		res->img->Draw(backDraw.OffsetCopy(pos_), backSrc_);
		
		//���g
		ML::Box2D insideDraw(backDraw);
		insideDraw.w *= gaugeValue_.GetNormalizeValue();
		
		auto insideSrc = insideSrc_;
		insideSrc.w = insideSrc_.w * gaugeValue_.GetNormalizeValue();

		res->img->Draw(insideDraw.OffsetCopy(pos_), insideSrc);
	}

	//===================================================================
	//�Z�b�^�[
	//===================================================================
	void Object::Set(const int max, const string& path)
	{
		SetMax(max);
		SetImg(path);
	}
	void Object::Set(const ML::Percentage& value)
	{
		if (value.GetMaxValue() == value.GetMinValue())
		{
			assert(!"max��min�������l���ƃQ�[�W��`��ł��܂���B");
		}

		gaugeValue_ = value;
	}
	void  Object::SetMax(const int max)
	{
		gaugeValue_.SetMaxValue(max);
	}
	void Object::SetMin(const int min)
	{
		gaugeValue_.SetMinValue(min);
	}
	void Object::SetPos(const ML::Vec2& pos)
	{
		pos_ = pos;
	}
	void Object::SetPos(const float x, const float y)
	{
		SetPos(ML::Vec2(x, y));
	}
	void Object::SetDrawSize(const int width, const int height)
	{
		size_.x = width;
		size_.y = height;
	}
	void Object::SetImg(const string& path)
	{
		if (res->img != nullptr) {
			res->img->ReLoad(path);
		}
		else {
			res->img = DG::Image::Create(path);
		}
	}
	void Object::SetSupportScroll(const bool isSupportScroll)
	{
		isSupportScroll_ = isSupportScroll;
	}
	void Object::SetVisible(const bool visible)
	{
		isVisible_ = visible;
	}
	void Object::SetBackSrc(const ML::Box2D& backSrc)
	{
		backSrc_ = backSrc;
	}
	void Object::SetInsideSrc(const ML::Box2D& insideSrc)
	{
		insideSrc_ = insideSrc;
	}
	//===================================================================
	//�Q�b�^�[
	//===================================================================
	int  Object::Getmax() const
	{
		return gaugeValue_.GetMaxValue();
	}
	bool Object::IsMax() const
	{
		return gaugeValue_.GetPercent() == 100.0f;
	}
	ML::Point Object::GetSize() const
	{
		return size_;
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