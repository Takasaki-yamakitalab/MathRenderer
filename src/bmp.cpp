#include "MathRenderer/bmp.hpp"
#include <fstream>
#include <stdio.h>

namespace bmp {
    Image loadBMP1bit(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        Image img = { -1, -1, std::vector<uint8_t>(0) };
        if (!file) {
            fprintf(stderr, "[ERROR] Failed to open file `%s`.\n", filename.c_str());
            return img;
        }
        BMPFileHeader fileHeader;
        BMPInfoHeader infoHeader;
        file.read((char*)&fileHeader, sizeof(fileHeader));
        file.read((char*)&infoHeader, sizeof(infoHeader));
        if (fileHeader.bfType != 0x4D42) {
            fprintf(stderr, "[ERROR] File '%s' is not BMP.\n", filename.c_str());
            return img;
        }
        if (infoHeader.biBitConunt != 1) {
            fprintf(stderr, "[ERROR] Only 1bit BMP supported.\n");
            return img;
        }

        img.width = infoHeader.biWidth;
        img.height = infoHeader.biHeight;
        size_t rowSize = ((img.width + 31) / 32) * 4;
        img.data.resize(rowSize * img.height);

        file.seekg(fileHeader.bfOffBits, std::ios::beg);
        file.read((char*)img.data.data(), img.data.size());

        file.close();

        return img;
    }

    Image loadBMP1bit(const char* data, size_t size) {
        Image img = { -1, -1, std::vector<uint8_t>(0) };
        if (data == NULL) return img;
        BMPFileHeader fh;
        BMPInfoHeader ih;
        memcpy(&fh, data, sizeof(fh));
        memcpy(&ih, data + sizeof(fh), sizeof(ih));
        if (fh.bfType != 0x4D42) {
            return img;
        }
        if (ih.biBitConunt != 1) {
            return img;
        }
        img.width = ih.biWidth;
        img.height = ih.biHeight;
        size_t rowSize = ((img.width + 31) / 32) * 4;
        img.data.resize(rowSize * img.height);
        memcpy(img.data.data(), data + fh.bfOffBits, img.data.size());
        return img;
    }

    bool getPixel(const Image& img, int x, int y) {
        int rowSize = ((img.width + 31) / 32) * 4;
        int realY = img.height - 1 - y;
        int byteIndex = realY * rowSize + x / 8;
        int bitIndex = 7 - (x % 8);
        return (img.data[byteIndex] >> bitIndex) & 1;
    }

    Glyph extractGlyph(const Image& img, int index, int glyphWidth, int glyphHeight) {
        Glyph g = {
            glyphWidth,
            glyphHeight,
            std::vector<std::vector<bool>>(glyphHeight, std::vector<bool>(glyphWidth, false))
        };
        g.height = glyphHeight;
        g.width = glyphWidth;
        int col = index % 32;
        int row = index / 32;
        int startX = col * glyphWidth;
        int startY = row * glyphHeight;
        for (int y = 0; y < glyphHeight; y++) {
            for (int x = 0; x < glyphWidth; x++) {
                g.pixels[y][x] = !getPixel(img, startX + x, startY + y);
            }
        }
        return g;
    }

    void drawGlyph(uint32_t* framebuffer, int fbWidth, int fbHeight, int posX, int posY, const Glyph& g, int scale, uint32_t color) {
        if (fbWidth < posX || fbHeight < posY || posX + g.width * scale < 0 || posY + g.height * scale < 0)
            return;

        for (int y = 0; y < g.height; y++) {
            for (int x = 0; x < g.width; x++) {
                if (!g.pixels[y][x]) continue;

                for (int dy = 0; dy < scale; dy++) {
                    for (int dx = 0; dx < scale; dx++) {
                        int px = posX + x * scale + dx;
                        int py = posY + y * scale + dy;
                        if (0 <= px && px < fbWidth && 0 <= py && py < fbHeight)
                            framebuffer[py * fbWidth + px] = color;
                    }
                }
            }
        }
    }

    void drawExtendGlyph(uint32_t* framebuffer, int fbWidth, int fbHeight, int posX, int posY, const Glyph& g, int dstWidth, int dstHeight, uint32_t color) {
        if (dstWidth <= 0 || dstHeight <= 0) return;
        if (g.width <= 0 || g.height <= 0) return;

        if (fbWidth < posX || fbHeight < posY || posX + dstWidth < 0 || posY + dstHeight < 0)
            return;

        for (int dy = 0; dy < dstHeight; dy++) {
            int srcY = dy * g.height / dstHeight;
            for (int dx = 0; dx < dstWidth; dx++) {
                int srcX = dx * g.width / dstWidth;
                if (!g.pixels[srcY][srcX]) continue;
                int px = posX + dx;
                int py = posY + dy;
                if (0 <= px && px < fbWidth &&
                    0 <= py && py < fbHeight)
                    framebuffer[py * fbWidth + px] = color;
            }
        }
    }

