#include "ME_PCH.h"

#define STBI_NO_SIMD  // disable due to dependence on emmintrin.h, see https://github.com/nothings/stb/issues/1738
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
