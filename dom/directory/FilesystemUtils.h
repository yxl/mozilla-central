/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_FilesystemUtils_h__
#define mozilla_dom_FilesystemUtils_h__

#include "js/Value.h"
#include "nsAutoPtr.h"
#include "nsStringGlue.h"

class nsPIDOMWindow;
class nsIFile;
class nsISupports;
class nsWrapperCache;

struct nsID;
typedef nsID nsIID;

namespace mozilla {
namespace dom {

/*
 * This class is for error handling.
 * All methods in this class are static.
 */
class FilesystemUtils
{
public:
  static JS::Value WrapperCacheObjectToJsval(JSContext* cx,
                                             nsPIDOMWindow* aWindow,
                                             nsWrapperCache* aObject);

  static JS::Value InterfaceToJsval(JSContext* cx,
                                    nsPIDOMWindow* aWindow,
                                    nsISupports* aObject,
                                    const nsIID* aIID);

  /*
   * Create nsIFile object with the given real path (absolute DOM path).
   */
  static already_AddRefed<nsIFile> CetLocalFile(const nsAString& aRealPath);

public:
  static const nsString& ErrorNameFromCode(const nsresult& aErrorCode);

  static const nsString DOM_ERROR_INVALID_PATH;
  static const nsString DOM_ERROR_INVALID_MODIFICATION;
  static const nsString DOM_ERROR_INVALID_STATE;
  static const nsString DOM_ERROR_NOT_FOUND;
  static const nsString DOM_ERROR_NOT_READABLE;
  static const nsString DOM_ERROR_NO_MODIFICATION_ALLOWED;
  static const nsString DOM_ERROR_PATH_EXISTS;
  static const nsString DOM_ERROR_QUOTA_EXCEEDED;
  static const nsString DOM_ERROR_SECURITY;
  static const nsString DOM_ERROR_TYPE_MISMATCH;
  static const nsString DOM_ERROR_UNKNOWN;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_FilesystemUtils_h__
