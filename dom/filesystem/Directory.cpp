/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Directory.h"
#include "CreateDirectoryTask.h"
#include "GetFileOrDirectoryTask.h"
#include "EventStream.h"
#include "AbortableProgressPromise.h"
#include "FilesystemFile.h"
#include "FilesystemUtils.h"

#include "DeviceStorage.h"
#include "nsXULAppAPI.h"
#include "nsWeakReference.h"
#include "nsStringGlue.h"

#include "mozilla/dom/DirectoryBinding.h"
#include "mozilla/dom/Promise.h"

namespace mozilla {
namespace dom {

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(Directory)
NS_IMPL_CYCLE_COLLECTING_ADDREF(Directory)
NS_IMPL_CYCLE_COLLECTING_RELEASE(Directory)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(Directory)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

// static
already_AddRefed<Promise>
Directory::GetRoot(nsDOMDeviceStorage* aDeviceStorage)
{
  nsRefPtr<GetFileOrDirectoryTask> task = new GetFileOrDirectoryTask(
      aDeviceStorage,
      NS_LITERAL_STRING("/sdcard"));
  return task->GetPromise();
}

Directory::Directory(nsDOMDeviceStorage* aDeviceStorage,
                     FilesystemFile* aFile)
  : mDeviceStorage(do_GetWeakReference(static_cast<nsIDOMDeviceStorage*>(aDeviceStorage))),
    mFile(aFile)
{
  SetIsDOMBinding();
}

Directory::~Directory()
{
}

Directory*
Directory::GetParentObject() const
{
  return nullptr;
}

JSObject*
Directory::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aScope)
{
  return DirectoryBinding::Wrap(aCx, aScope, this);
}

void
Directory::GetName(nsString& retval) const
{
  mFile->GetName(retval);
}

already_AddRefed<Promise>
Directory::CreateFile(const nsAString& path, const CreateFileOptions& options, ErrorResult& aRv)
{
  aRv.Throw(NS_ERROR_NOT_IMPLEMENTED);
  return nullptr;
}

already_AddRefed<Promise>
Directory::CreateDirectory(const nsAString& aPath, ErrorResult& aRv)
{
  nsRefPtr<CreateDirectoryTask> task = new CreateDirectoryTask(this, aPath);
  return task->GetPromise();
}

already_AddRefed<Promise>
Directory::Get(const nsAString& path, ErrorResult& aRv)
{
  aRv.Throw(NS_ERROR_NOT_IMPLEMENTED);
  return nullptr;
}

already_AddRefed<AbortableProgressPromise>
Directory::Move(const StringOrFileOrDirectory& path, const StringOrDirectoryOrDestinationDict& dest, ErrorResult& aRv)
{
  aRv.Throw(NS_ERROR_NOT_IMPLEMENTED);
  return nullptr;
}

already_AddRefed<Promise>
Directory::Remove(const StringOrFileOrDirectory& path, ErrorResult& aRv)
{
  aRv.Throw(NS_ERROR_NOT_IMPLEMENTED);
  return nullptr;
}

already_AddRefed<Promise>
Directory::RemoveDeep(const StringOrFileOrDirectory& path, ErrorResult& aRv)
{
  aRv.Throw(NS_ERROR_NOT_IMPLEMENTED);
  return nullptr;
}

already_AddRefed<Promise>
Directory::OpenRead(const StringOrFile& path, ErrorResult& aRv)
{
  aRv.Throw(NS_ERROR_NOT_IMPLEMENTED);
  return nullptr;
}

already_AddRefed<Promise>
Directory::OpenWrite(const StringOrFile& path, const OpenWriteOptions& options, ErrorResult& aRv)
{
  aRv.Throw(NS_ERROR_NOT_IMPLEMENTED);
  return nullptr;
}

already_AddRefed<EventStream>
Directory::Enumerate(const Optional<nsAString >& path)
{
  return nullptr;
}

already_AddRefed<EventStream>
Directory::EnumerateDeep(const Optional<nsAString >& path)
{
  return nullptr;
}

already_AddRefed<nsDOMDeviceStorage>
Directory::GetDeviceStorage()
{
  nsRefPtr<nsIDOMDeviceStorage> target = do_QueryReferent(mDeviceStorage);
  if (!target) {
    return nullptr;
  }
  nsRefPtr<nsDOMDeviceStorage> storage = static_cast<nsDOMDeviceStorage*>(target.get());
  return storage.forget();
}

bool
Directory::DOMPathToRealPath(const nsAString& aPath, nsAString& aRealPath)
{
  nsString relativePath;

  // Normalize the DOM path to remove the leading "./".
  if (StringBeginsWith(aPath, NS_LITERAL_STRING("./"))) {
    relativePath = Substring(aPath, 2);
  } else {
    relativePath = aPath;
  }

  if (!FilesystemFile::IsValidRelativePath(relativePath)) {
    return false;
  }

  aRealPath = mFile->GetPath() + relativePath;

  return true;
}

} // namespace dom
} // namespace mozilla
