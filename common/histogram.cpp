#include "histogram.hpp"
#include <fstream>

namespace images::common {

  void histogram::add_color(pixel p) noexcept {
    channels[red_channel][p.red()]++;
    channels[green_channel][p.green()]++;
    channels[blue_channel][p.blue()]++;
  }

  void histogram::write(std::ostream & os) const noexcept {
    for (const auto x: channels[red_channel]) {
      os << x << '\n';
    }
    for (const auto x: channels[green_channel]) {
      os << x << '\n';
    }
    for (const auto x: channels[blue_channel]) {
      os << x << '\n';
    }
  }

}
