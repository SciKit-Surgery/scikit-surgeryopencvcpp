/*=============================================================================

  SKSURGERYOPENCVCPP: Image-guided surgery functions, in C++, using OpenCV.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sks_exception_h
#define sks_exception_h

#include <stdexcept>
#include <ostream>
#include <sstream>

namespace sks {

/**
 * \brief Custom exception class that captures source file location.
 *
 * Extends std::exception to include the file name and line number where
 * the exception was thrown. Supports stream-style message building via
 * operator<<, allowing descriptive error messages to be composed inline:
 *
 *   sksExceptionThrow() << "Expected 3 rows, got " << mat.rows;
 *
 * Typically constructed via the sksExceptionThrow() macro rather than
 * directly, so that __FILE__ and __LINE__ are captured automatically.
 */
class Exception : public std::exception
{
public:

  /**
   * \brief Constructs an exception with source location information.
   * \param fileName The source file where the exception was thrown (typically __FILE__).
   * \param lineNumber The line number where the exception was thrown (typically __LINE__).
   */
  Exception(const std::string& fileName, int lineNumber);

  virtual ~Exception();

  /**
   * \brief Returns the source file name where the exception was thrown.
   * \return std::string containing the file path.
   */
  std::string GetFileName() const;

  /**
   * \brief Returns the line number where the exception was thrown.
   * \return int line number.
   */
  int GetLineNumber() const;

  /**
   * \brief Returns the human-readable error description.
   * \return std::string containing the accumulated error message.
   */
  std::string GetDescription() const;

  /**
   * \brief Sets the error description string.
   * \param desc The error message to store.
   */
  void SetDescription(const std::string& desc);

  /**
   * \brief Returns the error description as a C string.
   * \return const char* pointing to the description.
   */
  virtual const char* What();

  /**
   * \brief Stream operator for appending stream manipulators (e.g. std::endl).
   * \param func A stream manipulator function.
   * \return Reference to this exception for chaining.
   */
  Exception& operator<<(std::ostream& (*func)(std::ostream&))
  {
    std::ostringstream ss;
    ss << this->GetDescription() << func;
    this->SetDescription(ss.str());
    return *this;
  }

  /**
   * \brief Stream operator for appending arbitrary data to the error message.
   * \tparam T The type of data to append (must support operator<< to ostream).
   * \param data The value to append to the description.
   * \return Reference to this exception for chaining.
   */
  template <class T> inline Exception& operator<<(T& data)
  {
    std::ostringstream ss;
    ss << this->GetDescription() << data;
    this->SetDescription(ss.str());
    return *this;
  }

  /**
   * \brief Stream operator for appending const data to the error message.
   * \tparam T The type of data to append (must support operator<< to ostream).
   * \param data The const value to append to the description.
   * \return Reference to this exception for chaining.
   */
  template <class T> inline Exception& operator<<(const T& data)
  {
    std::ostringstream ss;
    ss << this->GetDescription() << data;
    this->SetDescription(ss.str());
    return *this;
  }

private:
  std::string m_Description;
  std::string m_FileName;
  int         m_LineNumber;
};

} // end namespace

#endif
