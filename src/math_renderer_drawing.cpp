#include "MathRenderer/math_renderer.hpp"

namespace math_renderer {
    int MathRenderer::clear() {
        m_drawBuffer = std::vector<uint32_t>(m_drawBuffer.size(), 0);
        return 0;
    }
    int MathRenderer::draw_letter(int x, int y, int scale, int letter) {
        if (letter >= dot_font::LETTER_NUM) return 0;
        bmp::drawGlyph(m_drawBuffer.data(), m_display_width, m_display_height, x, y, m_letters[letter], scale);
        return 0;
    }
    int MathRenderer::draw_token(const Token* token) {
        if (token == NULL) return 0;
        if (token->m_letters.size() == 0) return 0;
        int cnt = 0;
        int x = token->x;
        int y = token->y;
        int scale = token->scale;
        for (cnt = 0; cnt < token->m_letters.size(); cnt++) {
            draw_letter(x, y, scale, token->m_letters[cnt]);
            x += BASE_W * scale;
        }
        return cnt;
    }
    int MathRenderer::draw_tokens(const std::vector<Token*>& tokens) {
        int cnt = 0;
        for (int i = 0; i < tokens.size(); i++) {
            cnt += draw_token(tokens[i]);
        }
        return cnt;
    }

    int MathRenderer::draw_special_object(const SpecialObject& obj) {
        if (obj.type == SPOBJ_EMPTY) {
            bmp::drawRectangle(m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, obj.x2, obj.y2, false);
            return 0;
        }
        if (obj.type == SPOBJ_FRAC_BAR ||
            obj.type == SPOBJ_SQRT_BAR) {
            bmp::drawLine(this->m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, obj.x2, obj.y2);
            return 0;
        }
        if (obj.type == SPOBJ_SQRT_SYMBOL) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, m_letters[dot_font::LETTER_SQRT],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_EQUAL) {
            bmp::drawGlyph(this->m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, m_letters[dot_font::LETTER_EQUAL], SMALL_SCALE);
            return 0;
        }
        if (obj.type == SPOBJ_OPEN_BRACKET) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, m_letters[dot_font::LETTER_OPEN_BRACKET],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_CLOSE_BRACKET) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, m_letters[dot_font::LETTER_CLOSE_BRACKET],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_INTEGRAL_SYMBOL) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, m_letters[dot_font::LETTER_INTEGRAL],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_INTEGRAL_D) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, m_letters[dot_font::LETTER_d],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_PERM_SYMBOL) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, m_letters[dot_font::LETTER_P],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_COMB_SYMBOL) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, m_display_height, obj.x1, obj.y1, m_letters[dot_font::LETTER_C],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        return 0;
    }
    int MathRenderer::draw_special_objects(const std::vector<SpecialObject>& obj) {
        for (auto& o : obj) {
            draw_special_object(o);
        }
        return 0;
    }

    int MathRenderer::draw_letter(int x, int y, int scale, int letter, int fbX1, int fbY1, int fbX2, int fbY2) {
        if (letter >= dot_font::LETTER_NUM) return 0;
        bmp::drawGlyph(m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, x, y, m_letters[letter], scale);
        return 0;
    }
    int MathRenderer::draw_token(const Token* token, int fbX1, int fbY1, int fbX2, int fbY2) {
        if (token == NULL) return 0;
        if (token->m_letters.size() == 0) return 0;
        int cnt = 0;
        int x = token->x;
        int y = token->y;
        int scale = token->scale;
        for (cnt = 0; cnt < token->m_letters.size(); cnt++) {
            draw_letter(x, y, scale, token->m_letters[cnt], fbX1, fbY1, fbX2, fbY2);
            x += BASE_W * scale;
        }
        return cnt;
    }
    int MathRenderer::draw_tokens(const std::vector<Token*>& tokens, int fbX1, int fbY1, int fbX2, int fbY2) {
        int cnt = 0;
        for (int i = 0; i < tokens.size(); i++) {
            cnt += draw_token(tokens[i], fbX1, fbY1, fbX2, fbY2);
        }
        return cnt;
    }
    int MathRenderer::draw_special_object(const SpecialObject& obj, int fbX1, int fbY1, int fbX2, int fbY2) {
        if (obj.type == SPOBJ_EMPTY) {
            bmp::drawRectangle(m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, obj.x2, obj.y2, false);
            return 0;
        }
        if (obj.type == SPOBJ_FRAC_BAR ||
            obj.type == SPOBJ_SQRT_BAR) {
            bmp::drawLine(this->m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, obj.x2, obj.y2);
            return 0;
        }
        if (obj.type == SPOBJ_SQRT_SYMBOL) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, m_letters[dot_font::LETTER_SQRT],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_EQUAL) {
            bmp::drawGlyph(this->m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, m_letters[dot_font::LETTER_EQUAL], SMALL_SCALE);
            return 0;
        }
        if (obj.type == SPOBJ_OPEN_BRACKET) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, m_letters[dot_font::LETTER_OPEN_BRACKET],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_CLOSE_BRACKET) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, m_letters[dot_font::LETTER_CLOSE_BRACKET],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_INTEGRAL_SYMBOL) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, m_letters[dot_font::LETTER_INTEGRAL],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_INTEGRAL_D) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, m_letters[dot_font::LETTER_d],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_PERM_SYMBOL) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, m_letters[dot_font::LETTER_P],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        if (obj.type == SPOBJ_COMB_SYMBOL) {
            bmp::drawExtendGlyph(this->m_drawBuffer.data(), m_display_width, fbX1, fbY1, fbX2, fbY2, obj.x1, obj.y1, m_letters[dot_font::LETTER_C],
                std::abs(obj.x2 - obj.x1), std::abs(obj.y2 - obj.y1));
            return 0;
        }
        return 0;
    }
    int MathRenderer::draw_special_objects(const std::vector<SpecialObject>& obj, int fbX1, int fbY1, int fbX2, int fbY2) {
        for (auto& o : obj) {
            draw_special_object(o, fbX1, fbY1, fbX2, fbY2);
        }
        return 0;
    }
}
