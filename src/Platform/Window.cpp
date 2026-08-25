#include "Platform/Window.hpp"
#include "Debug/Logger.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstring>
#include <cstdlib>

namespace Engine {
namespace Platform {

Window::Window(const WindowProps& props)
    : m_props(props), m_shouldClose(false), m_displayHandle(nullptr), m_windowHandle(0), m_xImage(nullptr) {
    m_pixelBuffer.resize(m_props.width * m_props.height, 0xFF111118);
}

Window::~Window() {
    close();
}

bool Window::initialize(const WindowProps& props) {
    m_props = props;
    m_pixelBuffer.resize(m_props.width * m_props.height, 0xFF111118);

    LOG_INFO("Initializing Native OS Graphical Window ('" + m_props.title + "' " + 
             std::to_string(m_props.width) + "x" + std::to_string(m_props.height) + ")...");

    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        LOG_WARN("Could not open X11 Display connection (headless environment fallback enabled).");
        return true; // Graceful fallback
    }

    m_displayHandle = display;
    int screen = DefaultScreen(display);

    ::Window win = XCreateSimpleWindow(
        display, RootWindow(display, screen),
        100, 100, m_props.width, m_props.height, 1,
        BlackPixel(display, screen), WhitePixel(display, screen)
    );

    m_windowHandle = static_cast<uint64_t>(win);

    XStoreName(display, win, m_props.title.c_str());
    XSelectInput(display, win, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
    XMapWindow(display, win);

    // Prepare XImage for framebuffer blitting
    XImage* ximage = XCreateImage(
        display, DefaultVisual(display, screen), DefaultDepth(display, screen),
        ZPixmap, 0, reinterpret_cast<char*>(m_pixelBuffer.data()),
        m_props.width, m_props.height, 32, 0
    );

    m_xImage = ximage;
    LOG_INFO("Native OS Graphical Window created successfully!");
    return true;
}

void Window::poll_events() {
    if (!m_displayHandle) return;

    Display* display = static_cast<Display*>(m_displayHandle);
    while (XPending(display) > 0) {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == DestroyNotify) {
            m_shouldClose = true;
        } else if (event.type == MotionNotify) {
            m_mouseX = event.xmotion.x;
            m_mouseY = event.xmotion.y;
        } else if (event.type == ButtonPress) {
            m_mouseX = event.xbutton.x;
            m_mouseY = event.xbutton.y;
            if (event.xbutton.button == 1) {
                m_mouseLeftDown = true;
                m_mouseLeftClicked = true;
            } else if (event.xbutton.button == 3) {
                m_mouseRightDown = true;
                m_mouseRightClicked = true;
            }
        } else if (event.type == ButtonRelease) {
            m_mouseX = event.xbutton.x;
            m_mouseY = event.xbutton.y;
            if (event.xbutton.button == 1) {
                m_mouseLeftDown = false;
            } else if (event.xbutton.button == 3) {
                m_mouseRightDown = false;
            }
        } else if (event.type == KeyPress) {
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            m_lastKeysym = static_cast<uint32_t>(keysym);
            if (keysym == XK_Escape) {
                m_shouldClose = true;
            }
            char buf[8] = {0};
            if (XLookupString(&event.xkey, buf, sizeof(buf), nullptr, nullptr) > 0) {
                m_lastKeyChar = buf[0];
            }
        }
    }
}

void Window::set_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < static_cast<int>(m_props.width) && y >= 0 && y < static_cast<int>(m_props.height)) {
        m_pixelBuffer[y * m_props.width + x] = color;
    }
}

void Window::clear_buffer(uint32_t color) {
    std::fill(m_pixelBuffer.begin(), m_pixelBuffer.end(), color);
}

void Window::swap_buffers() {
    if (!m_displayHandle || !m_xImage) return;

    Display* display = static_cast<Display*>(m_displayHandle);
    ::Window win = static_cast<::Window>(m_windowHandle);
    XImage* ximage = static_cast<XImage*>(m_xImage);

    GC gc = DefaultGC(display, DefaultScreen(display));
    XPutImage(display, win, gc, ximage, 0, 0, 0, 0, m_props.width, m_props.height);
    XFlush(display);
}

void Window::close() {
    if (m_displayHandle) {
        Display* display = static_cast<Display*>(m_displayHandle);
        ::Window win = static_cast<::Window>(m_windowHandle);
        XDestroyWindow(display, win);
        XCloseDisplay(display);
        m_displayHandle = nullptr;
        m_windowHandle = 0;
        m_xImage = nullptr;
        LOG_INFO("Graphical OS Window closed.");
    }
}

} // namespace Platform
} // namespace Engine
