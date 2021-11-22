#include "include/Layers/BlackAndWhiteLayer.h"
#include "include/Utils/Utils.h"
#include "include/Controls/Slider.h"
#include "include/Controls/Button.h"

#include "include/core/SkImage.h"

#include <iostream>

namespace
{
	constexpr SkScalar kPanelSize = 300.0f;  // in px
	constexpr SkScalar kPanelPadding = 10.0f;  // in px

	enum class Controls
	{
		kRed,
		kYellow,
		kGreen,
		kCyan,
		kBlue,
		kMagenta
	};

	struct BWParameters
	{
		float red = 40.0f;
		float yellow = 60.0f;
		float green = 40.0f;
		float cyan = 60.0f;
		float blue = 20.0f;
		float magenta = 80.0f;
	};
	static const BWParameters kBWDefault;

	SliderParams CreateSliderParams(Controls control)
	{
		constexpr SkScalar kMin = -200.0f;
		constexpr SkScalar kMax = 300.0f;
		static const SliderParams kDefaultParams(0.0f, kMin, kMax, 5.0f);

		SliderParams params = kDefaultParams;
		switch (control)
		{
		case Controls::kRed:
			params.m_Value = kBWDefault.red;
			break;
		case Controls::kYellow:
			params.m_Value = kBWDefault.yellow;
			break;
		case Controls::kGreen:
			params.m_Value = kBWDefault.green;
			break;
		case Controls::kCyan:
			params.m_Value = kBWDefault.cyan;
			break;
		case Controls::kBlue:
			params.m_Value = kBWDefault.blue;
			break;
		case Controls::kMagenta:
			params.m_Value = kBWDefault.magenta;
			break;
		}
		return params;
	}

	const std::string BW_SHADER = R"---(
		uniform shader texture;
		uniform float red;
		uniform float yellow;
		uniform float green;
		uniform float cyan;
		uniform float blue;
		uniform float magenta;

		half3 rgb2hsv(half3 c)
		{
			half4 K = half4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
			half4 p = mix(half4(c.bg, K.wz), half4(c.gb, K.xy), step(c.b, c.g));
			half4 q = mix(half4(p.xyw, c.r), half4(c.r, p.yzx), step(p.x, c.r));
			float d = q.x - min(q.w, q.y);
			float e = 1.0e-10;
			return half3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
		}

		half3 hsv2rgb(half3 c)
		{
			half4 K = half4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
			half3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
			return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
		}

		half4 main(float2 p) {
			half3 base = texture.eval(p).rgb;
			base = rgb2hsv(base);

			float hueInterval = 1.0 / 6.0;
			float startBrightness = 0.0;
			float endBrightness = 0.0;
			if (0.0 <= base.x && base.x < hueInterval)
			{
					startBrightness = red;
					endBrightness = yellow;
			}
			else if (hueInterval <= base.x && base.x < 2.0 * hueInterval)
			{
					startBrightness = yellow;
					endBrightness = green;
			}
			else if (2.0 * hueInterval <= base.x && base.x < 3.0 * hueInterval)
			{
					startBrightness = green;
					endBrightness = cyan;
			}
			else if (3.0 * hueInterval <= base.x && base.x < 4.0 * hueInterval)
			{
					startBrightness = cyan;
					endBrightness = blue;
			}
			else if (4.0 * hueInterval <= base.x && base.x < 5.0 * hueInterval)
			{
					startBrightness = blue;
					endBrightness = magenta;
			}
			else
			{
					startBrightness = magenta;
					endBrightness = red;
			}

			float fStartHue = (hueInterval - mod(base.x, hueInterval)) / hueInterval * 100.0;
			float fSaturation = base.y * 100.0;
			float fBrightness = base.z * 100.0;
	
			float fNewBrightness = (fStartHue * startBrightness / 100.0) + (100.0 - fStartHue) * (endBrightness / 100.0);
			fNewBrightness = ((100.0 - fSaturation) * fBrightness) / 100.0 + ((((fNewBrightness * fSaturation) / 100.0) * fBrightness) / 100.0);
			fNewBrightness = clamp(fNewBrightness / 100.0, 0.0, 1.0);

			half3 res = hsv2rgb(half3(0.0, 0.0, fNewBrightness));
			return half4(res, 1.0);
		})---";

	sk_sp<SkShader> CreateBWShader(sk_sp<SkImage> image, sk_sp<SkRuntimeEffect> effect, BWParameters& params)
	{
		if (!effect)
			return nullptr;

		sk_sp<SkShader> pColorizerShader = image->makeShader(SkTileMode::kDecal, SkTileMode::kDecal, {}, SkMatrix::I());
		sk_sp<SkData> dataInput = SkData::MakeWithCopy(&params, sizeof(BWParameters));
		sk_sp<SkShader> children[] = { pColorizerShader };
		return effect->makeShader(dataInput, children, 1, nullptr, false);
	}

	SkRect GetPanelRect(SkRect bounds)
	{
		bounds.fLeft = bounds.fRight - kPanelSize;
		return bounds.makeInset(kPanelPadding, kPanelPadding);
	}
}

