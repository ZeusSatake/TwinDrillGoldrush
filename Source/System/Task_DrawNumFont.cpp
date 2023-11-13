//-------------------------------------------------------------------
//�������摜�ŕ`��
//-------------------------------------------------------------------
#include  "../../MyPG.h"
#include  "Task_DrawNumFont.h"

namespace DrawNumFont
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
		this->render2D_Priority[1] = 0.1f;
		this->img = nullptr;

		//this->valueStr = nullptr;

		drawDigit = 0;
		charSrc = ML::Box2D(0, 0, 0, 0);
		charDraw = ML::Box2D(0, 0, 0, 0);

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		this->img.reset();
		//�|�C���^���
		//free(this->valueStr);
		//this->valueStr = nullptr;

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
		//�����w��
		char digitStr[2];
		sprintf(digitStr, "%d", drawDigit);
		string format = "%0";
		format.append(digitStr);
		format.append("d");

		//���l�𕶎���ɕϊ�
		char valueStr[10];
		sprintf(valueStr, format.c_str(), drawValue);

		string param = "";

		//���l�\��
		for (int i = 0; i < drawDigit; ++i) {
			ML::Box2D draw = this->charDraw;
			draw.x = i * draw.w;
			ML::Box2D src = this->charSrc;
			src.x = (valueStr[i] - '0') * src.w;
			
			img->Draw(draw.OffsetCopy(drawPos), src);
		}
	}

	void Object::SetDrawValue(const int drawValue)
	{
		//�����l�������牽�����Ȃ�
		if (drawValue == this->drawValue)
			return;

		//�l�X�V
		this->drawValue = drawValue;
		////�������̊m��
		//if (valueStr == nullptr)
		//	valueStr = (char*)malloc(drawValue * sizeof(char));
		//else
		//	valueStr = (char*)realloc(valueStr, drawValue * sizeof(char));
	}
	void Object::SetImg(const string& imgPath)
	{
		if (img == nullptr)
			img = DG::Image::Create(imgPath);
		else
			img->ReLoad(imgPath);
	}
	void Object::SetCharDraw(const ML::Box2D& charDraw)
	{
		this->charDraw = charDraw;
	}
	ML::Box2D Object::GetCharDraw() const
	{
		return this->charDraw;
	}
	void Object::SetCharSrc(const ML::Box2D& charSrc)
	{
		this->charSrc = charSrc;
	}
	//9���܂�
	void Object::SetDrawDigit(const int drawDigit)
	{
		if (drawDigit < 0) {
			assert(!"�\�����錅����0�����ł�");
		}

		this->drawDigit = drawDigit;
		if (this->drawDigit > 9)
			this->drawDigit = 9;
	}
	void Object::SetDrawPos(const ML::Vec2& drawPos)
	{
		this->drawPos = drawPos;
	}
	void Object::SetUp(const string& imgPath, const ML::Box2D& charSrc, const ML::Box2D& charDraw, const int drawDigit, const ML::Vec2& drawPos)
	{
		SetImg(imgPath);
		SetCharSrc(charSrc);
		SetCharDraw(charDraw);
		SetDrawDigit(drawDigit);
		SetDrawPos(drawPos);
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