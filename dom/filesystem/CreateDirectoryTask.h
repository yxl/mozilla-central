/*
 * CreateDirectoryTask.h
 *
 *  Created on: Sep 30, 2013
 *      Author: yuan
 */

#ifndef CREATEDIRECTORYTASK_H_
#define CREATEDIRECTORYTASK_H_

#include "TaskBase.h"
#include "nsAutoPtr.h"
#include "nsWeakReference.h"

class nsString;
class nsDOMDeviceStorage;

namespace mozilla {
namespace dom {

class FilesystemFile;
class FilesystemCreateDirectoryParams;
class Directory;

class CreateDirectoryTask : public TaskBase
{
public:
  CreateDirectoryTask(Directory* aDir, const nsAString& aPath);
  CreateDirectoryTask(const FilesystemCreateDirectoryParams& aParam,
                      FilesystemRequestParent* aParent);

  virtual ~CreateDirectoryTask();

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
  nsRefPtr<FilesystemFile> mTargetFile;
};

}
}

#endif /* CREATEDIRECTORYTASK_H_ */
