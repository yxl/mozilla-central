/*
 * FilesystemBase.cpp
 *
 *  Created on: Oct 25, 2013
 *      Author: yuan
 */

#include "FilesystemBase.h"
#include "nsStringGlue.h"

namespace mozilla {
namespace dom {

FilesystemBase::FilesystemBase()
{
  // TODO Auto-generated constructor stub

}

FilesystemBase::~FilesystemBase()
{
  // TODO Auto-generated destructor stub
}

const nsString&
FilesystemBase::GetInvalidPathChars() const
{
  static const nsString kInvalidChars = NS_LITERAL_STRING("\0");
  return kInvalidChars;
}

} // namespace dom
} // namespace mozilla
