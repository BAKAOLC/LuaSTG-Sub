#include "AppFrame.h"
#include "utf8.hpp"

namespace luastg
{
	// luastg plus interface
	
	constexpr int TEXT_ALIGN_LEFT = 0x00;
	constexpr int TEXT_ALIGN_CENTER = 0x01;
	constexpr int TEXT_ALIGN_RIGHT = 0x02;

	constexpr int TEXT_ALIGN_TOP = 0x00;
	constexpr int TEXT_ALIGN_VCENTER = 0x04;
	constexpr int TEXT_ALIGN_BOTTOM = 0x08;

	constexpr int TEXT_FLAG_WORDBREAK = 0x10;

	bool AppFrame::RenderText(IResourceFont* p, wchar_t* strBuf, core::RectF rect, core::Vector2F scale, FontAlignHorizontal halign, FontAlignVertical valign, bool bWordBreak)noexcept
	{
		using namespace core;
		using namespace core::Graphics;

		IGlyphManager* pGlyphManager = p->GetGlyphManager();
		
		// 准备渲染字体
		m_pTextRenderer->setGlyphManager(pGlyphManager);
		m_pTextRenderer->setScale(scale);

		// 设置混合和颜色
		updateGraph2DBlendMode(p->GetBlendMode());
		m_pTextRenderer->setColor(p->GetBlendColor());
		
		// 第一次遍历计算要渲染多少行
		const wchar_t* pText = strBuf;
		int iLineCount = 1;
		float fLineWidth = 0.f;
		while (*pText)
		{
			bool bNewLine = false;
			if (*pText == L'\n')
				bNewLine = true;
			else
			{
				GlyphInfo tGlyphInfo{};
				if (pGlyphManager->getGlyph(*pText, &tGlyphInfo, true))
				{
					float adv = tGlyphInfo.advance.x * scale.x;
					if (bWordBreak && fLineWidth + adv > std::abs(rect.a.x - rect.b.x))  // 截断模式
					{
						if (pText == strBuf || *(pText - 1) == L'\n')
						{
							++pText;  // 防止一个字符都不渲染导致死循环
							if (*pText == L'\0')
								break;
						}
						bNewLine = true;
					}
					else
						fLineWidth += adv;
				}
			}
			if (bNewLine)
			{
				++iLineCount;
				fLineWidth = 0.f;
			}
			if (*pText != L'\0')
				++pText;
		}
		
		// 计算起笔位置
		float fTotalLineHeight = pGlyphManager->getLineHeight() * iLineCount * scale.y;
		core::Vector2F vRenderPos;
		switch (valign)
		{
		case FontAlignVertical::Bottom:
			vRenderPos.y = rect.b.y + fTotalLineHeight;
			break;
		case FontAlignVertical::Middle:
			vRenderPos.y = rect.a.y - std::abs(rect.a.y - rect.b.y) / 2.f + fTotalLineHeight / 2.f;
			break;
		case FontAlignVertical::Top:
		default:
			vRenderPos.y = rect.a.y;
			break;
		}
		vRenderPos.x = rect.a.x;
		vRenderPos.y -= pGlyphManager->getAscender() * scale.y;
		
		// 逐行渲染文字
		wchar_t* pScanner = strBuf;
		wchar_t c = 0;
		bool bEOS = false;
		fLineWidth = 0.f;
		pText = pScanner;
		while (!bEOS)
		{
			// 寻找断句位置，换行、EOF、或者行溢出
			while (*pScanner != L'\0' && *pScanner != '\n')
			{
				GlyphInfo tGlyphInfo{};
				if (pGlyphManager->getGlyph(*pScanner, &tGlyphInfo, true))
				{
					float adv = tGlyphInfo.advance.x * scale.x;

					// 检查当前字符渲染后会不会导致行溢出
					if (bWordBreak && fLineWidth + adv > std::abs(rect.a.x - rect.b.x))
					{
						if (pScanner == pText)  // 防止一个字符都不渲染导致死循环
							++pScanner;
						break;
					}
					fLineWidth += adv;
				}
				++pScanner;
			}
			
			// 在断句位置写入\0
			c = *pScanner;
			if (c == L'\0')
				bEOS = true;
			else
				*pScanner = L'\0';
			
			// 渲染从pText~pScanner的文字
			std::string u8_str(utf8::to_string(pText));
			Vector2F ignore_;
			switch (halign)
			{
			case FontAlignHorizontal::Right:
				m_pTextRenderer->drawText(u8_str, Vector2F(
					vRenderPos.x + std::abs(rect.a.x - rect.b.x) - fLineWidth,
					vRenderPos.y
				), &ignore_);
				break;
			case FontAlignHorizontal::Center:
				m_pTextRenderer->drawText(u8_str, Vector2F(
					vRenderPos.x + std::abs(rect.a.x - rect.b.x) / 2.f - fLineWidth / 2.f,
					vRenderPos.y
				), &ignore_);
				break;
			case FontAlignHorizontal::Left:
			default:
				m_pTextRenderer->drawText(u8_str, vRenderPos, &ignore_);
				break;
			}

			// 恢复断句处字符
			*pScanner = c;
			fLineWidth = 0.f;
			if (c == L'\n')
				pText = ++pScanner;
			else
				pText = pScanner;
			
			// 移动y轴
			vRenderPos.y -= pGlyphManager->getLineHeight() * scale.y;
		}

		return true;
	}
	
