/* Inspired by https://cdn.xcancel.com/pic/orig/B6AAD0F330486/media%2FGlOmPxqWgAA6G4U.png */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <X11/Xlib.h>

#include <X11/keysym.h>

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

#define BG_COLOR	(0xFF000000)

static void render(Display *display, Window window, GC gc, int cwidth, int cheight) {
    for (int i = 0; i < cheight; ++i) {
        for (int j = 0; j < cwidth; ++j) {
            float x = (float)j;
            float y = (float)i;
            
            float w = (float)cwidth;
            float h = (float)cheight;
            
            float circ_term1 = (((2 * x) - w) / h) * (((2 * x) - w) / h);
            float circ_term2 = (((2 * y) - h) / h) * (((2 * y) - h) / h);            
            
            float alpha_term = 0.5f + (((0.01f * h) / (2 * (x - y) + (h - w))));
            
            float sqrt_term = sqrtf(circ_term1 + circ_term2);
            float abs_sub = fabsf(sqrt_term - alpha_term);         
            float result = 0.1f / abs_sub;
            if (result >= 1.0f) result = 1.0f;

            unsigned long pixel = (unsigned long)(255 * result);
                        
            unsigned long color = (0xFF << (8 * 3)) | (pixel << (8 * 2)) | (pixel << (8 * 1) | pixel);
            
            XSetForeground(display, gc, color);
            XDrawPoint(display, window, gc, (int)x, (int)y);
        }
    }    
}

int main(void) {
    int result = 0;
    Display *display = NULL;
    Window root_window = 0; 
    Window window = 0;
    XEvent event = {0};
    GC gc = 0;
    bool quit = false;
    
    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Couldn't Open XDisplay!\n");
        result = 1;
        return result;
    } 
    root_window = XDefaultRootWindow(display);
    if (!root_window) {
        fprintf(stderr, "Couldn't set up root window!\n");
        result = 1;
        goto done;
    }
    window = XCreateSimpleWindow(display, root_window, 0, 0, WINDOW_WIDTH, 
              WINDOW_HEIGHT, 0, 0, BG_COLOR);
    if (!window) {
        fprintf(stderr, "Couldn't set up default window!\n");
        result = 1;
        goto done;
    }
    
    gc = XCreateGC(display, window, 0, (XGCValues*)(GCFillRule | GCFillStyle));
    if (!gc) {
        fprintf(stderr, "Couldn't create graphics context\n");
        result = 1;
        goto done;
    }

    XStoreName(display, window, "Event Horizon");
    XMapWindow(display, window);
    
    XFlush(display);

    XSelectInput(display, window, (KeyPressMask | ExposureMask));
        while (!quit) {
        XNextEvent(display, &event);
        
        XWindowAttributes attrs = {0};
        if (!XGetWindowAttributes(display, window, &attrs)) {
            fprintf(stderr, "[ERROR]: Couldn't get current window's attributes\n");
            result = 1;
            goto done;
        }        
        int current_width = attrs.width;
        int current_height = attrs.height;
        
        if (event.type == KeyPress) {
            if (XLookupKeysym(&event.xkey, 0) == XK_q) {
                printf("[INFO]: Quit\n");
                quit = true;
            }
        }
        else if (event.type == Expose) {
            render(display, window, gc, current_width, current_height);
        }
    }
            
done:
    if (gc) XFreeGC(display, gc);
    if (root_window) XDestroyWindow(display, root_window);
    if (window) XDestroyWindow(display, window);
    if (display) XCloseDisplay(display);
    return result;
}
