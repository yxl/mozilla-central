/*
 * CreateDirectoryTask.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: yuan
 */

#include "CreateDirectoryTask.h"
#include "nsString.h"
#include "Directory.h"
#include "Error.h"
#include "FileUtils.h"
#include "nsIFile.h"
#include "FileUtils.h"
#include "DeviceStorage.h"

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

  if (aDir->GetRealPath(aPath, mTargetRealPath)) {
    Start();
  } else {
    SetError(Error::DOM_ERROR_ENCODING);
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
  return FilesystemEntranceParams(mTargetRealPath);
}

FilesystemResponseValue
CreateDirectoryTask::GetSuccessRequestResult()
{
  return DirectoryResponse(mTargetInfo.realPath, mTargetInfo.name);
}

void
CreateDirectoryTask::SetSuccessRequestResult(const FilesystemResponseValue& aValue)
{
  DirectoryResponse r = aValue;
  mTargetInfo.realPath = r.realPath();
  mTargetInfo.name = r.name();
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

  rv = FileUtils::GetFileInfo(file, mTargetInfo);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }

  if (mTargetInfo.exists) {
    SetError(Error::DOM_ERROR_PATH_EXISTS);
    return;
  }

  rv = file->Create(nsIFile::DIRECTORY_TYPE, CREATE_DIRECTORY_PERMISSION);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }

  rv = FileUtils::GetFileInfo(file, mTargetInfo);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }
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
    nsRefPtr<Directory> dir = FileUtils::CreateDirectory(d,
      mTargetInfo.realPath, mTargetInfo.name);
    if (dir) {
      Optional<JS::Handle<JS::Value> > val(cx,
          OBJECT_TO_JSVAL(dir->WrapObject(cx, global)));
      mPromise->MaybeResolve(cx, val);
      return;
    }
    mErrorName = Error::DOM_ERROR_SECURITY;
  }
  nsRefPtr<DOMError> domError = Error::GetDOMError(mErrorName);
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

