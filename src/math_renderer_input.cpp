#include "MathRenderer/math_renderer.hpp"

namespace math_renderer {
Token make_token(int type, const std::string& name, const std::vector<std::string>& letters) {
    Token tok;
    tok.type = type;
    tok.m_math_str = name;
    for (int i = 0; i < letters.size(); i++) {
        int id = 0;
        for (id = 0; id < LETTER_NUM; id++) {
            if (LETTERS[id] == letters[i]) {
                tok.m_letters.push_back(id);
                break;
            }
        }
    }
    return tok;
}

Token make_token(int type, const std::string& name, const std::string& letter) {
    Token tok;
    tok.type = type;
    tok.m_math_str = name;
    int id = 0;
    for (id = 0; id < LETTER_NUM; id++) {
        if (LETTERS[id] == letter) break;
    }
    tok.m_letters.push_back(id);
    return tok;
}

Token make_token(int type, const std::string& name) {
    Token tok;
    tok.type = type;
    tok.m_math_str = name;
    int id = 0;
    for (id = 0; id < LETTER_NUM; id++) {
        if (LETTERS[id] == name) break;
    }
    if (name != "{" && name != "}" && name != "[" && name != "]" && name[0] != '\\')
        tok.m_letters.push_back(id);
    return tok;
}

inline std::vector<Token> make_const_down_tokens(
    const std::string& value, const std::vector<std::string>& base, const std::vector<std::string>& sub) {
    return {
        make_token(TK_DOWN, "\\down"),
        make_token(TK_OPEN_SQUARE, "["),
        make_token(TK_SYMBOL, value, base),
        make_token(TK_CLOSE_SQUARE, "]"),
        make_token(TK_OPEN_SQUARE, "["),
        make_token(TK_SYMBOL, "", sub),
        make_token(TK_CLOSE_SQUARE, "]")};
}
inline std::vector<Token> make_const_down_tokens(
    const std::string& value, const std::string& base, const std::vector<std::string>& sub) {
    return {
        make_token(TK_DOWN, "\\down"),
        make_token(TK_OPEN_SQUARE, "["),
        make_token(TK_SYMBOL, value, base),
        make_token(TK_CLOSE_SQUARE, "]"),
        make_token(TK_OPEN_SQUARE, "["),
        make_token(TK_SYMBOL, "", sub),
        make_token(TK_CLOSE_SQUARE, "]")};
}
inline std::vector<Token> make_const_down_tokens(
    const std::string& value, const std::vector<std::string>& base, const std::string& sub) {
    return {
        make_token(TK_DOWN, "\\down"),
        make_token(TK_OPEN_SQUARE, "["),
        make_token(TK_SYMBOL, value, base),
        make_token(TK_CLOSE_SQUARE, "]"),
        make_token(TK_OPEN_SQUARE, "["),
        make_token(TK_SYMBOL, "", sub),
        make_token(TK_CLOSE_SQUARE, "]")};
}
inline std::vector<Token> make_const_down_tokens(
    const std::string& value, const std::string& base, const std::string& sub) {
    return {
        make_token(TK_DOWN, "\\down"),
        make_token(TK_OPEN_SQUARE, "["),
        make_token(TK_SYMBOL, value, base),
        make_token(TK_CLOSE_SQUARE, "]"),
        make_token(TK_OPEN_SQUARE, "["),
        make_token(TK_SYMBOL, "", sub),
        make_token(TK_CLOSE_SQUARE, "]")};
}

static std::vector<Token> make_tokens_from_input(const std::string& input) {
    if (input.size() == 0) return {};

    if (input.size() == 1) return {make_token(TK_SYMBOL, input)};

    // ignore special functions
    if (input.find("sf_") == 0) return {};

    if (input.find("mf_") == 0) {
        if (input == "mf_sin") return {make_token(TK_SYMBOL, "@sin", {"s", "i", "n"})};
        if (input == "mf_cos") return {make_token(TK_SYMBOL, "@cos", {"c", "o", "s"})};
        if (input == "mf_tan") return {make_token(TK_SYMBOL, "@tan", {"t", "a", "n"})};
        if (input == "mf_asin") return {make_token(TK_SYMBOL, "@arcsin", {"a", "s", "i", "n"})};
        if (input == "mf_acos") return {make_token(TK_SYMBOL, "@arccos", {"a", "c", "o", "s"})};
        if (input == "mf_atan") return {make_token(TK_SYMBOL, "@arctan", {"a", "t", "a", "n"})};
        if (input == "mf_sinh") return {make_token(TK_SYMBOL, "@sinh", {"s", "i", "n", "h"})};
        if (input == "mf_cosh") return {make_token(TK_SYMBOL, "@cosh", {"c", "o", "s", "h"})};
        if (input == "mf_tanh") return {make_token(TK_SYMBOL, "@tanh", {"t", "a", "n", "h"})};
        if (input == "mf_asinh") return {make_token(TK_SYMBOL, "@arcsinh", {"a", "s", "i", "n", "h"})};
        if (input == "mf_acosh") return {make_token(TK_SYMBOL, "@arccosh", {"a", "c", "o", "s", "h"})};
        if (input == "mf_atanh") return {make_token(TK_SYMBOL, "@arctanh", {"a", "t", "a", "n", "h"})};

        if (input == "mf_exp") return {make_token(TK_SYMBOL, "@exp", {"e", "x", "p"})};
        if (input == "mf_log") return make_const_down_tokens("@log", std::vector<std::string>{"l", "o", "g"}, std::vector<std::string>{"1", "0"});
        if (input == "mf_ln") return make_const_down_tokens("@ln", {"l", "o", "g"}, "e");

        if (input == "mf_gamma") return {make_token(TK_SYMBOL, "@gamma", "Gamma")};

        return {};
    }

    if (input.find("const_") == 0) {
        // constants

        // mathematical constants
        if (input == "const_mt_pi") return {make_token(TK_SYMBOL, "(@arccos(-1))", "pi")};
        if (input == "const_mt_e") return {make_token(TK_SYMBOL, "(@exp(1))", "e")};

        // universal constants
        if (input == "const_uc_c_0") return make_const_down_tokens("(299792458)", "c", "0");
        if (input == "const_uc_mu_0") return make_const_down_tokens("(1.25663706127*10^(-6))", "mu", "0");
        if (input == "const_uc_epsilon_0") return make_const_down_tokens("(8.854187818*10^(-12))", "epsilon", "0");
        if (input == "const_uc_Z_0") return make_const_down_tokens("(376.730313412)", "Z", "0");
        if (input == "const_uc_G") return {make_token(TK_SYMBOL, "(6.67430*10^(-11))", "G")};
        if (input == "const_uc_h") return {make_token(TK_SYMBOL, "(6.62607015*10^(-34))", "h")};
        if (input == "const_uc_hbar") return {make_token(TK_SYMBOL, "(1.054571817*10^(-34))", "hbar")};

        // electromagnetic constants
        if (input == "const_em_e") return {make_token(TK_SYMBOL, "(1.602176634*10^(-19))", "e")};
        if (input == "const_em_Phi_0") return make_const_down_tokens("(1.602176634*10^(-19))", "Phi", "0");
        if (input == "const_em_G_0") return make_const_down_tokens("(7.7480917298637*10^(-5))", "G", "0");
        if (input == "const_em_R_0") return make_const_down_tokens("(12906.403729652)", "R", "0");
        if (input == "const_em_K_J") return make_const_down_tokens("(483597.8484*10^(9))", "K", "J");
        if (input == "const_em_R_K") return make_const_down_tokens("(25812.807459305)", "R", "K");
        if (input == "const_em_mu_B") return make_const_down_tokens("(9.2740100657*10^(-24))", "mu", "B");
        if (input == "const_em_mu_N") return make_const_down_tokens("(5.050783739*10^(-27))", "mu", "N");

        // atomic and nuclear physics constants
        if (input == "const_anp_alpha") return {make_token(TK_SYMBOL, "(7.297352564*10^(-3))", "alpha")};
        if (input == "const_anp_R_infty") return make_const_down_tokens("(10973731.568157)", "R", "infty");
        if (input == "const_anp_alpha_0") return make_const_down_tokens("(5.29177210544*10^(-11))", "alpha", "0");
        if (input == "const_anp_E_h") return make_const_down_tokens("(4.3597447222060*10^(-18))", "E", "h");

        if (input == "const_anp_m_e") return make_const_down_tokens("(9.1093837139*10^(-31))", "m", "e");
        if (input == "const_anp_lambda_e") return make_const_down_tokens("(2.42631023538*10^(-12))", "lambda", "e");
        if (input == "const_anp_r_e") return make_const_down_tokens("(2.8179403205*10^(-15))", "r", "e");
        if (input == "const_anp_sigma_e") return make_const_down_tokens("(6.6524587051*10^(-29))", "sigma", "e");
        if (input == "const_anp_mu_e") return make_const_down_tokens("(-9.2847646917*10^(-24))", "mu", "e");
        if (input == "const_anp_g_e") return make_const_down_tokens("(-2.00231930436092)", "g", "e");
        if (input == "const_anp_a_e") return make_const_down_tokens("(1.15965218046*10^(-3))", "a", "e");
        if (input == "const_anp_gamma_e") return make_const_down_tokens("(1.76085962784*10^(11))", "gamma", "e");

        if (input == "const_anp_m_p") return make_const_down_tokens("(1.67262192595*10^(-27))", "m", "p");
        if (input == "const_anp_lambda_p") return make_const_down_tokens("(1.32140985360*10^(-15))", "lambda", "p");
        if (input == "const_anp_mu_p") return make_const_down_tokens("(1.41060679545*10^(-26))", "mu", "p");
        if (input == "const_anp_g_p") return make_const_down_tokens("(5.5856946893)", "g", "p");
        if (input == "const_anp_gamma_p") return make_const_down_tokens("(2.6752215808*10^(8))", "gamma", "p");

        if (input == "const_anp_m_n") return make_const_down_tokens("(1.67492750056*10^(-27))", "m", "n");
        if (input == "const_anp_lambda_n") return make_const_down_tokens("(1.31959090382*10^(-15))", "lambda", "n");
        if (input == "const_anp_mu_n") return make_const_down_tokens("(-9.6623653*10^(-27))", "mu", "n");
        if (input == "const_anp_g_n") return make_const_down_tokens("(-3.82608552)", "g", "n");
        if (input == "const_anp_gamma_n") return make_const_down_tokens("(1.83247174*10^(8))", "gamma", "n");

        // physical and chemical constants
        if (input == "const_pc_k") return {make_token(TK_SYMBOL, "(1.380649*10^(-23))", "k")};
        if (input == "const_pc_N_A") return make_const_down_tokens("(6.02214076*10^(23))", "N", "A");
        if (input == "const_pc_F") return {make_token(TK_SYMBOL, "(96485.33212331)", "F")};
        if (input == "const_pc_R") return {make_token(TK_SYMBOL, "(8.3144626181532)", "R")};
        if (input == "const_pc_m_u") return make_const_down_tokens("(1.66053906892*10^(-27))", "m", "u");
        if (input == "const_pc_M_u") return make_const_down_tokens("(1.00000000105*10^(-3))", "M", "u");

        if (input == "const_pc_V_m") return make_const_down_tokens("(22.41396954*10^(-3))", "V", "m");
        if (input == "const_pc_n_0") return make_const_down_tokens("(2.686780111*10^(25))", "n", "0");

        if (input == "const_pc_sigma") return {make_token(TK_SYMBOL, "(5.6703744191844*10^(-8))", "sigma")};
        if (input == "const_pc_c_1") return make_const_down_tokens("(3.7417718521928*10^(-16))", "c", "1");
        if (input == "const_pc_c_1L") return make_const_down_tokens("(1.1910429723972*10^(-16))", "c", "1L");
        if (input == "const_pc_c_2") return make_const_down_tokens("(1.438776877*10^(-2))", "c", "2");
        if (input == "const_pc_b") return {make_token(TK_SYMBOL, "(2.897771955*10^(-3))", "b")};

        // agreed values
        if (input == "const_av_g") return {make_token(TK_SYMBOL, "(9.80665)", "g")};
        if (input == "const_av_1atm") return {make_token(TK_SYMBOL, "(101325)", {"1", " ", "a", "t", "m"})};

        return {};
    }

    if (input == "dot") return {make_token(TK_SYMBOL, ".")};

    if (input == "open_bracket") return {make_token(TK_OPEN_PARENTHESES, "(", "")};
    if (input == "close_bracket") return {make_token(TK_CLOSE_PARENTHESES, ")", "")};

    // operators
    if (input == "plus") return {make_token(TK_SYMBOL, "+")};
    if (input == "minus") return {make_token(TK_SYMBOL, "-")};
    if (input == "times") return {make_token(TK_SYMBOL, "*", "times")};
    if (input == "div") return {make_token(TK_SYMBOL, "/", "div")};
    if (input == "equal") return {make_token(TK_SYMBOL, "=")};
    if (input == "percent") return {make_token(TK_SYMBOL, "*(100)", "%")};
    if (input == "defeq") return {make_token(TK_SYMBOL, ":=")};
    if (input == "eqdef") return {make_token(TK_SYMBOL, "=:")};
    if (input == "lt") return {make_token(TK_SYMBOL, "<")};
    if (input == "gt") return {make_token(TK_SYMBOL, ">")};
    if (input == "le") return {make_token(TK_SYMBOL, "<=")};
    if (input == "ge") return {make_token(TK_SYMBOL, ">=")};
    if (input == "deg") return {make_token(TK_SYMBOL, "*(@arccos(-1)/180)", "deg")};

    // greek letters
    for (int i = LETTER_CAP_ALPHA; i <= LETTER_OMEGA; i++) {
        if (input == LETTERS[i]) return {make_token(TK_SYMBOL, input)};
    }

    if (input == "comma") return {make_token(TK_SYMBOL, ",")};
    if (input == "space") return {make_token(TK_SYMBOL, " ")};
    if (input == "ans") return {make_token(TK_SYMBOL, "ans", {"a", "n", "s"})};
    if (input == "preans") return {make_token(TK_SYMBOL, "preans", {"p", "r", "e", "a", "n", "s"})};

    return {};
}

inline Token make_special_function_head_token(const std::string& input) {
    if (input == "sf_up") return make_token(TK_UP, "\\up", "");
    if (input == "sf_down") return make_token(TK_DOWN, "\\down", "");
    if (input == "sf_frac") return make_token(TK_FRAC, "\\frac", "");
    if (input == "sf_sqrt") return make_token(TK_SQRT, "\\sqrt", "");
    if (input == "sf_sum") return make_token(TK_SUM, "\\sum", "Sigma");
    if (input == "sf_prod") return make_token(TK_PROD, "\\prod", "Pi");
    if (input == "sf_int") return make_token(TK_INT, "\\int", "");
    if (input == "sf_diff") return make_token(TK_DIFF, "\\diff", "");
    if (input == "sf_perm") return make_token(TK_PERM, "\\perm", "");
    if (input == "sf_comb") return make_token(TK_COMB, "\\comb", "");
    if (input == "sf_log") return make_token(TK_LOG, "\\log", {"l", "o", "g"});
    return Token();
}

inline std::vector<Token> make_special_function_tokens(const std::string& input) {
    if (input.find("sf_") != 0) return {};
    if (input == "sf_sqrt") {
        return {
            make_special_function_head_token(input),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", "")};
    }
    if (input == "sf_up" ||
        input == "sf_down" ||
        input == "sf_frac" ||
        input == "sf_perm" ||
        input == "sf_comb" ||
        input == "sf_log") {
        return {
            make_special_function_head_token(input),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", ""),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", "")};
    }
    if (input == "sf_diff") {
        return {
            make_special_function_head_token(input),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", ""),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", ""),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", "")};
    }
    if (input == "sf_sum" ||
        input == "sf_prod" ||
        input == "sf_int") {
        return {
            make_special_function_head_token(input),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", ""),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", ""),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", ""),
            make_token(TK_OPEN_BRACE, "{", ""),
            make_token(TK_CLOSE_BRACE, "}", "")};
    }
    return {};
}

