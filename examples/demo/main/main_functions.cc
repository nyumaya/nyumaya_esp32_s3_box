#include "main_functions.h"

#include "esp_log.h"
#include "libnyumaya.h"
#include "marvin_model.h"

#include "driver/i2s.h"
#include "bsp_i2s.h"
#include "ringbuf.h"

#define I2S_CHANNEL_NUM (1)
#define feature_buffer_size 80*16

namespace {
	int bufsize = 2600;
	int16_t *audio_buffer = nullptr;
	FeatureExtractor *extractor = nullptr;
	AudioRecognitionImpl* recognizer = nullptr;
	ringbuf_t* feature_ringbuffer = nullptr;
	uint8_t feature_buffer[feature_buffer_size];
	uint8_t feature_buffer_out[feature_buffer_size];

	size_t bytes_read = 0;
	float gain = 1.0;

}  // namespace

void setup_mic()
{
	extractor = createFeatureExtractor();
	if (NULL == extractor) {
		esp_system_abort("No memory for feature Extractor");
	}
	audio_buffer = (int16_t*) heap_caps_malloc(bufsize * sizeof(int16_t) * I2S_CHANNEL_NUM, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
	if (NULL == audio_buffer) {
		esp_system_abort("No memory for audio buffer");
	}

	feature_ringbuffer = rb_init("feature_ringbuffer", feature_buffer_size*2);
	if (NULL == feature_ringbuffer) {
		ESP_LOGE("setup_mic", "Error creating ring buffer");
		return;
	}
}

void setup()
{
	recognizer = createAudioRecognition();
	if (NULL == recognizer) {
		esp_system_abort("No memory for Audio Recognition");
	}
	bufsize = getInputDataSize(recognizer);
	int modelNumber;
	int res = addModelFromBuffer(recognizer, marvin_model, marvin_model_len, modelNumber);
	setSensitivity(recognizer, 0.1, modelNumber);
}


void mic_loop()
{
	/* Read audio data from I2S bus (1 channel 16 bit PCM) */
	i2s_read(I2S_NUM_0, audio_buffer, bufsize * I2S_CHANNEL_NUM * sizeof(int16_t), &bytes_read, portMAX_DELAY);
	signalToMel(extractor, audio_buffer, bytes_read/sizeof(int16_t), feature_buffer, gain);
	int bytes_written = rb_write(feature_ringbuffer,(uint8_t*)feature_buffer, feature_buffer_size, 2000);
}


void loop()
{
	if(NULL != feature_ringbuffer){
		int r = rb_read(feature_ringbuffer, feature_buffer_out, feature_buffer_size, 2000);
	}
	int result = runDetection(recognizer, feature_buffer_out, feature_buffer_size);
	if(result == 1){
		ESP_LOGE("main", "Keyword detected!");
	}
}

void end()
{
	heap_caps_free(audio_buffer);
}


