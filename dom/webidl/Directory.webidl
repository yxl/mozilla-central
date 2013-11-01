/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

interface File;

interface Directory {
  readonly attribute DOMString name;

  [NewObject]
  Promise createFile(DOMString path, optional CreateFileOptions options);

  [NewObject]
  Promise createDirectory(DOMString path);

  [NewObject]
  Promise get(DOMString path);

  [NewObject]
  AbortableProgressPromise
    move((DOMString or File or Directory) path,
         optional (DOMString or Directory or DestinationDict) dest);

  [NewObject]
  Promise remove((DOMString or File or Directory) path);
  [NewObject]
  Promise removeDeep((DOMString or File or Directory) path);

  [NewObject]
  Promise openRead((DOMString or File) path);

  [NewObject]
  Promise openWrite((DOMString or File) path, optional OpenWriteOptions options);

  EventStream enumerate(optional DOMString path);
  EventStream enumerateDeep(optional DOMString path);
};

callback VoidAnyCallback = void (optional any value);
interface AbortableProgressPromise : Promise {
  void abort();
  AbortableProgressPromise progress(VoidAnyCallback callback);
};

callback ForEachCallback = Promise? (optional any value);
interface EventStream {
  Promise forEach(ForEachCallback callback);
};

dictionary OpenWriteOptions {
  OpenIfNotExistsMode ifNotExists = "create";
  OpenIfExistsMode ifExists = "open";
};

enum CreateIfExistsMode { "replace", "fail" };
enum OpenIfExistsMode { "open", "fail" };
enum OpenIfNotExistsMode { "create", "fail" };

dictionary DestinationDict {
  Directory dir;
  DOMString name;
};

enum FileOpenMode { "readonly", "readwrite" };