int move_cursor_horizontal(const std::vector<Token>& tokens, int& cursor, int dir) {
    if (dir == 0) return 0;
    if (dir > 0) {
        cursor++;
        if (cursor > tokens.size()) {
            cursor = tokens.size();
            return 0;
        }
        while (cursor < tokens.size() && tokens[cursor].type == TK_OPEN_BRACE) {
            // the cursor must not be on '{'
            cursor++;
        }
        while (cursor < tokens.size() && tokens[cursor].type == TK_OPEN_SQUARE) {
            // the cursor must not be on '['
            // if then, move the cursor to the character following the ']'
            // (the cursor must not enter "[]")
            int cnt = 1;
            while (cnt != 0 && cursor < tokens.size() - 1) {
                cursor++;
                if (tokens[cursor].type == TK_OPEN_SQUARE) cnt++;
                if (tokens[cursor].type == TK_CLOSE_SQUARE) cnt--;
                if (cnt < 0) return -1;
            }
            // here, the cursor is on ']'
            cursor++;
        }
    } else if (dir < 0) {
        cursor--;
        if (cursor < 0) {
            cursor = 0;
            return 0;
        }
        while (0 < cursor && cursor < tokens.size() && tokens[cursor].type == TK_OPEN_BRACE) {
            // the cursor must not be on '{'
            cursor--;
        }
        while (0 < cursor && cursor < tokens.size() && tokens[cursor].type == TK_CLOSE_SQUARE) {
            // the cursor must not be on ']'
            // if then, move the cursor to the character before the '['
            // (the cursor must not enter "[]")
            int cnt = 0;
            while (0 < cursor && cursor < tokens.size()) {
                if (tokens[cursor].type == TK_CLOSE_SQUARE) cnt++;
                if (tokens[cursor].type == TK_OPEN_SQUARE) cnt--;
                if (cnt < 0) return -1;
                if (cnt == 0) break;
                cursor--;
            }
            // here, the cursor is on '['
            cursor--;
        }
    }
    if (cursor < 0) cursor = 0;
    if (tokens.size() < cursor) cursor = tokens.size();
    return 0;
}

