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
#include "FileUtils.h"
#include "nsWeakReference.h"

class nsString;
class nsDOMDeviceStorage;

namespace mozilla {
namespace dom {

class FilesystemEntranceParams;
class Directory;

class GetFileOrDirectoryTask : public TaskBase
{
public:
  GetFileOrDirectoryTask(nsDOMDeviceStorage* aDeviceStorage,
                         const nsString& aTargetPath);
  GetFileOrDirectoryTask(const FilesystemGetFileOrDirectoryParams& aParam,
                      FilesystemRequestParent* aParent);

  virtual ~GetFileOrDirectoryTask();

protected:
  virtual FilesystemParams GetRequestParams() MOZ_OVERRIDE;

  virtual FilesystemResponseValue GetSuccessRequestResult() MOZ_OVERRIDE;
  virtual void SetSuccessRequestResult(const FilesystemResponseValue& aValue) MOZ_OVERRIDE;

  virtual void Work() MOZ_OVERRIDE;
  virtual void HandlerCallback() MOZ_OVERRIDE;

private:
  static const uint32_t CREATE_DIRECTORY_PERMISSION = 0700;
  // Weak reference to nsDOMDeviceStorage
  nsWeakPtr mDeviceStorage;

  already_AddRefed<nsDOMDeviceStorage> GetDeviceStorage();

  nsString mTargetRealPath;
  FileInfo mTargetInfo;
};

}
}

#endif // mozilla_dom_getentrance_h__
