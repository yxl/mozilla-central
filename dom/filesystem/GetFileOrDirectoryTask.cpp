/*
 * GetFileOrDirectoryTask.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: yuan
 */

#include "GetFileOrDirectoryTask.h"
#include "Directory.h"
#include "FilesystemBase.h"
#include "FilesystemUtils.h"
#include "FilesystemFile.h"

#include "nsIFile.h"
#include "nsString.h"

#include "mozilla/dom/Promise.h"
#include "mozilla/dom/PContent.h"

namespace mozilla {
namespace dom {

GetFileOrDirectoryTask::GetFileOrDirectoryTask(
  FilesystemBase* aFilesystem,
  const nsString& aTargetPath)
  : mFilesystem(new FilesystemWeakRef(aFilesystem))
  , mTargetRealPath(aTargetPath)
{
  mPromise = new Promise(aFilesystem->GetWindow());
  Start();
}

GetFileOrDirectoryTask::GetFileOrDirectoryTask(
  const FilesystemGetFileOrDirectoryParams& aParam,
  FilesystemRequestParent* aParent)
  : TaskBase(aParent)
{
  mTargetRealPath = aParam.realPath();
  Start();
}

GetFileOrDirectoryTask::~GetFileOrDirectoryTask()
{
}

FilesystemParams
GetFileOrDirectoryTask::GetRequestParams()
{
  return FilesystemGetFileOrDirectoryParams(mTargetRealPath);
}

FilesystemResponseValue
GetFileOrDirectoryTask::GetSuccessRequestResult()
{
  return FilesystemFileResponse(mTargetFile->GetPath(), mTargetFile->IsDirectory());
}

void
GetFileOrDirectoryTask::SetSuccessRequestResult(const FilesystemResponseValue& aValue)
{
  FilesystemFileResponse r = aValue;
  mTargetFile = new FilesystemFile(r.realPath(), r.isDirectory());
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

  bool ret;
  rv = file->Exists(&ret);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }

  if (!ret) {
    SetError(FilesystemError::DOM_ERROR_NOT_FOUND);
    return;
  }

  bool isDirectory = false;

  // Get isDirectory.
  rv = file->IsDirectory(&isDirectory);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }

  if (!isDirectory) {
    // Get isFile
    rv = file->IsFile(&ret);
    if (NS_FAILED(rv)) {
      SetError(rv);
      return;
    }
    if (!ret) {
      // Neither directory or file.
      SetError(FilesystemError::DOM_ERROR_TYPE_MISMATCH);
      return;
    }
  }

  mTargetFile = new FilesystemFile(mTargetRealPath, isDirectory);
}

void
GetFileOrDirectoryTask::HandlerCallback()
{
  nsRefPtr<FilesystemBase> filesystem = mFilesystem->Get();
  if (!filesystem) {
    return;
  }
  nsCOMPtr<nsIGlobalObject> globalObject = do_QueryInterface(
      filesystem->GetWindow());
  if (!globalObject) {
    return;
  }

  AutoSafeJSContext cx;
  JS::Rooted<JSObject*> global(cx, globalObject->GetGlobalJSObject());

  if (!HasError()) {
    nsRefPtr<Directory> dir = new Directory(filesystem, mTargetFile);
    if (dir) {
      Optional<JS::Handle<JS::Value> > val(cx,
          OBJECT_TO_JSVAL(dir->WrapObject(cx, global)));
      mPromise->MaybeResolve(cx, val);
      return;
    }
    mErrorName = FilesystemError::DOM_ERROR_SECURITY;
  }
  nsRefPtr<DOMError> domError = new DOMError(filesystem->GetWindow(), mErrorName);
  Optional<JS::Handle<JS::Value> > val(cx,
      OBJECT_TO_JSVAL(domError->WrapObject(cx, global)));
  mPromise->MaybeReject(cx, val);
}

}
}
