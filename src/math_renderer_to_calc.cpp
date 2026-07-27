#include "MathRenderer/math_renderer.hpp"
#include "MathRenderer/math_renderer_defines.hpp"
#include <stdio.h>

namespace math_renderer {
int make_math_str(const std::vector<Token>& tokens, int& index, std::string& str) {
    while (index < tokens.size() && tokens[index].type != TK_CLOSE_BRACE && tokens[index].type != TK_CLOSE_SQUARE) {
        if (tokens[index].type == TK_OPEN_SQUARE ||
            tokens[index].type == TK_CLOSE_SQUARE) {
            index++;
            continue;
        }
        if (tokens[index].type == TK_SYMBOL ||
            tokens[index].type == TK_OPEN_PARENTHESES ||
            tokens[index].type == TK_CLOSE_PARENTHESES ||
            tokens[index].type == TK_OPEN_BRACE ||
            tokens[index].type == TK_CLOSE_BRACE) {
            str += tokens[index++].m_math_str;
            continue;
        }
        if (tokens[index].type == TK_UP) {
            index += 2;  // skip "\up" and "{"
            str += "((";
            make_math_str(tokens, index, str);
            index += 2;  // skip "}" and "{"
            str += ")^(";
            int size = str.size();
            make_math_str(tokens, index, str);
            index++;  // skip "}"
            if (size == str.size()) {
                // erase ")^("
                str.erase(str.size() - 3);
            }
            str += "))";
            continue;
        }
        if (tokens[index].type == TK_DOWN) {
            index += 2;  // skip "\down" and "{"
            make_math_str(tokens, index, str);
            index += 2;  // skip "}" and "{"
            str += "_";
            int size = str.size();
            make_math_str(tokens, index, str);
            index++;  // skip "}"
            if (size == str.size()) {
                // erase '_'
                str.erase(str.size() - 1);
            }
            continue;
        }
        if (tokens[index].type == TK_FRAC) {
            index += 2;  // skip "\frac" and "{"
            str += "((";
            make_math_str(tokens, index, str);
            index += 2;  // skip "}" and "{"
            str += ")/(";
            make_math_str(tokens, index, str);
            index++;  // skip "}"
            str += "))";
            continue;
        }
        if (tokens[index].type == TK_SQRT) {
            index += 2;  // skip "\sqrt" and "{"
            str += "@sqrt(";
            make_math_str(tokens, index, str);
            index++;  // skip "}"
            str += ")";
            continue;
        }
        if (tokens[index].type == TK_SUM ||
            tokens[index].type == TK_PROD ||
            tokens[index].type == TK_INT) {
            str += tokens[index++].m_math_str;
            index++;  // skip "{"
            str += "{";
            make_math_str(tokens, index, str);  // arg1 (body)
            index += 2;                         // skip "}" and "{"
            str += "}{";
            make_math_str(tokens, index, str);  // arg2 (var name)
            index += 2;                         // skip "}" and "{"
            str += "}{";
            make_math_str(tokens, index, str);  // arg3 (start)
            index += 2;                         // skip "}" and "{"
            str += "}{";
            make_math_str(tokens, index, str);  // arg4 (end)
            index++;                            // skip "}"
            str += "}";
            continue;
        }
        if (tokens[index].type == TK_DIFF) {
            str += tokens[index++].m_math_str;
            index++;  // skip "{"
            str += "{";
            make_math_str(tokens, index, str);  // arg1 (body)
            index += 2;                         // skip "}" and "{"
            str += "}{";
            make_math_str(tokens, index, str);  // arg2 (var name)
            index += 2;                         // skip "}" and "{"
            str += "}{";
            make_math_str(tokens, index, str);  // arg3 (start)
            index++;                            // skip "}"
            str += "}";
            continue;
        }
        if (tokens[index].type == TK_PERM ||
            tokens[index].type == TK_COMB ||
            tokens[index].type == TK_LOG) {
            str += tokens[index++].m_math_str;
            index++;  // skip "{"
            str += "{";
            make_math_str(tokens, index, str);  // arg1
            index += 2;                         // skip "}" and "{"
            str += "}{";
            make_math_str(tokens, index, str);  // arg2
            index++;                            // skip "}"
            str += "}";
            continue;
        }
    }
    return 0;
}

static int is_bracket(const char c) {
    if (c == '(' || c == '{') return 1;
    if (c == ')' || c == '}') return -1;
    return 0;
}
static std::string rm_brackets_once(std::string& src) {
    if (!is_bracket(src[0]) || !is_bracket(src[src.size() - 1])) return src;
    int brackets = 0;
    for (size_t i = 0; i < src.size(); i++) {
        brackets += is_bracket(src[i]);
        if (brackets == 0 && i != src.size() - 1) return src;
    }
    if (brackets > 0) return src;
    if (brackets < 0) {
        fprintf(stderr, "[ERROR] Improper brackets.\n");
        return std::string();
    }
    src.erase(0, 1);
    src.erase(src.size() - 1, 1);
    return src;
}
static std::string rm_brackets(std::string& src) {
    size_t size = src.size();
    while (1) {
        src = rm_brackets_once(src);
        if (size == src.size()) break;
        size = src.size();
    }
    return src;
}
static std::string split_by_ope(std::string& formula, const char* ope) {
    if (formula.find(ope) == std::string::npos) return std::string();
    int brackets = 0;
    size_t index = 0;
    for (index = formula.size() - 1; index >= 0; index--) {
        if (brackets == 0 && strncmp(formula.c_str() + index, ope, strlen(ope)) == 0)
            break;
        brackets -= is_bracket(formula[index]);
        if (index == 0) {
            return std::string();
        }
    }
    std::string right(formula.substr(index + strlen(ope)));
    formula.erase(index);
    return right;
}
static std::vector<Token> return_with_bracket(std::vector<Token>& tokens, int need_bracket) {
    if (need_bracket) {
        tokens.insert(tokens.begin(), make_token(TK_OPEN_PARENTHESES, "(", ""));
        tokens.push_back(make_token(TK_CLOSE_PARENTHESES, ")", ""));
    }
    return tokens;
}
static std::vector<Token> parse_numbers(std::string& formula) {
    int index = 0;
    int period = 0;
    for (index = 0; index < formula.size(); index++) {
        if (formula[index] == '.') {
            if (period) break;
            period++;
            continue;
        }
        if (formula[index] == '0' || formula[index] == '1' ||
            formula[index] == '2' || formula[index] == '3' ||
            formula[index] == '4' || formula[index] == '5' ||
            formula[index] == '6' || formula[index] == '7' ||
            formula[index] == '8' || formula[index] == '9') {
            continue;
        }
        break;
    }
    std::string right(formula.substr(index));
    formula.erase(index);
    std::vector<Token> ret;
    for (auto& c : formula) {
        ret.push_back(make_token(TK_SYMBOL, {c}));
    }
    std::vector<Token> temp = make_tokens(right);
    ret.insert(ret.end(), temp.begin(), temp.end());
    return ret;
}
static std::vector<Token> parse_math_function(std::string& formula) {
    const std::vector<std::pair<std::string, std::vector<std::string>>> MATH_FUNC_LIST = {
        { "@arcsinh",   { "a", "s", "i", "n", "h" } },
        { "@arccosh",   { "a", "c", "o", "s", "h" } },
        { "@arctanh",   { "a", "t", "a", "n", "h" } },
        { "@arcsin",    { "a", "s", "i", "n" } },
        { "@arccos",    { "a", "c", "o", "s" } },
        { "@arctan",    { "a", "t", "a", "n" } },
        { "@gamma",     { "Gamma" } },
        { "@sinh",      { "s", "i", "n", "h" } },
        { "@cosh",      { "c", "o", "s", "h" } },
        { "@tanh",      { "t", "a", "n", "h" } },
        { "@sin",       { "s", "i", "n" } },
        { "@cos",       { "c", "o", "s" } },
        { "@tan",       { "t", "a", "n" } },
        { "@exp",       { "e", "x", "p" }},
        { "@log",       { "l", "o", "g" }},
        { "@ln",        { "l", "n" }},
    };

    std::vector<Token> ret;
    for (size_t i = 0; i < MATH_FUNC_LIST.size(); i++) {
        if (formula.find(MATH_FUNC_LIST[i].first) == 0) {
            ret.push_back(make_token(TK_SYMBOL, MATH_FUNC_LIST[i].first, MATH_FUNC_LIST[i].second));
            formula = formula.substr(MATH_FUNC_LIST[i].first.length());
            break;
        }
    }
    std::vector<Token> temp = make_tokens(formula);
    ret.insert(ret.end(), temp.begin(), temp.end());
    return ret;
}
static std::vector<Token> parse_special_function(std::string& formula) {
    const std::vector<std::pair<std::string, std::pair<int, std::vector<std::string>>>> SPECIAL_FUNC_LIST = {
        { "\\prod", { TK_PROD, { "Pi" } }},
        { "\\perm", { TK_PERM, { "" } }},
        { "\\comb", { TK_COMB, { "" } }},
        { "\\diff", { TK_DIFF, { "" } }},
        { "\\frac", { TK_FRAC, { "" } }},
        { "\\sum",  { TK_SUM, { "Sigma" } }},
        { "\\int",  { TK_INT, { "" } }},
        { "\\log",  { TK_LOG, { "l", "o", "g" } }},
    };

    std::vector<Token> ret;
    for (size_t i = 0; i < SPECIAL_FUNC_LIST.size(); i++) {
        if (formula.find(SPECIAL_FUNC_LIST[i].first) == 0) {
            ret.push_back(make_token(SPECIAL_FUNC_LIST[i].second.first, SPECIAL_FUNC_LIST[i].first, SPECIAL_FUNC_LIST[i].second.second));
            formula.erase(SPECIAL_FUNC_LIST[i].first.length());
        }
    }
    while (formula.find("{") == 0) {
        int brace = 0;
        std::string term;
        for (size_t index = 0; index < formula.length(); index++) {
            if (formula[index] == '{') brace++;
            if (formula[index] == '}') brace--;
            if (brace == 0) {
                term = formula.substr(1, index - 1);
                formula = formula.substr(index + 1);
                break;
            }
        }
        ret.push_back(make_token(TK_OPEN_BRACE, "{", ""));
        std::vector<Token> temp = make_tokens(term);
        ret.insert(ret.end(), temp.begin(), temp.end());
        ret.push_back(make_token(TK_CLOSE_BRACE, "}", ""));
    }
    return ret;
}
static std::string find_greek(const std::string& str) {
    const std::vector<std::string> GREEK_LIST(&dot_font::LETTERS[dot_font::LETTER_CAP_ALPHA], &dot_font::LETTERS[dot_font::LETTER_OMEGA + 1]);
    if (str[0] != '~') return "";
    for (auto& greek : GREEK_LIST) {
        if (str.find(greek) == 1) {
            std::string ret = str.substr(1, greek.length());
            return ret;
        }
    }
    return "";
}
std::vector<Token> make_tokens(std::string& math_str) {
    std::string left = math_str;
    std::string right;
    int need_bracket = 0;

    left = rm_brackets(left);
    if (math_str != left) need_bracket = 1;
    if (left.size() == 0) return {};

    std::vector<std::string> operators = {":=", "=:", "+", "-"};
    for (auto& ope : operators) {
        if ((right = split_by_ope(left, ope.c_str())).size() != 0) {
            std::vector<Token> ret = make_tokens(left);
            std::vector<Token> temp = make_tokens(right);
            ret.push_back(make_token(TK_SYMBOL, ope));
            ret.insert(ret.end(), temp.begin(), temp.end());
            return return_with_bracket(ret, need_bracket);
        }
    }

    if ((right = split_by_ope(left, "*")).size() != 0) {
        std::vector<Token> ret = make_tokens(left);
        std::vector<Token> temp = make_tokens(right);
        ret.push_back(make_token(TK_SYMBOL, "*", "times"));
        ret.insert(ret.end(), temp.begin(), temp.end());
        return return_with_bracket(ret, need_bracket);
    }

    if ((right = split_by_ope(left, "/")).size() != 0) {
        std::vector<Token> ret = {
            make_token(TK_FRAC, "\\frac", ""),
            make_token(TK_OPEN_BRACE, "{", "")};
        left = rm_brackets(left);
        std::vector<Token> temp = make_tokens(left);
        ret.insert(ret.end(), temp.begin(), temp.end());
        ret.push_back(make_token(TK_CLOSE_BRACE, "}", ""));
        ret.push_back(make_token(TK_OPEN_BRACE, "{", ""));
        right = rm_brackets(right);
        temp = make_tokens(right);
        ret.insert(ret.end(), temp.begin(), temp.end());
        ret.push_back(make_token(TK_CLOSE_BRACE, "}", ""));
        return return_with_bracket(ret, need_bracket);
    }

    if ((right = split_by_ope(left, "^")).size() != 0) {
        std::vector<Token> ret = {
            make_token(TK_UP, "\\up", ""),
            make_token(TK_OPEN_BRACE, "{", "")};
        std::vector<Token> temp = make_tokens(left);
        ret.insert(ret.end(), temp.begin(), temp.end());
        ret.push_back(make_token(TK_CLOSE_BRACE, "}", ""));
        ret.push_back(make_token(TK_OPEN_BRACE, "{", ""));
        right = rm_brackets(right);
        temp = make_tokens(right);
        ret.insert(ret.end(), temp.begin(), temp.end());
        ret.push_back(make_token(TK_CLOSE_BRACE, "}", ""));
        return return_with_bracket(ret, need_bracket);
    }

    if ((right = split_by_ope(left, "_")).size() != 0) {
        std::vector<Token> ret = {
            make_token(TK_DOWN, "\\down", ""),
            make_token(TK_OPEN_BRACE, "{", "")};
        std::vector<Token> temp = make_tokens(left);
        ret.insert(ret.end(), temp.begin(), temp.end());
        ret.push_back(make_token(TK_CLOSE_BRACE, "}", ""));
        ret.push_back(make_token(TK_OPEN_BRACE, "{", ""));
        right = rm_brackets(right);
        temp = make_tokens(right);
        ret.insert(ret.end(), temp.begin(), temp.end());
        ret.push_back(make_token(TK_CLOSE_BRACE, "}", ""));
        return return_with_bracket(ret, need_bracket);
    }

    std::vector<Token> ret;
    switch (left[0]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            ret = parse_numbers(left);
            return return_with_bracket(ret, need_bracket);
            break;
        case '\\':
            return return_with_bracket(ret, need_bracket);
            break;
        case '@':
            ret = parse_math_function(left);
            return return_with_bracket(ret, need_bracket);
            break;
        default:
            break;
    }

    for (size_t i = 0; i < left.length(); i++) {
        char c = left[i];
        if (c != '~') {
            ret.push_back(make_token(TK_SYMBOL, {c}));
            continue;
        }
        std::string str = find_greek(left.substr(i));
        if (str == "") continue;
        ret.push_back(make_token(TK_SYMBOL, "~" + str, str));
        i += str.size();
    }
    return return_with_bracket(ret, need_bracket);
}
}  // namespace math_renderer
