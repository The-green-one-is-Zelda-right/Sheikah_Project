// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2023 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifndef OMNI_PVD_EXTENSION_SAMPLER_H
#define OMNI_PVD_EXTENSION_SAMPLER_H


#if PX_SUPPORT_OMNI_PVD

#include "foundation/PxUserAllocated.h"

#include "ExtOmniPvdRegistrationData.h"


class OmniPvdPxExtensionsSampler : public physx::PxUserAllocated
{
public:
	OmniPvdPxExtensionsSampler();
	~OmniPvdPxExtensionsSampler();
	void setOmniPvdWriter(OmniPvdWriter* omniPvdWriter);
	OmniPvdWriter* getOmniPvdWriter() const { return mWriter; }
	void registerClasses();

	const physx::Ext::OmniPvdPxExtensionsRegistrationData& getRegistrationData() const { return mRegistrationData; }

	// OmniPvdPxExtensionsSampler singleton
	static bool createInstance();
	static OmniPvdPxExtensionsSampler* getInstance();
	static void destroyInstance();

private:
	OmniPvdWriter* mWriter;
	physx::Ext::OmniPvdPxExtensionsRegistrationData mRegistrationData;
};


namespace physx
{
namespace Ext
{

const OmniPvdPxExtensionsRegistrationData* OmniPvdGetPxExtensionsRegistrationData();
OmniPvdWriter* OmniPvdGetWriter();

}
}

#endif

#endif
