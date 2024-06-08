#ifndef SEAM_CARVER_HPP
#define SEAM_CARVER_HPP

#include "image_ppm.hpp"

class SeamCarver {
public:
  // default constructor
  //
  // Not tested, but you are welcome to intialize (or not) member variables
  // however you like in this function
  SeamCarver() = default;

  // overloaded constructor
  //
  // sets the instances' image_ as a DEEP COPY of the image parameter. Note
  // that if ImagePPM's copy constructor is implemented correctly, you
  // shouldn't have to worry about the details of a deep copy here
  SeamCarver(const ImagePPM& image);

  // sets the instances' image_ as a DEEP COPY of the image parameter. Same
  // note as above regarding ImagePPM
  void SetImage(const ImagePPM& image);

  // returns the instance's image_
  const ImagePPM& GetImage() const;

  // returns the image's height
  int GetHeight() const;

  // returns the image's width
  int GetWidth() const;

  // returns the energy of the pixel at row col in image_
  int GetEnergy(int row, int col) const;

  // returns the horizontal seam of image_ with the least amount of
  // energy
  //
  // the ith int in the returned array corresponds to which row at
  // col i is in the seam. example:
  //
  //    | x |   |
  // ---+---+---+---
  //  x |   | x |
  // ---+---+---+---
  //    |   |   | x
  // returns {1, 0, 1, 2}
  int* GetHorizontalSeam() const;

  // returns the vertical seam of image_ with the least amount of
  // energy
  //
  // the ith int in the returned array corresponds to which col at
  // row i is in the seam. example:
  //
  //    | x |   |
  // ---+---+---+---
  //    |   | x |
  // ---+---+---+---
  //    |   | x |
  // returns {1, 2, 2}
  int* GetVerticalSeam() const;

  // removes one horizontal seam in image_. example:
  //
  // image_ before:
  //  0 | 1 | 2 | 3
  // ---+---+---+---
  //  4 | 5 | 6 | 7
  // ---+---+---+---
  //  8 | 9 | 10| 11
  //
  // seam to remove:
  //    | x |   |
  // ---+---+---+---
  //  x |   | x |
  // ---+---+---+---
  //    |   |   | x
  //
  // image_ after:
  //  0 | 5 | 2 | 3
  // ---+---+---+---
  //  8 | 9 | 10| 7
  void RemoveHorizontalSeam();

  // removes one vertical seam in image_. example:
  //
  // image_ before:
  //  0 | 1 | 2 | 3
  // ---+---+---+---
  //  4 | 5 | 6 | 7
  // ---+---+---+---
  //  8 | 9 | 10| 11
  //
  // seam to remove:
  //    | x |   |
  // ---+---+---+---
  //    |   | x |
  // ---+---+---+---
  //    |   | x |
  //
  // image_ after:
  //  0 | 2 | 3
  // ---+---+---
  //  4 | 5 | 7
  // ---+---+---
  //  8 | 9 | 11
  void RemoveVerticalSeam();

  /**
   * Add any helper methods you may need
   */

private:
  ImagePPM image_;
  int height_ = 0;
  int width_ = 0;

  /**
   * Add any helper methods you may need
   */

  // helper functions for vtb
  void HandleMiddleColumn(
      int** array, int row, int current_col, int mid, int& next_col) const;
  void HandleLeftmostColumn(
      int** array, int row, int current_col, int mid, int& next_col) const;
  void HandleRightmostColumn(
      int** array, int row, int current_col, int mid, int& next_col) const;

  // helper functions for htb
  void HandleMiddleRow(
      int** array, int col, int current_row, int mid, int& next_row) const;
  void HandleTopmostRow(
      int** array, int col, int current_row, int mid, int& next_row) const;
  void HandleBottommostRow(
      int** array, int col, int current_row, int mid, int& next_row) const;

  // updates the energy of each pixel beyond the first col/row
  void UpdateEnergyVertical(int** array, int row, int col) const;

  void UpdateEnergyHorizontal(int** array, int row, int col) const;
  // fills a complete array of energies from the image_
  int** InitializeHelperArray() const;
  // finds the minimum energy across the
  int MinEnergy(int** array, bool direction) const;

  void VerticalTraceBack(int** array, int* seam) const;
  void HorizontalTraceBack(int** array, int* seam) const;

  void TraceSeam(int** array, bool direction, int* seam) const;

  void DeleteArray(int** array) const;

  void FillArray(int** array, bool direction) const;
};

#endif