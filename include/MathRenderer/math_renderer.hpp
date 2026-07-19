#ifndef MATH_RENDERER_HPP
#define MATH_RENDERER_HPP

#include <string>
#include <vector>
#include <BitmapTool/bmp.hpp>
#include "math_renderer_defines.hpp"

namespace math_renderer {
class LayoutEngine {
   private:
    std::vector<Token>& input;

   public:
    LayoutEngine(std::vector<Token>& in);
    LayoutBox layout();
    LayoutBox layout(int scale);
    LayoutBox parse_expr(int& pos, int scale, bool stop_on_parentheses = false);
    LayoutBox parse_elem(int& pos, int scale);
    LayoutBox parse_group(int& pos, int scale);
    LayoutBox parse_symbol(int& pos, int scale);
    LayoutBox parse_open_bracket(int& pos, int scale);
    LayoutBox parse_close_bracket(int& pos, int scale);
    LayoutBox parse_up(int& pos, int scale);
    LayoutBox parse_down(int& pos, int scale);
    LayoutBox parse_frac(int& pos, int scale);
    LayoutBox parse_sqrt(int& pos, int scale);
    LayoutBox parse_sum(int& pos, int scale);
    inline LayoutBox parse_prod(int& pos, int scale);
    LayoutBox parse_int(int& pos, int scale);
    LayoutBox parse_diff(int& pos, int scale);
    LayoutBox parse_perm(int& pos, int scale);
    LayoutBox parse_comb(int& pos, int scale);
    LayoutBox parse_log(int& pos, int scale);
};

class MathRenderer {
   private:
    std::vector<uint32_t> m_drawBuffer;
    std::vector<Letter> m_letter_list;
    std::vector<bmp::Glyph> m_letters;
    int m_display_width;
    int m_display_height;

   public:
    MathRenderer(int width, int height);
    ~MathRenderer();
    int load_glyph(const std::string& fname);
    int load_glyph_list(const std::vector<bmp::Glyph>& glyph_list);
    int init_letter_list();
    int get_letter_id(const std::string& name);
    int write2BMP(const std::string& fname);
    uint32_t* get_draw_buffer();

    int clear();
    int draw_letter(int x, int y, int scale, int letter);
    int draw_token(const Token* token);
    int draw_tokens(const std::vector<Token*>& tokens);
    int draw_special_object(const SpecialObject& obj);
    int draw_special_objects(const std::vector<SpecialObject>& obj);

    int draw_letter(int x, int y, int scale, int letter, int fbX1, int fbY1, int fbX2, int fbY2);
    int draw_token(const Token* token, int fbX1, int fbY1, int fbX2, int fbY2);
    int draw_tokens(const std::vector<Token*>& tokens, int fbX1, int fbY1, int fbX2, int fbY2);
    int draw_special_object(const SpecialObject& obj, int fbX1, int fbY1, int fbX2, int fbY2);
    int draw_special_objects(const std::vector<SpecialObject>& obj, int fbX1, int fbY1, int fbX2, int fbY2);
};

int shift_for_cursor(LayoutBox& box, const std::vector<Token>& cur_token, const int cursor, const int width,
                     const int height, int& offsetX, int& offsetY);
int ascent_for_symbol(int symbol, int scale);
int descent_for_symbol(int symbol, int scale);
int width_for_symbol(int symbol, int scale);
int ascent_for_letters(const std::vector<int>& letters, int scale);
int descent_for_letters(const std::vector<int>& letters, int scale);
int width_for_letters(const std::vector<int>& letters, int scale);

Token make_token(int type, const std::string& name, const std::vector<std::string>& letters);
Token make_token(int type, const std::string& name, const std::string& letters);
Token make_token(int type, const std::string& name);

int move_cursor_horizontal(const std::vector<Token>& tokens, int& cursor, int dir);
int insert_input(std::vector<Token>& tokens, int& cursor, const std::string& input);
int process_delete_key(std::vector<Token>& tokens, int& cursor);

/**
 * @param [out] tokens : vector of input tokens
 * @param [out] cursor : cursor position (index of the token)
 * @param [in] input : input command
 * @return status :
 *      - 0 : continue (default)
 *      - 1 : execute calculation
 *          - please pass the token vector to make_math_str() and use SciCalc::calc()
 *      - 2 : plot 2D graph
 *          - please pass the token vector to DrawGraph::plot()
 *      - 3 : execute expand
 *          - please pass the token vector to make_math_str() and use SciCalc::expand()
 *      - -1 : error
 */
int process_input(std::vector<Token>& tokens, int& cursor, const std::string& input);

int make_math_str(const std::vector<Token>& tokens, int& index, std::string& str);
std::vector<Token> make_tokens(std::string& math_str);

int move_cursor_touch(const std::vector<Token>& tokens, int& cursor, const int touchX, const int touchY);
}  // namespace math_renderer

#endif
