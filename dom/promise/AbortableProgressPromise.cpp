/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 40 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "AbortableProgressPromise.h"
#include "mozilla/dom/AbortableProgressPromiseBinding.h"

namespace mozilla {
namespace dom {

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(AbortableProgressPromise)
NS_IMPL_CYCLE_COLLECTING_ADDREF(AbortableProgressPromise)
NS_IMPL_CYCLE_COLLECTING_RELEASE(AbortableProgressPromise)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(AbortableProgressPromise)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

AbortableProgressPromise::AbortableProgressPromise(nsPIDOMWindow* aWindow)
  : Promise(aWindow)
{
}

AbortableProgressPromise::~AbortableProgressPromise()
{
}

JSObject*
AbortableProgressPromise::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aScope)
{
  return AbortableProgressPromiseBinding::Wrap(aCx, aScope, this);
}

void
AbortableProgressPromise::Abort()
{
}

// Mark this as resultNotAddRefed to return raw pointers
already_AddRefed<AbortableProgressPromise>
AbortableProgressPromise::Progress(VoidAnyCallback& callback)
{
  return nullptr;
}

} // namespace dom
} // namespace mozilla
