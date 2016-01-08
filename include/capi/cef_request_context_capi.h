// Copyright (c) 2016 Marshall A. Greenblatt. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//

#ifndef CEF_INCLUDE_CAPI_CEF_REQUEST_CONTEXT_CAPI_H_
#define CEF_INCLUDE_CAPI_CEF_REQUEST_CONTEXT_CAPI_H_
#pragma once

#include "include/capi/cef_cookie_capi.h"
#include "include/capi/cef_request_context_handler_capi.h"
#include "include/capi/cef_values_capi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _cef_scheme_handler_factory_t;

///
// A request context provides request handling for a set of related browser or
// URL request objects. A request context can be specified when creating a new
// browser via the cef_browser_host_t static factory functions or when creating
// a new URL request via the cef_urlrequest_t static factory functions. Browser
// objects with different request contexts will never be hosted in the same
// render process. Browser objects with the same request context may or may not
// be hosted in the same render process depending on the process model. Browser
// objects created indirectly via the JavaScript window.open function or
// targeted links will share the same render process and the same request
// context as the source browser. When running in single-process mode there is
// only a single render process (the main process) and so all browsers created
// in single-process mode will share the same request context. This will be the
// first request context passed into a cef_browser_host_t static factory
// function and all other request context objects will be ignored.
///
typedef struct _cef_request_context_t {
  ///
  // Base structure.
  ///
  cef_base_t base;

  ///
  // Returns true (1) if this object is pointing to the same context as |that|
  // object.
  ///
  int (CEF_CALLBACK *is_same)(struct _cef_request_context_t* self,
      struct _cef_request_context_t* other);

  ///
  // Returns true (1) if this object is sharing the same storage as |that|
  // object.
  ///
  int (CEF_CALLBACK *is_sharing_with)(struct _cef_request_context_t* self,
      struct _cef_request_context_t* other);

  ///
  // Returns true (1) if this object is the global context. The global context
  // is used by default when creating a browser or URL request with a NULL
  // context argument.
  ///
  int (CEF_CALLBACK *is_global)(struct _cef_request_context_t* self);

  ///
  // Returns the handler for this context if any.
  ///
  struct _cef_request_context_handler_t* (CEF_CALLBACK *get_handler)(
      struct _cef_request_context_t* self);

  ///
  // Returns the cache path for this object. If NULL an "incognito mode" in-
  // memory cache is being used.
  ///
  // The resulting string must be freed by calling cef_string_userfree_free().
  cef_string_userfree_t (CEF_CALLBACK *get_cache_path)(
      struct _cef_request_context_t* self);

  ///
  // Returns the default cookie manager for this object. This will be the global
  // cookie manager if this object is the global request context. Otherwise,
  // this will be the default cookie manager used when this request context does
  // not receive a value via cef_request_tContextHandler::get_cookie_manager().
  // If |callback| is non-NULL it will be executed asnychronously on the IO
  // thread after the manager's storage has been initialized.
  ///
  struct _cef_cookie_manager_t* (CEF_CALLBACK *get_default_cookie_manager)(
      struct _cef_request_context_t* self,
      struct _cef_completion_callback_t* callback);

  ///
  // Register a scheme handler factory for the specified |scheme_name| and
  // optional |domain_name|. An NULL |domain_name| value for a standard scheme
  // will cause the factory to match all domain names. The |domain_name| value
  // will be ignored for non-standard schemes. If |scheme_name| is a built-in
  // scheme and no handler is returned by |factory| then the built-in scheme
  // handler factory will be called. If |scheme_name| is a custom scheme then
  // you must also implement the cef_app_t::on_register_custom_schemes()
  // function in all processes. This function may be called multiple times to
  // change or remove the factory that matches the specified |scheme_name| and
  // optional |domain_name|. Returns false (0) if an error occurs. This function
  // may be called on any thread in the browser process.
  ///
  int (CEF_CALLBACK *register_scheme_handler_factory)(
      struct _cef_request_context_t* self, const cef_string_t* scheme_name,
      const cef_string_t* domain_name,
      struct _cef_scheme_handler_factory_t* factory);

  ///
  // Clear all registered scheme handler factories. Returns false (0) on error.
  // This function may be called on any thread in the browser process.
  ///
  int (CEF_CALLBACK *clear_scheme_handler_factories)(
      struct _cef_request_context_t* self);

  ///
  // Tells all renderer processes associated with this context to throw away
  // their plugin list cache. If |reload_pages| is true (1) they will also
  // reload all pages with plugins.
  // cef_request_tContextHandler::OnBeforePluginLoad may be called to rebuild
  // the plugin list cache.
  ///
  void (CEF_CALLBACK *purge_plugin_list_cache)(
      struct _cef_request_context_t* self, int reload_pages);

  ///
  // Returns true (1) if a preference with the specified |name| exists. This
  // function must be called on the browser process UI thread.
  ///
  int (CEF_CALLBACK *has_preference)(struct _cef_request_context_t* self,
      const cef_string_t* name);

  ///
  // Returns the value for the preference with the specified |name|. Returns
  // NULL if the preference does not exist. The returned object contains a copy
  // of the underlying preference value and modifications to the returned object
  // will not modify the underlying preference value. This function must be
  // called on the browser process UI thread.
  ///
  struct _cef_value_t* (CEF_CALLBACK *get_preference)(
      struct _cef_request_context_t* self, const cef_string_t* name);

  ///
  // Returns all preferences as a dictionary. If |include_defaults| is true (1)
  // then preferences currently at their default value will be included. The
  // returned object contains a copy of the underlying preference values and
  // modifications to the returned object will not modify the underlying
  // preference values. This function must be called on the browser process UI
  // thread.
  ///
  struct _cef_dictionary_value_t* (CEF_CALLBACK *get_all_preferences)(
      struct _cef_request_context_t* self, int include_defaults);

  ///
  // Returns true (1) if the preference with the specified |name| can be
  // modified using SetPreference. As one example preferences set via the
  // command-line usually cannot be modified. This function must be called on
  // the browser process UI thread.
  ///
  int (CEF_CALLBACK *can_set_preference)(struct _cef_request_context_t* self,
      const cef_string_t* name);

  ///
  // Set the |value| associated with preference |name|. Returns true (1) if the
  // value is set successfully and false (0) otherwise. If |value| is NULL the
  // preference will be restored to its default value. If setting the preference
  // fails then |error| will be populated with a detailed description of the
  // problem. This function must be called on the browser process UI thread.
  ///
  int (CEF_CALLBACK *set_preference)(struct _cef_request_context_t* self,
      const cef_string_t* name, struct _cef_value_t* value,
      cef_string_t* error);
} cef_request_context_t;


///
// Returns the global context object.
///
CEF_EXPORT cef_request_context_t* cef_request_context_get_global_context();

///
// Creates a new context object with the specified |settings| and optional
// |handler|.
///
CEF_EXPORT cef_request_context_t* cef_request_context_create_context(
    const struct _cef_request_context_settings_t* settings,
    struct _cef_request_context_handler_t* handler);

///
// Creates a new context object that shares storage with |other| and uses an
// optional |handler|.
///
CEF_EXPORT cef_request_context_t* create_context_shared(
    cef_request_context_t* other,
    struct _cef_request_context_handler_t* handler);


#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_CEF_REQUEST_CONTEXT_CAPI_H_
