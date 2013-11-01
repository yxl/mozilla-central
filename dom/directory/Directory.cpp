/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Directory.h"
#include "CreateDirectoryTask.h"
#include "EventStream.h"
#include "FilesystemBase.h"
#include "FilesystemFile.h"
#include "FilesystemUtils.h"
#include "GetFileOrDirectoryTask.h"

#include "nsStringGlue.h"

#include "mozilla/dom/AbortableProgressPromise.h"
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
Directory::GetRoot(FilesystemBase* aFilesystem)
{
  nsString root;
  aFilesystem->GetRootDirectory(root);
  nsString errorName;
  nsRefPtr<GetFileOrDirectoryTask> task = new GetFileOrDirectoryTask(
    aFilesystem, root, errorName, true);
  return task->GetPromise();
}

Directory::Directory(FilesystemBase* aFilesystem,
                     FilesystemFile* aFile)
  : mFilesystem(new FilesystemWeakRef(aFilesystem))
  , mFile(aFile)
{
  SetIsDOMBinding();
}

Directory::~Directory()
{
}

nsPIDOMWindow*
Directory::GetParentObject() const
{
  nsRefPtr<FilesystemBase> fs = mFilesystem->Get();
  if (!fs) {
    return nullptr;
  }
  return fs->GetWindow();
}

JSObject*
Directory::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aScope)
{
  return DirectoryBinding::Wrap(aCx, aScope, this);
}

void
Directory::GetName(nsString& retval) const
{
  nsRefPtr<FilesystemBase> fs = mFilesystem->Get();
  NS_ENSURE_TRUE_VOID(fs);

  nsString root;
  fs->GetRootDirectory(root);
  if (root.Equals(mFile->GetPath())) {
    // If it is the root directory, returns a virtual name other than the real
    // directory name.
    fs->GetRootName(retval);
  } else {
    mFile->GetName(retval);
  }
}

already_AddRefed<Promise>
Directory::CreateFile(const nsAString& path, const CreateFileOptions& options)
{
  return nullptr;
}

already_AddRefed<Promise>
Directory::CreateDirectory(const nsAString& aPath)
{
  nsString errorName;
  nsString realPath;
  if (!DOMPathToRealPath(aPath, realPath)) {
    errorName = FilesystemUtils::DOM_ERROR_INVALID_PATH;
  }
  nsRefPtr<FilesystemBase> fs = mFilesystem->Get();
  nsRefPtr<CreateDirectoryTask> task =
    new CreateDirectoryTask(fs, realPath, errorName);
  return task->GetPromise();
}

already_AddRefed<Promise>
Directory::Get(const nsAString& aPath)
{
  nsString errorName;
  nsString realPath;
  if (!DOMPathToRealPath(aPath, realPath)) {
    errorName = FilesystemUtils::DOM_ERROR_INVALID_PATH;
  }
  nsRefPtr<FilesystemBase> fs = mFilesystem->Get();
  nsRefPtr<GetFileOrDirectoryTask> task =
    new GetFileOrDirectoryTask(fs, realPath, errorName);
  return task->GetPromise();
}

already_AddRefed<AbortableProgressPromise>
Directory::Move(const StringOrFileOrDirectory& path, const StringOrDirectoryOrDestinationDict& dest)
{
  return nullptr;
}

already_AddRefed<Promise>
Directory::Remove(const StringOrFileOrDirectory& path)
{
  return nullptr;
}

already_AddRefed<Promise>
Directory::RemoveDeep(const StringOrFileOrDirectory& path)
{
  return nullptr;
}

already_AddRefed<Promise>
Directory::OpenRead(const StringOrFile& path)
{
  return nullptr;
}

already_AddRefed<Promise>
Directory::OpenWrite(const StringOrFile& path, const OpenWriteOptions& options)
{
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

bool
Directory::DOMPathToRealPath(const nsAString& aPath, nsAString& aRealPath)
{
  nsString relativePath;

  // Normalize the DOM path to remove the leading "./" and the trailing "/"
  if (StringBeginsWith(aPath, NS_LITERAL_STRING("./"))) {
    relativePath = Substring(aPath, 2);
  } else {
    relativePath = aPath;
  }
  if (StringEndsWith(relativePath, NS_LITERAL_STRING("/"))) {
    relativePath = Substring(relativePath, 0, relativePath.Length() - 1);
  }

  nsRefPtr<FilesystemBase> fs = mFilesystem->Get();
  if (!FilesystemFile::IsValidRelativePath(fs, relativePath)) {
    return false;
  }

  aRealPath = mFile->GetPath() + NS_LITERAL_STRING("/") + relativePath;

  return true;
}

} // namespace dom
} // namespace mozilla