    void drawLine(uint32_t* framebuffer, int fbWidth, int fbHeight, int x1, int y1, int x2, int y2, uint32_t color) {
        if (fbWidth < std::min(x1, x2) || fbHeight < std::min(y1, y2) || std::max(x1, x2) < 0 || std::max(y1, y2) < 0)
            return;

        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        int x = x1;
        int y = y1;
        while (true) {
            if (0 <= x && x < fbWidth &&
                0 <= y && y < fbHeight)
                framebuffer[y * fbWidth + x] = color;
            if (x == x2 && y == y2) break;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }

    void drawLine(uint32_t* framebuffer, int fbWidth, int fbHeight, int x1, int y1, int x2, int y2, int weight, uint32_t color) {
        if (fbWidth < std::min(x1, x2) || fbHeight < std::min(y1, y2) || std::max(x1, x2) < 0 || std::max(y1, y2) < 0) return;
        if (weight <= 1) drawLine(framebuffer, fbWidth, fbHeight, x1, y1, x2, y2, color);

        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        int half = weight / 2;
        int x = x1;
        int y = y1;
        while (true) {
            drawCircle(framebuffer, fbWidth, fbHeight, x, y, half, true, color);
            if (x == x2 && y == y2) break;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }

    void drawRectangle(uint32_t* framebuffer, int fbWidth, int fbHeight, int x1, int y1, int x2, int y2, bool fill, uint32_t color) {
        if (fbWidth < std::min(x1, x2) || fbHeight < std::min(y1, y2) || std::max(x1, x2) < 0 || std::max(y1, y2) < 0)
            return;

        if (x2 < x1) {
            int temp = x1;
            x1 = x2;
            x2 = temp;
        }
        if (y2 < y1) {
            int temp = y1;
            y1 = y2;
            y2 = temp;
        }
        drawLine(framebuffer, fbWidth, fbHeight, x1, y1, x2, y1, color);
        drawLine(framebuffer, fbWidth, fbHeight, x1, y2, x2, y2, color);
        drawLine(framebuffer, fbWidth, fbHeight, x1, y1, x1, y2, color);
        drawLine(framebuffer, fbWidth, fbHeight, x2, y1, x2, y2, color);
        if (!fill) return;
        for (int y = y1 + 1; y < y2; y++) {
            for (int x = x1 + 1; x < x2; x++) {
                if (x < 0 || fbWidth <= x || y < 0 || fbHeight <= y) continue;
                framebuffer[y * fbWidth + x] = color;
            }
        }
    }

    void drawCircle(uint32_t* framebuffer, int fbWidth, int fbHeight, int x, int y, int r, bool fill, uint32_t color) {
        if (x + r < 0 || fbWidth <= x - r || y + r < 0 || fbHeight <= y - r) return;

        int startX = std::max(0, x - r);
        int startY = std::max(0, y - r);
        int endX = std::min(fbWidth - 1, x + r);
        int endY = std::min(fbHeight - 1, y + r);

        for (int i = startY; i <= endY; i++) {
            for (int j = startX; j <= endX; j++) {
                if ((i - y) * (i - y) + (j - x) * (j - x) <= r * r) {
                    framebuffer[i * fbWidth + j] = color;
                }
            }
        }
    }

    void drawGlyph(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int posX, int posY, const Glyph& g, int scale, uint32_t color) {
        if (fbX2 < posX || fbY2 < posY || posX + g.width * scale < fbX1 || posY + g.height * scale < fbY1)
            return;

        for (int y = 0; y < g.height; y++) {
            for (int x = 0; x < g.width; x++) {
                if (!g.pixels[y][x]) continue;

                for (int dy = 0; dy < scale; dy++) {
                    for (int dx = 0; dx < scale; dx++) {
                        int px = posX + x * scale + dx;
                        int py = posY + y * scale + dy;
                        if (fbX1 <= px && px < fbX2 && fbY1 <= py && py < fbY2)
                            framebuffer[py * fbWidth + px] = color;
                    }
                }
            }
        }
    }

    void drawExtendGlyph(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int posX, int posY, const Glyph& g, int dstWidth, int dstHeight, uint32_t color) {
        if (dstWidth <= 0 || dstHeight <= 0) return;
        if (g.width <= 0 || g.height <= 0) return;

        if (fbX2 < posX || fbY2 < posY || posX + dstWidth < fbX1 || posY + dstHeight < fbY1)
            return;

        for (int dy = 0; dy < dstHeight; dy++) {
            int srcY = dy * g.height / dstHeight;
            for (int dx = 0; dx < dstWidth; dx++) {
                int srcX = dx * g.width / dstWidth;
                if (!g.pixels[srcY][srcX]) continue;
                int px = posX + dx;
                int py = posY + dy;
                if (fbX1 <= px && px < fbX2 &&
                    fbY1 <= py && py < fbY2)
                    framebuffer[py * fbWidth + px] = color;
            }
        }
    }

    void drawLine(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int x1, int y1, int x2, int y2, uint32_t color) {
        if (fbX2 < std::min(x1, x2) || fbY2 < std::min(y1, y2) || std::max(x1, x2) < fbX1 || std::max(y1, y2) < fbY1)
            return;

        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        int x = x1;
        int y = y1;
        while (true) {
            if (fbX1 <= x && x < fbX2 &&
                fbY1 <= y && y < fbY2)
                framebuffer[y * fbWidth + x] = color;
            if (x >= x2 && y >= y2) break;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }

    void drawLine(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int x1, int y1, int x2, int y2, int weight, uint32_t color) {
        if (fbX2 < std::min(x1, x2) || fbY2 < std::min(y1, y2) || std::max(x1, x2) < fbX1 || std::max(y1, y2) < fbY1) return;
        if (weight <= 1) drawLine(framebuffer, fbWidth, fbX1, fbY1, fbX2, fbY2, x1, y1, x2, y2, color);

        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        int half = weight / 2;
        int x = x1;
        int y = y1;
        while (true) {
            drawCircle(framebuffer, fbWidth, fbX1, fbY1, fbX2, fbY2, x, y, half, true, color);
            if (x >= x2 && y >= y2) break;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }

    void drawRectangle(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int x1, int y1, int x2, int y2, bool fill, uint32_t color) {
        if (fbX2 < std::min(x1, x2) || fbY2 < std::min(y1, y2) || std::max(x1, x2) < fbX1 || std::max(y1, y2) < fbY1)
            return;

        if (x2 < x1) {
            int temp = x1;
            x1 = x2;
            x2 = temp;
        }
        if (y2 < y1) {
            int temp = y1;
            y1 = y2;
            y2 = temp;
        }
        drawLine(framebuffer, fbWidth, fbX1, fbY1, fbX2, fbY2, x1, y1, x2, y1, color);
        drawLine(framebuffer, fbWidth, fbX1, fbY1, fbX2, fbY2, x1, y2, x2, y2, color);
        drawLine(framebuffer, fbWidth, fbX1, fbY1, fbX2, fbY2, x1, y1, x1, y2, color);
        drawLine(framebuffer, fbWidth, fbX1, fbY1, fbX2, fbY2, x2, y1, x2, y2, color);
        if (!fill) return;
        for (int y = y1 + 1; y < y2; y++) {
            for (int x = x1 + 1; x < x2; x++) {
                if (x < fbX1 || fbX2 <= x || y < fbY1 || fbY2 <= y) continue;
                framebuffer[y * fbWidth + x] = color;
            }
        }
    }

    void drawCircle(uint32_t* framebuffer, int fbWidth, int fbX1, int fbY1, int fbX2, int fbY2, int x, int y, int r, bool fill, uint32_t color) {
        if (x + r < fbX1 || fbX2 <= x - r || y + r < fbY1 || fbY2 <= y - r) return;

        int startX = std::max(0, std::max(fbX1, x - r));
        int startY = std::max(0, std::max(fbX1, y - r));
        int endX = std::min(fbWidth, std::min(fbX2 - 1, x + r));
        int endY = std::min(fbY2 - 1, y + r);

        for (int i = startY; i <= endY; i++) {
            for (int j = startX; j <= endX; j++) {
                if ((i - y) * (i - y) + (j - x) * (j - x) == r * r) {
                    framebuffer[i * fbWidth + j] = color;
                }
            }
        }
        if (fill == false) return;

        for (int i = startY; i <= endY; i++) {
            for (int j = startX; j <= endX; j++) {
                if ((i - y) * (i - y) + (j - x) * (j - x) <= r * r) {
                    framebuffer[i * fbWidth + j] = color;
                }
            }
        }
    }

    void save32bitBMP(const std::string& fname, int width, int height, const uint32_t* framebuffer) {
        int dataSize = width * height * 4;
        BMPFileHeader bfh;
        bfh.bfType = 0x4D42;
        bfh.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + dataSize;
        bfh.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

        BMPInfoHeader bih;
        bih.biSize = sizeof(BMPInfoHeader);
        bih.biWidth = width;
        bih.biHeight = -height;
        bih.biPlanes = 1;
        bih.biBitConunt = 32;
        bih.biCompression = 0;
        bih.biSizeImage = dataSize;
        bih.biXPelsPerMeter = 0;
        bih.biYPelsPerMeter = 0;
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;

        std::ofstream file(fname, std::ios::binary);
        if (!file) return;

        file.write(reinterpret_cast<char*>(&bfh), sizeof(bfh));
        file.write(reinterpret_cast<char*>(&bih), sizeof(bih));
        file.write(reinterpret_cast<const char*>(framebuffer), dataSize);
        file.close();
    }
}