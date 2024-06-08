#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here
const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

int SeamCarver::GetEnergy(int row, int col) const {
  Pixel r_minus = image_.GetPixel(0, 0);
  Pixel r_plus = image_.GetPixel(0, 0);
  Pixel c_minus = image_.GetPixel(0, 0);
  Pixel c_plus = image_.GetPixel(0, 0);
  if (row == 0) {
    r_minus = image_.GetPixel(height_ - 1, col);
  } else {
    r_minus = image_.GetPixel(row - 1, col);
  }
  if (row == (height_ - 1)) {
    r_plus = image_.GetPixel(0, col);
  } else {
    r_plus = image_.GetPixel(row + 1, col);
  }
  if (col == 0) {
    c_minus = image_.GetPixel(row, width_ - 1);
  } else {
    c_minus = image_.GetPixel(row, col - 1);
  }
  if (col == (width_ - 1)) {
    c_plus = image_.GetPixel(row, 0);
  } else {
    c_plus = image_.GetPixel(row, col + 1);
  }

  int r_col = c_minus.GetRed() - c_plus.GetRed();
  int g_col = c_minus.GetGreen() - c_plus.GetGreen();
  int b_col = c_minus.GetBlue() - c_plus.GetBlue();

  int r_row = r_minus.GetRed() - r_plus.GetRed();
  int g_row = r_minus.GetGreen() - r_plus.GetGreen();
  int b_row = r_minus.GetBlue() - r_plus.GetBlue();

  int row_prod = (r_row * r_row) + (g_row * g_row) + (b_row * b_row);
  int col_prod = (r_col * r_col) + (g_col * g_col) + (b_col * b_col);

  int energy = row_prod + col_prod;
  return energy;
}

// ill need an int array of my energies
// direction true == vertical, false == horizontal

// helper functions
// _____________________

int SeamCarver::MinEnergy(int** array, bool direction) const {
  // index of lowest energy
  int min_e_index = 0;
  // cell of lowest energy (contains energy value)
  int min_e = array[0][0];
  // establishes the size of traversal
  int size = direction ? width_ : height_;
  // finds the topmost lowest energy, ignores everything else that isn't less
  for (int i = 0; i < size; ++i) {
    // finds compared cell
    int energy = direction ? array[0][i] : array[i][0];
    // ifF compared cell energy is less than the lowest energy so far, then swap
    if (energy < min_e) {
      // lowest energy index is now i
      min_e_index = i;
      // lowest energy cell is now compared cell
      min_e = energy;
    }
  }
  // return the index, we always start at the 0th row/col
  return min_e_index;
}

int** SeamCarver::InitializeHelperArray() const {
  int** array = new int*[height_];
  for (int i = 0; i < height_; ++i) {
    array[i] = new int[width_];
  }
  for (int col = 0; col < width_; ++col) {
    for (int row = 0; row < height_; ++row) {
      array[row][col] = GetEnergy(row, col);
    }
  }
  return array;
}

// Helper functions for VerticalTraceBack
void SeamCarver::HandleMiddleColumn(
    int** array, int row, int current_col, int mid, int& next_col) const {
  int left = array[row][current_col - 1];
  int right = array[row][current_col + 1];

  if ((left == mid || mid == right || left == right) &&
      (mid == left || mid == right)) {
    next_col = current_col;
  } else {
    next_col = (left <= mid && left <= right)
                   ? current_col - 1
                   : (right <= mid ? current_col + 1 : current_col);
  }
}

void SeamCarver::HandleLeftmostColumn(
    int** array, int row, int current_col, int mid, int& next_col) const {
  next_col =
      (mid <= array[row][current_col + 1]) ? current_col : current_col + 1;
}

void SeamCarver::HandleRightmostColumn(
    int** array, int row, int current_col, int mid, int& next_col) const {
  next_col =
      (mid <= array[row][current_col - 1]) ? current_col : current_col - 1;
}

// Helper functions for HorizontalTraceBack
void SeamCarver::HandleMiddleRow(
    int** array, int col, int current_row, int mid, int& next_row) const {
  int top = array[current_row - 1][col];
  int bottom = array[current_row + 1][col];

  if ((top == mid || mid == bottom || top == bottom) &&
      (mid == top || mid == bottom)) {
    next_row = current_row;
  } else {
    next_row = (top <= mid && top <= bottom)
                   ? current_row - 1
                   : (bottom <= mid ? current_row + 1 : current_row);
  }
}

void SeamCarver::HandleTopmostRow(
    int** array, int col, int current_row, int mid, int& next_row) const {
  next_row =
      (mid <= array[current_row + 1][col]) ? current_row : current_row + 1;
}

void SeamCarver::HandleBottommostRow(
    int** array, int col, int current_row, int mid, int& next_row) const {
  next_row =
      (mid <= array[current_row - 1][col]) ? current_row : current_row - 1;
}

void SeamCarver::VerticalTraceBack(int** array, int* seam) const {
  for (int row = 1; row < height_; ++row) {
    int current_col = seam[row - 1];
    int next_col = current_col;
    int mid = array[row][current_col];

    if (current_col != 0 && current_col != width_ - 1) {
      HandleMiddleColumn(array, row, current_col, mid, next_col);
    } else if (current_col == 0) {
      HandleLeftmostColumn(array, row, current_col, mid, next_col);
    } else if (current_col == width_ - 1) {
      HandleRightmostColumn(array, row, current_col, mid, next_col);
    }

    seam[row] = next_col;
  }
}

