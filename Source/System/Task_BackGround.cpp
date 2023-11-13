//-------------------------------------------------------------------
//�w�i�\��
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_BackGround.h"

namespace BackGround
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
		this->render2D_Priority[1] = 1.0f;
		this->scrollSpeed = 0;
		this->pos = ML::Vec2(0, 0);
		this->isScroll = false;
		this->isSubScreen = false;
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		this->img.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (!isScroll)
			return;

		pos += scrollDirTable[(int)scrollDirection] * (float)scrollSpeed;

		int outPos;
		switch (scrollDirection) {
		case ScrollDirection::Up:
			outPos = (int)(resetPos.y - drawSize_Full.y);
			if (pos.y <= outPos) {
				pos.y = resetPos.y + drawSize_Full.y;
			}
			break;

		case ScrollDirection::Down:
			outPos = (int)(resetPos.y + drawSize_Full.y);
			if (pos.y >= outPos) {
				pos.y = resetPos.y - drawSize_Full.y;
			}
			break;

		case ScrollDirection::Left:
			outPos = (int)(resetPos.x - drawSize_Full.x);
			if (pos.x <= outPos) {
				pos.x = resetPos.x + drawSize_Full.x;
			}

		case ScrollDirection::Right:
			outPos = (int)(resetPos.x + drawSize_Full.x);
			if (pos.x >= outPos) {
				pos.x = resetPos.x - drawSize_Full.x;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D src(0, 0, this->imgSize.x, this->imgSize.y);

		for (int i = 0; i < this->spritNum; ++i) {
			ML::Box2D draw(0, 0, drawSize_Sprit.x, drawSize_Sprit.y);

			switch (renderSize) {
			case RenderSize::Sprit_Horizontal:
				draw.x = drawSize_Sprit.x * i;
				break;
				
			case RenderSize::Sprit_Vertical:
				draw.y = drawSize_Sprit.y * i;
			
			default:
				break;
			}

			draw.Offset(pos);
			
			this->img->Draw(draw, src);
		}
	}
	//-------------------------------------------------------------------
	//�Z�b�g�A�b�v �p�X�A�T�C�Y�A���[�v��
	void Object::SetUp(string imgPath_, ML::Point imgSize_, ML::Point drawSize_, RenderSize renderSize_, int spritNum_)
	{
		this->imgPath = imgPath_;
		this->img = DG::Image::Create(imgPath);
		this->imgSize = imgSize_;
		this->renderSize = renderSize_;
		
		if (spritNum_ < 1)
			this->spritNum = 1;
		else
			this->spritNum = spritNum_;

		SetDrawSize(drawSize_, this->renderSize, this->spritNum);
	}
	//�`��T�C�Y�ݒ�
	void Object::SetDrawSize(ML::Point drawSize_, RenderSize renderSize_, int spritNum_)
	{
		this->drawSize_Full = drawSize_;
		this->drawSize_Sprit = this->drawSize_Full;

		if (spritNum_ == 0)
			return;

		//�T�C�Y����
		switch (renderSize_)
		{
		case RenderSize::Sprit_Horizontal:
			this->drawSize_Sprit.x /= spritNum_;
			break;

		case RenderSize::Sprit_Vertical:
			this->drawSize_Sprit.y /= spritNum_;
			break;

		case RenderSize::FullScreen:
			break;
		}
	}

	void Object::SetScroll(ML::Vec2 pos_, int speed_, ScrollDirection scrollDirection_)
	{
		this->isScroll = true;
		this->pos = pos_;
		this->resetPos = pos_;
		this->scrollSpeed = speed_;
		this->scrollDirection = scrollDirection_;

		if (!this->isSubScreen)
			CreateSubScreen();
	}
	ML::Point Object::GetDrawSize()
	{
		return drawSize_Sprit;
	}
	void Object::CreateSubScreen()
	{
		if (!isScroll)
			return;

		auto subScreen = BackGround::Object::Create(true);
		subScreen->isSubScreen = true;
		subScreen->SetUp(this->imgPath, this->imgSize, this->drawSize_Full, this->renderSize, this->spritNum);

		ML::Vec2 subPos = this->resetPos;
		switch (this->scrollDirection) {
		case ScrollDirection::Up:
			subPos.y += this->drawSize_Full.y;
			break;

		case ScrollDirection::Down:
			subPos.y -= this->drawSize_Full.y;
			break;

		case ScrollDirection::Left:
			subPos.x += this->drawSize_Full.x;
			break;

		case ScrollDirection::Right:
			subPos.x -= this->drawSize_Full.x;
			break;

		default:
				break;
		}

		subScreen->SetScroll(subPos, this->scrollSpeed, this->scrollDirection);
		subScreen->resetPos = resetPos;
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