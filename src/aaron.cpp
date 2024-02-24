#include "../include/aaron.h"
#include "../include/csv_util.h"

////////////////////////////////////// TASK 1 //////////////////////////////////////

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

/// @brief ISODATA algo that returns 2 most dominant colors in the image
/// @param src 
/// @param dst 
/// @param means 
/// @return 
int isodata(Mat src, Mat& dst, vector<Vec3b>& means){
  //apply kmeans
  std::vector<cv::Vec3b> data;
  int B = 4; // setting it to sample 1/16 of the pixels of the image randomly (4*4 = 16 and then nested for loop below selects one of those pixels)
  for(int i=0;i<src.rows - B;i += B) {
    for(int j=0;j<src.cols - B;j += B) {
    int jx = rand() % B;
    int jy = rand() % B;
    data.push_back( src.at<cv::Vec3b>(i+jy, j+jx) );
    }
  }

  int *labels = new int[data.size()];
  int ncolors = 2; // I think this means k = 2? Apparently this means ISODATA algo

  if(kmeans( data, means, labels, ncolors ) ) {
    printf("Erro using kmeans\n");
    return(-1);
  }


  // //print means just to see values
  // for (Vec3b i:means){
  //   for (int j = 0; j < 3; j++){
  //     cout << static_cast<float>(i.val[j]) << endl;
  //   }
  // }

  //closest color
  dst.create( src.size(), src.type() );
  for(int i=0;i<src.rows;i++) {
    for(int j=0;j<src.cols;j++) {
    dst.at<cv::Vec3b>(i,j) = closestColor(src.at<cv::Vec3b>(i,j), means);
    }
  }

  return(0);
}

/// @brief Finds the darkest pixel value in the image (corresponds to foreground in our case)
/// @param means 
/// @param mean_index 
/// @return 
int find_darkest_pix(vector<Vec3b> means, int& mean_index){
  Vec3b darkest(0,0,0); //darkest pixel is black and whichever has least SSD to this is darkest

  mean_index = 0;
  int mindist = SSD(darkest, means[0]);

  for(int i = 1; i < means.size(); i++){
    int sse = SSD(darkest, means[i]);
    if(sse < mindist){
      mindist = sse;
      mean_index = i;
    }
  }

  return(0);
}

/// @brief Takes the output of the ISODATA and returns binary image with black back and white foreground
/// @param src 
/// @param dst 
/// @param means 
/// @return 
int make_binary_img(Mat src, Mat&dst){
  //darkest color is the foreground and need to set it to white
  // int mean_index;
  // find_darkest_pix(means, mean_index);
  
  //threshold the image by value
  threshold(src, dst, 120, 255, THRESH_BINARY);

  // dst.create(src.size(), src.type());
  // for(int i = 0; i<src.rows; i++){
  //   for(int j = 0; j<src.cols; j++){
  //     Vec3b current = src.at<Vec3b>(i,j);
  //     if(current == means[mean_index]){
  //       dst.at<Vec3b>(i,j) = Vec3b(255,255,255);
  //     }
  //     else{
  //       dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
  //     }
  //   }
  // }

  return(0);
}

////////////////////////////////////// TASK 2 //////////////////////////////////////

/// @brief Shrinking kernel which is 4 connected (only works on binary images with white as foreground and black as background)
/// @param src 
/// @param dst 
/// @param means 
/// @return 
int four_conn_shrink(Mat src, Mat& dst){
  //copy src to dst
  dst.create(src.size(), src.type());
  src.copyTo(dst);

  //foreground
  uchar foreground = 255;

  //if any foreground neighbors a background pixel (4 connected), it is set to background
  for(int i = 1; i < src.rows - 1; i++){
    for(int j = 1;j < src.cols - 1; j++){
      uchar current = src.at<uchar>(i,j);    

      if (current == foreground){
        uchar upper = src.at<uchar>(i-1,j);
        uchar lower = src.at<uchar>(i+1,j);
        uchar right = src.at<uchar>(i,j+1);
        uchar left = src.at<uchar>(i,j-1);
        if (current != upper || current != lower || current != right || current != left){
          dst.at<uchar>(i,j) = 0;
        }
      }
    }
  }
  return(0);
}

/// @brief Growing kernel which is 4 connected (only works on binary images with white as foreground and black as background)
/// @param src 
/// @param dst 
/// @param means 
/// @return 
int four_conn_grow(Mat src, Mat& dst){
  //copy src to dst
  dst.create(src.size(), src.type());
  src.copyTo(dst);

  //background
  uchar background = 0;

  //if any background pixel neighbors a foreground pixel (4 connected), it is set to foreground
  for(int i = 1; i < src.rows - 1; i++){
    for(int j = 1;j < src.cols - 1; j++){
      uchar current = src.at<uchar>(i,j);    

      if (current == background){
        uchar upper = src.at<uchar>(i-1,j);
        uchar lower = src.at<uchar>(i+1,j);
        uchar right = src.at<uchar>(i,j+1);
        uchar left = src.at<uchar>(i,j-1);
        if (current != upper || current != lower || current != right || current != left){
          dst.at<uchar>(i,j) = 255;
        }
      }
    }
  }
  return(0);
}

int task2(Mat src, Mat& dst){
  //shrinking and growing pass to get rid of noise in background
  Mat shrink1, grow1;
  four_conn_shrink(src, shrink1);
  four_conn_grow(shrink1, grow1);

  //growing and shirinking pass to get rid of holes in foreground
  Mat shrink2, grow2;
  four_conn_grow(grow1, grow2);
  four_conn_shrink(grow2, shrink2);

  dst.create(shrink2.size(), shrink2.type());
  shrink2.copyTo(dst);

  return(0);
}

////////////////////////////////////// TASK 6 //////////////////////////////////////

vector<pair<string, float>> baseline_feature_matching(string csv_path, vector<float> target_vector){
  char* csv_file_path = const_cast<char*>(csv_path.c_str());
  // creating vector of <char *> of image file names
  vector<char *> image_filenames;
  vector<vector<float>> feature_vectors_data;
  read_image_data_csv(csv_file_path, image_filenames, feature_vectors_data, 0);

  
  vector<pair<string, float>> dist_vectors_data;

  for(int i = 0; i < feature_vectors_data.size(); i++){
    float dist = 0;
    string object = image_filenames[i];

    for(int j = 0; j < feature_vectors_data[i].size(); j++){
      dist += static_cast<float>((target_vector[j]-feature_vectors_data[i][j])*(target_vector[j]-feature_vectors_data[i][j]));
    }
    dist = sqrt(dist);
    dist_vectors_data.push_back(make_pair(object, dist));
  }

  return dist_vectors_data;
}

bool cmp_least(pair<string, float>& a, 
  pair<string, float>& b) 
{ 
  return a.second < b.second; 
} 

int sort_vec_ascending(vector<pair<string, float>>& dist_vectors_data, int size){
  sort(dist_vectors_data.begin(), dist_vectors_data.end(), cmp_least);
  dist_vectors_data.resize(size);
  return 0;
}

int task6(string csv_path, vector<float> feature_vector){
  vector<pair<string, float>> object_dist_vec = baseline_feature_matching(csv_path, feature_vector);
  sort_vec_ascending(object_dist_vec, 3);
  return(0);
}