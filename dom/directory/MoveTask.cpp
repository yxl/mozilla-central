/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MoveTask.h"
#include "Directory.h"
#include "FilesystemBase.h"
#include "FilesystemFile.h"
#include "FilesystemUtils.h"

#include "nsIFile.h"
#include "nsStringGlue.h"

#include "mozilla/dom/AbortableProgressPromise.h"

namespace mozilla {
namespace dom {

MoveTask::MoveTask(FilesystemBase* aFilesystem,
                   const nsAString& aSrcPath,
                   const nsAString& aDestPath,
                   const nsAString& aErrorName)
  : mFilesystem(new FilesystemWeakRef(aFilesystem))
  , mSrcRealPath(aSrcPath)
  , mDestRealPath(aDestPath)
  , mReturnValue(false)
{
  mPromise = new AbortableProgressPromise(aFilesystem->GetWindow());

  if (aErrorName.IsEmpty()) {
    Start();
  } else {
    SetError(aErrorName);
    HandlerCallback();
  }
}

MoveTask::MoveTask(const FilesystemMoveParams& aParam,
                   FilesystemRequestParent* aParent)
  : TaskBase(aParam, aParent)
  , mReturnValue(false)
{
  mSrcRealPath = aParam.srcRealPath();
  mDestRealPath = aParam.destRealPath();
  Start();
}

MoveTask::~MoveTask()
{

}

already_AddRefed<AbortableProgressPromise>
MoveTask::GetPromise()
{
  return nsRefPtr<AbortableProgressPromise>(mPromise).forget();
}

FilesystemParams
MoveTask::GetRequestParams()
{
  return FilesystemMoveParams(mSrcRealPath, mDestRealPath);
}

FilesystemResponseValue
MoveTask::GetSuccessRequestResult()
{
  return FilesystemBooleanResponse(mReturnValue);
}

void
MoveTask::SetSuccessRequestResult(const FilesystemResponseValue& aValue)
{
  FilesystemBooleanResponse r = aValue;
  mReturnValue = r.success();
}

void
MoveTask::Work()
{
  nsCOMPtr<nsIFile> srcFile = FilesystemUtils::CetLocalFile(mSrcRealPath);
  if (!srcFile) {
    SetError(FilesystemUtils::DOM_ERROR_UNKNOWN);
    return;
  }

  bool exists;

  // Check if the source entry exits.
  nsresult rv = srcFile->Exists(&exists);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }
  if (!exists) {
    SetError(FilesystemUtils::DOM_ERROR_NOT_FOUND );
    return;
  }

  // Check the source file type.
  bool isDirectory;
  rv = srcFile->IsDirectory(&isDirectory);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }
  bool isFile;
  rv = srcFile->IsFile(&isFile);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }
  if (!(isDirectory || isFile)) {
    // Neither directory or file.
    SetError(FilesystemUtils::DOM_ERROR_TYPE_MISMATCH);
    return;
  }

  nsCOMPtr<nsIFile> destFile = FilesystemUtils::CetLocalFile(mDestRealPath);
  if (!destFile) {
    SetError(FilesystemUtils::DOM_ERROR_UNKNOWN);
    return;
  }

  // Check if destination exists.
  rv = destFile->Exists(&exists);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }
  if (exists) {
    SetError(FilesystemUtils::DOM_ERROR_PATH_EXISTS);
    return;
  }

  // Move the entry.

  nsCOMPtr<nsIFile> destParent;
  rv = destFile->GetParent(getter_AddRefs(destParent));
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }
  nsString destName;
  rv = destFile->GetLeafName(destName);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }
  srcFile->MoveTo(destParent, destName);
  if (NS_FAILED(rv)) {
    SetError(rv);
    return;
  }

  mReturnValue = true;
}

void
MoveTask::HandlerCallback()
{
  nsRefPtr<FilesystemBase> filesystem = mFilesystem->Get();
  if (!filesystem) {
    return;
  }

  AutoSafeJSContext cx;

  if (!HasError()) {
    JS::Value value = JS::UndefinedValue();
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