BlackAndWhiteLayer::BlackAndWhiteLayer()
{
	m_Image = LoadImageFromFile(SkString("resources/4k.jpg"));

	m_RedSlider = m_Container.AddControl<Slider>(CreateSliderParams(Controls::kRed), SkString{"Red:"});
	m_YellowSlider = m_Container.AddControl<Slider>(CreateSliderParams(Controls::kYellow), SkString{"Yellow:"});
	m_GreenSlider = m_Container.AddControl<Slider>(CreateSliderParams(Controls::kGreen), SkString{"Green:"});
	m_CyanSlider = m_Container.AddControl<Slider>(CreateSliderParams(Controls::kCyan), SkString{"Cyan:"});
	m_BlueSlider = m_Container.AddControl<Slider>(CreateSliderParams(Controls::kBlue), SkString{"Blue:"});
	m_MagentaSlider = m_Container.AddControl<Slider>(CreateSliderParams(Controls::kMagenta), SkString{"Magenta:"});
	m_Container.AddControl<Button>([this]() {
		m_RedSlider.lock()->SetValue(kBWDefault.red);
		m_YellowSlider.lock()->SetValue(kBWDefault.yellow);
		m_GreenSlider.lock()->SetValue(kBWDefault.green);
		m_CyanSlider.lock()->SetValue(kBWDefault.cyan);
		m_BlueSlider.lock()->SetValue(kBWDefault.blue);
		m_MagentaSlider.lock()->SetValue(kBWDefault.magenta);
	}, SkString{"Reset"});

	const SkRuntimeEffect::Result effect = SkRuntimeEffect::MakeForShader(SkString{BW_SHADER.c_str()});
	if (!effect.effect)
		std::cerr << effect.errorText.c_str();
	m_Effect = effect.effect;
}

void BlackAndWhiteLayer::Draw(SkCanvas* canvas)
{
	// clear canvas with black color
	canvas->clear(SkColors::kBlack);
	const SkRect bounds = GetBounds(canvas);

	if (m_Effect)
	{
		SkAutoCanvasRestore guard(canvas, true);
		const SkRect imageRect = SkRect::MakeWH(m_Image->width(), m_Image->height());
		canvas->setMatrix(SkMatrix::RectToRect(imageRect, bounds, SkMatrix::kCenter_ScaleToFit));

		BWParameters params;
		params.red = m_RedSlider.lock()->GetValue();
		params.yellow = m_YellowSlider.lock()->GetValue();
		params.green = m_GreenSlider.lock()->GetValue();
		params.cyan = m_CyanSlider.lock()->GetValue();
		params.blue = m_BlueSlider.lock()->GetValue();
		params.magenta = m_MagentaSlider.lock()->GetValue();

		SkPaint paint;
		paint.setShader(CreateBWShader(m_Image, m_Effect, params));
		canvas->drawPaint(paint);
	}

	// draw controls
	m_Container.Draw(canvas, GetPanelRect(bounds));
}

bool BlackAndWhiteLayer::ProcessKey(Key key, InputState state, ModifierKey modifiers)
{
	return m_Container.ProcessKey(key, state, modifiers);
}

bool BlackAndWhiteLayer::ProcessMouse(int x, int y, InputState state, ModifierKey modifiers)
{
	return m_Container.ProcessMouse(x, y, state, modifiers);
}