#include "../include/MathRenderer/math_renderer.hpp"
#include "../include/MathRenderer/bmp.hpp"

#include <stdio.h>
#include <chrono>
#include <iostream>

int main(void) {
    using namespace math_renderer;

    std::vector<Token> input;

    MathRenderer mr(400, 300);

    int cursor = 0;
    //process_input(input, cursor, "sf_sum");
    //process_input(input, cursor, "sf_frac");
    process_input(input, cursor, "sf_diff");
    process_input(input, cursor, "mf_sin");
    process_input(input, cursor, "x");
    process_input(input, cursor, "sf_frac");
    process_input(input, cursor, "2");
    process_input(input, cursor, "right");
    process_input(input, cursor, "right");
    process_input(input, cursor, "x");
    process_input(input, cursor, "right");
    process_input(input, cursor, "0");
    // process_input(input, cursor, "sf_sqrt");
    // process_input(input, cursor, "sf_sqrt");
    // process_input(input, cursor, "right");
    // process_input(input, cursor, "right");
    // process_input(input, cursor, "del");

    LayoutEngine le(input);
    LayoutBox box = le.layout();
    printf("-----\n");
    for (int i = 0; i < input.size(); i++) {
        printf("%s\tat %d, %d with scale %d", input[i].m_math_str.c_str(), input[i].x, input[i].y, input[i].scale);
        if (i == cursor) {
            printf("  <- here!");
        }
        printf("\n");
    }

    printf("start!\n");
    auto start = std::chrono::high_resolution_clock::now();
    int max_iter = 100;
    for (int i = 0; i < max_iter; i++) {
        le.layout();
        mr.clear();
        mr.draw_tokens(box.tokens);
        mr.draw_special_objects(box.spobj);
        printf(".");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    printf("\nduration: %lld micro sec.\n", duration.count() / max_iter);

    printf("convert input tokens to math string...\n");
    int index = 0;
    std::string math_str;
    make_math_str(input, index, math_str);
    printf("result:\n%s\n", math_str.c_str());

    mr.write2BMP("result.bmp");

    std::cout << "Press Enter to exit" << std::endl;
    std::cin.get();

    return 0;
}