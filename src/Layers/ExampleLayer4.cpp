#include "include/Layers/ExampleLayer4.h"


void ExampleLayer4::Draw(SkCanvas* canvas)
{
	// clear canvas with black color
	canvas->clear(SkColors::kBlack);

	sk_sp<SkFontMgr> fontManager = SkFontMgr::RefDefault();
	sk_sp<SkTypeface> typeface(fontManager->matchFamilyStyle("NotoSansHans-Regular.otf", {}));

	SkFont font1(typeface, 64.0f, 1.0f, 1.0f);
	SkFont font2(typeface, 64.0f, 1.5f, 1.0f);
	font1.setEdging(SkFont::Edging::kAntiAlias);
	font2.setEdging(SkFont::Edging::kAntiAlias);

	SkPaint paint1;
	//sk_sp<SkTypeface> font = SkTypeface::MakeFromName("NotoSansHans-Regular.otf" , SkFontStyle::Normal());
	paint1.setAntiAlias(true);
	paint1.setColor(SkColorSetARGB(0xFF, 0x42, 0x85, 0xF4));
	canvas->drawString("Skia", 200.0f, 240.0f, font1, paint1);

	SkPaint paint2;
	paint2.setAntiAlias(true);
	paint2.setColor(SkColorSetARGB(0xFF, 0xDB, 0x44, 0x37));
	paint2.setStyle(SkPaint::kStroke_Style);
	paint2.setStrokeWidth(2.f);
	

	SkPaint paint3;
	paint3.setAntiAlias(true);
	paint3.setColor(SkColorSetARGB(0xFF, 0x0F, 0x9D, 0x58));



	canvas->drawString("Skia", 200.0f, 344.0f, font2, paint2);

	canvas->drawString("Skia", 200.0f, 344.0f, font2, paint3);
	SkSurface* surface = canvas->getSurface(); 
	std::string testStr = "AndroidStudio"; 
	SkColor color = SkColorSetARGB(0xFF, 0x42, 0x85, 0xF4);
	SkColor strokerColor = SkColorSetARGB(0xFF, 0xf2, 0xaa, 0x05);
	drawText(canvas, testStr.c_str() , color 
		, 180.0f ,  200.f , 200.f 
		, 0.0f , true  , 0.0f ,true , &strokerColor  , 10.f); 

	//canvas->flush(); 
	__super::Draw(canvas);
	
	m_FPS.Calc();
	m_FPS.Draw(canvas);
}

void ExampleLayer4::drawText(SkCanvas* canvas, const char text[]
	, SkColor& textColor, float textSize,
	float locX , float locY , float locZ ,
	bool bold, float skew , bool underline
	, SkColor* strokerColor , int strokerWidth 
	, SkColor* shadowColor  , int shadowLength ,
	int layoutPattern  , int textSpace  , int lineSpace  
	, SkColor* bgColor  , int bgRadius  ) {
	sk_sp<SkFontMgr> fontManager = SkFontMgr::RefDefault();
	std::string fontPath ="NotoSansHans-Regular.otf";
	if (bold) {
		//fontPath += "bold";
	}
	sk_sp<SkTypeface> typeface(fontManager->matchFamilyStyle(fontPath.c_str(), {}));

	SkFont font1(typeface, textSize, 1.0f, skew);
	SkPaint paint;
	paint.setColor(textColor);
	paint.setStrokeWidth(10.F); 
	//paint.computeFastBounds()
	if (underline) {
		drawUnderline(canvas, textColor , locX , textSize * sizeof(text), locY);
	}

	if (strokerWidth > 0) {
		SkPaint paint3;
		paint3.setColor(textColor);
		paint3.setStrokeWidth(10.F);
		paint3.setColor(*strokerColor);
		paint3.setStyle(SkPaint::kStroke_Style);
		paint3.setStrokeWidth(50.f);
		SkFont font2(typeface, textSize, 1.0f, skew);
		canvas->drawString(text, locX, locY, font2, paint3);
		//SkPaint strokerPaint(typeface , textSize , ); 
	}
	canvas->drawString(text, locX, locY, font1, paint);

}

void ExampleLayer4::drawUnderline(SkCanvas* canvas,
	SkColor& argb,
	float locX, float Width , float locY) {
	//SkColor color = SkColorSetARGB(a, r, g, b);
	SkPoint rectPts[] = { {locX, locY}, {locX + Width , locY + 5.} }; //default height
	SkRect rect;
	rect.set(rectPts[0], rectPts[1]);
	drawRect(canvas, argb, rect);
}


void ExampleLayer4::drawBackground(SkCanvas* canvas, int r 
	, int g ,int b , int a , float left 
	, float right , float top , float bottom) {
	SkColor color = SkColorSetARGB(a , r , g ,b);
	SkPoint rectPts[] = { {left, top}, {right, bottom} };
	SkRect rect;
	rect.set(rectPts[0], rectPts[1]);
	drawRect(canvas, color, rect); 
}

void ExampleLayer4::drawRect(SkCanvas* canvas , SkColor& argb, SkRect& rect) {
	SkPaint bkPaint;
	bkPaint.setColor(argb);
	
	canvas->drawRect(rect, bkPaint);
}