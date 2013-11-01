/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_FilesystemThreadPool_h__
#define mozilla_dom_FilesystemThreadPool_h__

#include "nsIObserver.h"
#include "nsCOMPtr.h"
#include "mozilla/StaticPtr.h"

class nsIRunnable;
class nsIThreadPool;

namespace mozilla {
namespace dom {

/*
 * This class is for error handling.
 * All methods in this class are static.
 */
class FilesystemThreadPool MOZ_FINAL : public nsIObserver
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIOBSERVER

  /*
   * Dispatch a runnable to the thread pool.
   * A thread pool will be created the first time calling this function.
   * The thread pool will be automatically destroyed before
   * "profile-before-change".
   */
  static void Dispatch(nsIRunnable *event);

private:
  FilesystemThreadPool() {}

  bool Init();
  void Shutdown();

  // Only used on main thread. Don't need a lock.
  nsCOMPtr<nsIThreadPool> mThreadPool;

  static const uint32_t kThreadLimit = 5;
  static const uint32_t kIdleThreadLimit = 5;
  static const uint32_t kIdleThreadTimeoutMs = 30000;
  static StaticRefPtr<FilesystemThreadPool> sInstance;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_FilesystemThreadPool_h__
