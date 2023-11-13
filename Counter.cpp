#include "Counter.h"

//==============================================
//�R���X�g���N�^
//==============================================
Counter::Counter(int initVal_, int addVal_, int countVal_)
	:
	value(initVal_),
	addValue(addVal_),
	countValue(countVal_)
{
};

//==============================================
//�I�y���[�^�[
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

//��u�C���N�������g �ύX�͂��邪�߂�l�͕ύX�O�̂��� 
//Update()���ōŏ��ɃJ�E���^++����̂́A�㕔�ɏ�����悤�ɂ��A0���������邽�߁H
Counter  Counter::operator++(int)
{
	Counter old(*this);
	Update();
	return old;
}

//==============================================
//���\�b�h
//==============================================
void Counter::Update()
{
	++value;
	if (value > countValue) {
		value = 0;
	}
}