/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FilesystemUtils.h"
#include "nsCxPusher.h"
#include "nsIGlobalObject.h"
#include "nsPIDOMWindow.h"

namespace mozilla {
namespace dom {

// static
JS::Value
FilesystemUtils::WrapperCacheObjectToJsval(JSContext* cx,
                                           nsPIDOMWindow* aWindow,
                                           nsWrapperCache* aObject)
{
  nsCOMPtr<nsIGlobalObject> globalObject = do_QueryInterface(aWindow);
  if (!globalObject) {
    return JSVAL_NULL;
  }

  JS::Rooted<JSObject*> global(cx, globalObject->GetGlobalJSObject());

  return OBJECT_TO_JSVAL(aObject->WrapObject(cx, global));
}

const nsString FilesystemUtils::DOM_ERROR_INVALID_PATH =
  NS_LITERAL_STRING("EncodingError");
const nsString FilesystemUtils::DOM_ERROR_INVALID_MODIFICATION =
  NS_LITERAL_STRING("InvalidModificationError");
const nsString FilesystemUtils::DOM_ERROR_INVALID_STATE =
  NS_LITERAL_STRING("InvalidStateError");
const nsString FilesystemUtils::DOM_ERROR_NOT_FOUND =
  NS_LITERAL_STRING("NotFoundError");
const nsString FilesystemUtils::DOM_ERROR_NOT_READABLE =
  NS_LITERAL_STRING("NotReadableError");
const nsString FilesystemUtils::DOM_ERROR_NO_MODIFICATION_ALLOWED =
  NS_LITERAL_STRING("NoModificationAllowedError");
const nsString FilesystemUtils::DOM_ERROR_PATH_EXISTS =
  NS_LITERAL_STRING("PathExistsError");
const nsString FilesystemUtils::DOM_ERROR_QUOTA_EXCEEDED =
  NS_LITERAL_STRING("QuotaExceededError");
const nsString FilesystemUtils::DOM_ERROR_SECURITY =
  NS_LITERAL_STRING("SecurityError");
const nsString FilesystemUtils::DOM_ERROR_TYPE_MISMATCH =
  NS_LITERAL_STRING("TypeMismatchError");
const nsString FilesystemUtils::DOM_ERROR_UNKNOWN =
  NS_LITERAL_STRING("Unknown");

// static
const nsString&
FilesystemUtils::ErrorNameFromCode(const nsresult& aErrorCode)
{
  switch (aErrorCode) {

    case NS_ERROR_FILE_INVALID_PATH:
    case NS_ERROR_FILE_UNRECOGNIZED_PATH:
      return DOM_ERROR_INVALID_PATH;

    case NS_ERROR_FILE_DESTINATION_NOT_DIR:
      return DOM_ERROR_INVALID_MODIFICATION;

    case NS_ERROR_FILE_ACCESS_DENIED:
    case NS_ERROR_FILE_DIR_NOT_EMPTY:
      return DOM_ERROR_NO_MODIFICATION_ALLOWED;

    case NS_ERROR_FILE_TARGET_DOES_NOT_EXIST:
    case NS_ERROR_NOT_AVAILABLE:
      return DOM_ERROR_NOT_FOUND;

    case NS_ERROR_FILE_ALREADY_EXISTS:
      return DOM_ERROR_PATH_EXISTS;

    case NS_ERROR_FILE_NOT_DIRECTORY:
      return DOM_ERROR_TYPE_MISMATCH;

    case NS_ERROR_UNEXPECTED:
    default:
      return DOM_ERROR_UNKNOWN;
  }
}

} // namespace dom
} // namespace mozilla
