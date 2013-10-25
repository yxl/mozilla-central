/*
 * CreateDirectoryTask.cpp
 *
 *  Created on: Sep 30, 2013
 *      Author: yuan
 */

#include "CreateDirectoryTask.h"
#include "Directory.h"
#include "FilesystemBase.h"
#include "FilesystemFile.h"
#include "FilesystemUtils.h"

#include "nsString.h"
#include "nsIFile.h"

#include "mozilla/dom/Promise.h"
#include "mozilla/dom/PContent.h"

namespace mozilla {
namespace dom {

CreateDirectoryTask::CreateDirectoryTask(FilesystemBase* aFilesystem,
                                         const nsAString& aPath,
                                         const nsAString& aErrorName)
  : mFilesystem(new FilesystemWeakRef(aFilesystem))
  , mTargetRealPath(aPath)
{
  mPromise = new Promise(aFilesystem->GetWindow());

  if (aErrorName.IsEmpty()) {
    Start();
  } else {
    SetError(aErrorName);
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
  return FilesystemFileResponse(mTargetFile->GetPath(), mTargetFile->IsDirectory());
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