void SeamCarver::HorizontalTraceBack(int** array, int* seam) const {
  for (int col = 1; col < width_; ++col) {
    int current_row = seam[col - 1];
    int next_row = current_row;
    int mid = array[current_row][col];

    if (current_row != 0 && current_row != height_ - 1) {
      HandleMiddleRow(array, col, current_row, mid, next_row);
    } else if (current_row == 0) {
      HandleTopmostRow(array, col, current_row, mid, next_row);
    } else if (current_row == height_ - 1) {
      HandleBottommostRow(array, col, current_row, mid, next_row);
    }

    seam[col] = next_row;
  }
}

void SeamCarver::TraceSeam(int** array, bool direction, int* seam) const {
  // finds the starting point
  int min_e_index = MinEnergy(array, direction);
  // create the starting point to trace, array[0][x] for vert, array[x][0] for
  // horz
  // int starting_point = direction ? array[0][min_e_index] :
  // array[min_e_index][0];

  seam[0] = min_e_index;
  // begin tracing through the code starting at seam[0]
  // I'm gonna do two traceBacks,
  if (direction) {
    VerticalTraceBack(array, seam);
  } else {
    HorizontalTraceBack(array, seam);
  }
}

void SeamCarver::DeleteArray(int** array) const {
  for (int i = 0; i < height_; ++i) {
    delete[] array[i];
  }
  delete[] array;
}

void SeamCarver::UpdateEnergyVertical(int** array, int row, int col) const {
  if (col != 0 && col != width_ - 1) {
    int lowest = array[row + 1][col - 1];
    for (int offset = 0; offset <= 1; offset++) {
      int current = array[row + 1][col + offset];
      if (current <= lowest) lowest = current;
    }
    array[row][col] += lowest;
  }
  if (col == 0) {
    if (array[row + 1][col] < array[row + 1][1]) {
      array[row][col] += array[row + 1][col];
    } else {
      array[row][col] += array[row + 1][1];
    }
  }
  if (col == width_ - 1) {
    if (array[row + 1][col] < array[row + 1][col - 1]) {
      array[row][col] += array[row + 1][col];
    } else {
      array[row][col] += array[row + 1][col - 1];
    }
  }
}

void SeamCarver::UpdateEnergyHorizontal(int** array, int row, int col) const {
  if (row != 0 && row != height_ - 1) {
    int lowest = array[row - 1][col + 1];
    for (int offset = 0; offset <= 1; offset++) {
      int current = array[row + offset][col + 1];
      if (current <= lowest) lowest = current;
    }
    array[row][col] += lowest;
  }
  if (row == 0) {
    if (array[row][col + 1] < array[row + 1][col + 1]) {
      array[row][col] += array[row][col + 1];
    } else {
      array[row][col] += array[row + 1][col + 1];
    }
  }
  if (row == height_ - 1) {
    if (array[row][col + 1] < array[row - 1][col + 1]) {
      array[row][col] += array[row][col + 1];
    } else {
      array[row][col] += array[row - 1][col + 1];
    }
  }
}

void SeamCarver::FillArray(int** array, bool direction) const {
  if (direction) {
    // Fill the helper array for vertical seam
    for (int row = height_ - 2; row >= 0; --row) {
      for (int col = 0; col < width_; ++col) {
        UpdateEnergyVertical(array, row, col);
      }
    }
  } else {
    // Fill the helper array for horizontal seam
    for (int col = width_ - 2; col >= 0; --col) {
      for (int row = 0; row < height_; ++row) {
        UpdateEnergyHorizontal(array, row, col);
      }
    }
  }
}

// ______________________________________________

int* SeamCarver::GetHorizontalSeam() const {
  int** array = InitializeHelperArray();
  int* seam = new int[width_];

  FillArray(array, false);
  TraceSeam(array, false, seam);
  DeleteArray(array);
  return seam;
}

int* SeamCarver::GetVerticalSeam() const {
  int** array = InitializeHelperArray();
  int* seam = new int[height_];

  FillArray(array, true);
  TraceSeam(array, true, seam);
  DeleteArray(array);
  return seam;
}

// seam is interpreted diff
// for horz, seam[i] is a row, the i is the column
// for vert, seam[i] is a col, the i is the row

void SeamCarver::RemoveHorizontalSeam() {
  // create horizontal seam
  int* seam = GetHorizontalSeam();
  for (int col = 0; col < width_; ++col) {
    // Get the row index of the seam for the current column
    int seam_row = seam[col];
    if (seam_row != height_) {
      // Move all pixels below the row up one
      for (int row = seam_row + 1; row < height_; ++row) {
        image_.SetPixel(row - 1, col, image_.GetPixel(row, col));
      }
    }
    // else continue, it will be ignored
  }

  // Decrease the height of the image by 1
  delete[] image_.GetPixels()[height_ - 1];
  image_.SetHeight(height_ - 1);
  height_ = height_ - 1;

  delete[] seam;
}

void SeamCarver::RemoveVerticalSeam() {
  // create vert seam
  int* seam = GetVerticalSeam();
  // Iterate through each row
  for (int row = 0; row < height_; ++row) {
    // Get the column index of the seam for the current row
    int seam_col = seam[row];
    if (seam_col != width_) {
      // Move all pixels to the right of the seam one column left
      for (int col = seam_col + 1; col < width_; ++col) {
        image_.SetPixel(row, col - 1, image_.GetPixel(row, col));
      }
    }
  }
  // Decrease the width of the image by 1
  image_.SetWidth(width_ - 1);
  width_ = width_ - 1;
  // what am i supposed to do instead of deallocating it?
  // set them all to 0?

  delete[] seam;
}

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}
