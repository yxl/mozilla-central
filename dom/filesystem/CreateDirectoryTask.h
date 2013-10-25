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

class nsString;

namespace mozilla {
namespace dom {

class FilesystemBase;
class FilesystemCreateDirectoryParams;
class FilesystemFile;
class FilesystemWeakRef;

class CreateDirectoryTask : public TaskBase
{
public:
  CreateDirectoryTask(FilesystemBase* aFilesystem,
                      const nsAString& aPath,
                      const nsAString& aErrorName);
  CreateDirectoryTask(const FilesystemCreateDirectoryParams& aParam,
                      FilesystemRequestParent* aParent);

  virtual ~CreateDirectoryTask();

  already_AddRefed<Promise> GetPromise();

protected:
  virtual FilesystemParams GetRequestParams() MOZ_OVERRIDE;

  virtual FilesystemResponseValue GetSuccessRequestResult() MOZ_OVERRIDE;
  virtual void SetSuccessRequestResult(const FilesystemResponseValue& aValue) MOZ_OVERRIDE;

  virtual void Work() MOZ_OVERRIDE;
  virtual void HandlerCallback() MOZ_OVERRIDE;

private:
  static const uint32_t CREATE_DIRECTORY_PERMISSION = 0700;
  nsRefPtr<Promise> mPromise;
  nsAutoPtr<FilesystemWeakRef> mFilesystem;
  nsString mTargetRealPath;
  nsRefPtr<FilesystemFile> mTargetFile;
};

}
}

#endif /* CREATEDIRECTORYTASK_H_ */
