/*
 * GetFileOrDirectoryTask.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: yuan
 */

#include "GetFileOrDirectoryTask.h"
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

GetFileOrDirectoryTask::GetFileOrDirectoryTask(nsDOMDeviceStorage* aDeviceStorage)
{
  mPromise = new Promise(aDeviceStorage->GetOwner());
  mDeviceStorage = do_GetWeakReference(static_cast<nsIDOMDeviceStorage*>(aDeviceStorage));

  mTargetRealPath = NS_LITERAL_STRING("/sdcard");
  Start();
}

GetFileOrDirectoryTask::GetFileOrDirectoryTask(
  const FilesystemEntranceParams& aParam,
  FilesystemRequestParent* aParent)
  : TaskBase(aParent)
{
  mTargetRealPath = aParam.basePath();
  Start();
}

GetFileOrDirectoryTask::~GetFileOrDirectoryTask()
{
}

FilesystemParams
GetFileOrDirectoryTask::GetRequestParams()
{
  return FilesystemEntranceParams(mTargetRealPath);
}

FilesystemResponseValue
GetFileOrDirectoryTask::GetSuccessRequestResult()
{
  return DirectoryResponse(mTargetInfo.realPath, mTargetInfo.name);
}

void
GetFileOrDirectoryTask::SetSuccessRequestResult(const FilesystemResponseValue& aValue)
{
  DirectoryResponse r = aValue;
  mTargetInfo.realPath = r.realPath();
  mTargetInfo.name = r.name();
}

void
GetFileOrDirectoryTask::Work()
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

  if (!mTargetInfo.exists) {
    SetError(Error::DOM_ERROR_NOT_FOUND);
    return;
  }

  if (!mTargetInfo.isDirectory && !mTargetInfo.isFile) {
    SetError(Error::DOM_ERROR_TYPE_MISMATCH);
    return;
  }
}

void
GetFileOrDirectoryTask::HandlerCallback()
{
  nsRefPtr<nsDOMDeviceStorage> storage = GetDeviceStorage();
  if (!storage) {
    return;
  }
  nsCOMPtr<nsIGlobalObject> globalObject = do_QueryInterface(
      storage->GetOwner());
  if (!globalObject) {
    return;
  }

  AutoSafeJSContext cx;
  JS::Rooted<JSObject*> global(cx, globalObject->GetGlobalJSObject());

  if (!HasError()) {
    nsRefPtr<Directory> dir = FileUtils::CreateDirectory(storage,
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
GetFileOrDirectoryTask::GetDeviceStorage()
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
