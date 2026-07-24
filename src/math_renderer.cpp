#include "MathRenderer/math_renderer.hpp"
#include <BitmapTool/bmp.hpp>

namespace math_renderer {

MathRenderer::MathRenderer(int width, int height)
    : m_drawBuffer(width * height, 0), m_display_width(width), m_display_height(height) {
    init_letter_list();
    load_glyph();
}
MathRenderer::~MathRenderer() { write2BMP("result.bmp"); }

int MathRenderer::load_glyph() {
    bmp::Image img = bmp::loadBMP1bit((char*)dot_font::img, sizeof(dot_font::img));
    m_letters.clear();
    for (int i = 0; i < dot_font::LETTER_NUM; i++) {
        m_letters.push_back(bmp::extractGlyph(img, i, BASE_W, BASE_H));
    }
    return 0;
}
int MathRenderer::load_glyph(const std::string& fname) {
    bmp::Image img = bmp::loadBMP1bit(fname);
    m_letters.clear();
    for (int i = 0; i < dot_font::LETTER_NUM; i++) {
        m_letters.push_back(bmp::extractGlyph(img, i, BASE_W, BASE_H));
    }
    return 0;
}
int MathRenderer::load_glyph_list(const std::vector<bmp::Glyph>& glyph_list) {
    m_letters = glyph_list;
    return 0;
}

int MathRenderer::init_letter_list() {
    for (int i = 0; i < dot_font::LETTER_NUM; i++) {
        m_letter_list.emplace_back(i, dot_font::LETTERS[i]);
    }
    return 0;
}

int MathRenderer::get_letter_id(const std::string& name) {
    if (name.size() == 0) return dot_font::LETTER_SPACE;
    for (int i = 0; i < m_letter_list.size(); i++) {
        if (m_letter_list[i].m_name == name) return m_letter_list[i].m_id;
    }
    return dot_font::LETTER_SPACE;
}

int MathRenderer::write2BMP(const std::string& fname) {
    bmp::save32bitBMP(fname, m_display_width, m_display_height, m_drawBuffer.data());
    return 0;
}

uint32_t* MathRenderer::get_draw_buffer() { return m_drawBuffer.data(); }
}  // namespace math_renderer
