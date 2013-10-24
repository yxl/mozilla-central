/*
 * CreateDirectoryTask.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: yuan
 */

#include "CreateDirectoryTask.h"
#include "nsString.h"
#include "Directory.h"
#include "FilesystemUtils.h"
#include "nsIFile.h"
#include "DeviceStorage.h"
#include "FilesystemFile.h"

#include "mozilla/dom/Promise.h"
#include "mozilla/dom/PContent.h"

namespace mozilla {
namespace dom {

CreateDirectoryTask::CreateDirectoryTask(
  Directory* aDir, const nsAString& aPath)
{
  nsRefPtr<nsDOMDeviceStorage> s = aDir->GetDeviceStorage();
  mPromise = new Promise(s->GetOwner());
  mDeviceStorage = do_GetWeakReference(static_cast<nsIDOMDeviceStorage*>(s));

  if (aDir->DOMPathToRealPath(aPath, mTargetRealPath)) {
    Start();
  } else {
    SetError(FilesystemError::DOM_ERROR_INVALID_PATH);
    HandlerCallback();
  }
}

CreateDirectoryTask::CreateDirectoryTask(
  const FilesystemCreateDirectoryParams& aParam,
  FilesystemRequestParent* aParent)
  : TaskBase(aParent)
{
  mTargetRealPath = aParam.realPath();
  Start();
}

CreateDirectoryTask::~CreateDirectoryTask()
{
}

FilesystemParams
CreateDirectoryTask::GetRequestParams()
{
  return FilesystemCreateDirectoryParams(mTargetRealPath);
}

FilesystemResponseValue
CreateDirectoryTask::GetSuccessRequestResult()
{
  return FilesystemFileResponse(mTargetFile->getPath(), mTargetFile->isDirectory());
}

void
CreateDirectoryTask::SetSuccessRequestResult(const FilesystemResponseValue& aValue)
{
  FilesystemFileResponse r = aValue;
  mTargetFile = new FilesystemFile(r.realPath(), r.isDirectory());
}

void
CreateDirectoryTask::Work()
{
  // Resolve nsIFile from mTargetRealPath.
  nsCOMPtr<nsIFile> file;
  nsresult rv = NS_NewLocalFile(mTargetRealPath, false, getter_AddRefs(file));
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }

  bool ret;
  rv = file->Exists(&ret);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }

  if (ret) {
    SetError(FilesystemError::DOM_ERROR_PATH_EXISTS);
    return;
  }

  rv = file->Create(nsIFile::DIRECTORY_TYPE, CREATE_DIRECTORY_PERMISSION);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }

  mTargetFile = new FilesystemFile(mTargetRealPath, true);
}

void
CreateDirectoryTask::HandlerCallback()
{
  nsRefPtr<nsDOMDeviceStorage> d = GetDeviceStorage();
  if (!d) {
    return;
  }
  nsCOMPtr<nsIGlobalObject> globalObject = do_QueryInterface(
      d->GetOwner());
  if (!globalObject) {
    return;
  }

  AutoSafeJSContext cx;
  JS::Rooted<JSObject*> global(cx, globalObject->GetGlobalJSObject());

  if (!HasError()) {
    nsRefPtr<Directory> dir = new Directory(d, mTargetFile);
    if (dir) {
      Optional<JS::Handle<JS::Value> > val(cx,
          OBJECT_TO_JSVAL(dir->WrapObject(cx, global)));
      mPromise->MaybeResolve(cx, val);
      return;
    }
    mErrorName = FilesystemError::DOM_ERROR_SECURITY;
  }
  nsRefPtr<DOMError> domError = new DOMError(d->GetOwner(), mErrorName);
  Optional<JS::Handle<JS::Value> > val(cx,
      OBJECT_TO_JSVAL(domError->WrapObject(cx, global)));
  mPromise->MaybeReject(cx, val);
}

already_AddRefed<nsDOMDeviceStorage>
CreateDirectoryTask::GetDeviceStorage()
{
  nsRefPtr<nsIDOMDeviceStorage> target = do_QueryReferent(mDeviceStorage);
  if (!target) {
    return nullptr;
  }
  nsRefPtr<nsDOMDeviceStorage> storage = static_cast<nsDOMDeviceStorage*>(target.get());
  return storage.forget();
}

}
}

