/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DocAccessibleWrap.h"

#include "Compatibility.h"
#include "nsWinUtils.h"
#include "mozilla/dom/TabChild.h"
#include "Role.h"
#include "RootAccessible.h"
#include "sdnDocAccessible.h"
#include "Statistics.h"

#include "nsIDocShell.h"
#include "nsIInterfaceRequestorUtils.h"

using namespace mozilla;
using namespace mozilla::a11y;

////////////////////////////////////////////////////////////////////////////////
// DocAccessibleWrap
////////////////////////////////////////////////////////////////////////////////

DocAccessibleWrap::
  DocAccessibleWrap(nsIDocument* aDocument, nsIPresShell* aPresShell) :
  DocAccessible(aDocument, aPresShell), mHWND(nullptr)
{
}

DocAccessibleWrap::~DocAccessibleWrap()
{
}

IMPL_IUNKNOWN_QUERY_HEAD(DocAccessibleWrap)
  if (aIID == IID_ISimpleDOMDocument) {
    statistics::ISimpleDOMUsed();
    *aInstancePtr = static_cast<ISimpleDOMDocument*>(new sdnDocAccessible(this));
    static_cast<IUnknown*>(*aInstancePtr)->AddRef();
    return S_OK;
  }
IMPL_IUNKNOWN_QUERY_TAIL_INHERITED(HyperTextAccessibleWrap)

STDMETHODIMP
DocAccessibleWrap::get_accValue(VARIANT aVarChild, BSTR __RPC_FAR* aValue)
{
  A11Y_TRYBLOCK_BEGIN

  if (!aValue)
    return E_INVALIDARG;
  *aValue = nullptr;

  // For backwards-compat, we still support old MSAA hack to provide URL in accValue
  // Check for real value first
  HRESULT hr = AccessibleWrap::get_accValue(aVarChild, aValue);
  if (FAILED(hr) || *aValue || aVarChild.lVal != CHILDID_SELF)
    return hr;

  // If document is being used to create a widget, don't use the URL hack
  roles::Role role = Role();
  if (role != roles::DOCUMENT && role != roles::APPLICATION &&
      role != roles::DIALOG && role != roles::ALERT)
    return hr;

  nsAutoString url;
  URL(url);
  if (url.IsEmpty())
    return S_FALSE;

  *aValue = ::SysAllocStringLen(url.get(), url.Length());
  return *aValue ? S_OK : E_OUTOFMEMORY;

  A11Y_TRYBLOCK_END
}

////////////////////////////////////////////////////////////////////////////////
// Accessible

void
DocAccessibleWrap::Shutdown()
{
  // Do window emulation specific shutdown if emulation was started.
  if (nsWinUtils::IsWindowEmulationStarted()) {
    // Destroy window created for root document.
    if (mDocFlags & eTabDocument) {
      nsWinUtils::sHWNDCache->Remove(mHWND);
      ::DestroyWindow(static_cast<HWND>(mHWND));
    }

    mHWND = nullptr;
  }

  DocAccessible::Shutdown();
}

////////////////////////////////////////////////////////////////////////////////
// DocAccessible public

void*
DocAccessibleWrap::GetNativeWindow() const
{
  return mHWND ? mHWND : DocAccessible::GetNativeWindow();
}

////////////////////////////////////////////////////////////////////////////////
// DocAccessible protected

void
DocAccessibleWrap::DoInitialUpdate()
{
  DocAccessible::DoInitialUpdate();

  if (nsWinUtils::IsWindowEmulationStarted()) {
    // Create window for tab document.
    if (mDocFlags & eTabDocument) {
      mozilla::dom::TabChild* tabChild =
        mozilla::dom::TabChild::GetFrom(mDocumentNode->GetShell());

      a11y::RootAccessible* rootDocument = RootAccessible();

      mozilla::WindowsHandle nativeData = 0;
      if (tabChild)
        tabChild->SendGetWidgetNativeData(&nativeData);
      else
        nativeData = reinterpret_cast<mozilla::WindowsHandle>(
          rootDocument->GetNativeWindow());

      bool isActive = true;
      nsIntRect rect(CW_USEDEFAULT, CW_USEDEFAULT, 0, 0);
      if (Compatibility::IsDolphin()) {
        rect = Bounds();
        nsIntRect rootRect = rootDocument->Bounds();
        rect.x = rootRect.x - rect.x;
        rect.y -= rootRect.y;

        nsCOMPtr<nsISupports> container = mDocumentNode->GetContainer();
        nsCOMPtr<nsIDocShell> docShell = do_QueryInterface(container);
        docShell->GetIsActive(&isActive);
      }

      HWND parentWnd = reinterpret_cast<HWND>(nativeData);
      mHWND = nsWinUtils::CreateNativeWindow(kClassNameTabContent, parentWnd,
                                             rect.x, rect.y,
                                             rect.width, rect.height, isActive);

      nsWinUtils::sHWNDCache->Put(mHWND, this);

    } else {
      DocAccessible* parentDocument = ParentDocument();
      if (parentDocument)
        mHWND = parentDocument->GetNativeWindow();
    }
  }
}
