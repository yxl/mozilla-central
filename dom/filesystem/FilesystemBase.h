/*
 * FilesystemBase.h
 *
 *  Created on: Oct 25, 2013
 *      Author: yuan
 */

#ifndef mozilla_dom_filesystembase_h__
#define mozilla_dom_filesystembase_h__

#include "nsWeakReference.h"

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

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_filesystembase_h__
