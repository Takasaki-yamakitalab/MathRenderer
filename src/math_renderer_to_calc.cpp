#include "MathRenderer/math_renderer.hpp"
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
                index += 2;		// skip "\up" and "{"
                str += "((";
                make_math_str(tokens, index, str);
                index += 2;		// skip "}" and "{"
                str += ")^(";
                int size = str.size();
                make_math_str(tokens, index, str);
                index++;		// skip "}"
                if (size == str.size()) {
                    // erase ")^("
                    str.erase(str.size() - 3);
                }
                str += "))";
                continue;
            }
            if (tokens[index].type == TK_DOWN) {
                index += 2;		// skip "\down" and "{"
                make_math_str(tokens, index, str);
                index += 2;		// skip "}" and "{"
                str += "_";
                int size = str.size();
                make_math_str(tokens, index, str);
                index++;		// skip "}"
                if (size == str.size()) {
                    // erase '_'
                    str.erase(str.size() - 1);
                }
                continue;
            }
            if (tokens[index].type == TK_FRAC) {
                index += 2;		// skip "\frac" and "{"
                str += "((";
                make_math_str(tokens, index, str);
                index += 2;		// skip "}" and "{"
                str += ")/(";
                make_math_str(tokens, index, str);
                index++;		// skip "}"
                str += "))";
                continue;
            }
            if (tokens[index].type == TK_SQRT) {
                index += 2;		// skip "\sqrt" and "{"
                str += "@sqrt(";
                make_math_str(tokens, index, str);
                index++;		// skip "}"
                str += ")";
                continue;
            }
            if (tokens[index].type == TK_SUM ||
                tokens[index].type == TK_PROD ||
                tokens[index].type == TK_INT) {
                str += tokens[index++].m_math_str;
                index++;		// skip "{"
                str += "{";
                make_math_str(tokens, index, str);	// arg1 (body)
                index += 2;		// skip "}" and "{"
                str += "}{";
                make_math_str(tokens, index, str);	// arg2 (var name)
                index += 2;		// skip "}" and "{"
                str += "}{";
                make_math_str(tokens, index, str);	// arg3 (start)
                index += 2;		// skip "}" and "{"
                str += "}{";
                make_math_str(tokens, index, str);	// arg4 (end)
                index++;		// skip "}"
                str += "}";
                continue;
            }
            if (tokens[index].type == TK_DIFF) {
                str += tokens[index++].m_math_str;
                index++;		// skip "{"
                str += "{";
                make_math_str(tokens, index, str);	// arg1 (body)
                index += 2;		// skip "}" and "{"
                str += "}{";
                make_math_str(tokens, index, str);	// arg2 (var name)
                index += 2;		// skip "}" and "{"
                str += "}{";
                make_math_str(tokens, index, str);	// arg3 (start)
                index++;		// skip "}"
                str += "}";
                continue;
            }
            if (tokens[index].type == TK_PERM ||
                tokens[index].type == TK_COMB ||
                tokens[index].type == TK_LOG) {
                str += tokens[index++].m_math_str;
                index++;		// skip "{"
                str += "{";
                make_math_str(tokens, index, str);	// arg1
                index += 2;		// skip "}" and "{"
                str += "}{";
                make_math_str(tokens, index, str);	// arg2
                index++;		// skip "}"
                str += "}";
                continue;
            }
        }
        return 0;
    }
}