/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_FilesystemBase_h__
#define mozilla_dom_FilesystemBase_h__

#include "nsWeakReference.h"
#include "nsAutoPtr.h"
#include "nsStringGlue.h"

class nsPIDOMWindow; // You need |#include "nsPIDOMWindow.h"| in CPP files.

namespace mozilla {
namespace dom {

class FilesystemBase : public nsSupportsWeakReference
{
public:
  FilesystemBase();
  virtual const nsString& GetInvalidPathChars() const;
  virtual nsPIDOMWindow* GetWindow() const = 0;
  virtual void GetRootDirectory(nsAString& aRoot) const = 0;
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
