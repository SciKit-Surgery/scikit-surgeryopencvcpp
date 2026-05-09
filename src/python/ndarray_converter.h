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
 * \brief Utility class for converting between NumPy ndarrays and cv::Mat.
 *
 * Provides static methods to convert in both directions. Also registers
 * a pybind11 type_caster (below) so that cv::Mat can be used directly
 * in pybind11 function signatures — numpy arrays are converted automatically
 * on the Python/C++ boundary.
 *
 * Supported conversions:
 *   - 1D array -> single-column cv::Mat
 *   - 2D array -> single-channel cv::Mat
 *   - 3D array (H x W x C) -> multi-channel cv::Mat
 *
 * Supported dtypes: uint8, int8, uint16, int16, int32, float32, float64.
 *
 * All conversions create copies (no shared memory between Python and C++),
 * ensuring safe ownership semantics.
 */
struct NDArrayConverter
{
  /**
   * \brief Initialises the NumPy C API. Must be called once before any
   * conversion operations.
   * \return true if initialisation succeeded, false otherwise.
   */
  static bool init_numpy();

  /**
   * \brief Converts a NumPy ndarray to a cv::Mat.
   *
   * Creates a deep copy of the array data. The resulting cv::Mat owns
   * its memory independently of the Python array.
   *
   * \param o pybind11::array the input NumPy array.
   * \return cv::Mat with matching dimensions, type, and data.
   * \throws std::runtime_error if the array has unsupported dimensions (>3D)
   *         or an unsupported dtype.
   */
  static cv::Mat toMat(const pybind11::array& o);

  /**
   * \brief Converts a cv::Mat to a NumPy ndarray.
   *
   * Creates a deep copy of the matrix data. The resulting array owns
   * its memory independently of the cv::Mat.
   *
   * Single-channel matrices produce 2D arrays (rows x cols).
   * Multi-channel matrices produce 3D arrays (rows x cols x channels).
   *
   * \param mat cv::Mat the input matrix.
   * \return pybind11::array with matching shape, dtype, and data.
   * \throws std::runtime_error if the matrix depth is unsupported.
   */
  static pybind11::array toNDArray(const cv::Mat& mat);
};

} // end namespace

namespace pybind11 { namespace detail {

/**
 * \brief pybind11 type caster enabling automatic cv::Mat <-> numpy.ndarray
 * conversion in function signatures.
 *
 * When a C++ function takes or returns cv::Mat, pybind11 will automatically
 * use this caster to convert to/from numpy arrays at the Python boundary.
 */
template <>
struct type_caster<cv::Mat>
{
public:
  PYBIND11_TYPE_CASTER(cv::Mat, const_name("numpy.ndarray"));

  /**
   * \brief Python -> C++ conversion (load).
   * \param src Python object (expected to be a numpy array).
   * \param (unused) implicit conversion flag.
   * \return true if conversion succeeded.
   */
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

  /**
   * \brief C++ -> Python conversion (cast).
   * \param m The cv::Mat to convert.
   * \param (unused) return value policy.
   * \param (unused) parent handle.
   * \return Python numpy array handle.
   */
  static handle cast(const cv::Mat& m, return_value_policy, handle)
  {
    return sks::NDArrayConverter::toNDArray(m).release();
  }
};

}} // namespace pybind11::detail

#endif
