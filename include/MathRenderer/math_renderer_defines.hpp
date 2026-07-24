#ifndef MATH_RENDERER_DEFINES_HPP
#define MATH_RENDERER_DEFINES_HPP

#include <string>
#include <vector>
#include <memory>
#include <DotFont/dot_font.hpp>

namespace math_renderer {

enum TOKEN_TYPE {
    TK_SYMBOL,
    TK_OPEN_PARENTHESES,    // (
    TK_CLOSE_PARENTHESES,   // )
    TK_OPEN_BRACE,          // {
    TK_CLOSE_BRACE,         // }
    TK_OPEN_SQUARE,         // [
    TK_CLOSE_SQUARE,        // ]
    TK_UP,
    TK_DOWN,
    TK_FRAC,
    TK_SQRT,
    TK_SUM,
    TK_PROD,
    TK_INT,
    TK_DIFF,
    TK_PERM,
    TK_COMB,
    TK_LOG,
};
enum SPOBJ_TYPE {
    SPOBJ_EMPTY,
    SPOBJ_FRAC_BAR,
    SPOBJ_SQRT_SYMBOL,
    SPOBJ_SQRT_BAR,
    SPOBJ_EQUAL,
    SPOBJ_OPEN_BRACKET,
    SPOBJ_CLOSE_BRACKET,
    SPOBJ_INTEGRAL_SYMBOL,
    SPOBJ_INTEGRAL_D,
    SPOBJ_PERM_SYMBOL,
    SPOBJ_COMB_SYMBOL
};

constexpr int BASE_W = dot_font::BASE_W;
constexpr int BASE_H = dot_font::BASE_H;
constexpr int NORMAL_SCALE = 2;
constexpr int SMALL_SCALE = 1;
constexpr int NORMAL_W = BASE_W * NORMAL_SCALE;
constexpr int NORMAL_H = BASE_H * NORMAL_SCALE;
constexpr int SMALL_W = BASE_W * SMALL_SCALE;
constexpr int SMALL_H = BASE_H * SMALL_SCALE;
constexpr int SUPER_OFFSET = 14 * SMALL_SCALE;
constexpr int SUB_OFFSET = 20 * SMALL_SCALE;
constexpr int FRAC_GAP = 2;
constexpr int ROOT_SYMBOL_WIDTH = BASE_W;
constexpr int FRAC_SIDE_PADDING = 3;
constexpr int FRAC_SIDE_MARGIN = 1;
constexpr int ROOT_RIGHT_PADDING = 2;
constexpr int ROOT_LEFT_PADDING = 2;
constexpr int ROOT_TOP_PADDING = 2;
constexpr int SUM_PROD_SUBSCRIPT_OFFSET = 4;

struct Letter {
    int m_id;
    std::string m_name;
    Letter(int i, const std::string& n) : m_id(i), m_name(n) {}
};

struct Token {
    int type = 0;
    std::string m_math_str = "";
    std::vector<int> m_letters = {};
    int x = -10000;
    int y = -10000;
    int scale = 0;
    bool can_be_pointed = true;
    Token() {}
    Token(int ty) : type(ty) {}
};

struct SpecialObject {
    int type = 0;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
};

struct LayoutBox {
    int width = 0;
    int ascent = 0;
    int descent = 0;
    std::vector<Token*> tokens = {};
    std::vector<SpecialObject> spobj = {};

    void shift(int dx, int dy) {
        for (auto* t : tokens) {
            t->x += dx;
            t->y += dy;
        }
        for (auto& t : spobj) {
            t.x1 += dx;
            t.y1 += dy;
            t.x2 += dx;
            t.y2 += dy;
        }
    }
    void merge(const LayoutBox& other) {
        if (other.tokens.size() != 0)
            tokens.insert(tokens.end(), other.tokens.begin(), other.tokens.end());
        if (other.spobj.size() != 0)
            spobj.insert(spobj.end(), other.spobj.begin(), other.spobj.end());
    }
};

inline int width_for_scale(int scale) {
    return BASE_W * scale;
}
inline int ascent_for_scale(int scale) {
    return 9 * scale;
}
inline int descent_for_scale(int scale) {
    return (BASE_H - 9) * scale;
}
}

#endif
