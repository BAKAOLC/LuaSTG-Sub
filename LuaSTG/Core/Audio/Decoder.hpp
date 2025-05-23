﻿#pragma once
#include "Core/Type.hpp"
#include "core/ReferenceCounted.hpp"

namespace core::Audio
{
	struct IDecoder : public IReferenceCounted
	{
		virtual uint16_t getSampleSize() = 0;   // 1byte(8bit) 2bytes(16bits) 3byte(24bits) 4byte(32bits)
		virtual uint16_t getChannelCount() = 0; // 1channels 2channels
		virtual uint16_t getFrameSize() = 0;    // = getChannelCount() * getSampleSize()
		virtual uint32_t getSampleRate() = 0;   // 22050Hz 44100Hz 48000Hz
		virtual uint32_t getByteRate() = 0;     // = getSampleRate() * getFrameSize()
		virtual uint32_t getFrameCount() = 0;   // how many pcm frames?

		virtual bool seek(uint32_t pcm_frame) = 0;
		virtual bool seekByTime(double sec) = 0;
		virtual bool tell(uint32_t* pcm_frame) = 0;
		virtual bool tellAsTime(double* sec) = 0;
		virtual bool read(uint32_t pcm_frame, void* buffer, uint32_t* read_pcm_frame) = 0; // s16

		static bool create(StringView path, IDecoder** pp_decoder);
	};
}

namespace core {
	// UUID v5
	// ns:URL
	// https://www.luastg-sub.com/core.IDecoder
	template<> constexpr InterfaceId getInterfaceId<Audio::IDecoder>() { return UUID::parse("d4b8ac05-c486-5795-a545-dbe6b65b3828"); }
}
