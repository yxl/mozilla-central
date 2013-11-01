/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FilesystemBase.h"

namespace mozilla {
namespace dom {

FilesystemBase::FilesystemBase()
{
}

FilesystemBase::~FilesystemBase()
{
}

FilesystemWeakRef::FilesystemWeakRef(FilesystemBase* aFilesystem)
{
  mFilesystem = do_GetWeakReference(aFilesystem);
}

already_AddRefed<FilesystemBase>
FilesystemWeakRef::Get()
{
  nsRefPtr<FilesystemBase> target = do_QueryReferent(mFilesystem);
  if (!target) {
    return nullptr;
  }
  return target.forget();
}

} // namespace dom
} // namespace mozilla
