/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "AbortableProgressPromise.h"
#include "mozilla/dom/DirectoryBinding.h"
#include "nsContentUtils.h"

namespace mozilla {
namespace dom {


NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(AbortableProgressPromise)
NS_IMPL_CYCLE_COLLECTING_ADDREF(AbortableProgressPromise)
NS_IMPL_CYCLE_COLLECTING_RELEASE(AbortableProgressPromise)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(AbortableProgressPromise)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

AbortableProgressPromise::AbortableProgressPromise()
{
  SetIsDOMBinding();
}

AbortableProgressPromise::~AbortableProgressPromise()
{
}

AbortableProgressPromise*
AbortableProgressPromise::GetParentObject() const
{
  return nullptr;
}

JSObject*
AbortableProgressPromise::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aScope)
{
  return AbortableProgressPromiseBinding::Wrap(aCx, aScope, this);
}

void
AbortableProgressPromise::Abort()
{
  // TODO
}

// Mark this as resultNotAddRefed to return raw pointers
already_AddRefed<AbortableProgressPromise>
AbortableProgressPromise::Progress(VoidAnyCallback& callback)
{
  return nullptr;
}

} // namespace dom
} // namespace mozilla
