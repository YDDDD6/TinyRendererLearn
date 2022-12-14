/*
 * @Descripttion: LinEngine
 * @version: 1.0.0
 * @Author: 晋毓麟
 * @Date: 2022-08-16 21:41:00
 * @LastEditors: jy1lnz
 * @LastEditTime: 2022-08-17 00:05:08
 */
#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

#pragma pack(push,1)
struct TGAHeader {
    std::uint8_t  idlength{};
    std::uint8_t  colormaptype{};
    std::uint8_t  datatypecode{};
    std::uint16_t colormaporigin{};
    std::uint16_t colormaplength{};
    std::uint8_t  colormapdepth{};
    std::uint16_t x_origin{};
    std::uint16_t y_origin{};
    std::uint16_t width{};
    std::uint16_t height{};
    std::uint8_t  bitsperpixel{};
    std::uint8_t  imagedescriptor{};
};
#pragma pack(pop)

struct TGAColor {
    std::uint8_t bgra[4] = {0,0,0,0};
    std::uint8_t bytespp = {0};

    TGAColor() = default;
    TGAColor(const std::uint8_t R, const std::uint8_t G, const std::uint8_t B, const std::uint8_t A=255) : bgra{B,G,R,A}, bytespp(4) { }
    TGAColor(const std::uint8_t *p, const std::uint8_t bpp) : bytespp(bpp) {
        for (int i=bpp; i--; bgra[i] = p[i]);
    }
    TGAColor(const std::uint8_t c) {
        for (int i = 0;i < 4; ++i) bgra[i] = c;
    }
    std::uint8_t& operator[](const int i) { return bgra[i]; }
    std::uint8_t operator[](const int i) const { return bgra[i]; }
    inline TGAColor operator*(float intensity)
    {
        return TGAColor((float)bgra[2]*intensity,
                        (float)bgra[1]*intensity,
                        (float)bgra[0]*intensity,
                        (float)bgra[3]*intensity);
    }
    inline TGAColor operator+(const TGAColor& rhs)
    {
        return TGAColor(std::min(bgra[0] + rhs.bgra[0], 255),
            std::min(bgra[1] + rhs.bgra[1], 255),
            std::min(bgra[2] + rhs.bgra[2], 255),
            std::min(bgra[3] + rhs.bgra[3], 255));
    }
    bool operator==(const TGAColor& rhs)
    {
        for (int i = 0;i < 4; ++i)
            if (bgra[i] != rhs.bgra[i])
                return false;
        return true;
    }
};

struct TGAImage {
    enum Format { GRAYSCALE=1, RGB=3, RGBA=4 };

    TGAImage() = default;
    TGAImage(const int w, const int h, const int bpp);
    bool  read_tga_file(const std::string filename);
    bool write_tga_file(const std::string filename, const bool vflip=true, const bool rle=true) const;
    void flip_horizontally();
    void flip_vertically();
    TGAColor get(const int x, const int y) const;
    void set(const int x, const int y, const TGAColor &c);
    int width()  const;
    int height() const;
private:
    bool   load_rle_data(std::ifstream &in);
    bool unload_rle_data(std::ofstream &out) const;

    int w   = 0;
    int h   = 0;
    int bpp = 0;
    std::vector<std::uint8_t> data = {};
};

