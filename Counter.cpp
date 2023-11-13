#include "Counter.h"

//==============================================
//コンストラクタ
//==============================================
Counter::Counter(int initVal_, int addVal_, int countVal_)
	:
	value(initVal_),
	addValue(addVal_),
	countValue(countVal_)
{
};

//==============================================
//オペレーター
//==============================================
void Counter::operator=(const int& cnt)
{
	value = cnt;
}

int Counter::operator()() const
{
	return value;
}

Counter& Counter::operator++()
{
	Update();
	return *this;
}

//後置インクリメント 変更はするが戻り値は変更前のもの 
//Update()内で最初にカウンタ++するのは、上部に書けるようにしつつ、0を加味するため？
Counter  Counter::operator++(int)
{
	Counter old(*this);
	Update();
	return old;
}

//==============================================
//メソッド
//==============================================
void Counter::Update()
{
	++value;
	if (value > countValue) {
		value = 0;
	}
}