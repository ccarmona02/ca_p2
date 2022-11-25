#include "histogram.hpp"
#include <fstream>
#include<omp.h>
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
    void histogram::merge_histos(std::vector<histogram>h, int nthreads){
        for(int i=0; i<256; i++){ //maximum of lines(channels) that a histo can have.
            int sred=0, sgreen=0, sblue=0; //reset the sum
            for(int j=0; j<nthreads; j++){
                sred += h[j].get_red_frequency(i); //accumulate the partial values of the frequency to obtain the total
                sblue += h[j].get_blue_frequency(i);
                sgreen += h[j].get_green_frequency(i);
            }
            channels[red_channel][i]= sred;
            channels[green_channel][i]= sgreen;
            channels[blue_channel][i] = sblue;
        }
    }

}
