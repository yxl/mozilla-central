/*
 * FilesystemFile.h
 *
 *  Created on: Oct 23, 2013
 *      Author: yuan
 */

#ifndef FILESYSTEMFILE_H_
#define FILESYSTEMFILE_H_

#include "nsISupportsUtils.h"
#include "nsString.h"

class nsIFile;

namespace mozilla {
namespace dom {

class FilesystemFile MOZ_FINAL : public nsISupports
{
  NS_DECL_THREADSAFE_ISUPPORTS
public:
  FilesystemFile(const nsString& aPath, bool aIsDirectory);
  virtual ~FilesystemFile();

  const nsString& getPath() const { return mPath; }
  bool isDirectory() const { return mIsDirectory; }
private:
  nsString mPath;
  bool mIsDirectory;
};

} /* namespace dom */
} /* namespace mozilla */
#endif /* FILESYSTEMFILE_H_ */
