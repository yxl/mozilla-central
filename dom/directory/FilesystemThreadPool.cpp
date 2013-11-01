/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FilesystemThreadPool.h"

#include "string.h"

#include "nsComponentManagerUtils.h"
#include "nsIObserverService.h"
#include "nsIThreadPool.h"
#include "nsStringGlue.h"
#include "nsXPCOMCIDInternal.h"
#include "mozilla/Services.h"

namespace mozilla {
namespace dom {

static const char* PROFILE_BEFORE_CHANGE_OBSERVER_ID = "profile-before-change";

StaticRefPtr<FilesystemThreadPool> FilesystemThreadPool::sInstance;

NS_IMPL_ISUPPORTS1(FilesystemThreadPool, nsIObserver)

NS_IMETHODIMP
FilesystemThreadPool::Observe(nsISupports* aSubject,
                      const char* aTopic,
                      const PRUnichar* aData)
{
  if (sInstance && !strcmp(aTopic, PROFILE_BEFORE_CHANGE_OBSERVER_ID)) {
    sInstance->Shutdown();
  }
  return NS_OK;
}

//static
void FilesystemThreadPool::Dispatch(nsIRunnable *event)
{
  NS_ASSERTION(NS_IsMainThread(), "Should be on main thread!");

  // Create thread pool if not exits.
  if (!sInstance) {
    sInstance = new FilesystemThreadPool();
    if (!sInstance->Init()) {
      sInstance = nullptr;
      return;
    }
  }

  if (sInstance->mThreadPool) {
    sInstance->mThreadPool->Dispatch(event, NS_DISPATCH_NORMAL);
  }
}

bool
FilesystemThreadPool::Init()
{
  nsresult rv;
  mThreadPool = do_CreateInstance(NS_THREADPOOL_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, false);

  rv = mThreadPool->SetName(NS_LITERAL_CSTRING("FilesystemThreadPool"));
  NS_ENSURE_SUCCESS(rv, false);

  rv = mThreadPool->SetThreadLimit(kThreadLimit);
  NS_ENSURE_SUCCESS(rv, false);

  rv = mThreadPool->SetIdleThreadLimit(kIdleThreadLimit);
  NS_ENSURE_SUCCESS(rv, false);

  rv = mThreadPool->SetIdleThreadTimeout(kIdleThreadTimeoutMs);
  NS_ENSURE_SUCCESS(rv, false);

  nsCOMPtr<nsIObserverService> obs = mozilla::services::GetObserverService();
  NS_ENSURE_TRUE(obs, false);

  // We need this callback to know when to shut down all our threads.
  rv = obs->AddObserver(this, PROFILE_BEFORE_CHANGE_OBSERVER_ID, false);
  NS_ENSURE_SUCCESS(rv, false);

  return true;
}
void
FilesystemThreadPool::Shutdown()
{
  nsresult rv = mThreadPool->Shutdown();
  NS_ENSURE_SUCCESS_VOID(rv);
}

} // namespace dom
} // namespace mozilla
