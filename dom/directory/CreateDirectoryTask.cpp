/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CreateDirectoryTask.h"
#include "Directory.h"
#include "FilesystemBase.h"
#include "FilesystemFile.h"
#include "FilesystemUtils.h"

#include "nsIFile.h"
#include "nsStringGlue.h"

#include "mozilla/dom/Promise.h"

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
  FilesystemRequestParent* aParent) : TaskBase(aParam, aParent)
{
  mTargetRealPath = aParam.realPath();
  Start();
}

CreateDirectoryTask::~CreateDirectoryTask()
{

}

already_AddRefed<Promise>
CreateDirectoryTask::GetPromise()
{
  return nsRefPtr<Promise>(mPromise).forget();
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
  nsCOMPtr<nsIFile> file = FilesystemUtils::CetLocalFile(mTargetRealPath);
  if (!file) {
    SetError(FilesystemUtils::DOM_ERROR_UNKNOWN);
    return;
  }

   bool ret;
   nsresult rv = file->Exists(&ret);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }

  if (ret) {
    SetError(FilesystemUtils::DOM_ERROR_PATH_EXISTS);
    return;
  }

  rv = file->Create(nsIFile::DIRECTORY_TYPE, 0700);
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

  AutoSafeJSContext cx;

  if (!HasError()) {
    JS::Value value = mTargetFile->ToJsValue(cx, filesystem);
    MOZ_ASSERT(!JSVAL_IS_NULL(value));
    JS::Rooted<JS::Value> val(cx, value);
    mPromise->MaybeResolve(cx, val);
  } else {
    nsRefPtr<DOMError> domError = new DOMError(filesystem->GetWindow(),
        mErrorName);
    JS::Value error = FilesystemUtils::WrapperCacheObjectToJsval(cx,
        filesystem->GetWindow(), domError);
    JS::Rooted<JS::Value> val(cx, error);
    mPromise->MaybeReject(cx, val);
  }
}

} // namespace dom
} // namespace mozilla
