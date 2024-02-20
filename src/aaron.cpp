#include "aaron.h"

/// @brief closest color for kmeans
/// @param pix 
/// @param colors 
/// @return 
cv::Vec3b closestColor( cv::Vec3b &pix, std::vector<cv::Vec3b> &colors ) {
  int mincolor = 0;
  int mindist = SSD( pix, colors[0] );

  for(int i=1;i<colors.size();i++) {
    int sse = SSD( pix, colors[i] );
    if( sse < mindist ) {
      mindist = sse;
      mincolor = i;
    }
  }

  return( colors[mincolor] );
}