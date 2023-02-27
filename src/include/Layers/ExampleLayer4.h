#ifndef EXAMPLELAYER4_H_420BD2A9_ED0B_4F15_95AC_39D749018DA5
#define EXAMPLELAYER4_H_420BD2A9_ED0B_4F15_95AC_39D749018DA5

#include "Utils/RandomShapes.h"
#include "Utils/FPS.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontMgr.h"

class ExampleLayer4 final : public BaseRandomShapeLayer
{
public:
	std::wstring GetTitle() const override { return L"Example #4: Draw FPS for random shapes"; };
	void Draw(SkCanvas* canvas) override;
	void drawRect(SkCanvas* canvas, SkColor& argb, SkRect& rect);
	void drawBackground(SkCanvas* canvas, int r
		, int g, int b, int a, float left
		, float right, float top, float bottom);

	void drawUnderline(SkCanvas* canvas, 
		SkColor& argb,
		float locX, float Width, float locY);

	void drawText(SkCanvas* canvas  , const char text[], SkColor& textColor , float textSize = 128.f,
		float locX = 0.f, float locY = 0.f, float locZ = 0. , 
		bool bold =false, float skew = 0.0f , bool underline = false  
		, SkColor* strokerColor = NULL , int strokerWidth = 0 
		,SkColor* shadowColor = NULL , int shadowLength= 0  ,
		int layoutPattern  = 0 ,int textSpace= 5 , int lineSpace = 5
		, SkColor* bgColor  = NULL , int bgRadius = 0 );
private:
	FPS m_FPS;
};

#endif // EXAMPLELAYER4_H_420BD2A9_ED0B_4F15_95AC_39D749018DA5
