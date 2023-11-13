#pragma once

class Counter
{
private:
	int value;
	int addValue;
	int countValue;
public:
	//==============================================
	//�R���X�g���N�^
	//==============================================
	Counter(int initVal_, int addVal_, int countVal_);
		
	//==============================================
	//�I�y���[�^�[
	//==============================================
	void operator=(const int& cnt);
	int operator()() const;
	Counter& operator++();
	Counter  operator++(int);

	//==============================================
	//���\�b�h
	//==============================================
	void Update();
};