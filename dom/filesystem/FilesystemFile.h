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

  const nsString& GetPath() const { return mPath; }
  bool IsDirectory() const { return mIsDirectory; }
  void GetName(nsAString& aName) const;
private:
  const nsString mPath;
  bool mIsDirectory;
};

} /* namespace dom */
} /* namespace mozilla */
#endif /* FILESYSTEMFILE_H_ */
