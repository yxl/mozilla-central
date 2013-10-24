/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_directory_h__
#define mozilla_dom_directory_h__

#include "mozilla/Attributes.h"
#include "mozilla/ErrorResult.h"
#include "mozilla/dom/BindingDeclarations.h"
#include "nsCycleCollectionParticipant.h"
#include "nsWrapperCache.h"
#include "nsWeakReference.h"
#include "nsAutoPtr.h"
#include "nsDOMFile.h"

class nsDOMDeviceStorage;

namespace mozilla {
namespace dom {

class Promise;
class AbortableProgressPromise;
class CreateFileOptions;
class EventStream;
class OpenWriteOptions;
class StringOrDirectoryOrDestinationDict;
class StringOrFileOrDirectory;
class StringOrFile;
class FilesystemFile;

class Directory MOZ_FINAL : public nsISupports,
                            public nsWrapperCache
{
public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(Directory)

public:
  static already_AddRefed<Promise> GetRoot(nsDOMDeviceStorage* aDeviceStorage);

  Directory(nsDOMDeviceStorage* aDeviceStorage, FilesystemFile* aFile);
  ~Directory();

  // ========= Begin WebIDL bindings. ===========

  // TODO: return something sensible here, and change the return type
  Directory* GetParentObject() const;

  virtual JSObject*
  WrapObject(JSContext* aCx, JS::Handle<JSObject*> aScope) MOZ_OVERRIDE;

  void GetName(nsString& retval) const;

  already_AddRefed<Promise> CreateFile(const nsAString& path, const CreateFileOptions& options, ErrorResult& aRv);

  already_AddRefed<Promise> CreateDirectory(const nsAString& path, ErrorResult& aRv);

  already_AddRefed<Promise> Get(const nsAString& path, ErrorResult& aRv);

  already_AddRefed<AbortableProgressPromise> Move(const StringOrFileOrDirectory& path, const StringOrDirectoryOrDestinationDict& dest, ErrorResult& aRv);

  already_AddRefed<Promise> Remove(const StringOrFileOrDirectory& path, ErrorResult& aRv);

  already_AddRefed<Promise> RemoveDeep(const StringOrFileOrDirectory& path, ErrorResult& aRv);

  already_AddRefed<Promise> OpenRead(const StringOrFile& path, ErrorResult& aRv);

  already_AddRefed<Promise> OpenWrite(const StringOrFile& path, const OpenWriteOptions& options, ErrorResult& aRv);

  // Mark this as resultNotAddRefed to return raw pointers
  already_AddRefed<EventStream> Enumerate(const Optional<nsAString >& path);

  // Mark this as resultNotAddRefed to return raw pointers
  already_AddRefed<EventStream> EnumerateDeep(const Optional<nsAString >& path);

  // =========== End WebIDL bindings.============
public:
  already_AddRefed<nsDOMDeviceStorage> GetDeviceStorage();
  /*
   * Convert relative DOM path to the absolute real path.
   * @return true if succeed. false if the DOM path is invalid.
   */
  bool DOMPathToRealPath(const nsAString& aPath, nsAString& aRealPath);

private:
  static bool IsValidDOMPath(const nsString& aPath);

  // Weak reference to nsDOMDeviceStorage
  nsWeakPtr mDeviceStorage;
  nsRefPtr<FilesystemFile> mFile;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_directory_h__
