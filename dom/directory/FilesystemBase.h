/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_FilesystemBase_h__
#define mozilla_dom_FilesystemBase_h__

#include "nsWeakReference.h"
#include "nsAutoPtr.h"

class nsAString;
class nsPIDOMWindow; // You need |#include "nsPIDOMWindow.h"| in CPP files.

namespace mozilla {
namespace dom {

/*
 * To make FilesystemBase as a weak reference, so that before the child window
 * is closed and the FilesystemBase is destroyed, we don't need to notify the
 * TaskBase instances, which hold the FilesystemBase reference, to cancel and
 * wait until the instances finish.
 */
class FilesystemBase : public nsSupportsWeakReference
{
public:
  FilesystemBase();

  virtual nsPIDOMWindow* GetWindow() const = 0;

  /*
   * Get the real path of the root directory of the Filesystem. The path should
   * use "/" as separator.
   */
  virtual void GetRootDirectory(nsAString& aRootRealPath) const = 0;

  /*
   * Get the virtual name of the root directory. This name can be exposed to
   * the content page.
   */
  virtual void GetRootName(nsAString& aRoot) const = 0;
protected:
  ~FilesystemBase();
};

/*
 * Helper class for holding and accessing a weak reference of FilesystemBase.
 */
class FilesystemWeakRef
{
public:
  FilesystemWeakRef(FilesystemBase* aFilesystem);

  already_AddRefed<FilesystemBase> Get();
private:
  nsWeakPtr mFilesystem;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_FilesystemBase_h__
