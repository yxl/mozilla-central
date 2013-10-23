/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_fileutils_h__
#define mozilla_dom_fileutils_h__

#include "nsString.h"

class nsIFile;
class nsDOMDeviceStorage;

namespace mozilla {
namespace dom {
class Directory;

struct FileInfo
{
  bool exists;
  bool isDirectory;
  bool isFile;
  nsString realPath;
};

/*
 * nsIFile related utilities.
 * All methods in this class are static.
 */
class FileUtils
{
public:
  static nsresult GetFileInfo(const nsAString& aPath,
                              FileInfo& aInfo);
  static nsresult GetFileInfo(nsIFile* aFile,
                              FileInfo& aInfo);
  static Directory* CreateDirectory(nsDOMDeviceStorage* aDeviceStorage,
                                    const nsAString& aRealPath);
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_fileutils_h__
