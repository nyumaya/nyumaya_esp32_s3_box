# nyumaya_esp32_s3_box
Example Application for nyumaya audiorecognition on ESP32-S3-BOX


## Steps to use your custom model the model:
The model is provided as unsigned char array. 
You have to provide this array to the function addModelFromBuffer.
In this example this array is just included via marvin_model.h and marvin_model.c as source files in the CMakeLists.txt

## Limitations:
Currently there is only support for listening to one keyword


