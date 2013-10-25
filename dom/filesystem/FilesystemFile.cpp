/*
 * FilesystemFile.cpp
 *
 *  Created on: Oct 23, 2013
 *      Author: yuan
 */

#include "FilesystemFile.h"
#include "Directory.h"
#include "FilesystemBase.h"

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
  aName = Substring(mPath, mPath.RFindChar(kSeparatorChar) + 1);
}

// static
bool
FilesystemFile::IsValidRelativePath(FilesystemBase* aFilesystem,
                                    const nsString& aPath)
{
  if (aPath.IsEmpty()) {
    return true;
  }

  // Absolute path is not allowed.
  if (aPath.First() == kSeparatorChar) {
    return false;
  }

  // Make sure there is no invalid path character.
  if (aPath.Find(aFilesystem->GetInvalidPathChars(), 0, -1) != kNotFound) {
    return false;
  }

  static const nsString kCurrentDir = NS_LITERAL_STRING(".");
  static const nsString kParentDir = NS_LITERAL_STRING("..");

  // Split path and check each path component.
  PRInt32 begin = 0;
  PRInt32 end;
  for (begin = 0;
       (end = aPath.FindChar(kSeparatorChar, begin)) != kNotFound;
       begin = end + 1) {
    // The path containing empty components, such as "foo//bar", is invalid.
    if (begin == end) {
      return false;
    }
    // We don't allow paths, such as "../foo", "foo/./bar" and "foo/../bar",
    // to walk up the directory.
    nsDependentSubstring pathComponent = Substring(aPath, begin, end - begin);
    if (pathComponent.Equals(kCurrentDir) || pathComponent.Equals(kParentDir)) {
      return false;
    }
  }

  return true;
}

} /* namespace dom */
} /* namespace mozilla */
