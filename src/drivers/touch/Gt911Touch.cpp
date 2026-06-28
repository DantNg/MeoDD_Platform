#include "Gt911Touch.h"

#if DRIVER_GT911
namespace drivers {

Gt911Touch::Gt911Touch(int sda, int scl, int intPin, int rstPin, uint16_t w,
                       uint16_t h, bool invertX, bool invertY)
    : dev_(sda, scl, intPin, rstPin, w, h),
      w_(w),
      h_(h),
      invX_(invertX),
      invY_(invertY) {}

bool Gt911Touch::begin() {
  dev_.begin();
  dev_.setRotation(ROTATION_NORMAL);
  return true;
}

bool Gt911Touch::read(hal::TouchPoint& out) {
  dev_.read();
  if (!dev_.isTouched) {
    out.pressed = false;
    return false;
  }
  int16_t rx = dev_.points[0].x;
  int16_t ry = dev_.points[0].y;
  // clamp into the frame
  if (rx < 0) rx = 0; else if (rx > (int16_t)w_ - 1) rx = (int16_t)w_ - 1;
  if (ry < 0) ry = 0; else if (ry > (int16_t)h_ - 1) ry = (int16_t)h_ - 1;
  out.x = invX_ ? (int16_t)(w_ - 1) - rx : rx;
  out.y = invY_ ? (int16_t)(h_ - 1) - ry : ry;
  out.pressed = true;
  return true;
}

}  // namespace drivers
#endif  // DRIVER_GT911
