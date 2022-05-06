/*
#define WIN32_LEAN_AND_MEAN

#undef   WINVER
#define  WINVER         0x0501
#undef  _WIN32_WINNT
#define _WIN32_WINNT    0x0501
#undef  _WIN32_IE
#define _WIN32_IE       0x0600

#include <windows.h>
#include <objbase.h>
#include <gdiplus.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "gdiplus.lib")

#include <vector>
#include <boost/assign.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/c_array.hpp>


BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian)

typedef boost::geometry::model::d2::point_xy<int>   point;
typedef boost::geometry::model::polygon<point>      polygon;

using boost::geometry::append;
using boost::geometry::intersection;
using boost::geometry::union_;
using boost::geometry::convex_hull;


int                             width;
int                             height;
ULONG_PTR                       token;
Gdiplus::GdiplusStartupInput    input;

void draw_polygon(Gdiplus::Graphics& graphics, Gdiplus::SolidBrush& brush, Gdiplus::Pen& pen,
                  const polygon& poly)
{
    polygon::ring_type          ring = poly.outer();
    std::vector<Gdiplus::Point> pv(ring.size());

    for (size_t i = 0; i < ring.size(); ++i)
    {
        pv[i].X = ring[i].x();
        pv[i].Y = ring[i].y();
    }

    graphics.FillPolygon(&brush, &*pv.begin(), static_cast<INT>(pv.size()));
    graphics.DrawLines(&pen, &*pv.begin(), static_cast<INT>(pv.size()));
}

void draw_intersection(Gdiplus::Graphics& graphics, const polygon& poly1, const polygon& poly2)
{
    std::vector<polygon> out;
    Gdiplus::Pen         pen(Gdiplus::Color(0x66, 0xff, 0x66), 3);
    Gdiplus::REAL        pattern[] = {5, 5};

    pen.SetDashPattern(pattern, 2);
	intersection(poly1, poly2, out);

    for (std::vector<polygon>::const_iterator itr = out.begin(), end = out.end(); itr != end; ++itr)
    {
        polygon::ring_type          ring = itr->outer();
        std::vector<Gdiplus::Point> pv(ring.size());

        for (size_t i = 0; i < ring.size(); ++i)
        {
            pv[i].X = ring[i].x();
            pv[i].Y = ring[i].y();
        }

        graphics.DrawLines(&pen, &*pv.begin(), static_cast<INT>(pv.size()));
    }
}

void draw_union(Gdiplus::Graphics& graphics, const polygon& poly1, const polygon& poly2)
{
    std::vector<polygon> out;
    Gdiplus::Pen         pen(Gdiplus::Color(0x66, 0xff, 0x66), 3);
    Gdiplus::REAL        pattern[] = {5, 5};

    pen.SetDashPattern(pattern, 2);
	union_(poly1, poly2, out);

    for (std::vector<polygon>::const_iterator itr = out.begin(), end = out.end(); itr != end; ++itr)
    {
        polygon::ring_type          ring = itr->outer();
        std::vector<Gdiplus::Point> pv(ring.size());

        for (size_t i = 0; i < ring.size(); ++i)
        {
            pv[i].X = ring[i].x();
            pv[i].Y = ring[i].y();
        }

        graphics.DrawLines(&pen, &*pv.begin(), static_cast<INT>(pv.size()));
    }
}

void draw_convex_hull(Gdiplus::Graphics& graphics, const polygon& poly)
{
    polygon       hull;
    Gdiplus::Pen  pen(Gdiplus::Color(0x66, 0xff, 0x66), 3);
    Gdiplus::REAL pattern[] = {5, 5};

    pen.SetDashPattern(pattern, 2);
	convex_hull(poly, hull);

    polygon::ring_type          ring = hull.outer();
    std::vector<Gdiplus::Point> pv(ring.size());

    for (size_t i = 0; i < ring.size(); ++i)
    {
        pv[i].X = ring[i].x();
        pv[i].Y = ring[i].y();
    }

    graphics.DrawLines(&pen, &*pv.begin(), static_cast<INT>(pv.size()));
}

void draw(HDC hdc)
{
    const int points1[][2] = { {203, 38},  {59,  61},  {38,  168}, {84,  197}, {119, 152},
                               {212, 162}, {259, 220}, {326, 134}, {203, 38} };
    const int points2[][2] = { {153, 287}, {309, 99},  {73,  18},  {153, 287} };

    ::SetMapMode(hdc, MM_ISOTROPIC);
    ::SetWindowExtEx(hdc, 1, 1, NULL);
    ::SetWindowOrgEx(hdc, 0, height, NULL);
    ::SetViewportExtEx(hdc, 1, -1, NULL);

    Gdiplus::Graphics   graphics(hdc);
    Gdiplus::SolidBrush brush1(Gdiplus::Color(0x40, 0xff, 0x00, 0x00));
    Gdiplus::SolidBrush brush2(Gdiplus::Color(0x40, 0x00, 0x00, 0xff));
    Gdiplus::Pen        pen1(Gdiplus::Color(0xff, 0x55, 0x55), 3);
    Gdiplus::Pen        pen2(Gdiplus::Color(0x33, 0x66, 0xff), 3);
    polygon             poly1, poly2;

	append(poly1, points1);
    append(poly2, points2);

    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    draw_polygon(graphics, brush1, pen1, poly1);
    draw_polygon(graphics, brush2, pen2, poly2);
    draw_intersection(graphics, poly1, poly2);
    //draw_union(graphics, poly1, poly2);
    //draw_convex_hull(graphics, poly1);
}

LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
        Gdiplus::GdiplusStartup(&token, &input, NULL);
        return 0;

    case WM_DESTROY:
        Gdiplus::GdiplusShutdown(token);
        ::PostQuitMessage(0);
        return 0;

    case WM_SIZE:
        width  = LOWORD(lparam);
        height = HIWORD(lparam);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC         hdc;

            hdc = ::BeginPaint(hwnd, &ps);
            draw(hdc);
            ::EndPaint(hwnd, &ps);

            return 0;
        }
    }

    return ::DefWindowProcW(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int)
{
    const wchar_t* const clsname = L"BOOST_GEOMETRY_SAMPLE";

    WNDCLASSEXW wc = {0};
    HWND        hwnd;
    MSG         msg;

    wc.cbSize        = sizeof(wc);
    wc.hCursor       = ::LoadCursorW(NULL, IDC_ARROW);
    wc.hIcon         = ::LoadIconW(NULL, IDI_APPLICATION);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
    wc.lpfnWndProc   = wndproc;
    wc.lpszClassName = clsname;
    wc.hInstance     = hinstance;

    if (::RegisterClassExW(&wc) == 0)
    {
        ::MessageBoxW(NULL, L"Failed to regist window class.", L"Error", MB_OK);
        return 0;
    }

    hwnd = ::CreateWindowEx(
        0,
        clsname,
        L"Sample",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        0,
        hinstance,
        NULL
    );

    if (hwnd == NULL)
    {
        ::MessageBoxW(NULL, L"Failed to create window.", L"Error", MB_OK);
        return 0;
    }

    ::ShowWindow(hwnd, SW_SHOW);
    ::UpdateWindow(hwnd);

    while (::GetMessageW(&msg, NULL, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
    }

    return 0;
}
*/