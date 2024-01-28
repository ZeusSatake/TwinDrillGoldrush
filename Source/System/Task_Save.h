#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Z�[�u
//-------------------------------------------------------------------
#include "../../GameObject.h"

namespace Save
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("System");	//�O���[�v��
	const  string  defName("Save");		//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()		override;
		bool  Finalize()		override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
	};
	//-------------------------------------------------------------------
	class  Object : public  GameObject
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		typedef  shared_ptr<vector<Object::SP>> SPV;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()		override;	//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()	override;	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
		enum class ValueKind
		{
			StageNo,
			MiningProgress,
			DrillLevel,
			DefenceLevel,
			SpeedLevel,
			HaveMoney,
			EndOfOpening,
			EndOfShopTutorial,
			EndOfBaseTutorial,

			//===============�ǉ�����ꍇ�͂�������==============
			End
		};
		enum class StringKind
		{
			Name,
			Company,

			//===============�ǉ�����ꍇ�͂�������==============
			End
		};
		typedef std::pair<ValueKind, float> ValueDatum;
		typedef std::map<ValueKind, float> ValueData;
		typedef std::pair<StringKind, std::string> StringDatum;
		typedef std::map<StringKind, std::string> StringData;

	private:
		static std::array<std::string, (int)ValueKind::End + (int)StringKind::End> dataFormat;
		static const std::string valueKindStrings[];
		static const std::string stringKindStrings[];
		static std::string saveDataPath;

		template<typename KindType>
		static std::string DataKindToString(const KindType kind);
		template<typename KindType>
		static KindType StringToDataKind(const std::string& name);

		ValueData valueData_;
		StringData stringData_;

		void AddValueDatum(const ValueKind kind, const float value);
		void AddStringDatum(const StringKind kind, const std::string& str);

		void WriteData();

		bool HaveIntegrity() const;
	public:
		void ReadData();

		void SetValue(const ValueKind kind, const float value);
		void SetString(const StringKind kind, const std::string& str);
		template <typename ReturnType>
		ReturnType GetValue(const ValueKind kind) const;
		std::string GetString(const StringKind kind) const;
	};
}
//�֐��e���v���[�g�𑼂̃t�@�C������Ăяo�������ꍇ�͒�`���w�b�_�ɏ����Ȃ��Ƃ����Ȃ����ۂ��H
//cpp�ɏ����Ɩ������̊O���V���{���������񂪏o�Ă���
template <typename ReturnType>
ReturnType Save::Object::GetValue(const ValueKind kind) const
{
	return (ReturnType)valueData_.at(kind);
}