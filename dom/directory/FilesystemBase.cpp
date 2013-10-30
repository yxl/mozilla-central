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

FilesystemWeakRef::FilesystemWeakRef(FilesystemBase* aFilesystem)
{
  mFilesystem = do_GetWeakReference(aFilesystem);
}

already_AddRefed<FilesystemBase>
FilesystemWeakRef::Get()
{
  nsRefPtr<FilesystemBase> target = do_QueryReferent(mFilesystem);
  if (!target) {
    return nullptr;
  }
  return target.forget();
}

} // namespace dom
} // namespace mozilla
