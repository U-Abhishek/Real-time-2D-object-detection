#include "../include/aaron.h"
#include "../include/csv_util.h"

////////////////////////////////////// TASK 1 //////////////////////////////////////

/// @brief closest color for kmeans
/// @param pix 
/// @param colors 
/// @return 
Vec3b closestColor(Vec3b &pix, vector<Vec3b> &colors ) {
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
  int ncolors = 2; // this means k = 2

  if(kmeans( data, means, labels, ncolors ) ) {
    printf("Erro using kmeans\n");
    return(-1);
  }

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

  //find which mean value is the darkest (closest to black pixel)
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
int make_binary_img(Mat src, Mat&dst, vector<Vec3b>& means){
  //darkest color is the foreground and need to set it to white
  int mean_index;
  find_darkest_pix(means, mean_index);

  //make darkest color white (foreground) and lightest color black (background)
  dst.create(src.size(), src.type());
  for(int i = 0; i<src.rows; i++){
    for(int j = 0; j<src.cols; j++){
      Vec3b current = src.at<Vec3b>(i,j);
      if(current == means[mean_index]){
        dst.at<Vec3b>(i,j) = Vec3b(255,255,255);
      }
      else{
        dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
      }
    }
  }
  return(0);
}

////////////////////////////////////// TASK 2 //////////////////////////////////////

/// @brief Shrinking kernel which is 4 connected (only works on binary images with white as foreground and black as background)
/// @param src 
/// @param dst 
/// @param means 
/// @return 
int four_conn_shrink(Mat src, Mat& dst, int n){
  //copy src to dst
  dst.create(src.size(), src.type());
  src.copyTo(dst);

  //foreground
  uchar foreground = 255;

  //if any foreground neighbors a background pixel (4 connected), it is set to background
  for(int loop = 0; loop < n; loop++){
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
  }
  return(0);
}

/// @brief Growing kernel which is 4 connected (only works on binary images with white as foreground and black as background)
/// @param src 
/// @param dst 
/// @param means 
/// @return 
int four_conn_grow(Mat src, Mat& dst, int n){
  //copy src to dst
  dst.create(src.size(), src.type());
  src.copyTo(dst);

  //background
  uchar background = 0;

  //if any background pixel neighbors a foreground pixel (4 connected), it is set to foreground
  for(int loop = 0; loop < n; loop ++){
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
  }
  return(0);
}

/// @brief Task 2 function to get rid of small noise and holes in the source image
/// @param src 
/// @param dst 
/// @return 
int task2(Mat src, Mat& dst){
  //shrink and grow (but grow more)
  Mat shrink, grow;
  four_conn_shrink(src, shrink, 8);
  four_conn_grow(shrink, grow, 16);

  //copy to dst
  dst.create(grow.size(), grow. type());
  grow.copyTo(dst);

  return(0);
}

////////////////////////////////////// TASK 6 //////////////////////////////////////

/// @brief Calculates euclidean distance of target object and database of training object
/// @param csv_path 
/// @param target_vector 
/// @param object_names 
/// @return 
vector<pair<string, float>> baseline_feature_matching(string csv_path, vector<float> target_vector, vector<string>& object_names){
  char* csv_file_path = const_cast<char*>(csv_path.c_str());
  // creating vector of <char *> of image file names
  vector<char *> image_filenames;
  vector<vector<float>> feature_vectors_data;
  read_image_data_csv(csv_file_path, image_filenames, feature_vectors_data, 0);


  //iterate through the char* elements and convert each to string
  vector<string> temp;
  for (const char* str : image_filenames) {
      temp.push_back(str);
  }

  //make a set object to sort and return unique objects
  set<string> uniqueStringsSet;
  for (const string& str : temp) {
      uniqueStringsSet.insert(str);
  }

  //convert set to a vector of strings and set the object_names variable
  vector<string> uniqueStringsVector(uniqueStringsSet.begin(), uniqueStringsSet.end());
  object_names = uniqueStringsVector;
  
  //initialize distance vector
  vector<pair<string, float>> dist_vectors_data;

  //loop through feature vector and calculate euclidean distance
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

/// @brief Comparison function to return the least between the 2
/// @param a 
/// @param b 
/// @return 
bool cmp_least(pair<string, float>& a, 
  pair<string, float>& b) 
{ 
  return a.second < b.second; 
} 

/// @brief Sorts vectors in ascending order
/// @param dist_vectors_data 
/// @param size 
/// @return 
int sort_vec_ascending(vector<pair<string, float>>& dist_vectors_data, int size){
  sort(dist_vectors_data.begin(), dist_vectors_data.end(), cmp_least);
  dist_vectors_data.resize(size);
  return 0;
}

/// @brief Finds which object has the least euclidean distance out of the dataset and labels the image
/// @param frame 
/// @param csv_path 
/// @param feature_vector 
/// @return 
int task6(Mat& frame, string csv_path, vector<float> feature_vector){
  //calculate euclidean distances
  vector<string> object_names;
  vector<pair<string, float>> object_dist_vec = baseline_feature_matching(csv_path, feature_vector, object_names);
  
  //sort in ascending order and return the object with the least distance
  sort_vec_ascending(object_dist_vec, 1);

  //put text on frame
  if(object_dist_vec[0].second < 0.05){
    putText(frame, object_dist_vec[0].first, Point(10,30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255,255,255), 2);
  }
  else{
    putText(frame, "unknown object", Point(10,30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,150), 2);
  }
  cout << "l2 distance is: " << object_dist_vec[0].second << ", press 'k' to switch to KNN Classification" << endl;

  return(0);
}

////////////////////////////////////// TASK 9 //////////////////////////////////////

/// @brief Adds up the top k distances for each input vector pair
/// @param distances 
/// @param k 
/// @return 
float add_k_distances(vector<pair<string, float>> distances, int k){
  //initialize distance
  float distance = 0.0;

  if(distances.size()<k){
    distance = distances[0].second *k ;
    return distance;
  }

  //adds up top k distances of the input vector
  for(int i=0; i<k; i++){
    distance += distances[i].second;
  }  
  return distance;
}

/// @brief K-nearest neighbor classification method which finds which object has the least distance
/// @param frame 
/// @param csv_path 
/// @param target_vector 
/// @param k 
/// @return 
int knn_classification(Mat& frame, string csv_path, vector<float> target_vector, int k){
  //calculate euclidean distances to each vector
  vector<string> object_names;
  vector<pair<string, float>> object_dist_vec = baseline_feature_matching(csv_path, target_vector, object_names);

  //initialize distances vector for least k distances and a temporary variables for the loop
  vector<pair<string, float>> k_distances;
  vector<pair<string, float>> temp;
  float distance;

  //loop through all unique object names and calcualte sum of least k distances
  for(int label = 0; label < object_names.size(); label++){
    temp.clear();

    //loop through all distances of all objects and create vector of distances that correlate to the current object
    for(int i = 0; i<object_dist_vec.size(); i++){
      if(object_dist_vec[i].first == object_names[label]){
        temp.push_back(make_pair(object_dist_vec[i].first,object_dist_vec[i].second)); 
      }
    }
    //sort distances for the current object in asceding order
    sort_vec_ascending(temp, temp.size());

    //add least "k" distances
    distance = add_k_distances(temp,k);

    //put this distance in a vector for that single object
    k_distances.push_back(make_pair(object_names[label], distance));
  }

  //sort the object and distance vector and return top value
  sort_vec_ascending(k_distances,1);

  //put text on the original frame
  if(k_distances[0].second < 0.25){
    putText(frame, k_distances[0].first, Point(10,30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255,255,255), 2);
  }
  else{
    putText(frame, "unknown object", Point(10,30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,150), 2);
  }
  cout << "knn distance is: " << k_distances[0].second << ", press 'l' to swtich to L2 distance" << endl;

  return(0);
}