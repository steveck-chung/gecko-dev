/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_VR_DISPLAY_PRESENTATION_H
#define GFX_VR_DISPLAY_PRESENTATION_H

#include "mozilla/RefPtr.h"
#include "mozilla/dom/VRDisplayBinding.h"

namespace mozilla {
namespace gfx {
class VRDisplayClient;
namespace vr {
class VRLayerChild;
} // namepsace vr

class VRDisplayPresentation final
{
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(VRDisplayPresentation)

public:
  VRDisplayPresentation(VRDisplayClient *aDisplayClient, const nsTArray<dom::VRLayer>& aLayers);
  void SubmitFrame(int32_t aInputFrameID);
  void GetDOMLayers(nsTArray<dom::VRLayer>& result);

private:
  ~VRDisplayPresentation();
  void CreateLayers();
  void DestroyLayers();

  RefPtr<VRDisplayClient> mDisplayClient;
  nsTArray<dom::VRLayer> mDOMLayers;
  nsTArray<RefPtr<VRLayerChild>> mLayers;
};

} // namespace gfx
} // namespace mozilla

#endif /* GFX_VR_DISPLAY_PRESENTAITON_H */
