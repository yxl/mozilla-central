/*
 * FilesystemFile.cpp
 *
 *  Created on: Oct 23, 2013
 *      Author: yuan
 */

#include "FilesystemFile.h"
#include "nsIFile.h"

namespace mozilla {
namespace dom {

NS_IMPL_ISUPPORTS0(FilesystemFile)

FilesystemFile::FilesystemFile(const nsString& aPath, bool aIsDirectory)
  : mPath(aPath)
  , mIsDirectory(aIsDirectory)
{
}

FilesystemFile::~FilesystemFile()
{
  // TODO Auto-generated destructor stub
}

} /* namespace dom */
} /* namespace mozilla */
