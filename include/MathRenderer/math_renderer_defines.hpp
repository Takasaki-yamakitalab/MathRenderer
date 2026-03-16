#ifndef MATH_RENDERER_DEFINES_HPP
#define MATH_RENDERER_DEFINES_HPP

#include <string>
#include <vector>
#include <memory>

namespace math_renderer {
    enum LETTER_ID {
        LETTER_0,
        LETTER_1,
        LETTER_2,
        LETTER_3,
        LETTER_4,
        LETTER_5,
        LETTER_6,
        LETTER_7,
        LETTER_8,
        LETTER_9,

        LETTER_UNDERSCORE,
        LETTER_HAT,
        LETTER_PLUS,
        LETTER_MINUS,
        LETTER_TIMES,
        LETTER_DIV,
        LETTER_EQUAL,
        LETTER_PERCENT,
        LETTER_SEMICOLON,
        LETTER_COLON,
        LETTER_LT,          // <
        LETTER_GT,          // >
        LETTER_LE,          // <=
        LETTER_GE,          // >=
        LETTER_DEFEQ,       // :=
        LETTER_EQDEF,       // =:
        LETTER_DEGREE,
        LETTER_QUOTE,
        LETTER_DQUOTE,
        LETTER_BAR,
        LETTER_NORM,
        LETTER_INFTY,

        LETTER_A,
        LETTER_B,
        LETTER_C,
        LETTER_D,
        LETTER_E,
        LETTER_F,
        LETTER_G,
        LETTER_H,
        LETTER_I,
        LETTER_J,
        LETTER_K,
        LETTER_L,
        LETTER_M,
        LETTER_N,
        LETTER_O,
        LETTER_P,
        LETTER_Q,
        LETTER_R,
        LETTER_S,
        LETTER_T,
        LETTER_U,
        LETTER_V,
        LETTER_W,
        LETTER_X,
        LETTER_Y,
        LETTER_Z,

        LETTER_a,
        LETTER_b,
        LETTER_c,
        LETTER_d,
        LETTER_e,
        LETTER_f,
        LETTER_g,
        LETTER_h,
        LETTER_i,
        LETTER_j,
        LETTER_k,
        LETTER_l,
        LETTER_m,
        LETTER_n,
        LETTER_o,
        LETTER_p,
        LETTER_q,
        LETTER_r,
        LETTER_s,
        LETTER_t,
        LETTER_u,
        LETTER_v,
        LETTER_w,
        LETTER_x,
        LETTER_y,
        LETTER_z,

        LETTER_EXCLAMATION,         // !
        LETTER_COMMA,               // ,
        LETTER_PERIOD,              // .
        LETTER_QUESTION,            // ?
        LETTER_OPEN_PARENTHESES,    // (
        LETTER_CLOSE_PARENTHESES,   // )
        LETTER_OPEN_BRACES,         // {
        LETTER_CLOSE_BRACES,        // }
        LETTER_OPEN_SQUARE,         // [
        LETTER_CLOSE_SQUARE,        // ]
        LETTER_SLASH,
        LETTER_BACKSLASH,

        LETTER_CAP_ALPHA,
        LETTER_CAP_BETA,
        LETTER_CAP_GAMMA,
        LETTER_CAP_DELTA,
        LETTER_CAP_EPSILON,
        LETTER_CAP_ZETA,
        LETTER_CAP_ETA,
        LETTER_CAP_THETA,
        LETTER_CAP_IOTA,
        LETTER_CAP_KAPPA,
        LETTER_CAP_LAMBDA,
        LETTER_CAP_MU,
        LETTER_CAP_NU,
        LETTER_CAP_XI,
        LETTER_CAP_OMICRON,
        LETTER_CAP_PI,
        LETTER_CAP_RHO,
        LETTER_CAP_SIGMA,
        LETTER_CAP_TAU,
        LETTER_CAP_UPSILON,
        LETTER_CAP_PHI,
        LETTER_CAP_CHI,
        LETTER_CAP_PSI,
        LETTER_CAP_OMEGA,

        LETTER_ALPHA,
        LETTER_BETA,
        LETTER_GAMMA,
        LETTER_DELTA,
        LETTER_EPSILON,
        LETTER_ZETA,
        LETTER_ETA,
        LETTER_THETA,
        LETTER_IOTA,
        LETTER_KAPPA,
        LETTER_LAMBDA,
        LETTER_MU,
        LETTER_NU,
        LETTER_XI,
        LETTER_OMICRON,
        LETTER_PI,
        LETTER_RHO,
        LETTER_SIGMA,
        LETTER_TAU,
        LETTER_UPSILON,
        LETTER_PHI,
        LETTER_CHI,
        LETTER_PSI,
        LETTER_OMEGA,

        LETTER_SQRT,

        LETTER_OPEN_BRACKET,
        LETTER_CLOSE_BRACKET,

        LETTER_INTEGRAL,

        LETTER_SPACE,

        LETTER_NUM
    };

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

    constexpr int BASE_W = 11;
    constexpr int BASE_H = 23;
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

    const std::string LETTERS[]{
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "_", "^", "+", "-", "times", "div", "=", "%", ";", ":",
        "<", ">", "<=", ">=", ":=", "=:", "deg", "'", "\"", "|", "||", "infty",
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
        "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
        "!", ",", ".", "?", "(", ")", "{", "}", "[", "]", "/", "\\",
        "Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta", "Eta", "Theta", "Iota", "Kappa", "Lambda", "Mu",
        "Nu", "Xi", "Omicron", "Pi", "Rho", "Sigma", "Tau", "Upsilon", "Phi", "Chi", "Psi", "Omega",
        "alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta", "iota", "kappa", "lambda", "mu",
        "nu", "xi", "omicron", "pi", "rho", "sigma", "tau", "upsilon", "phi", "chi", "psi", "omega",
        "sqrt", "(", ")", "integral",
        " "
    };

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
