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
 * \brief Base exception class.
 */
class Exception : public std::exception
{
public:

  Exception(const std::string& fileName, int lineNumber);
  virtual ~Exception();

  std::string GetFileName() const;
  int GetLineNumber() const;

  std::string GetDescription() const;
  void SetDescription(const std::string& desc);
  virtual const char* What();

  Exception& operator<<(std::ostream& (*func)(std::ostream&))
  {
    std::ostringstream ss;
    ss << this->GetDescription() << func;
    this->SetDescription(ss.str());
    return *this;
  }

  template <class T> inline Exception& operator<<(T& data)
  {
    std::ostringstream ss;
    ss << this->GetDescription() << data;
    this->SetDescription(ss.str());
    return *this;
  }

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
