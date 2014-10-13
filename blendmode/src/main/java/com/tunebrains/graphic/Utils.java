package com.tunebrains.graphic;

import android.graphics.Bitmap;

/**
 * Created by atimoshenko on 10/13/14.
 */
public class Utils {
    static{
        System.loadLibrary("GraphicUtils");
    }
    public static native void colorBlendMode(Bitmap sourse,int blendcolor,Bitmap dest);
    public static native void grayscale(Bitmap sourse, Bitmap dest);
}
