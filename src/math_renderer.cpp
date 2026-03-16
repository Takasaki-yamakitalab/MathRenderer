#include "MathRenderer/math_renderer.hpp"

namespace math_renderer {

#ifdef __ANDROID__
    MathRenderer::MathRenderer(int width, int height)
        : m_drawBuffer(width* height, 0)
        , m_display_width(width)
        , m_display_height(height) {
        init_letter_list();
    }
    MathRenderer::~MathRenderer() {
    }
#else
    MathRenderer::MathRenderer(int width, int height)
        : m_drawBuffer(width* height, 0)
        , m_display_width(width)
        , m_display_height(height) {
        init_letter_list();
        load_glyph("dot_font_x11y23.bmp");
    }
    MathRenderer::~MathRenderer() {
        write2BMP("result.bmp");
    }
#endif

    int MathRenderer::load_glyph(const std::string& fname) {
        bmp::Image img = bmp::loadBMP1bit(fname);
        m_letters.clear();
        for (int i = 0; i < LETTER_NUM; i++) {
            m_letters.push_back(bmp::extractGlyph(img, i, BASE_W, BASE_H));
        }
        return 0;
    }
    int MathRenderer::load_glyph_list(const std::vector<bmp::Glyph>& glyph_list) {
        m_letters = glyph_list;
        return 0;
    }

    int MathRenderer::init_letter_list() {
        for (int i = 0; i < LETTER_NUM; i++) {
            m_letter_list.emplace_back(i, LETTERS[i]);
        }
        return 0;
    }

    int MathRenderer::get_letter_id(const std::string& name) {
        if (name.size() == 0) return LETTER_SPACE;
        for (int i = 0; i < m_letter_list.size(); i++) {
            if (m_letter_list[i].m_name == name) return m_letter_list[i].m_id;
        }
        return LETTER_SPACE;
    }

    int MathRenderer::write2BMP(const std::string& fname) {
        bmp::save32bitBMP(fname, m_display_width, m_display_height, m_drawBuffer.data());
        return 0;
    }

    uint32_t* MathRenderer::get_draw_buffer() {
        return m_drawBuffer.data();
    }
}