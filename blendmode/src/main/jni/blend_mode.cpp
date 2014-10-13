#include <android/bitmap.h>
#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define  LOG_TAG    "image"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);

static uint32_t  HSLtoRGB_Subfunction(float temp1, float temp2, float temp3)
{
    uint32_t c = 0;
    if((temp3 * 6) < 1)
        c = (uint32_t) ((temp2 + (temp1 - temp2)*6*temp3)*100);
    else
    if((temp3 * 2) < 1)
        c = (uint32_t)(temp1*100);
    else
    if((temp3 * 3) < 2)
        c = (uint32_t)((temp2 + (temp1 - temp2)*(.66666 - temp3)*6)*100);
    else
        c = (uint32_t)(temp2*100);
    return c;
}
uint32_t hlsToRbg(float h,float l,float s){
        uint32_t r = 0;
        uint32_t g = 0;
        uint32_t b = 0;

        float L = ((float)l)/100;
        float S = ((float)s)/100;
        float H = ((float)h)/360;

        if(s == 0)
        {
            r = (uint32_t)l;
            g = (uint32_t)l;
            b = (uint32_t)l;
        }
        else
        {
            float temp1 = 0;
            if(L < .50)
            {
                temp1 = L*(1 + S);
            }
            else
            {
                temp1 = L + S - (L*S);
            }

            float temp2 = 2*L - temp1;

            float temp3 = 0;
            for(int i = 0 ; i < 3 ; i++)
            {
                switch(i)
                {
                    case 0: // red
                    {
                        temp3 = H + .33333f;
                        if(temp3 > 1)
                            temp3 -= 1;
                        r = HSLtoRGB_Subfunction(temp1,temp2,temp3);
                        break;
                    }
                    case 1: // green
                    {
                        temp3 = H;
                        g = HSLtoRGB_Subfunction(temp1,temp2,temp3);
                        break;
                    }
                    case 2: // blue
                    {
                        temp3 = H - .33333f;
                        if(temp3 < 0)
                            temp3 += 1;
                        b = HSLtoRGB_Subfunction(temp1,temp2,temp3);
                        break;
                    }
                    default:
                    {

                    }
                }
            }
        }
        r = (uint32_t)((((float)r)/100)*255);
        g = (uint32_t)((((float)g)/100)*255);
        b = (uint32_t)((((float)b)/100)*255);
        return (uint32_t)(0xFF << 24) | (b << 16) | (g << 8) | r;
}
float* rgbToHSL(int r,int g,int b){

        float r_percent = ((float)r)/255;
        float g_percent = ((float)g)/255;
        float b_percent = ((float)b)/255;

        float max_color = 0;
        if((r_percent >= g_percent) && (r_percent >= b_percent))
        {
            max_color = r_percent;
        }
        if((g_percent >= r_percent) && (g_percent >= b_percent))
            max_color = g_percent;
        if((b_percent >= r_percent) && (b_percent >= g_percent))
            max_color = b_percent;

        float min_color = 0;
        if((r_percent <= g_percent) && (r_percent <= b_percent))
            min_color = r_percent;
        if((g_percent <= r_percent) && (g_percent <= b_percent))
            min_color = g_percent;
        if((b_percent <= r_percent) && (b_percent <= g_percent))
            min_color = b_percent;

        float L = 0;
        float S = 0;
        float H = 0;

        L = (max_color + min_color)/2;

        if(max_color == min_color)
        {
            S = 0;
            H = 0;
        }
        else
        {
            if(L < .50)
            {
                S = (max_color - min_color)/(max_color + min_color);
            }
            else
            {
                S = (max_color - min_color)/(2 - max_color - min_color);
            }
            if(max_color == r_percent)
            {
                H = (g_percent - b_percent)/(max_color - min_color);
            }
            if(max_color == g_percent)
            {
                H = 2 + (b_percent - r_percent)/(max_color - min_color);
            }
            if(max_color == b_percent)
            {
                H = 4 + (r_percent - g_percent)/(max_color - min_color);
            }
        }
        S = (S*100);
        L = (L*100);
        H = H*60;
        if(H < 0)
            H += 360;
        float* res = new float[3];
        res[0] = H;
        res[1] = L;
        res[2] = S;
        return res;

}
static int rgb_clamp(int value) {
  if(value > 255) {
    return 255;
  }
  if(value < 0) {
    return 0;
  }
  return value;
}

extern "C"{
    void Java_com_tunebrains_graphic_Utils_colorBlendMode( JNIEnv* env,jobject thiz,jobject sourse,uint32_t overlaycolor, jobject dest)
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
        int red = (int) ((overlaycolor & 0x00FF0000) >> 16);
        int green = (int)((overlaycolor & 0x0000FF00) >> 8);
        int blue = (int) (overlaycolor & 0x00000FF );
        float*hslB = rgbToHSL(red,green,blue);

        for (int i=0;i<sourseBitmapInfo.height;++i){
            for (int j=0;j<sourseBitmapInfo.width;++j){
                uint32_t color =soursepixelsChar[i*sourseBitmapInfo.width+j];

                int blue = (int) ((color & 0x00FF0000) >> 16);
                int green = (int)((color & 0x0000FF00) >> 8);
                int red = (int) (color & 0x00000FF );

                float *hslA = rgbToHSL(red,green,blue);
                int newColor = hlsToRbg(hslB[0],hslA[1],hslB[2]);

                destpixelsChar[i*sourseBitmapInfo.width+j]=newColor;
                delete hslA;
            }
        }

        AndroidBitmap_unlockPixels(env, dest);
        AndroidBitmap_unlockPixels(env, sourse);
    }
}
