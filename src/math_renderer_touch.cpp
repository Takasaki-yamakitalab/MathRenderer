#include "MathRenderer/math_renderer.hpp"
#include "MathRenderer/math_renderer_defines.hpp"

namespace math_renderer {

inline int norm2(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

int move_cursor_touch(const std::vector<Token>& tokens, int& cursor, const int touchX, const int touchY) {
    if (tokens.size() == 0) return 0;
    cursor = 0;
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].x < 0 || tokens[i].y < 0) continue;
        // if (tokens[i].can_be_pointed == false) continue;
        if (tokens[i].type == TK_OPEN_BRACE || tokens[i].type == TK_OPEN_SQUARE || tokens[i].type == TK_CLOSE_SQUARE) continue;
        if (norm2(touchX, touchY, tokens[cursor].x, tokens[cursor].y + tokens[cursor].scale * BASE_H / 2) > norm2(touchX, touchY, tokens[i].x, tokens[i].y + tokens[i].scale * BASE_H / 2)) {
            cursor = i;
        }
    }
    return 0;
}
}  // namespace math_renderer
