#include <android/bitmap.h>
#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
int smallest(int x, int y, int z){
    return std::min(std::min(x, y), z);
}
int biggest(int x, int y, int z){
    return std::max(std::max(x, y), z);
}
#define  LOG_TAG    "image"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);


extern "C"{
    void Java_com_tunebrains_graphic_Utils_grayscale( JNIEnv* env,jobject thiz,jobject sourse,jobject dest)
    {
    //sourse
        AndroidBitmapInfo sourseBitmapInfo;
        void* soursepixels;
        AndroidBitmap_getInfo(env, sourse, &sourseBitmapInfo);
        AndroidBitmap_lockPixels(env, sourse, &soursepixels);
        uint32_t* soursepixelsChar = (uint32_t*) soursepixels;
    //dest
        AndroidBitmapInfo destBitmapInfo;
        void* destpixels;
        AndroidBitmap_getInfo(env, dest, &destBitmapInfo);
        AndroidBitmap_lockPixels(env, dest, &destpixels);
        uint32_t* destpixelsChar = (uint32_t*) destpixels;


        for (int i=0;i<sourseBitmapInfo.height;++i){
            for (int j=0;j<sourseBitmapInfo.width;++j){
                uint32_t color =soursepixelsChar[i*sourseBitmapInfo.width+j];

                int blue = (int) ((color & 0x00FF0000) >> 16);
                int green = (int)((color & 0x0000FF00) >> 8);
                int red = (int) (color & 0x00000FF );

                int v=(smallest(red,green,blue)+biggest(red,green,blue))/2;
//                destpixelsChar[i*sourseBitmapInfo.width+j]=(int)(red+green+blue)/3;
                destpixelsChar[i*sourseBitmapInfo.width+j]=(uint32_t)(0xFF << 24) | (v << 16) | (v << 8) | v;
//                LOGE("%d",(red+green+blue)/3);
            }
        }

        AndroidBitmap_unlockPixels(env, dest);
        AndroidBitmap_unlockPixels(env, sourse);
    }
}
