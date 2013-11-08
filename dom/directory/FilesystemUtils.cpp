/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FilesystemUtils.h"
#include "nsContentUtils.h"
#include "nsCxPusher.h"
#include "nsIDOMFile.h"
#include "nsIFile.h"
#include "nsIGlobalObject.h"
#include "nsPIDOMWindow.h"
#include "nsStringGlue.h"
#include "nsWrapperCache.h"

namespace mozilla {
namespace dom {

// static
JS::Value
FilesystemUtils::WrapperCacheObjectToJsval(JSContext* cx,
                                           nsPIDOMWindow* aWindow,
                                           nsWrapperCache* aObject)
{
  nsCOMPtr<nsIGlobalObject> globalObject = do_QueryInterface(aWindow);
  NS_ENSURE_TRUE(globalObject, JSVAL_NULL);

  JS::Rooted<JSObject*> global(cx, globalObject->GetGlobalJSObject());

  return OBJECT_TO_JSVAL(aObject->WrapObject(cx, global));
}

// static
JS::Value FilesystemUtils::InterfaceToJsval(JSContext* cx,
                                            nsPIDOMWindow* aWindow,
                                            nsISupports* aObject,
                                            const nsIID* aIID)
{
  nsCOMPtr<nsIGlobalObject> globalObject = do_QueryInterface(aWindow);
  NS_ENSURE_TRUE(globalObject, JSVAL_NULL);

  JS::Rooted<JSObject*> scopeObj(cx, globalObject->GetGlobalJSObject());
  NS_ENSURE_TRUE(scopeObj, JSVAL_NULL);
  JSAutoCompartment ac(cx, scopeObj);

  JS::Rooted<JS::Value> someJsVal(cx);
  nsresult rv =
    nsContentUtils::WrapNative(cx, scopeObj, aObject, aIID, &someJsVal);
  if (NS_FAILED(rv)) {
    return JSVAL_NULL;
  }

  return someJsVal;
}

// static
already_AddRefed<nsIFile>
FilesystemUtils::CetLocalFile(const nsAString& aRealPath)
{
  nsString localPath;
  NormalizedPathToLocalPath(aRealPath, localPath);
  nsCOMPtr<nsIFile> file;
  nsresult rv = NS_NewLocalFile(localPath, false, getter_AddRefs(file));
  NS_ENSURE_SUCCESS(rv, nullptr);
  return file.forget();
}

// static
void
FilesystemUtils::LocalPathToNormalizedPath(const nsAString& aLocal, nsAString& aNorm)
{
  nsString result;
  result = aNorm;
#if defined(XP_WIN)
  PRUnichar* cur = result.BeginWriting();
  PRUnichar* end = result.EndWriting();
  for (; cur < end; ++cur) {
    if (PRUnichar('\\') == *cur)
      *cur = PRUnichar('/');
  }
#endif
  aNorm = result;
}

// static
void
FilesystemUtils::NormalizedPathToLocalPath(const nsAString& aNorm, nsAString& aLocal)
{
  nsString result;
  result = aNorm;
#if defined(XP_WIN)
  PRUnichar* cur = result.BeginWriting();
  PRUnichar* end = result.EndWriting();
  for (; cur < end; ++cur) {
    if (PRUnichar('/') == *cur)
      *cur = PRUnichar('\\');
  }
#endif
  aLocal = result;
}

// static
void
FilesystemUtils::GetRealPath(nsIDOMFile *aFile, nsAString& aRealPath)
{
  NS_ASSERTION(aFile, "aFile Should not be nullptr.");
  if (NS_FAILED(aFile->GetMozFullPathInternal(aRealPath))) {
    aRealPath.Truncate();
    return;
  }
#if defined(XP_WIN)
  PRUnichar* cur = aRealPath.BeginWriting();
  PRUnichar* end = aRealPath.EndWriting();
  for (; cur < end; ++cur) {
    if (PRUnichar('\\') == *cur)
      *cur = PRUnichar('/');
  }
#endif
}

const nsString FilesystemUtils::DOM_ERROR_INVALID_PATH =
  NS_LITERAL_STRING("InvalidPath");
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
const nsString FilesystemUtils::DOM_ERROR_TYPE =
  NS_LITERAL_STRING("TypeError");

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