int insert_input(std::vector<Token>& tokens, int& cursor, const std::string& input) {
    if (input.empty()) return -1;
    if (input.size() == 1 || input.find("sf_") != 0) {
        std::vector<Token> temp = make_tokens_from_input(input);
        if (temp.empty()) return -1;
        tokens.insert(tokens.begin() + cursor, temp.begin(), temp.end());
        cursor += temp.size();
        return 0;
    }

    // here, the input must be special function ("sf_***")
    std::vector<Token> spfunc_tokens = make_special_function_tokens(input);
    if (cursor == 0 ||
        input == "sf_sqrt" ||
        input == "sf_sum" ||
        input == "sf_prod" ||
        input == "sf_int" ||
        input == "sf_diff" ||
        input == "sf_log" ||
        tokens[cursor - 1].m_math_str == "+" ||
        tokens[cursor - 1].m_math_str == "-" ||
        tokens[cursor - 1].m_math_str == "*" ||
        tokens[cursor - 1].m_math_str == "/" ||
        tokens[cursor - 1].m_math_str[0] == '@' ||
        tokens[cursor - 1].type == TK_OPEN_PARENTHESES ||
        tokens[cursor - 1].type == TK_OPEN_BRACE ||
        tokens[cursor - 1].type == TK_OPEN_SQUARE) {
        // insert the header and empty "{}" directly at the cursor position.
        tokens.insert(tokens.begin() + cursor, spfunc_tokens.begin(), spfunc_tokens.end());
        // move the cursor to the first '}'
        cursor += 2;
        return 0;
    }

    int cur;
    for (cur = cursor - 1; cur > 0; cur--) {
        if (tokens[cur].type == TK_CLOSE_PARENTHESES) {
            // if the cursor is on ')', then the "(***)" is included in the first argument.
            int paren = 1;
            int brace = 0;
            for (; cur > 0; cur--) {
                if (tokens[cur].type == TK_CLOSE_PARENTHESES) paren++;
                if (tokens[cur].type == TK_OPEN_PARENTHESES) paren--;
                if (tokens[cur].type == TK_CLOSE_BRACE) brace++;
                if (tokens[cur].type == TK_OPEN_BRACE) brace--;
                if (paren == 0 && brace == 0) break;
                if (brace == 0 && tokens[cur - 1].type == TK_OPEN_BRACE) break;
            }
        }
        if (tokens[cur].type == TK_CLOSE_BRACE) {
            // if the cursor is on '}', then the "\***{***}{***}...{***}" is included in the first argument.
            int brace = 1;
            for (; cur > 0; cur--) {
                if (tokens[cur].type == TK_CLOSE_BRACE) brace++;
                if (tokens[cur].type == TK_OPEN_BRACE) brace--;
                if (brace == 0 && tokens[cur].m_math_str.size() > 1 && tokens[cur].m_math_str[0] == '\\') break;
            }
        }
        if (tokens[cur].type == TK_CLOSE_SQUARE) {
            // if the cursor is on ']', the the "\***[***][***]...[***]" is included in the first argument.
            int square = 1;
            for (; cur > 0; cur--) {
                if (tokens[cur].type == TK_CLOSE_SQUARE) square++;
                if (tokens[cur].type == TK_OPEN_SQUARE) square--;
                if (square == 0 && tokens[cur].m_math_str.size() > 1 && tokens[cur].m_math_str[0] == '\\') break;
            }
        }
        if (cur == 0 ||
            tokens[cur - 1].m_math_str == "+" ||
            tokens[cur - 1].m_math_str == "-" ||
            tokens[cur - 1].m_math_str == "*" ||
            tokens[cur - 1].m_math_str == "/" ||
            tokens[cur - 1].m_math_str[0] == '@' ||
            tokens[cur - 1].type == TK_OPEN_PARENTHESES ||
            tokens[cur - 1].type == TK_OPEN_BRACE ||
            tokens[cur - 1].type == TK_OPEN_SQUARE) {
            break;
        }
    }
    tokens.insert(tokens.begin() + cursor, spfunc_tokens.begin() + 2, spfunc_tokens.end());
    tokens.insert(tokens.begin() + cur, spfunc_tokens.begin(), spfunc_tokens.begin() + 2);
    cursor += 4;
    return 0;
}

