/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GetFileOrDirectoryTask.h"
#include "Directory.h"
#include "FilesystemBase.h"
#include "FilesystemUtils.h"
#include "FilesystemFile.h"

#include "nsIFile.h"
#include "nsStringGlue.h"

#include "mozilla/dom/Promise.h"

namespace mozilla {
namespace dom {

GetFileOrDirectoryTask::GetFileOrDirectoryTask(
  FilesystemBase* aFilesystem,
  const nsAString& aTargetPath,
  const nsAString& aErrorName,
  bool aDirectoryOnly /*= false*/)
  : mFilesystem(new FilesystemWeakRef(aFilesystem))
  , mTargetRealPath(aTargetPath)
  , mDirectoryOnly(aDirectoryOnly)
{
  mPromise = new Promise(aFilesystem->GetWindow());
  if (aErrorName.IsEmpty()) {
    Start();
  } else {
    SetError(aErrorName);
    HandlerCallback();
  }
}

GetFileOrDirectoryTask::GetFileOrDirectoryTask(
  const FilesystemGetFileOrDirectoryParams& aParam,
  FilesystemRequestParent* aParent) : TaskBase(aParam, aParent)
{
  mTargetRealPath = aParam.realPath();
  mDirectoryOnly = aParam.directoryOnly();
  Start();
}

GetFileOrDirectoryTask::~GetFileOrDirectoryTask()
{
}

already_AddRefed<Promise>
GetFileOrDirectoryTask::GetPromise()
{
  return nsRefPtr<Promise>(mPromise).forget();
}

FilesystemParams
GetFileOrDirectoryTask::GetRequestParams()
{
  return FilesystemGetFileOrDirectoryParams(mTargetRealPath, mDirectoryOnly);
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

  if (!ret) {
    SetError(FilesystemUtils::DOM_ERROR_NOT_FOUND);
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
    if (mDirectoryOnly) {
      SetError(FilesystemUtils::DOM_ERROR_TYPE_MISMATCH);
      return;
    }

    // Get isFile
    rv = file->IsFile(&ret);
    if (NS_FAILED(rv)) {
      SetError(rv);
      return;
    }
    if (!ret) {
      // Neither directory or file.
      SetError(FilesystemUtils::DOM_ERROR_TYPE_MISMATCH);
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

  AutoSafeJSContext cx;

  if (!HasError()) {
    JS::Value value = mTargetFile->ToJsValue(cx, filesystem);
    NS_ASSERTION(!JSVAL_IS_NULL(value), "Failed to create DOM File!");
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
