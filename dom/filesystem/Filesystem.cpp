/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Filesystem.h"
#include "mozilla/ErrorResult.h"
#include "mozilla/dom/Promise.h"
#include "mozilla/dom/FilesystemBinding.h"
#include "mozilla/dom/DOMError.h"
#include "mozilla/dom/ContentChild.h"
#include "Directory.h"
#include "nsXULAppAPI.h"
#include "nsPIDOMWindow.h"
#include "PathManager.h"
#include "GetEntranceTask.h"

namespace mozilla {
namespace dom {
namespace filesystem {

NS_IMPL_ISUPPORTS1(Filesystem, nsISupportsWeakReference)

nsRefPtr<Filesystem> Filesystem::sSdcardFilesystem = nullptr;

Filesystem::Filesystem(nsPIDOMWindow* aWindow)
  : mWindow(aWindow)
{
  mPathManager = new PathManager(NS_LITERAL_STRING("/sdcard"));
}

Filesystem::~Filesystem()
{
}

// static
void
Filesystem::ShutdownAll()
{
  if (sSdcardFilesystem) {
      sSdcardFilesystem->Shutdown();
      sSdcardFilesystem = nullptr;
  }
}

void
Filesystem::Shutdown()
{
  // TODO Cancel all runnables
}

} // namespace filesystem
} // namespace dom
} // namespace mozilla
