/*
 * FilesystemFile.cpp
 *
 *  Created on: Oct 23, 2013
 *      Author: yuan
 */

#include "FilesystemFile.h"
#include "nsIFile.h"
#include "Directory.h"

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

void
FilesystemFile::GetName(nsAString& aName) const
{
  aName = Substring(mPath, mPath.RFindChar(Directory::kSeparatorChar) + 1);
}

} /* namespace dom */
} /* namespace mozilla */
