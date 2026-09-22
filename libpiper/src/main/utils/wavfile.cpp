#include "wavfile.hpp"
#include "wav_headers.hpp"
#include <cstdint>
#include <iostream>

void textToWavFile(piper_synthesizer *piper, piper_synthesize_options *options,
                   const char *string, std::ostream &stream) {
  piper_synthesize_start(piper, string, options /* NULL for defaults */);
  piper_audio_chunk chunk;
  bool isHeaderWritten = false;
  do {
    piper_synthesize_next(piper, &chunk);
    if (chunk.num_samples > 0) {
      if (!isHeaderWritten) {
        writeWavStreamHeader(stream, chunk.sample_rate);
        isHeaderWritten = true;
      }
      stream.write(reinterpret_cast<const char *>(chunk.samples),
                   // NOLINTNEXTLINE(bugprone-narrowing-conversions)
                   chunk.num_samples * sizeof(float));
    }
  } while (!chunk.is_last);
}

void textToRawFile(piper_synthesizer *piper, piper_synthesize_options *options,
                   const char *string, std::ostream &stream) {
  piper_synthesize_start(piper, string, options /* NULL for defaults */);
  piper_audio_chunk chunk;
  do {
    piper_synthesize_next(piper, &chunk);
    for (size_t i = 0; i < chunk.num_samples; i++) {
      // Clamp float sample to [-1.0, 1.0] then convert to 16-bit PCM.
      float s = chunk.samples[i];
      if (s < -1.0f) {
        s = -1.0f;
      } else if (s > 1.0f) {
        s = 1.0f;
      }
      int16_t sample = static_cast<int16_t>(s * 32767.0f);
      char bytes[2];
      bytes[0] = static_cast<char>(sample & 0xff);
      bytes[1] = static_cast<char>((sample >> 8) & 0xff);
      stream.write(bytes, 2);
    }
  } while (!chunk.is_last);
}
