#include <vector>
#include "geom_vector.h"
#include "track.h"
#include "labengine.h"

constexpr double TRACK_LENGTH = 1200;
constexpr double MIN_RADIUS = 1;
constexpr double MAX_RADIUS = 16;

using gv::geom_vector_t;
using tr::track_t;

int main() {
  geom_vector_t v, delta;
  int width, height;
  double deltaV = 0.01;

  if (LabInit()) {
    height = LabGetHeight();
    width = LabGetWidth();
    geom_vector_t point(width / 2, height / 2);
    std::vector<track_t> track;

    while (true) {
      int sym = 0;

      if(LabInputKeyReady())
        sym = LabInputKey();

      LabClear();

      // handle pressing
      switch (sym) {
        case LABKEY_UP:
          v += geom_vector_t(0, -deltaV);
          break;
        case LABKEY_DOWN:
          v += geom_vector_t(0, deltaV);
          break;
        case LABKEY_LEFT:
          v += geom_vector_t(-deltaV, 0);
          break;
        case LABKEY_RIGHT:
          v += geom_vector_t(deltaV, 0);
          break;
        case ' ':
          v = geom_vector_t(0, 0);
          break;
        case LABKEY_ESC:
          LabTerm();
          exit(0);
          break;
        default:
          v /= 1.001;
          if (v.Len() < 1e-3)
            v = (0, 0);
          break;
      }
      // normalize speed
      if (v.Len() > 1)
        v /= v.Len();
      // add trace element
      if (v.Len() != 0 && (delta.Len() / v.Len()) >= 20) {
        track.push_back(track_t(point, 1, 255));
        delta = geom_vector_t(0, 0);
      }

      point += v;
      delta += v;
      // handle going abroad
      if (point.x > width)
        point.x -= width;
      else if (point.x < 0)
        point.x += width;
      if (point.y > height)
        point.y -= height;
      else if (point.y < 0)
        point.y += height;

      LabSetColor(LABCOLOR_DARK_RED);
      LabDrawCircle(static_cast<int>(point.x), static_cast<int>(point.y), 2);
      // remove trace element
      if (!track.empty() && track.front().r >= MAX_RADIUS)
        track.erase(track.begin());
      //display the current picture
      double deltaR = (MAX_RADIUS - MIN_RADIUS) / TRACK_LENGTH;
      double deltaC = -255 / TRACK_LENGTH;
      for (auto& t : track) {
        LabSetColorRGB(0, 0, static_cast<int>(t.color));
        LabDrawCircle(static_cast<int>(t.coord.x), static_cast<int>(t.coord.y), static_cast<int>(t.r));
        t.rIncrease(deltaR);
        t.colorIncrease(deltaC);
      }
      LabDrawFlush();

    }
  }

  return 0;
}