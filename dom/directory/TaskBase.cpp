/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "TaskBase.h"
#include "FilesystemRequestParent.h"
#include "FilesystemUtils.h"

#include "nsXULAppAPI.h"
#include "mozilla/dom/ContentChild.h"
#include "mozilla/unused.h"
#include "mozilla/dom/Promise.h"
#include "mozilla/dom/PContent.h"

namespace mozilla {
namespace dom {

// static
bool
TaskBase::IsParentProcess()
{
  return XRE_GetProcessType() == GeckoProcessType_Default;
}

TaskBase::TaskBase(const FilesystemParams& aParam, FilesystemRequestParent* aParent)
  : mRequestParent(aParent)
{
}

void
TaskBase::Start()
{
  MOZ_ASSERT(NS_IsMainThread(), "Only call on main thread!");

  if (IsParentProcess()) {
    // Run in parent process.
    if (!mWorkerThread) {
      nsresult rv = NS_NewThread(getter_AddRefs(mWorkerThread));
      if (NS_FAILED(rv) ) {
        mWorkerThread = nullptr;
        // call error callback
        SetError(rv);
        HandleResult();
        return;
      }
      // Start worker thread
      mWorkerThread->Dispatch(this, NS_DISPATCH_NORMAL);
    }
  } else {
    // Run in child process.

    // Retain a reference so the task object isn't deleted without IPDL's
    // knowledge. The reference will be released by
    // mozilla::dom::ContentChild::DeallocPFilesystemRequestChild.
    AddRef();
    ContentChild::GetSingleton()->SendPFilesystemRequestConstructor(this,
      GetRequestParams());
  }
}

NS_IMETHODIMP
TaskBase::Run()
{
  if (!NS_IsMainThread()) {
    // Run worker thread tasks
    Work();
    // Dispatch itself to main thread
    NS_DispatchToMainThread(this);
  } else {
    // Shutdown mWorkerThread
    if (mWorkerThread) {
      mWorkerThread->Shutdown();
    }
    mWorkerThread = nullptr;
    // Run main thread tasks
    HandleResult();
  }

  return NS_OK;
}

void
TaskBase::HandleResult()
{
  MOZ_ASSERT(IsParentProcess(), "Only call from parent process!");
  if (mRequestParent) {
    unused << mRequestParent->Send__delete__(mRequestParent, GetRequestResult());
  } else {
    HandlerCallback();
  }
}

FilesystemResponseValue
TaskBase::GetRequestResult()
{
  MOZ_ASSERT(IsParentProcess(), "Only call from parent process!");
  if (HasError()) {
    return FilesystemErrorResponse(mErrorName);
  } else {
    return GetSuccessRequestResult();
  }
}

void
TaskBase::SetRequestResult(const FilesystemResponseValue& aValue)
{
  MOZ_ASSERT(!IsParentProcess(), "Only call from child process!");
  if (aValue.type() == FilesystemResponseValue::TFilesystemErrorResponse) {
    FilesystemErrorResponse r = aValue;
    mErrorName = r.error();
  } else {
    SetSuccessRequestResult(aValue);
  }
}

bool
TaskBase::Recv__delete__(const FilesystemResponseValue& aValue)
{
  SetRequestResult(aValue);
  HandlerCallback();
  return true;
}

void
TaskBase::SetError(const nsAString& aErrorName)
{
  mErrorName = aErrorName;
}

void
TaskBase::SetError(const nsresult& aErrorCode)
{
  mErrorName = FilesystemUtils::ErrorNameFromCode(aErrorCode);
}

} // namespace dom
} // namespace mozilla