int process_delete_key(std::vector<Token>& tokens, int& cursor) {
    if (cursor == 0) return 0;
    if (tokens[cursor - 1].type == TK_OPEN_BRACE) return 0;
    if (tokens[cursor - 1].type == TK_CLOSE_BRACE) {
        int brace = 0;
        int cur;
        for (cur = cursor - 1; cur > 0; cur--) {
            if (tokens[cur].type == TK_OPEN_BRACE) brace--;
            if (tokens[cur].type == TK_CLOSE_BRACE) brace++;
            if (brace == 0 && tokens[cur].m_math_str[0] == '\\') break;
        }
        tokens.erase(tokens.begin() + cur, tokens.begin() + cursor);
        cursor = cur;
        return 0;
    }
    if (tokens[cursor - 1].type == TK_CLOSE_SQUARE) {
        int square = 0;
        int cur;
        for (cur = cursor - 1; cur > 0; cur--) {
            if (tokens[cur].type == TK_OPEN_SQUARE) square--;
            if (tokens[cur].type == TK_CLOSE_SQUARE) square++;
            if (square == 0 && tokens[cur].m_math_str[0] == '\\') break;
        }
        tokens.erase(tokens.begin() + cur, tokens.begin() + cursor);
        cursor = cur;
        return 0;
    }
    cursor--;
    tokens.erase(tokens.begin() + cursor);
    return 0;
}

int process_input(std::vector<Token>& tokens, int& cursor, const std::string& input) {
    if (input == "ac") {
        tokens.clear();
        cursor = 0;
        return 0;
    }
    if (input == "equal") {
        return 1;
    }
    if (input == "plot") {
        return 2;
    }
    if (input == "expand") {
        return 3;
    }
    if (input == "left") {
        return move_cursor_horizontal(tokens, cursor, -1);
    }
    if (input == "right") {
        return move_cursor_horizontal(tokens, cursor, 1);
    }
    if (input == "del") {
        return process_delete_key(tokens, cursor);
    }
    return insert_input(tokens, cursor, input);
}
}  // namespace math_renderer
