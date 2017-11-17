#include "utils.h"
#include <cmath>

QColor LerpHSV (QColor &a, QColor &b, float t)
{
     // Hue interpolation
     float h;
     float d = b.hueF() - a.hueF();
     float bH = b.hueF(), aH = a.hueF();
     if (aH > bH) {
         // Swap (a.h, b.h)
         float h3 = bH;
         bH = aH;
         aH = h3;

         d = -d;
         t = 1 - t;
     }

     if (d > 0.5) { //180deg
        aH = aH + 1.0; // 360deg
        h = std::fmod(( aH + t * (bH - aH) ), 1); // 360deg
     }
     if (d <= 0.5) {
        h = aH + t * d;
     } //test
     // Interpolates the rest
     return QColor::fromHsvF
     (
         h, // H
         a.saturationF() + t * (b.saturationF()-a.saturationF()), // S
         a.valueF() + t * (b.valueF()-a.valueF()), // V
         1.0 // A
     );
}
