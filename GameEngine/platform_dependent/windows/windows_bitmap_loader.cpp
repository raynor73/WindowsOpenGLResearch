#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include "windows_bitmap_loader.h"
#include <game_engine/logger.h>
#include <sstream>
#include <stringapiset.h>

using namespace GameEngine;
using namespace Gdiplus;
using namespace std;

WindowsBitmapLoader::WindowsBitmapLoader(std::shared_ptr<ServiceLocator> serviceLocator) : m_serviceLocator(serviceLocator)
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiPlusToken, &gdiplusStartupInput, NULL);
}

GameEngine::WindowsBitmapLoader::~WindowsBitmapLoader()
{
    GdiplusShutdown(m_gdiPlusToken);
}

BitmapInfo WindowsBitmapLoader::loadBitmap(const string& path)
{
    const WCHAR* wcPath;
    int c = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, NULL, 0);
    wcPath = new WCHAR[c];
    MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, (LPWSTR) wcPath, c);

    auto bitmap = Bitmap::FromFile(wcPath, false);
    delete[] wcPath;

    BitmapInfo bitmapInfo;
    switch (bitmap->GetPixelFormat()) {
    case PixelFormat24bppRGB:
        for (uint32_t y = 0; y < bitmap->GetHeight(); y++) {
            for (uint32_t x = 0; x < bitmap->GetWidth(); x++) {
                Color color;
                bitmap->GetPixel(x, y, &color);

                bitmapInfo.data.push_back(color.GetR());
                bitmapInfo.data.push_back(color.GetG());
                bitmapInfo.data.push_back(color.GetB());
                bitmapInfo.data.push_back(255);
            }
        }
        break;

    /*case PixelFormat32bppARGB:
        break;*/

    default:
        stringstream ss;
        ss << "Unsupported pixel format: " << bitmap->GetPixelFormat();
        throw domain_error(ss.str());
    }

    delete bitmap;
    return bitmapInfo;
}
