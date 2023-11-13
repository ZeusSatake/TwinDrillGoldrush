#include "DG2014_Ver3_6.h"

namespace DG
{
	unordered_map<string, Font::WP>	 Font::rep;
	bool  Font::dt;
	Font::Font( )
	:font(nullptr)
	{
	}
	//-----------------------------------------------------------------------
	bool  Font::Initialize(
			const string&		fname_,
			WORD				width_,
			WORD				height_,
			WORD				weight_,
			BYTE				charSet_)
	{
		auto  dgi = DG::DGObject::GetInst( );	if(dgi == 0){	return false; }
		//
		D3DX10_FONT_DESC  desc = {
			height_, width_, weight_, 0,
			0, charSet_, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE
		};
		strcpy_s(desc.FaceName,  32,  fname_.c_str( ));

		HRESULT  hr;
		hr  =  D3DX10CreateFontIndirect(&dgi->Device( ),  &desc,  &font);
		if(FAILED(hr)){ return  false;}

		//---------------
		//2023/05/18 改造
		size.x = width_;
		size.y = height_;
		//---------------

		return true;
	}
	//-----------------------------------------------------------------------
	Font::~Font( )
	{
		ML::SafeRelease(&font);
	}
	//-----------------------------------------------------------------------
	//生成する
	Font::SP Font::Create(	
			const string&	fname_,
			WORD			width_,
			WORD			height_, 
			WORD			weight_,
			BYTE			charSet_)
	{
		if( Font::SP sp = Font::SP( new Font( ) ) ){
			if( sp->Initialize(fname_, width_, height_, weight_, charSet_) ){
				return sp;
			}
		}
		return nullptr;
	}
	//	文字列を表示する
	void Font::DrawS(
			const ML::Box2D&	draw_,	//	描画先
			const string&		tex_,
			ML::Color			color_,
			UINT				uFormat_)
	{
		dt = true;
		auto  dgi = DG::DGObject::GetInst( );	if(dgi == 0){	return; }
		ML::Mat4x4 matDraw;
		matDraw.Identity( );
		RECT d = draw_.Rect( );
		font->DrawText(nullptr,  tex_.c_str( ), (int)tex_.length( ), &d, uFormat_, color_.D3D( ));
	}
	//描画
	void Font::Draw(
			const ML::Box2D&		draw_,
			const string&			tex_,
			ML::Color				color_,
			UINT					uFormat_)
	{
		auto  dgi = DG::DGObject::GetInst( );	if(dgi == 0){	return; }
		dgi->End2D( );
		DrawS(draw_,  tex_,  color_,  uFormat_);
		dgi->Begin2D( );
	}
	//	フレーム付き文字列を表示する
	void Font::DrawF(
			const ML::Box2D&		draw_,	//	描画先
			const string&			tex_,
			FRAME					mode_,
			ML::Color				color_,
			ML::Color				fColor_,
			UINT					uFormat_)
	{
		auto  dgi = DG::DGObject::GetInst( );	if(dgi == 0){	return; }
		dgi->End2D( );
		ML::Box2D  d;
		if(mode_ == x1){
			d = draw_.OffsetCopy(+1, +1); 	DrawS(d, tex_, fColor_, uFormat_);
			DrawS(draw_, tex_, color_, uFormat_);
		}
		else if(mode_ == x2)
		{
			d = draw_.OffsetCopy(-1, -1);	DrawS(d, tex_, fColor_, uFormat_);
			d = draw_.OffsetCopy(+1, +1);	DrawS(d, tex_, fColor_, uFormat_);
			DrawS(draw_, tex_, color_, uFormat_);
		}
		else if(mode_ == x4)
		{
			d = draw_.OffsetCopy(-1,  0);	DrawS(d, tex_, fColor_, uFormat_);
			d = draw_.OffsetCopy(+1,  0);	DrawS(d, tex_, fColor_, uFormat_);
			d = draw_.OffsetCopy( 0, -1);	DrawS(d, tex_, fColor_, uFormat_);
			d = draw_.OffsetCopy( 0, +1);	DrawS(d, tex_, fColor_, uFormat_);
			DrawS(draw_, tex_, color_, uFormat_);
		}
		dgi->Begin2D( );
	}
	//-----------------------------------------------------------------------
	//2023/05/18 改造
	int Font::GetWidth()
	{
		return size.x;
	}
	int Font::GetHeight()
	{
		return size.y;
	}
	ML::Point Font::GetSize()
	{
		return size;
	}
	ML::Point Font::GetTextSize(const string& text)
	{
		//改行数、文字数カウント
		int newLineNum = 0;
		std::vector<char> charaNum(1);//行ごとの文字数を格納
		charaNum[0] = 0;
		for (char chara : text) {
			if (chara == '\n') {
				//次の行へ
				++newLineNum;
				charaNum.push_back(0);
			}
			else {
				//現在の行の文字数を+1
				++charaNum[newLineNum];
			}
		}
		
		//文字無し
		if (charaNum[0] == 0) {
			return ML::Point{ 0, 0 };
		}

		//改行無し
		if (newLineNum == 0) {
			ML::Point size_{
				GetWidth() * charaNum[0],
				GetHeight()
			};
			return size_;
		}

		//全ての行から一番多い文字数を取り出す
		int maxNum = charaNum[0];
		for (int i = 1; i < charaNum.size(); ++i) {
			if (charaNum[i] > maxNum) {
				maxNum = charaNum[i];
			}
		}

		ML::Point size_{
			GetWidth() * maxNum,
			GetHeight() * newLineNum
		};
		return size_;
	}
	//-----------------------------------------------------------------------
}
