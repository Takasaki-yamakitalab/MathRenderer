#ifndef BMP_HPP
#define BMP_HPP

#include <vector>
#include <string>

namespace bmp {
#pragma pack(push, 1)
    struct BMPFileHeader {
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    };
    struct BMPInfoHeader {
        uint32_t biSize;
        int32_t biWidth;
        int32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitConunt;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t biXPelsPerMeter;
        int32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };
#pragma pack(pop)
    struct Image {
        int width;
        int height;
        std::vector<uint8_t> data;
    };
    struct Glyph {
        int width;
        int height;
        std::vector<std::vector<bool>> pixels;
    };

#ifdef __ANDROID__
    constexpr uint32_t DEFAULT_FORWARD_COLOR = 0xFFFFBF00;
#else
    constexpr uint32_t DEFAULT_FORWARD_COLOR = 0xFF00BFFF;
#endif

    constexpr uint32_t DEFAULT_BACKWARD_COLOR = 0x00000000;

    Image loadBMP1bit(const std::string& filename);
    Image loadBMP1bit(const char* data, size_t size);
    bool getPixel(const Image& img, int x, int y);
    Glyph extractGlyph(const Image& img, int index, int glyphWidth, int glyphHeight);

    void drawGlyph(uint32_t* framebuffer, int fbWidth, int fbHeight, int posX, int posY, const Glyph& g, int scale, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawExtendGlyph(uint32_t* framebuffer, int fbWidth, int fbHeight, int posX, int posY, const Glyph& g, int dstWidth, int dstHeight, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawLine(uint32_t* framebuffer, int fbWidth, int fbHeight, int x1, int y1, int x2, int y2, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawLine(uint32_t* framebuffer, int fbWidth, int fbHeight, int x1, int y1, int x2, int y2, int weight, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawRectangle(uint32_t* framebuffer, int fbWidth, int fbHeight, int x1, int y1, int x2, int y2, bool fill, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawCircle(uint32_t* framebuffer, int fbWidth, int fbHeight, int x, int y, int r, bool fill, uint32_t color = DEFAULT_FORWARD_COLOR);

    void drawGlyph(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int posX, int posY, const Glyph& g, int scale, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawExtendGlyph(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int posX, int posY, const Glyph& g, int dstWidth, int dstHeight, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawLine(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int x1, int y1, int x2, int y2, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawLine(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int x1, int y1, int x2, int y2, int weight, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawRectangle(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int x1, int y1, int x2, int y2, bool fill, uint32_t color = DEFAULT_FORWARD_COLOR);
    void drawCircle(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int x, int y, int r, bool fill, uint32_t color = DEFAULT_FORWARD_COLOR);

    void save32bitBMP(const std::string& fname, int width, int height, const uint32_t* framebuffer);
}
#endif