	core::Vector2F AppFrame::CalcuTextSize(IResourceFont* p, const wchar_t* strBuf, core::Vector2F scale)noexcept
	{
		using namespace core;
		using namespace core::Graphics;

		IGlyphManager* pGlyphManager = p->GetGlyphManager();
		
		int iLineCount = 1;
		float fLineWidth = 0.f;
		float fMaxLineWidth = 0.f;
		while (*strBuf)
		{
			if (*strBuf == L'\n')
			{
				++iLineCount;
				fMaxLineWidth = std::max(fMaxLineWidth, fLineWidth);
				fLineWidth = 0.f;
			}
			else
			{
				GlyphInfo tGlyphInfo{};
				if (pGlyphManager->getGlyph(*strBuf, &tGlyphInfo, true))
					fLineWidth += tGlyphInfo.advance.x * scale.x;
			}
			++strBuf;
		}
		fMaxLineWidth = std::max(fMaxLineWidth, fLineWidth);
		
		return core::Vector2F(fMaxLineWidth, iLineCount * pGlyphManager->getLineHeight() * scale.y);
	}
	
	bool AppFrame::RenderText(const char* name, const char* str, float x, float y, float scale, FontAlignHorizontal halign, FontAlignVertical valign)noexcept
	{
		core::SmartReference<IResourceFont> p = m_ResourceMgr.FindSpriteFont(name);
		if (!p)
		{
			spdlog::error("[luastg] RenderText: 找不到字体资源'{}'", name);
			return false;
		}
		
		// 编码转换
		std::wstring s_TempStringBuf;
		try
		{
			s_TempStringBuf = utf8::to_wstring(str);
		}
		catch (const std::bad_alloc&)
		{
			spdlog::error("[luastg] RenderText: 内存不足");
			return false;
		}
		
		// 计算渲染位置
		core::Vector2F tSize = CalcuTextSize(p.get(), s_TempStringBuf.c_str(), core::Vector2F(scale, scale));
		switch (halign)
		{
		case FontAlignHorizontal::Right:
			x -= tSize.x;
			break;
		case FontAlignHorizontal::Center:
			x -= tSize.x / 2.f;
			break;
		case FontAlignHorizontal::Left:
		default:
			break;
		}
		switch (valign)
		{
		case FontAlignVertical::Bottom:
			y += tSize.y;
			break;
		case FontAlignVertical::Middle:
			y += tSize.y / 2.f;
			break;
		case FontAlignVertical::Top:
		default:
			break;
		}
		
		return RenderText(
			p.get(),
			const_cast<wchar_t*>(s_TempStringBuf.data()),
			core::RectF(x, y, x + tSize.x, y - tSize.y),
			core::Vector2F(scale, scale),
			halign,
			valign,
			false
			);
	}
	
