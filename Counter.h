#pragma once

class Counter
{
private:
	int value;
	int addValue;
	int countValue;
public:
	//==============================================
	//コンストラクタ
	//==============================================
	Counter(int initVal_, int addVal_, int countVal_);
		
	//==============================================
	//オペレーター
	//==============================================
	void operator=(const int& cnt);
	int operator()() const;
	Counter& operator++();
	Counter  operator++(int);

	//==============================================
	//メソッド
	//==============================================
	void Update();
};