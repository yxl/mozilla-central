/*
 * GetFileOrDirectoryTask.h
 *
 *  Created on: Sep 30, 2013
 *      Author: yuan
 */

#ifndef mozilla_dom_getentrance_h__
#define mozilla_dom_getentrance_h__

#include "TaskBase.h"
#include "nsAutoPtr.h"

class nsString;

namespace mozilla {
namespace dom {

class FilesystemBase;
class FilesystemFile;
class FilesystemWeakRef;

class GetFileOrDirectoryTask MOZ_FINAL : public TaskBase
{
public:
  GetFileOrDirectoryTask(FilesystemBase* aFilesystem,
                         const nsString& aTargetPath);
  GetFileOrDirectoryTask(const FilesystemGetFileOrDirectoryParams& aParam,
                      FilesystemRequestParent* aParent);

  virtual ~GetFileOrDirectoryTask();

  already_AddRefed<Promise> GetPromise();

protected:
  virtual FilesystemParams GetRequestParams() MOZ_OVERRIDE;

  virtual FilesystemResponseValue GetSuccessRequestResult() MOZ_OVERRIDE;
  virtual void SetSuccessRequestResult(const FilesystemResponseValue& aValue) MOZ_OVERRIDE;

  virtual void Work() MOZ_OVERRIDE;
  virtual void HandlerCallback() MOZ_OVERRIDE;

private:
  nsRefPtr<Promise> mPromise;
  nsAutoPtr<FilesystemWeakRef> mFilesystem;
  nsString mTargetRealPath;
  nsRefPtr<FilesystemFile> mTargetFile;

};

}
}

#endif // mozilla_dom_getentrance_h__
