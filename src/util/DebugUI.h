/** 
 * DebugUI.h 
 * created 04/20/25 by frank collebrusco
 */
#ifndef DEBUG_UI_H
#define DEBUG_UI_H
#include "../standard.h"

#define DBUISZ (16)
struct DebugUI {
    TextRenderer text;
    Stopwatch tbuf, tcpu, tren, tall;
    double buf[DBUISZ] = {0};
    double cpu[DBUISZ] = {0};
    double ren[DBUISZ] = {0};
    double all[DBUISZ] = {0};
    double dts[DBUISZ] = {0};
    size_t i = 0;

    void init() {
        TextRenderer::init_text_rendering();
        text.init();
        set_text(-1.,-1.,-1.,-1.,-1.);
    }
    void destroy() {
        text.destroy();
        text.destroy_text_rendering();
    }

    void set_text(double c, double b, double r, double a, double d) {
        text.set_text(
            "calc:   %.0fus\n"
            "buffer: %.0fus\n"
            "render: %.0fus\n"
            "all:    %.2fms\n"
            "fps:    %.0f",
            c, b, r, a / 1000.f, 1.f / d
        );
    }

    DebugUI() 
    : tbuf(Stopwatch::MICROSECONDS), 
      tcpu(Stopwatch::MICROSECONDS), 
      tren(Stopwatch::MICROSECONDS), 
      tall(Stopwatch::MICROSECONDS) {
    }

    void tick(double dt) {
        cpu[i] = tcpu.stop_reset();
        buf[i] = tbuf.stop_reset();
        ren[i] = tren.stop_reset();
        all[i] = tall.stop_reset();
        dts[i] = dt;
        i = (i+1) % DBUISZ;
        if (!i) {
            double tc = 0.f, tb = 0.f, tr = 0.f, ta = 0.f, td = 0.f;
            for (size_t j = 0; j < DBUISZ; j++) {
                tc += cpu[j];
                tb += buf[j];
                tr += ren[j];
                ta += all[i];
                td += dts[i];
            }
            tc /= static_cast<double>(DBUISZ);
            tb /= static_cast<double>(DBUISZ);
            tr /= static_cast<double>(DBUISZ);
            ta /= static_cast<double>(DBUISZ);
            td /= static_cast<double>(DBUISZ);
            set_text(tc, tb, tr, ta, td);
        }
        text.render(10, 45, 5);
    }

};


#endif /* DEBUG_UI_H */
