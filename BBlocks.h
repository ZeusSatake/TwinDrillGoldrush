#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�u���b�N�̒n��
//-------------------------------------------------------------------
#include	"Actor.h"

class BBlocks : public Actor
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BBlocks>		   SP;
	typedef weak_ptr<BBlocks>		   WP;
public:
	//�ύX������������������������������������������������������
	//�L�����N�^���ʃ����o�ϐ�
	ML::Vec2    pos;		//�u���b�N���W
	ML::Box2D   hitBase;	//�����蔻��͈�

private:

public:
	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
	BBlocks()
		: pos(0, 0)
		, hitBase(0, 0, 0, 0)
	{
	}
	virtual  ~BBlocks() {}
	
	//=================================

protected:

public:

	//�󂯂�
	virtual void Received(BBlocks* object);
	virtual void Received(const int pow);
	virtual bool Drilling(const string& targetGroupName);
	virtual bool Drilling(const string& targetGroupName, const ML::Box2D& attackArea);
	void LoadSe();


};