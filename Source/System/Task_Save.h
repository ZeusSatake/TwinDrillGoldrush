#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//セーブ
//-------------------------------------------------------------------
#include "../../GameObject.h"

namespace Save
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("System");	//グループ名
	const  string  defName("Save");		//タスク名
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
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
	};
	//-------------------------------------------------------------------
	class  Object : public  GameObject
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		typedef  shared_ptr<vector<Object::SP>> SPV;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()		override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()	override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
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

			//===============追加する場合はここより上==============
			End
		};
		enum class StringKind
		{
			Name,
			Company,

			//===============追加する場合はここより上==============
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
//関数テンプレートを他のファイルから呼び出したい場合は定義をヘッダに書かないといけないっぽい？
//cppに書くと未解決の外部シンボルおじさんが出てくる
template <typename ReturnType>
ReturnType Save::Object::GetValue(const ValueKind kind) const
{
	return (ReturnType)valueData_.at(kind);
}