	bool AppFrame::RenderTTF(const char* name, const char* str,
		float left, float right, float bottom, float top, float scale, int format, core::Color4B c)noexcept
	{
		core::SmartReference<IResourceFont> p = m_ResourceMgr.FindTTFFont(name);
		if (!p) {
			spdlog::error("[luastg] RenderTTF: 找不到字体资源'{}'", name);
			return false;
		}
		
		// 编码转换
		std::wstring s_TempStringBuf;
		try {
			s_TempStringBuf = utf8::to_wstring(str);
		}
		catch (const std::bad_alloc&) {
			spdlog::error("[luastg] RenderTTF: 内存不足");
			return false;
		}
		
		// 计算格式
		bool bWordBreak = false;
		FontAlignHorizontal halign = FontAlignHorizontal::Left;
		FontAlignVertical valign = FontAlignVertical::Top;
		
		if ((format & TEXT_ALIGN_CENTER) == TEXT_ALIGN_CENTER)
			halign = FontAlignHorizontal::Center;
		else if ((format & TEXT_ALIGN_RIGHT) == TEXT_ALIGN_RIGHT)
			halign = FontAlignHorizontal::Right;
		
		if ((format & TEXT_ALIGN_VCENTER) == TEXT_ALIGN_VCENTER)
			valign = FontAlignVertical::Middle;
		else if ((format & TEXT_ALIGN_BOTTOM) == TEXT_ALIGN_BOTTOM)
			valign = FontAlignVertical::Bottom;
		
		if ((format & TEXT_FLAG_WORDBREAK) == TEXT_FLAG_WORDBREAK)
			bWordBreak = true;
		
		p->SetBlendColor(c);
		return RenderText(
			p.get(),
			const_cast<wchar_t*>(s_TempStringBuf.data()),
			core::RectF(left, top, right, bottom),
			core::Vector2F(scale, scale) * 0.5f,  // TODO: 缩放系数=0.5 ????????????
			halign,
			valign,
			bWordBreak
		);
	}
	
	// native interface
	
	bool AppFrame::FontRenderer_SetFontProvider(const char* name)
	{
		core::SmartReference<IResourceFont> p = m_ResourceMgr.FindTTFFont(name);
		if (!p)
		{
			spdlog::error("[luastg] SetFontProvider: 找不到字体资源'{}'", name);
			return false;
		}
		m_pTextRenderer->setGlyphManager(p->GetGlyphManager());
		return true;
	}
	
	void AppFrame::FontRenderer_SetScale(core::Vector2F const& s)
	{
		m_pTextRenderer->setScale(s);
	}
	
	core::RectF AppFrame::FontRenderer_MeasureTextBoundary(const char* str, size_t len)
	{
		return m_pTextRenderer->getTextBoundary(core::StringView(str, len));
	}
	
	core::Vector2F AppFrame::FontRenderer_MeasureTextAdvance(const char* str, size_t len)
	{
		return m_pTextRenderer->getTextAdvance(core::StringView(str, len));
	}
	
	bool AppFrame::FontRenderer_RenderText(const char* str, size_t len, core::Vector2F& pos, const float z, const BlendMode blend, core::Color4B const& color)
	{
		float const last_z = m_pTextRenderer->getZ();

		updateGraph2DBlendMode(blend);
		m_pTextRenderer->setZ(z);
		m_pTextRenderer->setColor(color);
		
		core::Vector2F endpos;
		const bool result = m_pTextRenderer->drawText(core::StringView(str, len), pos, &endpos);
		pos = endpos;

		m_pTextRenderer->setZ(last_z);
		return result;
	}
	
	bool AppFrame::FontRenderer_RenderTextInSpace(const char* str, size_t len, core::Vector3F& pos, core::Vector3F const& rvec, core::Vector3F const& dvec, const BlendMode blend, core::Color4B const& color)
	{
		updateGraph2DBlendMode(blend);
		m_pTextRenderer->setColor(color);

		core::Vector3F endpos;
		const bool result = m_pTextRenderer->drawTextInSpace(
			core::StringView(str, len),
			pos,
			rvec,
			dvec,
			&endpos);
		pos = endpos;

		return result;
	}

	float AppFrame::FontRenderer_GetFontLineHeight()
	{
		auto* p = m_pTextRenderer->getGlyphManager();
		if (p) return p->getLineHeight();
		return 0.0f;
	}
	
	float AppFrame::FontRenderer_GetFontAscender()
	{
		auto* p = m_pTextRenderer->getGlyphManager();
		if (p) return p->getAscender();
		return 0.0f;
	}
	
	float AppFrame::FontRenderer_GetFontDescender()
	{
		auto* p = m_pTextRenderer->getGlyphManager();
		if (p) return p->getDescender();
		return 0.0f;
	}
};
