/*=============================================================================

  NumPy <-> cv::Mat converter for pybind11.

  Based on work by Yati Sagade and others.
  Adapted for pybind11 and modern NumPy C API.

=============================================================================*/

#ifndef sks_ndarray_converter_h
#define sks_ndarray_converter_h

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/core.hpp>

namespace sks {

/**
 * \brief Converts between NumPy ndarrays and cv::Mat.
 *
 * Register this with pybind11 to allow automatic conversion of
 * numpy arrays to/from cv::Mat in function signatures.
 */
struct NDArrayConverter
{
  static bool init_numpy();
  static cv::Mat toMat(const pybind11::array& o);
  static pybind11::array toNDArray(const cv::Mat& mat);
};

} // end namespace

namespace pybind11 { namespace detail {

template <>
struct type_caster<cv::Mat>
{
public:
  PYBIND11_TYPE_CASTER(cv::Mat, const_name("numpy.ndarray"));

  // Python -> C++
  bool load(handle src, bool)
  {
    if (!src || src.is_none())
      return false;

    auto arr = pybind11::array::ensure(src);
    if (!arr)
      return false;

    value = sks::NDArrayConverter::toMat(arr);
    return true;
  }

  // C++ -> Python
  static handle cast(const cv::Mat& m, return_value_policy, handle)
  {
    return sks::NDArrayConverter::toNDArray(m).release();
  }
};

}} // namespace pybind11::detail

#endif
