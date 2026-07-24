#include "MathRenderer/math_renderer.hpp"
#include "MathRenderer/math_renderer_defines.hpp"

namespace math_renderer {
LayoutEngine::LayoutEngine(std::vector<Token>& in) : input(in) {}

LayoutBox LayoutEngine::layout() {
    int pos = 0;
    LayoutBox root = parse_expr(pos, NORMAL_SCALE);
    root.shift(0, root.ascent);
    return root;
}

LayoutBox LayoutEngine::layout(int scale) {
    int pos = 0;
    LayoutBox root = parse_expr(pos, scale);
    root.shift(0, root.ascent);
    return root;
}

LayoutBox LayoutEngine::parse_expr(int& pos, int scale, bool stop_on_paretheses) {
    LayoutBox result;
    int curX = 0;
    while (pos < input.size() && input[pos].type != TK_CLOSE_BRACE && input[pos].type != TK_CLOSE_SQUARE) {
        if (stop_on_paretheses && input[pos].type == TK_CLOSE_PARENTHESES) break;
        LayoutBox elem = parse_elem(pos, scale);
        elem.shift(curX, 0);
        result.merge(elem);
        result.width += elem.width;
        result.ascent = std::max(result.ascent, elem.ascent);
        result.descent = std::max(result.descent, elem.descent);
        curX += elem.width;
    }
    return result;
}

LayoutBox LayoutEngine::parse_elem(int& pos, int scale) {
    Token& t = input[pos];
    switch (t.type) {
        case TK_SYMBOL:
            return parse_symbol(pos, scale);
        case TK_OPEN_PARENTHESES:
            return parse_open_bracket(pos, scale);
        case TK_CLOSE_PARENTHESES:
            return parse_close_bracket(pos, scale);
        case TK_UP:
            return parse_up(pos, scale);
        case TK_DOWN:
            return parse_down(pos, scale);
        case TK_FRAC:
            return parse_frac(pos, scale);
        case TK_SQRT:
            return parse_sqrt(pos, scale);
        case TK_SUM:
            return parse_sum(pos, scale);
        case TK_PROD:
            return parse_prod(pos, scale);
        case TK_INT:
            return parse_int(pos, scale);
        case TK_DIFF:
            return parse_diff(pos, scale);
        case TK_PERM:
            return parse_perm(pos, scale);
        case TK_COMB:
            return parse_comb(pos, scale);
        case TK_LOG:
            return parse_log(pos, scale);
        default:
            pos++;
            return LayoutBox{};
    }
}

LayoutBox LayoutEngine::parse_group(int& pos, int scale) {
    LayoutBox lbrace = parse_symbol(pos, scale);
    LayoutBox box = parse_expr(pos, scale);
    LayoutBox rbrace = parse_symbol(pos, scale);
    if (box.tokens.size() == 0 && box.spobj.size() == 0) {
        // empty {}
        SpecialObject obj;
        obj.type = SPOBJ_EMPTY;
        obj.x1 = 0;
        obj.x2 = width_for_scale(scale);
        obj.y1 = -ascent_for_symbol(0, scale);
        obj.y2 = descent_for_symbol(0, scale);
        box.spobj.push_back(obj);
        box.width = width_for_scale(scale);
        box.ascent = ascent_for_symbol(0, scale);
        box.descent = descent_for_symbol(0, scale);
    }
    rbrace.shift(box.width, 0);
    box.merge(lbrace);
    box.merge(rbrace);
    return box;
}

LayoutBox LayoutEngine::parse_symbol(int& pos, int scale) {
    Token& t = input[pos++];
    t.scale = scale;
    t.x = 0;
    t.y = -ascent_for_letters(t.m_letters, scale);

    LayoutBox box;
    box.width = width_for_letters(t.m_letters, scale);
    box.ascent = ascent_for_letters(t.m_letters, scale);
    box.descent = descent_for_letters(t.m_letters, scale);
    box.tokens.push_back(&t);
    return box;
}

LayoutBox LayoutEngine::parse_open_bracket(int& pos, int scale) {
    LayoutBox open = parse_symbol(pos, scale);
    LayoutBox body = parse_expr(pos, scale, true);
    body.shift(open.width, 0);

    LayoutBox result;
    result.width = body.width + width_for_scale(scale);
    result.ascent = std::max(open.ascent, body.ascent);
    result.descent = std::max(open.descent, body.descent);
    result.merge(open);
    result.merge(body);

    SpecialObject obj;
    obj.type = SPOBJ_OPEN_BRACKET;
    obj.x1 = 0;
    obj.y1 = -result.ascent;
    obj.x2 = width_for_scale(scale);
    obj.y2 = result.descent;
    result.spobj.push_back(obj);

    if (pos >= input.size() || input[pos].type != TK_CLOSE_PARENTHESES) return result;

    LayoutBox close = parse_symbol(pos, scale);
    close.shift(open.width + body.width, 0);
    result.merge(close);

    obj.type = SPOBJ_CLOSE_BRACKET;
    obj.x1 = width_for_scale(scale) + body.width;
    obj.x2 = obj.x1 + width_for_scale(scale);
    result.spobj.push_back(obj);
    result.width += width_for_scale(scale);
    return result;
}

LayoutBox LayoutEngine::parse_close_bracket(int& pos, int scale) {
    LayoutBox result = parse_symbol(pos, scale);

    SpecialObject obj;
    obj.type = SPOBJ_CLOSE_BRACKET;
    obj.x1 = 0;
    obj.y1 = -result.ascent;
    obj.x2 = width_for_scale(scale);
    obj.y2 = result.descent;
    result.spobj.push_back(obj);

    return result;
}

LayoutBox LayoutEngine::parse_up(int& pos, int scale) {
    LayoutBox head = parse_symbol(pos, scale);
    LayoutBox base = parse_group(pos, scale);
    LayoutBox sup = parse_group(pos, SMALL_SCALE);
    int offsetY = base.ascent + sup.descent - SUPER_OFFSET;
    sup.shift(base.width, -offsetY);

    LayoutBox result;
    result.width = base.width + sup.width;
    result.ascent = std::max(base.ascent, sup.ascent + offsetY);
    result.descent = base.descent;
    result.merge(head);
    result.merge(base);
    result.merge(sup);
    return result;
}

LayoutBox LayoutEngine::parse_down(int& pos, int scale) {
    LayoutBox head = parse_symbol(pos, scale);
    LayoutBox base = parse_group(pos, scale);
    LayoutBox sub = parse_group(pos, SMALL_SCALE);
    int offsetY = base.descent + sub.ascent - SUB_OFFSET;
    sub.shift(base.width, offsetY);

    LayoutBox result;
    result.width = base.width + sub.width;
    result.ascent = base.ascent;
    result.descent = std::max(base.descent, sub.descent + offsetY);
    result.merge(head);
    result.merge(base);
    result.merge(sub);
    return result;
}

LayoutBox LayoutEngine::parse_frac(int& pos, int scale) {
    LayoutBox head = parse_symbol(pos, scale);
    LayoutBox num = parse_group(pos, scale);
    LayoutBox den = parse_group(pos, scale);
    int width = std::max(num.width, den.width) + scale * FRAC_SIDE_PADDING * 2 + scale * FRAC_SIDE_MARGIN * 2;
    int num_h = num.ascent + num.descent;
    int den_h = den.ascent + den.descent;
    int num_offset = num.descent + FRAC_GAP * scale / 2;
    int den_offset = den.ascent + FRAC_GAP * scale / 2;
    num.shift((width - num.width) / 2, -num_offset);
    den.shift((width - den.width) / 2, den_offset);

    LayoutBox result;
    result.width = width;
    result.ascent = num_offset + num.ascent;
    result.descent = den_offset + den.descent;

    SpecialObject obj;
    obj.type = SPOBJ_FRAC_BAR;
    obj.x1 = FRAC_SIDE_MARGIN * scale;
    obj.x2 = width - FRAC_SIDE_MARGIN * scale;
    for (int i = 0; i < scale; i++) {
        obj.y1 = i;
        obj.y2 = i;
        result.spobj.push_back(obj);
    }

    result.merge(head);
    result.merge(num);
    result.merge(den);

    return result;
}

LayoutBox LayoutEngine::parse_sqrt(int& pos, int scale) {
    LayoutBox head = parse_symbol(pos, scale);
    LayoutBox body = parse_group(pos, scale);
    int bodyHeight = body.ascent + body.descent;

    body.shift(ROOT_SYMBOL_WIDTH * scale, 0);

    int width = ROOT_SYMBOL_WIDTH * scale + body.width + ROOT_RIGHT_PADDING * scale;

    LayoutBox result;
    result.width = width;
    result.ascent = body.ascent + ROOT_TOP_PADDING * scale;
    result.descent = body.descent;

    SpecialObject obj;
    obj.type = SPOBJ_SQRT_SYMBOL;
    obj.x1 = 0;
    obj.x2 = ROOT_SYMBOL_WIDTH * scale;
    obj.y1 = -body.ascent - ROOT_TOP_PADDING * scale;
    obj.y2 = body.descent;
    result.spobj.push_back(obj);

    obj.type = SPOBJ_SQRT_BAR;
    obj.x1 = ROOT_SYMBOL_WIDTH * scale - ROOT_LEFT_PADDING * scale;
    obj.x2 = width;
    for (int i = 0; i < scale; i++) {
        obj.y1 += i;
        obj.y2 = obj.y1;
        result.spobj.push_back(obj);
    }

    result.merge(head);
    result.merge(body);

    return result;
}

LayoutBox LayoutEngine::parse_sum(int& pos, int scale) {
    LayoutBox symbol = parse_symbol(pos, scale);
    LayoutBox body = parse_group(pos, scale);
    LayoutBox var = parse_group(pos, SMALL_SCALE);
    LayoutBox start = parse_group(pos, SMALL_SCALE);
    LayoutBox end = parse_group(pos, SMALL_SCALE);

    int sub_width = var.width + BASE_W * SMALL_SCALE + start.width;
    int head_width = std::max(symbol.width, std::max(sub_width, end.width));
    int sup_offset = symbol.ascent + end.descent;
    int sub_offset = symbol.descent + std::max(ascent_for_scale(SMALL_SCALE), std::max(var.ascent, start.ascent));

    symbol.shift((head_width - symbol.width) / 2, 0);
    body.shift(head_width + width_for_scale(scale), 0);
    var.shift((head_width - sub_width) / 2, sub_offset);
    start.shift((head_width - sub_width) / 2 + var.width + BASE_W * SMALL_SCALE, sub_offset);
    end.shift((head_width - end.width) / 2, -sup_offset);

    LayoutBox result;
    result.width = head_width + width_for_scale(scale) + body.width + width_for_scale(scale);
    result.ascent = sup_offset + end.ascent;
    result.descent = std::max(var.descent, start.descent) + sub_offset;

    SpecialObject obj;
    obj.type = SPOBJ_EQUAL;
    obj.x1 = (head_width - sub_width) / 2 + var.width;
    obj.x2 = obj.x1 + BASE_W * SMALL_SCALE;
    obj.y1 = sub_offset - ascent_for_scale(SMALL_SCALE);
    obj.y2 = obj.y1 + BASE_H * SMALL_SCALE;
    result.spobj.push_back(obj);

    obj.type = SPOBJ_OPEN_BRACKET;
    obj.x1 = head_width;
    obj.y1 = -body.ascent;
    obj.x2 = obj.x1 + width_for_scale(scale);
    obj.y2 = body.descent;
    result.spobj.push_back(obj);

    obj.type = SPOBJ_CLOSE_BRACKET;
    obj.x1 = head_width + width_for_scale(scale) + body.width;
    obj.x2 = obj.x1 + width_for_scale(scale);
    result.spobj.push_back(obj);

    result.merge(symbol);
    result.merge(body);
    result.merge(var);
    result.merge(start);
    result.merge(end);
    return result;
}

inline LayoutBox LayoutEngine::parse_prod(int& pos, int scale) { return parse_sum(pos, scale); }

LayoutBox LayoutEngine::parse_int(int& pos, int scale) {
    LayoutBox symbol = parse_symbol(pos, scale);
    LayoutBox body = parse_group(pos, scale);
    LayoutBox var = parse_group(pos, scale);
    LayoutBox start = parse_group(pos, SMALL_SCALE);
    LayoutBox end = parse_group(pos, SMALL_SCALE);

    int start_offset = body.descent + start.ascent;
    int end_offset = body.ascent + end.descent;
    int body_offset = width_for_scale(scale) + std::max(start.width, end.width);
    int var_offset = body_offset + body.width + width_for_scale(scale);
    start.shift(width_for_scale(scale), start_offset);
    end.shift(width_for_scale(scale), -end_offset);
    body.shift(body_offset, 0);
    var.shift(var_offset, 0);

    LayoutBox result;
    result.width =
        width_for_scale(scale) + std::max(start.width, end.width) + body.width + width_for_scale(scale) + var.width;
    result.ascent = std::max(std::max(end_offset + end.ascent, body.ascent), var.ascent);
    result.descent = std::max(std::max(start_offset + start.descent, body.descent), var.descent);

    SpecialObject obj;
    obj.type = SPOBJ_INTEGRAL_SYMBOL;
    obj.x1 = 0;
    obj.y1 = -end_offset - end.ascent;
    obj.x2 = width_for_scale(scale);
    obj.y2 = start_offset + start.descent;
    result.spobj.push_back(obj);

    obj.type = SPOBJ_INTEGRAL_D;
    obj.x1 = var_offset - width_for_scale(scale);
    obj.y1 = -ascent_for_scale(scale);
    obj.x2 = obj.x1 + width_for_scale(scale);
    obj.y2 = descent_for_scale(scale);
    result.spobj.push_back(obj);

    result.merge(symbol);
    result.merge(body);
    result.merge(var);
    result.merge(start);
    result.merge(end);

    return result;
}

LayoutBox LayoutEngine::parse_diff(int& pos, int scale) {
    LayoutBox symbol = parse_symbol(pos, scale);
    LayoutBox body = parse_group(pos, scale);
    LayoutBox var = parse_group(pos, scale);
    LayoutBox x = parse_group(pos, SMALL_SCALE);

    int var_offset = FRAC_SIDE_MARGIN * scale + FRAC_SIDE_PADDING * scale + width_for_scale(scale);
    int body_offset = var_offset + var.width + FRAC_SIDE_MARGIN * scale + FRAC_SIDE_PADDING * scale;
    int x_offsetX = body_offset + body.width + 3 * scale;

    int num_offset = descent_for_symbol(dot_font::LETTER_d, scale) + FRAC_GAP * scale / 2;
    int den_offset = var.ascent + FRAC_GAP * scale / 2;
    int frac_width = body_offset;

    int x_offsetY = std::max(body.descent, den_offset + var.descent) - x.descent;

    var.shift(var_offset, den_offset);
    body.shift(body_offset, 0);
    x.shift(x_offsetX, x_offsetY);

    LayoutBox result;
    result.width = x_offsetX + x.width;
    result.ascent = std::max(body.ascent, num_offset + ascent_for_symbol(dot_font::LETTER_d, scale));
    result.descent = std::max(body.descent, den_offset + var.descent);

    SpecialObject obj;
    obj.type = SPOBJ_INTEGRAL_D;
    obj.x1 = (body_offset - width_for_scale(scale)) / 2;
    obj.y1 = -(num_offset + ascent_for_scale(scale));
    obj.x2 = obj.x1 + width_for_scale(scale);
    obj.y2 = obj.y1 + BASE_H * scale;
    result.spobj.push_back(obj);

    obj.type = SPOBJ_INTEGRAL_D;
    obj.x1 = (body_offset - width_for_scale(scale) - var.width) / 2;
    obj.y1 = den_offset - std::max(ascent_for_symbol(dot_font::LETTER_d, scale), var.ascent);
    obj.x2 = obj.x1 + width_for_scale(scale);
    obj.y2 = obj.y1 + BASE_H * scale;
    result.spobj.push_back(obj);

    obj.type = SPOBJ_FRAC_BAR;
    obj.x1 = FRAC_SIDE_MARGIN * scale;
    obj.x2 = obj.x1 + frac_width - FRAC_SIDE_MARGIN * scale * 2;
    for (int i = 0; i < scale; i++) {
        obj.y1 = i;
        obj.y2 = obj.y1;
        result.spobj.push_back(obj);
    }

    obj.x1 = body_offset + body.width + 1 * scale;
    obj.y1 = -result.ascent + 2 * scale;
    obj.x2 = obj.x1;
    obj.y2 = result.descent - 2 * scale;
    for (int i = 0; i < scale; i++) {
        obj.x1++;
        obj.x2 = obj.x1;
        result.spobj.push_back(obj);
    }

    result.merge(symbol);
    result.merge(body);
    result.merge(var);
    result.merge(x);

    return result;
}

LayoutBox LayoutEngine::parse_perm(int& pos, int scale) {
    LayoutBox head = parse_symbol(pos, scale);
    LayoutBox left = parse_group(pos, SMALL_SCALE);
    LayoutBox right = parse_group(pos, SMALL_SCALE);

    int left_offsetY = descent_for_symbol(dot_font::LETTER_P, scale) + left.ascent - SUB_OFFSET;
    int right_offsetY = descent_for_symbol(dot_font::LETTER_P, scale) + right.ascent - SUB_OFFSET;
    int symbol_offset = left.width;
    int right_offsetX = symbol_offset + width_for_scale(scale);
    left.shift(0, left_offsetY);
    right.shift(right_offsetX, right_offsetY);

    LayoutBox result;
    result.width = left.width + width_for_scale(scale) + right.width;
    result.ascent = ascent_for_symbol(dot_font::LETTER_P, scale);
    result.descent = std::max(left_offsetY + left.descent, right_offsetY + right.descent);

    SpecialObject obj;
    obj.type = SPOBJ_PERM_SYMBOL;
    obj.x1 = symbol_offset;
    obj.y1 = -ascent_for_scale(scale);
    obj.x2 = symbol_offset + width_for_scale(scale);
    obj.y2 = descent_for_scale(scale);
    result.spobj.push_back(obj);

    result.merge(head);
    result.merge(left);
    result.merge(right);
    return result;
}
LayoutBox LayoutEngine::parse_comb(int& pos, int scale) {
    LayoutBox head = parse_symbol(pos, scale);
    LayoutBox left = parse_group(pos, SMALL_SCALE);
    LayoutBox right = parse_group(pos, SMALL_SCALE);

    int left_offsetY = descent_for_symbol(dot_font::LETTER_C, scale) + left.ascent - SUB_OFFSET;
    int right_offsetY = descent_for_symbol(dot_font::LETTER_C, scale) + right.ascent - SUB_OFFSET;
    int symbol_offset = left.width;
    int right_offsetX = symbol_offset + width_for_scale(scale);
    left.shift(0, left_offsetY);
    right.shift(right_offsetX, right_offsetY);

    LayoutBox result;
    result.width = left.width + width_for_scale(scale) + right.width;
    result.ascent = ascent_for_symbol(dot_font::LETTER_C, scale);
    result.descent = std::max(left_offsetY + left.descent, right_offsetY + right.descent);

    SpecialObject obj;
    obj.type = SPOBJ_COMB_SYMBOL;
    obj.x1 = symbol_offset;
    obj.y1 = -ascent_for_scale(scale);
    obj.x2 = symbol_offset + width_for_scale(scale);
    obj.y2 = descent_for_scale(scale);
    result.spobj.push_back(obj);

    result.merge(head);
    result.merge(left);
    result.merge(right);
    return result;
}

LayoutBox LayoutEngine::parse_log(int& pos, int scale) {
    LayoutBox head = parse_symbol(pos, scale);
    LayoutBox base = parse_group(pos, SMALL_SCALE);
    LayoutBox body = parse_group(pos, scale);

    int base_offsetX = head.width;
    int base_offsetY = head.descent + base.ascent - SUB_OFFSET;
    int body_offsetX = head.width + base.width + width_for_scale(scale);

    base.shift(base_offsetX, base_offsetY);
    body.shift(body_offsetX, 0);

    LayoutBox result;
    result.width = head.width + base.width + width_for_scale(scale) + body.width + width_for_scale(scale);
    result.ascent = std::max(head.ascent, body.ascent);
    result.descent = base_offsetY + base.descent;

    SpecialObject obj;
    obj.type = SPOBJ_OPEN_BRACKET;
    obj.x1 = head.width + base.width;
    obj.y1 = -body.ascent;
    obj.x2 = obj.x1 + width_for_scale(scale);
    obj.y2 = body.descent;
    result.spobj.push_back(obj);

    obj.type = SPOBJ_CLOSE_BRACKET;
    obj.x1 = body_offsetX + body.width;
    obj.x2 = obj.x1 + width_for_scale(scale);
    result.spobj.push_back(obj);

    result.merge(head);
    result.merge(base);
    result.merge(body);
    return result;
}

int shift_for_cursor(LayoutBox& box, const std::vector<Token>& tokens, const int cursor, const int width,
                     const int height, int& offsetX, int& offsetY) {
    int x1 = box.width + offsetX;
    int y1 = std::max(box.ascent - ascent_for_scale(NORMAL_SCALE), 0) + offsetY;
    int x2 = x1 + BASE_W * NORMAL_SCALE;
    int y2 = y1 + BASE_H * NORMAL_SCALE;

    if (cursor < tokens.size()) {
        x1 = tokens[cursor].x;
        y1 = tokens[cursor].y;
        x2 = tokens[cursor].x + BASE_W * tokens[cursor].scale * std::min((int)tokens[cursor].m_letters.size(), 1);
        y2 = tokens[cursor].y + ascent_for_letters(tokens[cursor].m_letters, tokens[cursor].scale) +
             descent_for_letters(tokens[cursor].m_letters, tokens[cursor].scale);
    }

    if (0 <= x1 && x2 <= width && 0 <= y1 && y2 <= height) return 0;

    int dx = 0;
    int dy = 0;
    if (x1 < 0) dx = -x1;
    if (x2 > width) dx = width - x2;
    if (y1 < 0) dy = -y1;
    if (y2 > height) dy = height - y2;
    box.shift(dx, dy);

    offsetX += dx;
    offsetY += dy;

    return 0;
}

int ascent_for_symbol(int symbol, int scale) {
    (void)symbol;
    return ascent_for_scale(scale);
}
int descent_for_symbol(int symbol, int scale) {
    switch (symbol) {
        case dot_font::LETTER_g:
        case dot_font::LETTER_j:
        case dot_font::LETTER_p:
        case dot_font::LETTER_q:
        case dot_font::LETTER_y:
        case dot_font::LETTER_BETA:
        case dot_font::LETTER_GAMMA:
        case dot_font::LETTER_ZETA:
        case dot_font::LETTER_ETA:
        case dot_font::LETTER_MU:
        case dot_font::LETTER_XI:
        case dot_font::LETTER_RHO:
        case dot_font::LETTER_PHI:
        case dot_font::LETTER_CHI:
        case dot_font::LETTER_PSI:
        case dot_font::LETTER_SQRT:
        case dot_font::LETTER_OPEN_PARENTHESES:
        case dot_font::LETTER_CLOSE_PARENTHESES:
        case dot_font::LETTER_INTEGRAL:
            return descent_for_scale(scale);
    }
    return descent_for_scale(scale) - 4 * scale;
}
int width_for_symbol(int symbol, int scale) {
    (void)symbol;
    return width_for_scale(scale);
}

int ascent_for_letters(const std::vector<int>& letters, int scale) {
    int ascent = ascent_for_symbol(0, scale);
    for (int i = 0; i < letters.size(); i++) {
        ascent = std::max(ascent_for_symbol(letters[i], scale), ascent);
    }
    return ascent;
}
int descent_for_letters(const std::vector<int>& letters, int scale) {
    int descent = descent_for_symbol(0, scale);
    for (int i = 0; i < letters.size(); i++) {
        descent = std::max(descent_for_symbol(letters[i], scale), descent);
    }
    return descent;
}
int width_for_letters(const std::vector<int>& letters, int scale) {
    int width = 0;
    for (int i = 0; i < letters.size(); i++) {
        width += width_for_symbol(letters[i], scale);
    }
    return width;
}
}  // namespace math_renderer
