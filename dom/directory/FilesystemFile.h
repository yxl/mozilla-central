/*
 * FilesystemFile.h
 *
 *  Created on: Oct 23, 2013
 *      Author: yuan
 */

#ifndef FILESYSTEMFILE_H_
#define FILESYSTEMFILE_H_

#include "js/Value.h"
#include "nsAutoPtr.h"
#include "nsISupportsUtils.h"
#include "nsString.h"

class nsIDOMFile;
class nsPIDOMWindow;

namespace mozilla {
namespace dom {

class FilesystemBase;

class FilesystemFile MOZ_FINAL : public nsISupports
{
  NS_DECL_THREADSAFE_ISUPPORTS
public:
  static bool IsValidRelativePath(FilesystemBase* aFilesystem,
                                  const nsString& aPath);

  FilesystemFile(const nsString& aPath, bool aIsDirectory);
  virtual ~FilesystemFile();

  // The trailing "/" is not included.
  const nsString& GetPath() const { return mPath; }
  bool IsDirectory() const { return mIsDirectory; }
  void GetName(nsAString& aName) const;

  JS::Value ToJsValue(JSContext* cx, FilesystemBase* aFilesystem) const;
private:
  static const PRUnichar kSeparatorChar = PRUnichar('/');

  nsString mPath;
  bool mIsDirectory;
};

} /* namespace dom */
} /* namespace mozilla */
#endif /* FILESYSTEMFILE_H_ */
