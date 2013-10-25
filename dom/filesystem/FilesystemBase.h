/*
 * FilesystemBase.h
 *
 *  Created on: Oct 25, 2013
 *      Author: yuan
 */

#ifndef mozilla_dom_filesystembase_h__
#define mozilla_dom_filesystembase_h__

#include "nsWeakReference.h"
#include "nsAutoPtr.h"

class nsString;
class nsPIDOMWindow; // You need |#include "nsPIDOMWindow.h"| in CPP file.

namespace mozilla {
namespace dom {

class FilesystemBase : public nsSupportsWeakReference
{
public:
  FilesystemBase();
  ~FilesystemBase();

  virtual const nsString& GetInvalidPathChars() const;
  virtual nsPIDOMWindow* GetWindow() const = 0;
};

/*
 * Helper class for holding and accessing a weak reference of FilesystemBase.
 */
class FilesystemWeakRef
{
public:
  FilesystemWeakRef(FilesystemBase* aFilesystem);

  already_AddRefed<FilesystemBase> Get();
private:
  nsWeakPtr mFilesystem;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_filesystembase_h__
