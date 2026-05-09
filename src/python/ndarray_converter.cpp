/*=============================================================================

  NumPy <-> cv::Mat converter for pybind11.

=============================================================================*/

#include "ndarray_converter.h"

#include <numpy/ndarrayobject.h>
#include <opencv2/core.hpp>
#include <stdexcept>

namespace sks {

bool NDArrayConverter::init_numpy()
{
  // import_array is a macro that returns NULL on failure in Python 3
  import_array1(false);
  return true;
}

static int numpy_type_from_cv(int cv_depth)
{
  switch (cv_depth)
  {
    case CV_8U:  return NPY_UINT8;
    case CV_8S:  return NPY_INT8;
    case CV_16U: return NPY_UINT16;
    case CV_16S: return NPY_INT16;
    case CV_32S: return NPY_INT32;
    case CV_32F: return NPY_FLOAT32;
    case CV_64F: return NPY_FLOAT64;
    default:
      throw std::runtime_error("Unsupported cv::Mat depth for numpy conversion");
  }
}

static int cv_depth_from_numpy(int numpy_type)
{
  switch (numpy_type)
  {
    case NPY_UINT8:   return CV_8U;
    case NPY_INT8:    return CV_8S;
    case NPY_UINT16:  return CV_16U;
    case NPY_INT16:   return CV_16S;
    case NPY_INT32:   return CV_32S;
    case NPY_FLOAT32: return CV_32F;
    case NPY_FLOAT64: return CV_64F;
    default:
      throw std::runtime_error("Unsupported numpy dtype for cv::Mat conversion");
  }
}

cv::Mat NDArrayConverter::toMat(const pybind11::array& o)
{
  if (!init_numpy())
    throw std::runtime_error("Failed to initialize numpy");

  auto buf = o.request();

  int ndims = static_cast<int>(buf.ndim);
  int numpy_type = static_cast<int>(pybind11::detail::npy_format_descriptor<double>::dtype().num());

  // Determine the actual numpy type from the array
  PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
  numpy_type = PyArray_TYPE(arr);

  int cv_depth = cv_depth_from_numpy(numpy_type);

  if (ndims == 2)
  {
    int rows = static_cast<int>(buf.shape[0]);
    int cols = static_cast<int>(buf.shape[1]);
    int type = CV_MAKETYPE(cv_depth, 1);

    // Make a copy to ensure contiguous memory owned by cv::Mat
    cv::Mat mat(rows, cols, type, buf.ptr,
                static_cast<size_t>(buf.strides[0]));
    return mat.clone();
  }
  else if (ndims == 3)
  {
    int rows = static_cast<int>(buf.shape[0]);
    int cols = static_cast<int>(buf.shape[1]);
    int channels = static_cast<int>(buf.shape[2]);
    int type = CV_MAKETYPE(cv_depth, channels);

    cv::Mat mat(rows, cols, type, buf.ptr,
                static_cast<size_t>(buf.strides[0]));
    return mat.clone();
  }
  else if (ndims == 1)
  {
    int rows = static_cast<int>(buf.shape[0]);
    int type = CV_MAKETYPE(cv_depth, 1);
    cv::Mat mat(rows, 1, type, buf.ptr);
    return mat.clone();
  }
  else
  {
    throw std::runtime_error("Only 1D, 2D, and 3D arrays are supported");
  }
}

pybind11::array NDArrayConverter::toNDArray(const cv::Mat& mat)
{
  if (!init_numpy())
    throw std::runtime_error("Failed to initialize numpy");

  cv::Mat continuous;
  if (!mat.isContinuous())
    continuous = mat.clone();
  else
    continuous = mat;

  int numpy_type = numpy_type_from_cv(continuous.depth());
  int channels = continuous.channels();

  std::vector<pybind11::ssize_t> shape;
  std::vector<pybind11::ssize_t> strides;

  if (channels == 1)
  {
    shape = {continuous.rows, continuous.cols};
    strides = {
      static_cast<pybind11::ssize_t>(continuous.step[0]),
      static_cast<pybind11::ssize_t>(continuous.step[1])
    };
  }
  else
  {
    shape = {continuous.rows, continuous.cols, channels};
    strides = {
      static_cast<pybind11::ssize_t>(continuous.step[0]),
      static_cast<pybind11::ssize_t>(continuous.step[1]),
      static_cast<pybind11::ssize_t>(continuous.elemSize1())
    };
  }

  // Create array that owns its data (copy from cv::Mat)
  pybind11::array result(pybind11::dtype(numpy_type), shape, strides, continuous.data);
  return result;
}

} // end namespace
